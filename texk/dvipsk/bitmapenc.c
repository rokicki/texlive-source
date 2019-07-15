/*
 *   This file implements support for adding proper character encodings to
 *   bitmap fonts, so searching and copying text works properly in PDFs
 *   generated by the dvips Postscript output that uses bitmap fonts.
 *
 *   Written by Tomas Rokicki in June 2019.
 *
 *   It turns out we need to do more than just add encodings to the fonts
 *   for most PDF viewers, so this file also takes care of those issues.
 */
/*
 *   Our first concern is finding and reading encoding files for bitmap
 *   fonts, and, if such cannot be found, determining a reasonable
 *   alternative.
 *
 *   We could use the code in writet1 to read an encoding file, but
 *   we would need to make some messy changes for that to work, and we
 *   really want this to be a minimal impact to existing code (especially
 *   code such as write_t1 that we don't have a good handle on).
 *
 *   We want to deduplicate (combine identical) encodings to lower the
 *   memory and file size impact.  But we have to be careful; if the
 *   document requires multiple sections, we need to redownload encodings
 *   just like we do fonts.
 */
/*
 *   Reading the encodings.
 *
 *   Encodings we read are returned as an array C strings.  We do not
 *   attempt to parse the contents of the encodings, because they
 *   may contain additional dictionaries or other PostScript code
 *   that can possibly do more advanced things.
 */
#include <stdio.h>
#include <string.h>
#ifndef STANDALONE
#include <kpathsea/config.h>
#endif
#include "dvips.h"
#include "protos.h"
/*
 *   Do we use this functionality?  If 0, then no; if 1, then yes, but
 *   suppress warnings; if 2, then warn.
 */
int encodetype3 = 1 ;
void bitmapencopt(int v) {
   if (0 <= v && v <= 2)
      encodetype3 = v ;
   else
      error("! Bad option to J") ;
}
#ifdef STANDALONE
#undef fopen
#undef fclose
#endif
#define ENCODING_CHAR_COUNT 256
#define MAX_LINE_LENGTH 256
/*
 *   A structure for storing deduplicated encodings.  These are deduplicated
 *   in dvips memory on load.  We also store information on whether the
 *   encoding has been downloaded in a particular section.
 */
struct bmenc {
   const char **enc ;   // the encoding itself
   int downloaded_seq ; // -1: not downloaded; else, sequence number
   struct bmenc *next ; // next encoding in linear linked list
} ;
/*
 *   This structure maintains a relationship between a font name and a
 *   bitmap encoding.  Again, linear search is used.
 */
struct bmfontenc {
   const char *fontname ;   // dvi font name
   struct bmenc *enc ;      // the encoding to use
   struct bmfontenc *next ; // next encoding
} ;
/*
 *   Parse the encoding file.  If use_all flag is set then we are parsing
 *   the dvips_all file which can have multiple encodings, each preceded
 *   by a set of lines with font names followed by a colon character.
 *
 *   If we are parsing them all, we build a sorted global list of entries
 *   for later lookup, and always return 0.  Otherwise we just expect a
 *   single encoding and we store that away.
 */
static int numstatic, capstatic, namedstatic ;
static struct allfontenc {
   const char *fontname ;
   const char **enc ;
} *bmfontarr ;
const char **working_enc ;
int in_working_enc = 0 ;
int working_enc_left = 0 ;
static void add_fontname(const char *fontname) {
   if (in_working_enc) { // flush
      const char **e = working_enc ;
      while (numstatic < namedstatic)
         bmfontarr[numstatic++].enc = e ;
      working_enc += in_working_enc + 1 ;
      working_enc_left -= in_working_enc + 1 ;
      in_working_enc = 0 ;
   }
   if (fontname == 0)
      return ;
   if (namedstatic >= capstatic) {
      int ncapstatic = capstatic * 2 + 20 ;
      struct allfontenc *nbmfontarr =
        (struct allfontenc *)mymalloc(sizeof(struct allfontenc) * ncapstatic) ;
      if (capstatic) {
         memcpy(nbmfontarr, bmfontarr, sizeof(struct allfontenc)*capstatic) ;
         free(bmfontarr) ;
      }
      bmfontarr = nbmfontarr ;
      for (int i=capstatic; i<ncapstatic; i++) {
         bmfontarr[i].fontname = 0 ;
         bmfontarr[i].enc = 0 ;
      }
      capstatic = ncapstatic ;
   }
   bmfontarr[namedstatic].fontname = fontname ;
   bmfontarr[namedstatic].enc = 0 ;
   namedstatic++ ;
}
static void add_encline(const char *encline) {
   if (in_working_enc + 1 >= working_enc_left) {
      int new_enc_size = 100 + 2 * in_working_enc ;
      const char **new_working_enc =
               (const char **)mymalloc(sizeof(const char **) * new_enc_size) ;
      for (int i=0; i<new_enc_size; i++)
         new_working_enc[i] = 0 ;
      for (int i=0; i<in_working_enc; i++)
         new_working_enc[i] = working_enc[i] ;
      working_enc = new_working_enc ;
      working_enc_left = new_enc_size - in_working_enc ;
   }
   working_enc[in_working_enc++] = encline ;
}
static const char **parseencoding(FILE *f, int use_all) {
   char encbuf[MAX_LINE_LENGTH+1] ;
   for (int i=0; i<sizeof(encbuf); i++)
      encbuf[i] = 0 ;
   while (fgets(encbuf, MAX_LINE_LENGTH, f) != 0) {
      char *p = encbuf ;
      char *q = p + strlen(p) - 1 ;
      while (q > p && *q < ' ') // kill line terminators
         *q-- = 0 ;
      q = p ;
      while (*q && *q != ' ' && *q != ':')
         q++ ;
      if (use_all && *q == ':') { // looks like a font name
         if (q[1] >= ' ' || !use_all)
            error(
       "! unexpected colon or extra stuff at end of line in encoding file?") ;
         *q = 0 ;
         add_fontname(strdup(p)) ;
      } else {
         add_encline(strdup(p)) ;
      }
   }
   if (use_all)
      add_fontname(0) ; // flushes last encoding
   if (use_all) {
      // sort the static fonts for quicker search; Shell sort
      int h = 1 ;
      while (h < numstatic)
         h = 3 * h + 1 ;
      while (h > 1) {
         h /= 3 ;
         for (int i=h; i<numstatic; i++) {
            int j = i ;
            while (j >= h &&
                   strcmp(bmfontarr[j-h].fontname, bmfontarr[j].fontname) > 0) {
               const char *na = bmfontarr[j-h].fontname ;
               const char **en = bmfontarr[j-h].enc ;
               bmfontarr[j-h].fontname = bmfontarr[j].fontname ;
               bmfontarr[j-h].enc = bmfontarr[j].enc ;
               bmfontarr[j].fontname = na ;
               bmfontarr[j].enc = en ;
               j -= h ;
            }
         }
      }
      return 0 ;
   } else {
      const char **e = working_enc ;
      if (e == 0)
         error("! No lines in encoding?") ;
      return e ;
   }
}
/*
 *   Given a font name, find an encoding file.
 */
#define MAX_NAME_SIZE 256
static FILE *bitmap_enc_search(const char *fontname) {
   char namebuf[MAX_NAME_SIZE+1] ;
   if (fontname == 0 || strlen(fontname) > 128)
      error("! excessively long font name") ;
   sprintf(namebuf, "dvips-%s.enc", fontname) ;
#ifdef STANDALONE
   return fopen(namebuf, FOPEN_RBIN_MODE) ;
#else
   return search(kpse_enc_format, namebuf, FOPEN_RBIN_MODE) ;
#endif
}
/*
 *   Are two raw encodings identical?
 */
static int eqencoding(const char **a, const char **b) {
   if (a == b)
      return 1 ;
   for (int i=0; a[i] != 0 && b[i] != 0; i++)
      if (a[i] != b[i] && (a[i] == 0 || b[i] == 0 || strcmp(a[i], b[i]) != 0))
         return 0 ;
   return 1 ;
}
/*
 *   Our list of encodings we've seen.
 */
static struct bmenc *bmlist ;
/*
 *   Add an encoding to our list.
 */
struct bmenc *addbmenc(const char **enc) {
   struct bmenc *r = (struct bmenc *)mymalloc(sizeof(struct bmenc)) ;
   r->downloaded_seq = -1 ;
   r->enc = enc ;
   r->next = bmlist ;
   bmlist = r ;
   return r ;
}
/*
 *   Given a particular encoding, walk through our encoding list and
 *   see if it already exists; if so, return the existing one and
 *   drop the new one.  The set of distinct encodings in a particular
 *   document is expected to be small (a few dozen at most).
 */
static struct bmenc *deduplicateencoding(const char **enc) {
   for (struct bmenc *p=bmlist; p!=0; p=p->next)
      if (eqencoding(p->enc, enc))
         return p ;
   return addbmenc(enc) ;
}
static const char **bitmap_enc_load(const char *fontname, int use_all) {
   FILE *f = bitmap_enc_search(use_all ? "all" : fontname) ;
   if (f != 0) {
      const char **r = parseencoding(f, use_all) ;
      fclose(f) ;
      return r ;
   }
   return 0 ;
}
/*
 *   Start a section: say we didn't download anything.
 */
static int curbmseq ;
void bmenc_startsection() {
   for (struct bmenc *p=bmlist; p!=0; p=p->next)
      p->downloaded_seq = -1 ;
   curbmseq = 0 ;
}
/*
 *   Try to find a font in the all encoding list.  This list is sorted
 *   so we can use binary search.
 */
static const char **bitmap_all_find(const char *fontname) {
   int ptr = 0 ;
   int bit = 1 ;
   while ((bit << 1) < numstatic)
      bit <<= 1 ;
   while (bit > 0) {
      int t = ptr + bit ;
      if (t < numstatic && strcmp(fontname, bmfontarr[t].fontname) >= 0)
         ptr = t ;
      bit >>= 1 ;
   }
   if (strcmp(fontname, bmfontarr[ptr].fontname) == 0)
      return bmfontarr[ptr].enc ;
   else
      return 0 ;
}
/*
 *   Download the encoding and set the sequence number.
 */
static void downloadenc(struct bmenc *enc) {
   int fresh = 0 ;
   if (enc->downloaded_seq < 0) {
      newline() ;
      for (int i=0; enc->enc[i]; i++)
         pslineout(enc->enc[i]) ;
      enc->downloaded_seq = curbmseq++ ;
      fresh = 1 ;
   }
   newline() ;
   char cmdbuf[16] ;
   sprintf(cmdbuf, "/EN%d", enc->downloaded_seq) ;
   if (fresh) {
      cmdout("A") ;
      psnameout(cmdbuf) ;
      cmdout("X") ;
   } else {
      // we use load here specifically to defer execution until the
      // new font dictionary is on the stack and being built.
      psnameout(cmdbuf) ;
      cmdout("load") ;
   }
}
/*
 *   Send out the new encoding, font bounding box, and font matrix.
 */
static int getencoding_seq(const char *fontname) ;
int downloadbmencoding(const char *name, double scale,
                       int llx, int lly, int urx, int ury) {
   int seq = getencoding_seq(name) ;
   if (seq < 0)
      return -1 ;
   cmdout("IEn") ;
   cmdout("S") ;
   psnameout("/IEn") ;
   cmdout("X") ;
   cmdout("FBB") ;
   cmdout("FMat") ;
   psnameout("/FMat") ;
   specialout('[') ;
   floatout(1.0/scale) ;
   numout(0) ;
   numout(0) ;
   floatout(-1.0/scale) ;
   numout(0) ;
   numout(0) ;
   specialout(']') ;
   cmdout("N") ;
   psnameout("/FBB") ;
   // we add a bit of slop here, because this is only used for
   // highlighting, and in theory if the bounding box is too
   // tight, on some RIPs, characters could be clipped.
   int slop = 1 ;
   specialout('[') ;
   numout(llx-slop) ;
   numout(lly-slop) ;
   numout(urx+slop) ;
   numout(ury+slop) ;
   specialout(']') ;
   cmdout("N") ;
   return seq ;
}
/*
 *   Finish up the downloaded encoding; scale the font, and restore the
 *   previous global definitions.
 */
void finishbitmapencoding(const char *name, double scale) {
   psnameout(name) ;
   cmdout("load") ;
   numout(0) ;
   cmdout(name+1) ;
   cmdout("currentfont") ;
   floatout(scale) ;
   cmdout("scalefont") ;
   cmdout("put") ;
   psnameout("/FMat") ;
   cmdout("X") ;
   psnameout("/FBB") ;
   cmdout("X") ;
   psnameout("/IEn") ;
   cmdout("X") ;
   newline();
}
/*
 *   This is the list of fonts we have seen so far.
 */
struct bmfontenc *bmfontenclist ;
/*
 *   We warn if we have to use a built-in encoding, and set this value to 1.
 *   We warn again if we cannot find a built-in encoding and have to
 *   default to StandardEncoding, and set this value to 2.
 */
static int warned_about_missing_encoding = 0 ;
/*
 *   Print a warning message.
 */
static void bmenc_warn(const char *fontname, const char *msg) {
   if (encodetype3 > 1)
      fprintf(stderr,
         "dvips: Static bitmap font encoding for font %s (and others?): %s.\n",
                   fontname, msg) ;
}
/*
 *   About to download a font; find the encoding sequence number.
 *   If needed, download a new sequence.  If we can't find one, use
 *   -1; this font will not work for copy/paste.
 */
static int tried_all = 0 ; // have we tried to load dvips-all.enc
static int getencoding_seq(const char *fontname) {
   struct bmenc *enc = 0 ;
   struct bmfontenc *p = bmfontenclist ;
   for (; p!=0; p=p->next)
      if (strcmp(fontname, p->fontname) == 0) {
         enc = p->enc ;
         if (enc == 0) // remember failures
            return -1 ;
         break ;
      }
   // not in list; try to load it from a file
   if (enc == 0) {
      const char **e = bitmap_enc_load(fontname, 0) ;
      // did not find it; try to load it from the dvips-all list.
      if (e == 0) {
         if (!tried_all) {
            bitmap_enc_load(fontname, 1) ;
            tried_all = 1 ;
         }
         e = bitmap_all_find(fontname) ;
      }
      if (e)
         enc = deduplicateencoding(e) ;
   }
   // did not find this in the list; add it to the list
   if (p == 0) {
      p = (struct bmfontenc *)mymalloc(sizeof(struct bmfontenc)) ;
      p->fontname = strdup(fontname) ;
      p->enc = enc ;
      p->next = bmfontenclist ;
      bmfontenclist = p ;
   }
   if (enc == 0) {
      if (warned_about_missing_encoding < 2) {
         bmenc_warn(fontname, "no match in static list; not encoding") ;
         warned_about_missing_encoding = 2 ;
      }
      return -1 ; // don't download an encoding
   }
   downloadenc(enc) ;
   return enc->downloaded_seq ;
}
#ifdef STANDALONE
/*
 *   Standalone test code:
 */
void error(const char *s) {
   fprintf(stderr, "Failed: %s\n", s) ;
   if (*s == '!')
      exit(0) ;
}
char *mymalloc(int sz) {
   return (char *)malloc(sz) ;
}
int pos = 0 ;
int idok = 1 ;
const int MAXLINE = 75 ;
void newline() {
   printf("\n") ;
   idok = 1 ;
   pos = 0 ;
}
void floatout(float f) {
   printf("%g", f) ;
   pos += 8 ;
}
void pslineout(const char *s) {
   printf("%s\n", s) ;
}
void numout(int num) {
   int len = 1 ;
   int t = num / 10 ;
   while (t>0) {
      len++ ;
      t /= 10 ;
   }
   if (pos + len >= MAXLINE)
      newline() ;
   if (!idok)
      specialout(' ') ;
   printf("%d", num) ;
   pos += len ;
   idok = 0 ;
}
void cmdout(const char *s) {
   if (pos + strlen(s) >= MAXLINE)
      newline() ;
   if (*s != '/' && !idok)
      specialout(' ') ;
   printf("%s", s) ;
   pos += strlen(s) ;
   idok = 0 ;
}
void psnameout(const char *s) {
   cmdout(s) ;
}
void specialout(char c) {
   if (pos + 1 >= MAXLINE)
      newline() ;
   printf("%c", c) ;
   pos++ ;
   idok = 1 ;
}
int main(int argc, char *argv[]) {
   bmenc_startsection() ;
   for (int i=1; i<argc; i++) {
      int r = getencoding_seq(argv[i]) ;
      printf("Result for %s is %d\n", argv[i], r) ;
   }
}
#endif
