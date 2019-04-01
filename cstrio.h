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

#endif  // #ifndef FOPS_H
