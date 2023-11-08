CC=gcc
CFLAGS=-Wall -Werror -Wextra

all : s21_cat
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

check: 
	clang-format -n *.c *.h

style:
	clang-format -i *.c *.h

test:
	bash test_cat.sh

clean:
	rm -rf s21_cat