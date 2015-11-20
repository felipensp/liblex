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

static void _llex_set_location(llex *lex, char *buffer, size_t len)
{
	lex->buffer_col_start = buffer - lex->current_line_pos;
	lex->buffer_line_start = lex->current_line;

	for (int i = 0; i < len; ++i) {
		if (buffer[i] == '\n') {
			++lex->current_line;
			lex->current_line_pos = buffer + i;
		}
	}

	lex->buffer_col_end = buffer + len - lex->current_line_pos;
	lex->buffer_line_end = lex->current_line;
}

inline static int _llex_find_next_token(llex *lex)
{
	lex->current_token = NULL;
	lex->current_len = 0;

	if (*lex->buffer_pos == '\0') return 0;

	if (lex->options & LLEX_IGNORE_WHITESPACE) {
		char *buffer_pos = lex->buffer_pos;

		if (!_llex_ltrim(lex)) return 0;

		if (buffer_pos != lex->buffer_pos)
			_llex_set_location(lex, buffer_pos, lex->buffer_pos - buffer_pos);
	}

	for (int i = 0; i < lex->token_array[lex->current_state].tokens_count; ++i) {
		const llex_token *t = &lex->token_array[lex->current_state].tokens[i];

		if ((t->type == LLEX_LITERAL_TOKEN || t->type == LLEX_LITERAL_TOKEN_CALLBACK)
			&& lex->buffer_pos[0] == t->token.literal.str[0]
			&& strncmp(lex->buffer_pos, t->token.literal.str, t->token.literal.len) == 0) {

			_llex_set_location(lex, lex->buffer_pos, t->token.literal.len);

			lex->current_token = lex->buffer_pos;
			lex->current_len = t->token.literal.len;
			lex->buffer_pos += t->token.literal.len;

			if (t->type == LLEX_LITERAL_TOKEN_CALLBACK) {
				return t->data.callback(lex, lex->current_token, lex->current_len);
			}

			return t->data.id;
		} else if (t->type == LLEX_REGEX_TOKEN || t->type == LLEX_REGEX_TOKEN_CALLBACK) {
			char *buffer_pos = lex->buffer_pos;

			if (llex_regex_match(lex, t->token.regex, lex->buffer_pos) == 0) {
				if (t->type == LLEX_REGEX_TOKEN_CALLBACK) {
					int r = t->data.callback(lex, lex->current_token, lex->current_len);

					_llex_set_location(lex, buffer_pos, lex->buffer_pos - buffer_pos);
					return r;
				}
				_llex_set_location(lex, buffer_pos, lex->buffer_pos - buffer_pos);
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
