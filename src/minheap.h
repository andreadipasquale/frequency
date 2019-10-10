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

#ifndef __MINHEAP_H__
#define __MINHEAP_H__

/* Word's out of index inside the minheap data structure. */
#define MINHEAP_OUTINDEX    -1

/**
 * Push/Update a word inside the trie's tree and minheap's tree.
 *
 * @param word  word of alphabetic characters
 */
extern void minheap_push(const char *word);

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
extern void minheap_popall_reverse(unsigned int count);

#endif  /* __MINHEAP_H__ */
