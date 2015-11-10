/*
 * Tokenizer
 * 
 * Author: Felipe Pena <felipensp at gmail.com>
 */
 
#include <string.h>
#include "liblex.h"
#include "regex.h"

inline static int _llex_ltrim(llex *lex)
{
	while (lex->buffer_pos) {
		switch (*lex->buffer_pos) {
			case '\0': return 0;
			case '\t':
			case '\r':
			case '\n':
			case '\v':
			case ' ':
				++lex->buffer_pos;
				continue;
			default:
				goto out;
		}
	}
out:
	return 1;
}

inline static int _llex_find_next_token(llex *lex)
{
	lex->current_token = NULL;
	lex->current_len = 0;
	
	if (*lex->buffer_pos == '\0') return 0;
	
	if (lex->options & LLEX_IGNORE_WHITESPACE) {
		if (!_llex_ltrim(lex)) return 0;
	}

	for (int i = 0; i < lex->token_array[lex->current_state].tokens_count; ++i) {
		const llex_token *t = &lex->token_array[lex->current_state].tokens[i];

		if ((t->type == LLEX_LITERAL_TOKEN || t->type == LLEX_LITERAL_TOKEN_CALLBACK)
			&& lex->buffer_pos[0] == t->token.literal.str[0]
			&& strncmp((char*)lex->buffer_pos, (char*)t->token.literal.str, t->token.literal.len) == 0) {
				
			lex->current_token = lex->buffer_pos;
			lex->current_len = t->token.literal.len;
			lex->buffer_pos += t->token.literal.len;
		
			if (t->type == LLEX_LITERAL_TOKEN_CALLBACK) {
				return t->data.callback(lex, lex->current_token, lex->current_len);
			}
			
			return t->data.id;
		} else if (t->type == LLEX_REGEX_TOKEN || t->type == LLEX_REGEX_TOKEN_CALLBACK) {
			if (llex_regex_match(lex, t->token.regex, lex->buffer_pos) == 0) {
				if (t->type == LLEX_REGEX_TOKEN_CALLBACK) {
					return t->data.callback(lex, lex->current_token, lex->current_len);
				}
				return t->data.id;
			}
		}
	}
	lex->current_token = lex->buffer_pos;

	return -1;
}

llex_token_id llex_tokenizer(llex *lex)
{	
	return _llex_find_next_token(lex);
}
