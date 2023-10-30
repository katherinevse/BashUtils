WWW = -Werror -Wextra -Wall
G = gcc

all : s21_grep
	$(G) $(WWW) test_grep.c -o s21_grep

check: 
	clang-format -style=google -n *.c *.h

style:
	clang-format -style=google -i *.c *.h

clean:
	rm -rf s21_grep s21_cat