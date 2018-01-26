# Projekt

![image](https://travis-ci.org/TimNo1/Projekt.svg?branch=minimizer)

C++ implementation of the tool for computing overlaps of long noisy genomic reads made by third generation sequencing tools.

## Dependencies

### Linux

1. gcc 4.8+
2. cmake 3.2+

## Usage

The program is run from the command line with two arguments, of which the second one is optional. First argument denotes the path to the .fasta file which will be processed, while the second one is used to choose the mode in which the program will be run. The second argument, if present, denotes the number of threads that the program will use. Otherwise the program will be run with the optimal number of threads for the computer it is run on. Multithreading is done with the help of https://github.com/rvaser/thread_pool.

## Description

The algorithm for finding the reads that overlap is based on finding minimizers in the reads, and uses an implementation of the lis algorithm (https://en.wikipedia.org/wiki/Longest_increasing_subsequence) to align them. Our algorithm is inspired by minimap and minimap2 (https://github.com/lh3/minimap2).