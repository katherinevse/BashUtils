CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11

all : s21_cat
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

check: 
	clang-format -style=google -n *.c *.h

style:
	clang-format -style=google -i *.c *.h

test:
	cd cat_tests && sh test_func_cat.sh

clean:
	rm -rf s21_cat