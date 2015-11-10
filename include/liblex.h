/*
 * liblex API
 * 
 * Author: Felipe Pena <felipensp at gmail.com>
 */
 
#ifndef LLEX_LIBLEX_H
#define LLEX_LIBLEX_H

#include <string.h>
#include <pcre.h>

#define LLEX_MAX_STATE  10  /* Max states */
#define LLEX_MAX_TOKEN 100  /* Max number of tokens */

#define LLEX_IGNORE_WHITESPACE 1<<0 /* Ignore whitespace */

typedef int llex_token_id;
typedef struct _llex llex;

#if defined(__GNUC__) && defined(__i386__)
#define LLEX_FASTCALL __attribute__ ((fastcall))
#else
#define LLEX_FASTCALL
#endif

typedef int (LLEX_FASTCALL *llex_callback)(llex*, unsigned char*, size_t); 

typedef enum {
	LLEX_LITERAL_TOKEN,          /* Literal token */	
	LLEX_LITERAL_TOKEN_CALLBACK, /* Literal token with callback */
	LLEX_REGEX_TOKEN,            /* Regex-based token */
	LLEX_REGEX_TOKEN_CALLBACK    /* Regex-based token with callback */
} llex_token_type;

typedef struct {	
	llex_token_type type;	
	union {
		struct {
			unsigned char *str;
			size_t len;	
		} literal;
		pcre *regex;
	} token;
	union {		
		llex_callback callback;
		llex_token_id id;	
	} data;
} llex_token;

typedef struct {
	llex_token tokens[LLEX_MAX_TOKEN]; /* Lexer tokens */
	unsigned int tokens_count;         /* Number of used tokens */
} llex_token_array;

typedef struct _llex {
	unsigned char *buffer;                        /* Buffer to be read */
	unsigned char *buffer_pos;                    /* Current buffer position */
	unsigned char *current_token;                 /* Current token */
	size_t current_len;                           /* Current token length */
	unsigned int current_state;                   /* Current state */
	llex_token_array token_array[LLEX_MAX_STATE]; /* Tokens by states */
	unsigned int options;                         /* Flags */
} llex;

int llex_init(llex*);
int llex_cleanup(llex*);
int llex_set_buffer(llex*, unsigned char*);
int llex_set_options(llex*, unsigned int);
int llex_set_state(llex*, unsigned int);
int llex_tokenizer(llex*);
int llex_add_token(llex*, unsigned char*, llex_token_id);
int llex_add_token_callback(llex*, unsigned char*, llex_callback);
int llex_add_token_regex(llex*, unsigned char*, llex_token_id);
int llex_add_token_regex_callback(llex*, unsigned char*, llex_callback);

#endif /* LLEX_LIBLEX_H */
