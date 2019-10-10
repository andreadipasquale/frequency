/*
 * Copyright (C) 2019 Andrea Di Pasquale <spikey.it@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR OR HIS RELATIVES BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF MIND, USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>

#include "globals.h"
#include "minheap.h"

/* Maximum string format length corresponding to the maximum size of buffer */
#define MAX_FORMAT_SIZE 32

/* Maximum buffer length corresponding to the maximum size of word. */
#define MAX_BUFFER_SIZE MAX_WORD_SIZE

static void file_parse_buffer(const char *buffer);

/**
 * The buffer processor which processes the specified buffer content.
 *
 * @param buffer    buffer of characters
 */
static void
file_parse_buffer(const char *buffer)
{
    char word[MAX_WORD_SIZE] = {0};
    unsigned int j = 0;

    assert(buffer != NULL);
    /**
     * Parse the buffer's character one by one in order to find and store
     * one or multiple words inside the trie and minheap data structures.
     */
    for (unsigned int i = 0; i < strlen(buffer); i++) {
        /* Lower alphabetic character, store it. */
        if (islower(buffer[i])) {
            word[j++] = buffer[i];
        /* Upper alphabetic character, convert and store it. */
        } else if (isupper(buffer[i])) {
            word[j++] = tolower(buffer[i]);
        /* No alphabetic character. */
        } else {
            /**
             * Store the alphabetic word (if available) inside the trie and
             * minheap data structures. After that, continues to find a new
             * word as other one or multiple words could be available.
             */
            if (j) {
                minheap_push(word);
                memset(word, '\0', MAX_WORD_SIZE);
                j = 0;
            }
        }
    }
    /**
     * Store the alphabetic word (if available) inside the trie and minheap
     * data structures. No other words are available in the buffer.
     */
    if (j) {
        minheap_push(word);
    }
}

/**
 * The file processor which processes the specified file content.
 *
 * @param file_name     file name
 */
void
file_process(const char *file_name)
{
    char format[MAX_FORMAT_SIZE];
    char buffer[MAX_BUFFER_SIZE];
    FILE *file = NULL;

    assert(file_name != NULL);
    /* Open the file. */
    if ((file = fopen(file_name, "r")) == NULL) {
        ERROR("%s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    /* Read the buffer one by one from file and process every single word. */
    snprintf(format, sizeof(format), "%%%ds", (MAX_BUFFER_SIZE - 1));
    while (fscanf(file, format, buffer) != EOF) {
        file_parse_buffer(buffer);
    }
    /* Close the file. */
    if (fclose(file) == EOF) {
        ERROR("%s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    /**
     * Print to the standard output, the 20 most frequently used
     * words in the file in order, along with their frequency.
     */
    minheap_popall_reverse(0);
}
