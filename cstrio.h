// File: cstrio.h

#ifndef FOPS_H
#define FOPS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Allocates memory for a line for exactly one line up to newline or EOF.
// Returns # of characters read.
int fetch_line(char **line, size_t *line_len, FILE *strm) {
	size_t line_size = 32;
	*line = (char *)malloc(sizeof(char) * line_size);
	*line_len = 0;
	char c;
	while ((c = fgetc(strm)) != '\n') {
		if (c == EOF) break;
		if (*line_len == line_size) {
			line_size *= 2;
			*line = (char *)realloc(*line, sizeof(char) * line_size);
		}
		(*line)[(*line_len)] = c;
		(*line_len)++;
	}
	if (*line_len == 0) {
		free(*line);
		*line = NULL;
		if (c == EOF)
			return -1;
	} else {
		//(*line_len)++;  // Space for the NULL byte.
		*line = (char *)realloc(*line, sizeof(char) * (*line_len) + 1);
		(*line)[(*line_len)] = '\0';
	}
	return (*line_len);
}

// Removes spaces from string str.
// Returns the new string. Returned string has it's own memory.
// Returned string is NULL terminated.
// Original string str remains unchanged.
// Original string must be NULL terminated.
char *remove_whitespace(const char *src) {
	assert(src != NULL);
	size_t dst_size = strlen(src);  // dst string will have at most the same # of chars as src.
	char *dst = (char *)malloc(sizeof(char) * dst_size);
	size_t c_char = 0;
	const char *src_char = src;
	while ((*src_char) != '\0') {
		dst[c_char] = *src_char++;
		if (dst[c_char] != ' ' || (c_char > 0 && dst[c_char - 1] != ' '))
			c_char++;
	}
	if (dst[c_char - 1] == ' ') {
		dst[c_char - 1] = '\0';
	} else {
		dst = (char *)realloc(dst, sizeof(char) * (c_char + 1));
		dst[c_char] = '\0';
	}
	return dst;
}

#endif  // #ifndef FOPS_H
