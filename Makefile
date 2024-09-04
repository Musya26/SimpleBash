GCC=gcc -Wall -Wextra -Werror -std=c11

all: s21_grep

s21_grep:
	$(GCC) $@.c -o $@
check:
	cppcheck --enable=all --suppress=missingIncludeSystem --inconclusive --check-config s21_grep.c
	cp ../../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../../materials/linters/cpplint.py --extensions=c s21_grep.c s21_grep.h
	rm CPPLINT.cfg
	make s21_grep	

rebuild: clean all

clean:
	rm -rf s21_grep *.o log.txt
