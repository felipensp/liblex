/*
 * API
 * 
 * Author: Felipe Pena <felipensp at gmail.com>
 */

#include "liblex.h"
#include "regex.h"

int llex_init(llex *lex)
{
	memset(lex, 0, sizeof(llex));
		
	return 1;
}

int llex_set_buffer(llex *lex, char *buffer)
{
	lex->current_line_pos = lex->buffer_pos = lex->buffer = buffer;
	lex->current_line = 1;
	
	return 1;
}

int llex_set_options(llex *lex, unsigned int flags)
{
	lex->options = flags;
	
	return 1;
}

int llex_set_state(llex *lex, unsigned int state)
{
	if (state > LLEX_MAX_STATE) return 0;
	
	lex->current_state = state;
	
	return 1;
}

int llex_cleanup(llex *lex)
{
	for (int i = 0; lex->token_array[i].tokens_count; ++i) {
		for (int j = 0; j < lex->token_array[i].tokens_count; ++j) {
			llex_token *t = &lex->token_array[i].tokens[j];
			if (t->type == LLEX_REGEX_TOKEN) {
				pcre_free(t->token.regex);
			}
		}		
	}
	
	return 1;
}

int llex_add_token(llex *lex, char *token, llex_token_id id)
{
	llex_token *next;
	llex_token_array *current_array = &lex->token_array[lex->current_state];
	
	if (current_array->tokens_count == LLEX_MAX_TOKEN) return 0;
	
	next = &current_array->tokens[current_array->tokens_count];
	
	next->token.literal.str = token;
	next->token.literal.len = strlen(token);
	next->data.id = id;	
	next->type = LLEX_LITERAL_TOKEN;
	
	++current_array->tokens_count;
		
	return 1;
}

int llex_add_token_callback(llex *lex, char *token, llex_callback callback)
{
	llex_token *next;
	llex_token_array *current_array = &lex->token_array[lex->current_state];
	
	if (current_array->tokens_count == LLEX_MAX_TOKEN) return 0;
	
	next = &current_array->tokens[current_array->tokens_count];
	
	next->token.literal.str = token;
	next->token.literal.len = strlen(token);
	next->type = LLEX_LITERAL_TOKEN_CALLBACK;	
	next->data.callback = callback;
	
	++current_array->tokens_count;
		
	return 1;
}

int llex_add_token_regex(llex *lex, char *token, llex_token_id id)
{
	llex_token *next;
	llex_token_array *current_array = &lex->token_array[lex->current_state];
	pcre *regex;
	
	if (current_array->tokens_count == LLEX_MAX_TOKEN) return 0;
	
	next = &current_array->tokens[current_array->tokens_count];
	
	regex = llex_regex_compile(token);
	
	if (regex == NULL) return 0;
	
	next->token.regex = regex;
	next->type = LLEX_REGEX_TOKEN;
	next->data.id = id;
		
	++current_array->tokens_count;
		
	return 1;
}

int llex_add_token_regex_callback(llex *lex, char *token, llex_callback callback)
{
	llex_token *next;
	llex_token_array *current_array = &lex->token_array[lex->current_state];
	pcre *regex;
	
	if (current_array->tokens_count == LLEX_MAX_TOKEN) return 0;
	
	next = &current_array->tokens[current_array->tokens_count];
	
	regex = llex_regex_compile(token);
	
	if (regex == NULL) return 0;
	
	next->token.regex = regex;
	next->type = LLEX_REGEX_TOKEN_CALLBACK;
	next->data.callback = callback;
		
	++current_array->tokens_count;
		
	return 1;
}

