INTRODUCTION
============

Given the attached text file as an argument, the program will read the file, and
output the 20 most frequently used words in the file in order, along with their
frequency.

CUSTOMIZE
=========

The program allow you to customize the setting of two most important parameters:

    - word length (default value 50)
    - number of most frequently used words (default value 20)

If you want to customize these settings, follow these guidelines:

1) open the file src/globals.h;
2) To customize word length:
   change the value of MAX WORD SIZE macro;
3) To customize number of most frequently used words:
   change the value of MINHEAP CAPACITY macro;
4) compile and execute.

COMPILE AND TEST
================

1) Compile and run automated tests:

    $ make

2) Compile in debug mode and run automated tests:

    $ make debug

EXECUTE
=======

1) Execute and show help page:

    $ ./main -h

2) Execute and process a file:

    $ ./main -f file

EXAMPLE
=======

1) Help page:

    $ ./main -h
    Usage for ./main:
      -f filename    file to process
      -h             print this help

2) Process mobydick.txt file:

    $ ./main -f files/mobydick.txt
      4284 the
      2192 and
      2185 of
      1861 a
      1685 to
      1366 in
      1056 i
      1024 that
      889 his
      821 it
      783 he
      616 but
      603 was
      595 with
      577 s
      564 is
      551 for
      542 all
      541 as
      458 at
