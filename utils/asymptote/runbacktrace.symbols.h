/*****
 * This file is automatically generated by findsym.pl
 * Changes will be overwritten.
 *****/

// If the ADDSYMBOL macro is not already defined, define it with the default
// purpose of referring to an external pre-translated symbol, such that
// SYM(name) also refers to that symbol.
#ifndef ADDSYMBOL
    #define ADDSYMBOL(name) extern sym::symbol PRETRANSLATED_SYMBOL_##name
    #define SYM(name) PRETRANSLATED_SYMBOL_##name
#endif

ADDSYMBOL(generate_random_backtrace);
ADDSYMBOL(n);
ADDSYMBOL(print_random_addresses);