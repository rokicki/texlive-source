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
 *   alternative.  Because we want the new dvips to work properly even
 *   when the encoding files cannot be found, we embed encodings for
 *   many common existing Metafont fonts (and complain about this once).
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
 *   Encodings we read are returned as an array of 256 pointers to
 *   character strings, with null for any values that are not defined.
 *   Further, we check for any explicit /.notdef entries and suppress
 *   those to null.  We read exactly one type of encoding file:  one
 *   that has precisely the following format:
 *
 *   /Encoding 256 array
 *   0 1 255 { 1 index exch /.notdef put} for
 *   dup 0 /Gamma put
 *   dup 1 /Delta put
 *   ...
 *   readonly def
 *
 *   Only characters that are defined need be in the file.  We
 *   skip every line that doesn't start with dup, and require the
 *   format of the dup lines to be precisely as shown, with one
 *   per line.
 *
 *   This is different from what writet1 reads, and is not general
 *   PostScript, but it matches the format for almost every Type 1
 *   font I have seen including the existing cm bitmap fonts which
 *   makes extraction from those PFB files easy.  And it's code that
 *   is easy to write and unlikely to have bugs.
 */
#include <stdio.h>
#include <string.h>
#ifndef STANDALONE
#include <kpathsea/config.h>
#endif
#include "dvips.h"
#include "protos.h"
#include "staticbmenc.h"
#include "bitmapenc.h"
#ifdef STANDALONE
#undef fopen
#undef fclose
#endif
#define ENCODING_CHAR_COUNT 256
#define MAX_LINE_LENGTH 256
const char **parseencoding(FILE *f) {
   char encbuf[MAX_LINE_LENGTH+1] ;
   const char **e = (const char **)
                      mymalloc(sizeof(const char *)*ENCODING_CHAR_COUNT) ;
   for (int i=0; i<ENCODING_CHAR_COUNT; i++)
      e[i] = 0 ;
   for (int i=0; i<sizeof(encbuf); i++)
      encbuf[i] = 0 ;
   int characters_loaded = 0 ;
   while (fgets(encbuf, MAX_LINE_LENGTH, f) != 0) {
      if (strncmp(encbuf, "dup", 3) == 0) { // possible line; parse it out
         char *p = encbuf + 3 ;
         const char *err = 0 ;
         char *charname = 0 ;
         int charnum = 0 ;
         do { // do/while(0); let us break to an error routine
            if (*p != ' ' && *p != '\t') {
               err = "Missing whitespace after dup line" ;
               break ;
            }
            while (*p == ' ' || *p == '\t')
               p++ ;
            int dig = 0 ;
            if ('0' > *p || *p > '9') {
               err = "Missing number after dup" ;
               break ;
            }
            while ('0' <= *p && *p <= '9') {
               dig = 10 * dig + *p++ - '0' ;
               if (dig > 255)
                  break ;
            }
            if (dig > 255) {
               err = "Code value too large" ;
               break ;
            }
            if (e[dig] != 0) {
               err = "Duplicated code value" ;
               break ;
            }
            charnum = dig ;
            if (*p != ' ' && *p != '\t') {
               err = "Missing whitespace after dup line" ;
               break ;
            }
            while (*p == ' ' || *p == '\t')
               p++ ;
            if (*p != '/') {
               err = "Glyph name must start with slash" ;
               break ;
            }
            charname = p++ ;
            // PostScript names can be anything but the basic
            // delimiters ()[]{}<>/%.
            while (*p > ' ' && 0 == strchr("[]{}()<>/%", *p))
               p++ ;
            if (p == charname + 1) {
               err = "Empty glyph name" ;
               break ;
            }
            if (*p != ' ' && *p != '\t') {
               err = "Missing whitespace after glyph name" ;
               break ;
            }
            *p++ = 0 ; // terminate the charname string and munge the buffer
            while (*p == ' ' || *p == '\t')
               p++ ;
            if (strncmp(p, "put", 3) != 0) {
               err = "Missing put after glyph name" ;
               break ;
            }
            p += 3 ;
            while (*p == ' ' || *p == '\t')
               p++ ;
            if (*p != 0 && *p != 10 && *p != 13 && *p != '%') {
               err = "Extra stuff after put" ;
               break ;
            }
         } while (0) ; // allow break for errors
         if (err) {
            strcpy("! Reading bitmap encoding file failed: ", encbuf) ;
            strcat(encbuf, err) ;
            error(encbuf) ;
         }
         if (strcmp(charname, "/.notdef") == 0)
            e[charnum] = 0 ;
         else {
            characters_loaded++ ;
            e[charnum] = strdup(charname) ;
         }
         if (e[charnum] == 0)
            error("! ran out of memory reading bitmap encoding") ;
      }
   }
   if (characters_loaded == 0)
      error("! did not find any valid character definitions in encoding file") ;
   return e ;
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
   for (int i=0; i<256; i++)
      if (a[i] != b[i] && (a[i] == 0 || b[i] == 0 || strcmp(a[i], b[i]) != 0))
         return 0 ;
   return 1 ;
}
/*
 *   Free an allocated encoding.
 */
static void freeencoding(const char **enc) {
   for (int i=0; i<256; i++)
      if (enc[i])
         free((void*)enc[i]) ;
   free(enc) ;
}
/*
 *   Our list of encodings we've seen.
 */
static struct bmenc *bmlist ;
/*
 *   Given a particular encoding, walk through our encoding list and
 *   see if it already exists; if so, return the existing one and
 *   free the new one.  The set of distinct encodings in a particular
 *   document is expected to be small (a few dozen at most).
 */
struct bmenc *deduplicateencoding(const char **enc) {
   for (struct bmenc *p=bmlist; p!=0; p=p->next)
      if (eqencoding(p->enc, enc)) {
         freeencoding(enc) ;
         return p ;
      }
   struct bmenc *r = (struct bmenc *)mymalloc(sizeof(struct bmenc)) ;
   r->downloaded_seq = -1 ;
   r->enc = enc ;
   r->next = bmlist ;
   bmlist = r ;
   return r ;
}
/*
 *   We warn if we have to use a built-in encoding, and set this value to 1.
 *   We warn again if we cannot find a built-in encoding and have to
 *   default to StandardEncoding, and set this value to 2.
 */
static int warned_about_missing_encoding = 0 ;
struct bmenc *bitmap_enc_load(const char *fontname) {
   FILE *f = bitmap_enc_search(fontname) ;
   if (f != 0) {
      const char **enc = parseencoding(f) ;
      struct bmenc *r = deduplicateencoding(enc) ;
      fclose(f) ;
      return r ;
   }
   return 0 ;
}
#ifdef STANDALONE
/*
 *   Standalone test code:
 */
void error(const char *s) {
   fprintf(stderr, "Failed: %s\n", s) ;
   exit(0) ;
}
char *mymalloc(int sz) {
   return (char *)malloc(sz) ;
}
int main(int argc, char *argv[]) {
   FILE *f = fopen(argv[1], "r") ;
   if (f == 0)
      error("! can't open file") ;
   const char **r = parseencoding(f) ;
   for (int i=0; i<256; i++) {
      printf("%d: %s\n", i, (r[i] ? r[i] : "/.notdef")) ;
   }
}
#endif
