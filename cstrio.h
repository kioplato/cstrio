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

// Tokenizes the string str to it's words.
// Returns the array of words. Each word has it's own memory.
// String str and returned words do not have overlapping memory like strtok does.
// Allocated memory is tightly allocated.
// TODO: Change strwords to take a string delims like strtok does.
char **strwords(const char *src, size_t *words_amnt) {
	assert(words_amnt != NULL);
	assert(src != NULL);
	char *str = remove_whitespace(src);
	char **words = NULL;  // Where the words will be stored.
	size_t c_word = 0;  // Current word index.
	size_t n_words = 2;  // Starting with space for two words.
	words = (char **)malloc(sizeof(char *) * n_words);
	assert(words != NULL);
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

#endif  // #ifndef FOPS_H
