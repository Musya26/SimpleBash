#!/bin/bash

DIFF_RES=""
COUNTER_SUCCESS=0
COUNTER_FAIL=0
NUMBER=1

GREEN='\033[32m'
RED='\033[31m'
NORMAL='\033[0m'
echo -e $GREEN "TESTS for GREP"

for var1 in -i -v -c -l -h -n -s -o
do
    for var2 in -i -v -c -l -h -n -s -o
    do
        for var3 in -i -v -c -l -h -n -s -o
        do
            TEST0="-e while $var1 $var2 $var3 Makefile s21_grep.h s21_grep.c -e handler"
            ./s21_grep $TEST0 > s21_grep.txt
            # leaks -atExit -- ./s21_grep $TEST0 >> RESULT_VALGRIND.txt
            grep $TEST0 > grep.txt
            DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
            if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
                then
                    echo -e $NORMAL TEST $NUMBER "$TEST0" $GREEN "\tSUCCESS"
                    (( COUNTER_SUCCESS++ ))
                else
                    echo -e $NORMAL TEST $NUMBER "$TEST0" $RED "\tFAIL"
                    echo "--------0---------" >> log.txt
                    echo "$TEST0" >> log.txt
                    echo "------------------" >> log.txt
                    (( COUNTER_FAIL++ ))
                fi
                ((NUMBER++))
            rm s21_grep.txt grep.txt
        done
    done
done

for var5 in -i -c -l -n -h -s -o -v
do  
    for var6 in i c l n h s o v
    do
            TEST0="$var5$var6 handler Makefile s21_grep.c s21_grep.h"
            echo >> RESULT_VALGRIND.txt
            echo >> RESULT_VALGRIND.txt
            # leaks -atExit -- ./s21_grep $TEST0 >> RESULT_VALGRIND.txt
            ./s21_grep $TEST0 > s21_grep.txt
            grep $TEST0 > grep.txt
            DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
            if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
                echo -e $NORMAL TEST $NUMBER "$TEST0" $GREEN "\tSUCCESS"
                (( COUNTER_SUCCESS++ ))
            else
                echo -e $NORMAL TEST $NUMBER "$TEST0" $RED "\tFAIL"
                echo "--------0---------" >> log.txt
                echo "$TEST0" >> log.txt
                echo "------------------" >> log.txt
                (( COUNTER_FAIL++ ))
            fi
            ((NUMBER++))
            rm s21_grep.txt grep.txt
    done
done

echo -e $GREEN SUCCESS $COUNTER_SUCCESS
echo -e $RED FAIL $COUNTER_FAIL
