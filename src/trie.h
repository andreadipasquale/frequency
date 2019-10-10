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

#ifndef __TRIE_H__
#define __TRIE_H__

/* Maximum count of English alphabet characters. */
#define MAX_ALPHABET_SIZE   26

/**
 * Trie node data structure.
 */
struct trie_node {
    /* Node's children corresponding recursively to the word. */
    struct trie_node *children[MAX_ALPHABET_SIZE];

    /* Node's count of occurrences of corresponding word. */
    unsigned int occurrences;

    /**
     * Node's index corresponding to the word's
     * index inside the minheap data structure.
     */
    int minheap_index;
};

/**
 * Deallocate the whole root's tree.
 */
extern void trie_clean(void);

/**
 * Insert/Update a word (with its count of occurrences) inside the root's tree.
 *
 * @param word  word of alphabetic characters
 * @return      new trie node data structure corresponding to last word's node
 */
extern struct trie_node *trie_insert(const char *word);

#endif  /* __TRIE_H__ */
