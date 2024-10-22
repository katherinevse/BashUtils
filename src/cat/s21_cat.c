#include "s21_cat.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        char ch = getchar();
        while (ch != EOF) {
            printf("%c", ch);
            ch = getchar();
        }
    } else
        parser(argc, argv, &options);
}

void parser(int argc, char *argv[], opt *options) {
    int flag = 0;
    static struct option long_options[] = {
        {"number-nonblank", 0, 0, 'b'}, {"number", 0, 0, 'n'}, {"squeeze-blank", 0, 0, 's'}, {0, 0, 0, 0}};
    while ((flag = getopt_long(argc, argv, "+benstvTE", long_options, 0)) != -1) {
        switch (flag) {
            case 'b':
                options->b = 1;
                break;
            case 'e':
                options->e = 1;
                options->v = 1;
                break;
            case 'n':
                options->n = 1;
                break;
            case 's':
                options->s = 1;
                break;
            case 't':
                options->t = 1;
                options->v = 1;
                break;
            case 'v':
                options->v = 1;
                break;
            case 'T':
                options->t = 1;
                break;
            case 'E':
                options->e = 1;
                break;
            default:
                fprintf(stderr, "usage: cat [-benstuv] [file ...]\n");
        }
    }
    read_file(argc, argv, options);
}

void read_file(int argc, char *argv[], opt *options) {
    int cur = 0;
    while (optind < argc) {
        print_options(cur, argv, options);
    }
}

void print_options(int cur, char *argv[], opt *options) {
    FILE *file = fopen(argv[optind], "r");
    if (file) {
        int str_count = 1;
        int counter = 1;  // empty 2 b n
        while ((cur = fgetc(file)) != EOF) {
            if (cur == '\n' && counter > 1 && options->s) {
                continue;
            }
            if (options->b && counter && cur != '\n') {
                printf("%6d\t", str_count++);
                counter = 0;
            }
            if ((options->n && counter) && !(options->b)) {
                printf("%6d\t", str_count++);
            }
            if (options->e && cur == '\n') {
                printf("$");
            }
            if (options->t && cur == '\t') {
                printf("^I");
                counter = 0;
                continue;
            }
            if (options->v) {
                if ((cur < 9 || cur > 10) && cur < 32) {
                    printf("%c", '^');
                    cur = cur + 64;
                }
                if (cur > 127 && cur < 160) {
                    printf("%c%c%c", 'M', '-', '^');
                    cur = cur + 192;
                } else if (cur == 127) {
                    cur = '?';
                    printf("^");
                }
            }
            if (cur == '\n') {
                counter++;
            } else {
                counter = 0;
            }
            putchar(cur);
        }
        fclose(file);
    } else {
        fprintf(stderr, "s21_cat: %s: No such file or directory\n", argv[optind]);
    }
    optind++;
}
