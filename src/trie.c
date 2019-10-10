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
#include <errno.h>
#include <assert.h>

#include "globals.h"
#include "minheap.h"
#include "trie.h"

/* Convert an alphabetic letter to the corresponding integer index. */
#define LETTER_TO_INDEX(c)  ((int)c - (int)'a')

/* Trie node data structure size. */
#define TRIE_NODE_SIZE      sizeof(struct trie_node)

static struct trie_node *trie_new_node(void);
static void trie_del_node(struct trie_node *current);

/* Root's tree. */
static struct trie_node *root = NULL;

/**
 * Allocate and initialize a new trie node data structure.
 *
 * @return  new trie node data structure
 */
static struct trie_node *
trie_new_node(void)
{
    struct trie_node *new = NULL;

    /* Allocate a new trie node data structure inside the heap memory. */
    if ((new = (struct trie_node *)malloc(TRIE_NODE_SIZE)) == NULL) {
        ERROR("%s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    /* Initialize the node's children trie nodes data structure. */
    for (unsigned int i = 0; i < MAX_ALPHABET_SIZE; i++) {
        new->children[i] = NULL;
    }
    /* Initialize the node's count of occurrences of the corresponding word. */
    new->occurrences = 0;
    /**
     * Initialize the node's index corresponding to the
     * word's index inside the minheap data structure.
     */
    new->minheap_index = MINHEAP_OUTINDEX;
    return new;
}

/**
 * Deallocate a trie node data structure (the whole node's tree).
 *
 * @param current   trie node data structure
 */
static void
trie_del_node(struct trie_node *current)
{

    assert(current != NULL);
    /**
     * Deallocate recursively the node's children trie
     * nodes data structure (the whole node's tree).
     */
    for (unsigned int i = 0; i < MAX_ALPHABET_SIZE; i++) {
        if (current->children[i] != NULL) {
            trie_del_node(current->children[i]);
        }
    }
    /* Deallocate the trie node data structure (itself) from heap memory. */
    free(current);
}

/**
 * Deallocate the whole root's tree.
 */
void
trie_clean(void)
{

    assert(root != NULL);
    /* Deallocate recursively the whole root's tree. */
    trie_del_node(root);
    root = NULL;
}

/**
 * Insert/Update a word (with its count of occurrences) inside the root's tree.
 *
 * @param word  word of alphabetic characters
 * @return      new trie node data structure corresponding to last word's node
 */
struct trie_node *
trie_insert(const char *word)
{
    struct trie_node *current = NULL;

    assert(word != NULL);
    /* First call, allocate and initialize the root trie data structure. */
    if (root == NULL) {
        root = trie_new_node();
    }
    /* Set root trie node as current trie node. */
    current = root;
    /**
     * Parse the word's alphabetic character one by one in order to insert
     * the word inside the root's tree. For each alphabetic character, there
     * is a child (son, nephew and so on) trie node data structure).
     */
    for (unsigned int i = 0; i < strlen(word); i++) {
        unsigned int index = LETTER_TO_INDEX(word[i]);

        /**
         * If necessary, allocate and initialize the child
         * (son, nephew and so on) trie node data structure.
         */
        if (current->children[index] == NULL) {
            current->children[index] = trie_new_node();
        }
        /* Update child trie node as current trie node */
        current = current->children[index];
    }
    /**
     * The word has been parsed, therefore the current trie node corresponding
     * to last word's alphabetic character inside the root's tree. Finally,
     * increase the count of occurrences of parsed word.
     */
    current->occurrences++;
    return current;
}
