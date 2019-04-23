// File: cstrio.h

#ifndef CSTRIO_H
#define CSTRIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Tokenizes the string str to it's words.
// Returns the array of words. Each word has it's own memory.
// String str and returned words do not have overlapping memory like strtok does.
// Allocated memory is tightly allocated.
// TODO: Change strwords to take a string delims like strtok does.
char **strwords(const char *src, size_t *words_amnt) {
	char *str = remove_whitespace(src);
	char **words = NULL;  // Where the words will be stored.
	size_t c_word = 0;  // Current word index.
	size_t n_words = 2;  // Starting with space for two words.
	words = (char **)malloc(sizeof(char *) * n_words);
	if (words == NULL) {
		perror("Error allocating memory for words");
		return NULL;
	}
	char *next_char = str;  // The next character to copy.
	size_t c_char = 0;  // The next character index in word.
	size_t word_size = 4;  // Starting with space for two characters.
	while ((*next_char) != '\0') {
		if ((*next_char) == ' ') {
			// Reallocate string memory for tight memory management.
			words[c_word] = (char *)realloc(words[c_word], sizeof(char) * c_char + 1);
			words[c_word][c_char] = '\0';  // Close off the word with the NULL character.
			c_word++;
			c_char = 0;
			word_size = 4;  // Reset the word_size to 4. Previous word could've increased it.
			next_char++;  // Ignore the space.
			if (c_word == n_words) {
				n_words *= 2;
				words = (char **)realloc(words, sizeof(char *) * n_words);
			}
			continue;  // Next character might be the NULL character.
		}
		if (c_char == 0)
			words[c_word] = (char *)malloc(sizeof(char) * word_size);
		if (c_char == word_size) {
			word_size *= 2;
			words[c_word] = (char *)realloc(words[c_word], sizeof(char) * word_size);
		}
		words[c_word][c_char++] = *next_char++;
	}
	free(str);
	words[c_word] = (char *)realloc(words[c_word], sizeof(char) * c_char + 1);
	words[c_word][c_char] = '\0';
	// Reallocate excess string pointers (char *) for tight memory management.
	c_word++;
	words = (char **)realloc(words, sizeof(char *) * c_word);
	*words_amnt = c_word;
	return words;
}

// Returns for each line in the file filename the respective words.
// filename is the path to the file.
// n_lines is the amount of lines stored in returned array.
// n_words is the amount of words per line.
char ***fetch_words_lines(char *filename, size_t *n_lines, size_t **n_words) {
	FILE *blnc_f = NULL;
	blnc_f = fopen(filename, "r");
	if (blnc_f == NULL) {
		perror("Error opening balances file");
		return NULL;
	}
	size_t c_line = 0;
	*n_lines = 16;
	char ***lines = NULL;
	lines = (char ***)malloc(sizeof(char **) * (*n_lines));
	*n_words = (size_t *)malloc(sizeof(size_t) * (*n_lines));
	char *line = NULL;
	size_t line_len = 0;
	while (fetch_line(&line, &line_len, blnc_f) != -1) {
		if (line_len == 0) continue;
		if (c_line == (*n_lines)) {
			(*n_lines) *= 2;
			lines = (char ***)realloc(lines, sizeof(char **) * (*n_lines));
		}
		if ((lines[c_line] = strwords(line, &((*n_words)[c_line]))) != NULL)
			c_line++;
		if (line != NULL) free(line);
	}
	if (c_line == 0) return NULL;
	lines = (char ***)realloc(lines, sizeof(char **) * c_line);
	*n_lines = c_line;
	fclose(blnc_f);
	return lines;
}

// Prints the words for each line at stdout seperated with newline.
// lines is the words per line, for n_lines lines.
// n_lines is the amount of lines in lines array.
// n_words is the amount of words per line.
void print_words_lines(char ***lines, size_t n_lines, size_t *n_words) {
	for (size_t c_line = 0; c_line < n_lines; ++c_line) {
		for (size_t c_word = 0; c_word < n_words[c_line]; ++c_word) {
			printf("%s", lines[c_line][c_word]);
			if (c_word < n_words[c_line] - 1)
				printf(" ");
		}
		printf("\n");
	}
}

void free_words_lines(char ***lines, size_t n_lines, size_t *n_words, int free_words) {
	for (size_t c_line = 0; c_line < n_lines; c_line++) {
		if (free_words == 1) {
			for (size_t c_word = 0; c_word < n_words[c_line]; c_word++)
				free(lines[c_line][c_word]);
		}
		free(lines[c_line]);
	}
	free(lines);
	free(n_words);
}

// TODO: Implement write_words_lines() function which writes lines of words to a file.

#endif  // #ifndef CSTRIO_H
