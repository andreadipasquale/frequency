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

/* Parent node. */
#define PARENT(i)           (i - 1) / 2

/* Parent's left child node. */
#define LEFT_CHILD(i)       (2 * i) + 1

/* Parent's right child node. */
#define RIGHT_CHILD(i)      (2 * i) + 2

/* Minheap node data structure size. */
#define MINHEAP_NODE_SIZE   sizeof(struct minheap_node)

/* Minheap data structure size. */
#define MINHEAP_SIZE        sizeof(struct minheap)

/**
 * Minheap node data structure.
 */
struct minheap_node {
    /* Word corresponding to the word in the trie's tree. */
    char word[MAX_WORD_SIZE];

    /**
     * Trie node data structure corresponding to
     * the last word's node in the trie's tree.
     */
    struct trie_node *trie_node_index;
};

/**
 * Minheap data structure.
 */
struct minheap {
    /* Array of node's children corresponding to the words. */
    struct minheap_node array[MINHEAP_CAPACITY];

    /* Array's counter of full slots compare to the maximum capacity. */
    unsigned int count;
};

static void minheap_init(void);
static void minheap_clean(void);
static void minheap_swap_node(struct minheap_node *a, struct minheap_node *b);
static void minheap_heapify_down(unsigned int i);
static void minheap_heapify_up(unsigned int i);
static int minheap_pop(struct minheap_node *minimum);

/* Minheap's tree. */
static struct minheap *minheap = NULL;

/**
 * Allocate and initialize the new minheap data structure.
 */
static void
minheap_init(void)
{

    /* Allocate the new minheap data structure inside the heap memory. */
    if ((minheap = (struct minheap *)malloc(MINHEAP_SIZE)) == NULL) {
        ERROR("%s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    /* Initialize the array of node's minheap node data structure. */
    for (unsigned int i = 0; i < MINHEAP_CAPACITY; i++) {
        memset(minheap->array[i].word, '\0', MAX_WORD_SIZE);
        minheap->array[i].trie_node_index = NULL;
    }
    /* Initialize the array's counter to empty. */
    minheap->count = 0;
}

/**
 * Deallocate the minheap's tree (the whole array of nodes tree).
 */
static void
minheap_clean(void)
{

    assert(minheap != NULL);
    /* Deallocate the minheap data structure from heap memory. */
    free(minheap);
    minheap = NULL;
}

/**
 * Swap a minheap node data structure with another one.
 *
 * @param a     first minheap node data structure
 * @param b     second minheap node data structure
 */
static void
minheap_swap_node(struct minheap_node *a, struct minheap_node *b)
{
    struct minheap_node temp;

    assert(a != NULL);
    assert(b != NULL);
    /**
     * Use a temporary node data structure in order
     * to swap the first one with the second one.
     */
    memcpy(&temp, a, MINHEAP_NODE_SIZE);
    memcpy(a, b, MINHEAP_NODE_SIZE);
    memcpy(b, &temp, MINHEAP_NODE_SIZE);
}

/**
 * Heapify down procedure.
 *
 * Heapify down is used when we update an element present in the heap; when we
 * replace the top element with a new one (for example when the minheap capacity
 * is full) or when we remove the top element from the heap and replace it with
 * the last element.
 *
 * In every operation, update or replace or remove of an element is done by
 * swapping the element itself with the smallest element at the bottom of its
 * tree, and then heapfying the new largest element down to maintain the heap
 * property.
 *
 * Because this moves down the heap tree, it must perform two comparisons per
 * iteration, with the left child and the right child elements, then swap with
 * the smaller one.
 *
 * @param i     Parent node's index
 */
static void
minheap_heapify_down(unsigned int i)
{
    unsigned int s, l, r;

    assert(i < MINHEAP_CAPACITY);
    assert(minheap != NULL);
    /* Smallest node's index = Parent node's index. */
    s = i;
    /* Parent's left child node's index. */
    l = LEFT_CHILD(i);
    /* Parent's right child node's index. */
    r = RIGHT_CHILD(i);

    /* Find the smallest between parent node and left child node. */
    assert(minheap->array[s].trie_node_index != NULL);
    if (l < minheap->count) {
        /**
         * To determine the smallest one, we check the counts of occurrences
         * corresponding to the words of left child node and parent node. Or
         * else, for same counts of occurrences, we check the alphabetic order
         * corresponding to the words of left child node and parent node.
         */
        assert(minheap->array[l].trie_node_index != NULL);
        if (minheap->array[l].trie_node_index->occurrences <
            minheap->array[s].trie_node_index->occurrences ||
            (minheap->array[l].trie_node_index->occurrences ==
             minheap->array[s].trie_node_index->occurrences &&
             strcmp(minheap->array[l].word, minheap->array[s].word) < 0)) {
            /* Set the smallest one the left child. */
            s = l;
        }
    }
    /* Find the smallest between parent/left child node and right child node. */
    if (r < minheap->count) {
        /*
         * To determine the smallest one, we check the counts of occurrences
         * corresponding to the words of right child node and parent node. Or
         * else, for same counts of occurrences, we check the alphabetic order
         * corresponding to the words of right child node and parent node.
         */
        assert(minheap->array[r].trie_node_index != NULL);
        if (minheap->array[r].trie_node_index->occurrences <
            minheap->array[s].trie_node_index->occurrences ||
            (minheap->array[r].trie_node_index->occurrences ==
             minheap->array[s].trie_node_index->occurrences &&
             strcmp(minheap->array[r].word, minheap->array[s].word) < 0)) {
            /* Set the smallest one the right child. */
            s = r;
        }
    }
    /* If the smallest one is not the parent node. */
    if (s != i) {
        /**
         * Swap the parent node's index (in the minheap) with the smallest
         * child node's index (in the minheap) in the trie data structure.
         */
        minheap->array[i].trie_node_index->minheap_index = s;
        minheap->array[s].trie_node_index->minheap_index = i;
        /* Swap the parent node with the smallest node in the minheap. */
        minheap_swap_node(&minheap->array[i], &minheap->array[s]);
        /* Heapify down the smallest node. */
        minheap_heapify_down(s);
    }
}

/**
 * Heapify up procedure.
 *
 * Heapify up is used when we insert a new element in the heap at the bottom of
 * the heap tree (for example when the minheap capacity is not full).
 *
 * In the operation, insert an element is done by swapping the element itself
 * with the parent element at the up of its tree, move up the smallest, and then
 * heapying the new smallest element up to maintain the heap property.
 *
 * Because this moves up the heap tree, it must perform only one comparison per
 * iteration, with the parent element, then swap the smaller one.
 *
 * param i      Parent node's index
 */
static void
minheap_heapify_up(unsigned int i)
{
    unsigned int p;

    assert(i < MINHEAP_CAPACITY);
    assert(minheap != NULL);
    /* Parent node's index. */
    p = PARENT(i);

    /* Find the smallest between parent node and child node. */
    if (i > 0) {
        /**
         * To determine the smallest one, we check the counts of occurrences
         * corresponding to the words of parent node and child node. Or else,
         * for same counts of occurrences, we check the alphabetic order
         * corresponding to the words of parent node and child node.
         */
        assert(minheap->array[i].trie_node_index != NULL);
        assert(minheap->array[p].trie_node_index != NULL);
        if (minheap->array[p].trie_node_index->occurrences >
            minheap->array[i].trie_node_index->occurrences ||
            (minheap->array[p].trie_node_index->occurrences ==
             minheap->array[i].trie_node_index->occurrences &&
             strcmp(minheap->array[p].word, minheap->array[i].word) > 0)) {
            /**
             * Swap the parent node's index (in the minheap) with the smallest
             * child node's index (in the minheap) in the trie data structure.
             */
            minheap->array[i].trie_node_index->minheap_index = p;
            minheap->array[p].trie_node_index->minheap_index = i;
            /* Swap the parent node with the smallest node in the minheap. */
            minheap_swap_node(&minheap->array[i], &minheap->array[p]);
            /* Heapify up the smallest node. */
            minheap_heapify_up(p);
        }
    }
}

/**
 *
 * Push/Update a word inside the trie's tree and minheap's tree.
 *
 * @param word  word of alphabetic characters
 */
void
minheap_push(const char *word)
{
    struct trie_node *t = NULL;

    assert(word != NULL);
    /* First call, allocate and initialize the minheap data structure. */
    if (minheap == NULL) {
        minheap_init();
    }
    /**
     * Insert/Update a word (with its count of occurrences) inside the root's
     * tree. If the operation is an update, it increments the counts of
     * occurrences of the word.
     */
    t = trie_insert(word);
    /**
     * If the word is present inside the minheap and its count of
     * occurrences has been incremented by one, then update minheap.
     */
    if (t->minheap_index > MINHEAP_OUTINDEX) {
        /* Get the word node's index from the trie data structure. */
        unsigned int i = t->minheap_index;

        /* Therefore after an update, heapify down the node. */
        minheap_heapify_down(i);
    /**
     * Else if the word is not present inside the minheap and the minheap
     * capacity is not full, then insert it in the minheap.
     */
    } else if (minheap->count < MINHEAP_CAPACITY) {
        /* Get the new word node's index from the last one available. */
        unsigned int c = minheap->count;

        /**
         * Update the corresponding word minheap node's index
         * data structure in the trie data structure.
         */
        t->minheap_index = c;
        /**
         * Set the node (last index available in the minheap) with the
         * new word and update the corresponding word's trie node data
         * structure in the minheap data structure.
         */
        memset(minheap->array[c].word, '\0', MAX_WORD_SIZE);
        strncpy(minheap->array[c].word, word, strlen(word));
        minheap->array[c].trie_node_index = t;
        /* Update the minheap array's counter. */
        minheap->count++;
        /* Therefore after an insert, heapify up the node. */
        minheap_heapify_up(c);
    /**
     * Else if the word is not present inside the minheap and the minheap
     * capacity is full, then replace it with the root node in the minheap
     * only if the new word's node is larger than the root node.
     */
    } else {
        /* Get the new word node's index from the root one. */
        unsigned int r = 0;

        /**
         * To determine the largest one, we check the counts of occurrences
         * corresponding to the words of word's node and root node. Or else,
         * for same counts of occurrences, we check the alphabetic order
         * corresponding to the words of left child node and parent node.
         */
        assert(minheap->array[r].trie_node_index != NULL);
        if (minheap->array[r].trie_node_index->occurrences < t->occurrences ||
            (minheap->array[r].trie_node_index->occurrences == t->occurrences &&
             strcmp(minheap->array[r].word, word) < 0)) {
            /**
             * The current root node will be replaced by the new word's node,
             * then update the corresponding word's trie node data structure.
             */
            minheap->array[r].trie_node_index->minheap_index = MINHEAP_OUTINDEX;
            /**
             * Update the corresponding word minheap node's index
             * data structure in the trie data structure.
             */
            t->minheap_index = r;
            /**
             * Set the node (root index now available in the minheap) with the
             * new word and update the corresponding word's trie node data
             * structure in the minheap data structure.
             */
            memset(minheap->array[r].word, '\0', MAX_WORD_SIZE);
            strncpy(minheap->array[r].word, word, strlen(word));
            minheap->array[r].trie_node_index = t;
            /* Therefore after a replace, heapify down the node. */
            minheap_heapify_down(r);
        }
    }
}

/**
 * Pop the root node from minheap's tree and update it inside the trie's tree.
 *
 * @param minimum   popped minheap node data structure
 * @return          0 on success (minheap is not empty)
 *                  -1 on failure (minheap is empty)
 */
static int
minheap_pop(struct minheap_node *minimum)
{
    unsigned int r = 0;

    /* Check if the minheap is not empty. */
    assert(minheap != NULL);
    if (! minheap->count) {
        return -1;
    }
    /* Pop the root node from the minheap data structure. */
    memcpy(minimum, &minheap->array[r], MINHEAP_NODE_SIZE);
    /**
     * The current root node will be replaced by the last one node,
     * then update the corresponding word's trie node data structure.
     */
    assert(minheap->array[r].trie_node_index != NULL);
    minheap->array[r].trie_node_index->minheap_index = MINHEAP_OUTINDEX;
    /* If the minheap has just the root node, remove it from minheap. */
    if (minheap->count == 1) {
        memset(minheap->array[r].word, '\0', MAX_WORD_SIZE);
        minheap->array[r].trie_node_index = NULL;
        /* Update the minheap array's counter. */
        minheap->count--;
    /* Else if the minheap has two or more nodes. */
    } else {
        /* Get the node's index from the last one available in the minheap. */
        unsigned c = minheap->count - 1;

        /**
         * The current last node will placed into the root node, then
         * update the corresponding word's trie node data structure.
         */
        assert(minheap->array[c].trie_node_index != NULL);
        minheap->array[c].trie_node_index->minheap_index = r;
        /**
         * Remove the current root node, then move the last node
         * into the root node and make available the old last node
         * in the minheap data structure.
         */
        memcpy(&minheap->array[r], &minheap->array[c], MINHEAP_NODE_SIZE);
        memset(minheap->array[c].word, '\0', MAX_WORD_SIZE);
        minheap->array[c].trie_node_index = NULL;
        /* Update the minheap array's counter. */
        minheap->count--;
        /* Therefor after a remove, heapify down the node. */
        minheap_heapify_down(r);
    }
    return 0;
}

/**
 * Pop all count words from minheap's tree and remove them from the trie's tree.
 *
 * Pop all count words from minheap's tree starting from root node from
 * the minheap. After that, print all popped nodes present in the minheap
 * in reverse order, that means from the largest one to the smallest one.
 * Finally, deallocate the minheap and trie data structures.
 *
 * @param count     number of words to pop
 */
void
minheap_popall_reverse(unsigned int count)
{

    /**
     * If the minheap is not empty, pop all count
     * words from minheap starting from root node.
     */
    assert(count <= MINHEAP_CAPACITY);
    assert(minheap != NULL);
    if (count < MINHEAP_CAPACITY) {
        struct minheap_node minimum;

        /**
         * Pop the root node from the minheap, save the popped node information
         * like word and its corresponding node in the trie's tree and and pop
         * recursively the rest until minheap is not empty.
         */
        if (! minheap_pop(&minimum)) {
            unsigned int occurrences;

            /**
             * For each pop node, save the count of occurrences of the word
             * in order to have memory of it after removing the corresponding
             * word from the trie data structure.
             */
            assert(minimum.trie_node_index != NULL);
            occurrences = minimum.trie_node_index->occurrences;
            /* Recursively pop all count nodes. */
            minheap_popall_reverse(++count);
            /* Print the count of occurrences of word and the word itself. */
            INFO("  %d %s", occurrences, minimum.word);
        } else {
            /* Minheap is empty, then call the last calling and go back. */
            minheap_popall_reverse(MINHEAP_CAPACITY);
        }
    } else {
        /**
         * The last calling as the minheap is empty, then deallocate
         * the minheap and trie data structures.
         **/
        trie_clean();
        minheap_clean();
    }
}
