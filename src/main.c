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
#include <unistd.h>

#include "file.h"
#include "globals.h"

static void help(const char *name);

/**
 * Print the help summary page corresponding to the program name.
 *
 * @param program_name  program name
 */
static void
help(const char *program_name)
{

    INFO("Usage for %s:", program_name);
    INFO("  -f filename    file to process");
    INFO("  -h             print this help");
}

/**
 * The main processor corresponding to the program.
 *
 * The main processor parse every argument from command line and after that,
 * process the given file in order to print the 20 most frequently used words
 * in the file in order, along with their frequency.
 *
 * The file processor reads item of data one by one from file, storing it one
 * by one inside a buffer. For every buffer which has been read from the file,
 * the buffer processor parses the buffer in order to find one or multiple
 * words. For every word which has been parsed in the file, the buffer processor
 * stores the word and the corresponding count of occurrences inside the trie
 * data structure; then stores the word inside the minheap data structures, in
 * order to:
 *
 * 1) keep in the memory the words and the corresponding count of occurrences
 *    (frequency) inside the trie data structure;
 * 2) keep in the memory the 20 most frequently used words in order, along with
 *    their occurrences (frequency) inside the minheap data structure.
 *
 * Of course this is possible thanks to a strong collaboration between the trie
 * and minheap data structures, which makes everything performant in terms of
 * used memory and needed computations.
 *
 * Finally, once that the file processor and the buffer processor have completed
 * the reading, parsing, ordering and storing of all words from the file to the
 * trie and minheap data structures, the file processor prints to the standard
 * output, the 20 most frequently used words in the file in order, along with
 * their frequency.
 *
 * @param argc  count of command line arguments
 * @param argv  vector of command line arguments
 * @return      0   on success
 *              -1  on failure
 */
int
main(int argc, char **argv)
{
    int opt;

    /* Parse the arguments from command line. */
    while ((opt = getopt(argc, argv, "f:h")) != -1) {
        switch (opt) {
            /* Process the specified file. */
            case 'f':
                file_process(optarg);
                return 0;
            /* Print the help summary page. */
            case 'h':
                break;
            case '?':
            default:
                help(argv[0]);
                return -1;
        }
    }
    help(argv[0]);
    return 0;
}
