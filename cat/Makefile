SHELL := /bin/bash
FLAG = -Wall -Wextra -Werror
FILE = cat_test.txt
CHECK=2>&1 valgrind --tool=memcheck --leack-check=yes

all: s21_cat
s21_cat :clean
	gcc $(FLAG) s21_cat.c -o s21_cat

clean:
	rm -rf s21_cat *.o
