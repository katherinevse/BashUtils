#include "s21_grep.h"

#include <string.h>

int main(int argc, char *argv[]) {
    regex_t regex;
    (void)argv;
    if (argc == 1) {
        fprintf(stderr,
                "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
                "[-C[num]]\n");
        fprintf(stderr, "\t[-e pattern] [-f file] [--binary-files=value] [--color=when]\n");
        fprintf(stderr,
                "\t[--context[=num]] [--directories=action] [--label] "
                "[--line-buffered]\n");
        fprintf(stderr, "\t[--null] [pattern] [file ...]\n");

    } else if (argc == 2) {
        char cur = getchar();
        while (cur != EOF) {
            cur = getchar();
        }

    } else if ((argc >= 3)) {
        parser(argc, argv, &options, &regex);
    }
    return 0;
}

void parser(int argc, char *argv[], opt *options, regex_t *regex) {
    int flag = 0;
    int reg_flag = 0;
    char pattern[1000] = {0};
    while ((flag = getopt_long(argc, argv, "e:i:v:c:l:n:s:o:h:", NULL, NULL)) != (-1)) {
        switch (flag) {
            case 'i':
                options->i = 1;
                flags(pattern, reg_flag, options);
                break;
            case 'v':
                options->v = 1;
                flags(pattern, reg_flag, options);
                break;
            case 'c':
                options->c = 1;
                flags(pattern, reg_flag, options);
                break;
            case 'l':
                options->l = 1;
                flags(pattern, reg_flag, options);
                break;
            case 'n':
                options->n = 1;
                flags(pattern, reg_flag, options);
                break;
            case 'h':
                options->h = 1;
                flags(pattern, reg_flag, options);
                break;
            case 's':
                options->s = 1;
                flags(pattern, reg_flag, options);
                break;
            case 'e':
                options->e = 1;
                flags(pattern, reg_flag, options);
                break;
        }
    }
    regcomp(regex, pattern, reg_flag);
    read_file(argc, argv, options, regex);
}

void flags(char *pattern, int reg_flag, opt *options) {
    strcat(pattern, optarg);
    strcat(pattern, "|");
    if (reg_flag == 0 && options->e) {
        reg_flag = REG_EXTENDED;  // 1
    } else if (reg_flag == 0 && options->i) {
        reg_flag = REG_ICASE;
    } else {
        reg_flag = REG_EXTENDED;
    }
    pattern[strlen(pattern) - 1] = '\0';
}

void read_file(int argc, char *argv[], opt *options, regex_t *regex) {
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    int read = 0;
    int search_reg = 0;
    int str_count = 0;
    int str_count_c = 0;
    int num_files = argc - optind;

    while (optind < argc) {
        file = fopen(argv[optind], "r");
        if (file) {
            int over = 0;
            while ((read = getline(&line, &len, file)) != EOF) {
                search_reg = regexec(regex, line, 0, NULL, 0);
                str_count++;

                if ((options->v && search_reg == REG_NOMATCH) ||
                    (search_reg == 0 && (options->v == 0 || options->e))) {
                    if (num_files > 1 && options->h == 0 && options->c == 0 && options->l == 0) {
                        printf("%s:", argv[optind]);
                    }
                    if (options->n && options->c == 0 && options->l == 0) {
                        printf("%d:%s", str_count, line);
                        if (line[strlen(line) - 1] != '\n') {
                            printf("\n");
                        }
                    } else if (options->c) {
                        str_count_c++;
                    } else if (options->l && over == 0) {
                        printf("%s\n", argv[optind]);
                        over++;
                    } else if (options->l == 0) {
                        printf("%s", line);
                        if (line[strlen(line) - 1] != '\n') {
                            printf("\n");
                        }
                    }
                }
            }
            if (options->c) {
                if (num_files > 1 && options->h == 0) {
                    printf("%s:%d\n", argv[optind], str_count_c);
                } else {
                    printf("%d\n", str_count_c);
                }
                str_count_c = 0;  // matching lines for next file
            }
            if (options->n) {
                str_count = 0;
            }
        } else {
            if (options->s == 0) {
                fprintf(stderr, "s21_grep: %s: No such file or directory\n", argv[optind]);
            }
        }
        optind++;
    }
    fclose(file);
    exit(0);
    free(line);
    regfree(regex);
}
