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

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

/*
 * Maximum word length corresponding to the maximum size of words read from
 * file content and stored in both trie and minheap data structures.
 */
#define MAX_WORD_SIZE       50

/**
 * Minheap capacity corresponding to the maximum count of most frequently used
 * words in the file in order, along with their frequency.
 */
#define MINHEAP_CAPACITY    20

/**
 * Produce output to standard error according to the string format.
 *
 * @param fmt   string format
 * @param ...   one or multiple arguments
 */
#define ERROR(fmt, ...) do {                            \
    fprintf(stderr, "ERROR: " fmt "\n", ##__VA_ARGS__); \
} while (0)

/**
 * Produce output to standard output according to the string format.
 *
 * @param fmt   string format
 * @param ...   one or multiple arguments
 **/
#define INFO(fmt, ...) do {                             \
    fprintf(stdout, fmt "\n", ##__VA_ARGS__);           \
} while (0)

#endif  /* __GLOBALS_H__ */
