CC=gcc 
CFLAGS=-Wall -Werror -Wextra -std=c11


all : s21_grep
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep

check_style: 
	clang-format -style=google -n *.c *.h

style:
	clang-format -style=google -i *.c *.h

test:
	
	
clean:
	rm -rf s21_grep s21_cat