#!/bin/zsh

clang -c main.c -g
clang -c regexengine.c -g
clang main.o regexengine.o -g
