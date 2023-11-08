CC=gcc 
CFLAGS=-Wall -Werror -Wextra

all : s21_grep
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep

check: 
	clang-format -n *.c *.h

style:
	clang-format -i *.c *.h

clean:
	rm -rf s21_grep s21_cat