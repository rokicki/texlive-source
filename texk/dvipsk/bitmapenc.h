/*
 *   Include file for proper bitmap encoding.
 */
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
 *   For StandardEncoding we use this magic (negative) constant.
 */
#define STANDARD_ENCODING_SEQ_MAGIC (-0xad0be)
