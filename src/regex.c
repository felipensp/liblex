/*
 * Regex support
 *
 * Author: Felipe Pena <felipensp at gmail.com> 
 */
 
#include "liblex.h"
#include "regex.h"

pcre *llex_regex_compile(char *regex)
{
	int erroroffset;
	const char *error;
	
	return pcre_compile((char*)regex, 
		PCRE_MULTILINE|PCRE_ANCHORED|PCRE_DOTALL|PCRE_NEWLINE_ANYCRLF|PCRE_ANCHORED|PCRE_NO_AUTO_CAPTURE,
		&error,
		&erroroffset,
		NULL);
}

int llex_regex_match(llex *lex, pcre *regex, char *buffer)
{
	int offsetcount;
	int offsets[1];
	const char *result;
  
	offsetcount = pcre_exec(regex, 
		NULL, 
		buffer,
		strlen(buffer),
		0,
		0,
		offsets,
		3);

	if (offsetcount <= 0) return 1;
	
	pcre_get_substring(buffer, offsets, offsetcount, 0, &result);

	lex->current_token = lex->buffer_pos;
	lex->current_len = strlen(result);
	lex->buffer_pos += strlen(result);
	
	pcre_free_substring(result);
		
	return 0;
}
