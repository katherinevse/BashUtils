#include "s21_cat.h"

int main(int argc, char *argv[]) {
    struct FlagsStructure flags;
    initFlags(&flags);
    processArgs(argc, argv, &flags);

    return 0;
}

void initFlags(struct FlagsStructure *flags) {
    flags->b_flag = 0;
    flags->e_flag = 0;
    flags->E_flag = 0;
    flags->n_flag = 0;
    flags->s_flag = 0;
    flags->t_flag = 0;
    flags->T_flag = 0;
    flags->v_flag = 0;
    flags->err_flag = 0;
}

void processArgs(int argc, char *argv[], struct FlagsStructure *flags) {
    int isFile = 0;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] != '-' && isFile == 0) {
            processSingleDashArg(argv[i], flags);
        } else if (argv[i][0] == '-' && argv[i][1] == '-' && isFile == 0) {
            processDoubleDashArg(argv[i], flags);
        } else {
            isFile++;
            FILE *readFile = fopen(argv[i], "r");
            if (readFile != NULL) {
                showFileContents(*flags, readFile);
                fclose(readFile);
            } else {
                fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
            }
        }
    }
}

void processSingleDashArg(char *arg, struct FlagsStructure *flags) {
    for (int j = 1; j < (int)strlen(arg); j++) {
        switch (arg[j]) {
            case 'b':
                flags->b_flag = 1;
                break;
            case 'e':
                flags->e_flag = 1;
                break;
            case 'n':
                flags->n_flag = 1;
                break;
            case 's':
                flags->s_flag = 1;
                break;
            case 't':
                flags->t_flag = 1;
                break;
            case 'v':
                flags->v_flag = 1;
                break;
            case 'E':
                flags->E_flag = 1;
                break;
            case 'T':
                flags->T_flag = 1;
                break;
            default:
                flags->err_flag = 1;
                break;
        }
    }
}

void processDoubleDashArg(char *arg, struct FlagsStructure *flags) {
    if (strcmp(arg, "--number-nonblank") == 0)
        flags->b_flag = 1;
    else if (strcmp(arg, "--number") == 0)
        flags->n_flag = 1;
    else if (strcmp(arg, "--squeeze-blank") == 0)
        flags->s_flag = 1;
    else
        flags->err_flag = 1;
}

void showFileContents(const struct FlagsStructure flags, FILE *file) {
    int ch;
    int prev;
    int blankLinesCount = 0;
    char *symbols[33] = {"^@", "^A", "^B", "^C", "^D", "^E", "^F",  "^G", "^H", "^I", "$",
                         "^K", "^L", "^M", "^N", "^O", "^P", "^Q",  "^R", "^S", "^T", "^U",
                         "^V", "^W", "^X", "^Y", "^Z", "^[", "^\\", "^]", "^^", "^_"};

    int isFirstChar = 1;

    if (!flags.err_flag) {
        if (file != NULL) {
            int lineNumber = 1;
            while (!feof(file) && !ferror(file)) {
                ch = getc(file);
                if (ch != EOF) {
                    if ((ch == '\n' && prev == '\n') || (isFirstChar == 1 && ch == '\n')) {
                        blankLinesCount++;
                    } else {
                        blankLinesCount = 0;
                    }

                    if (blankLinesCount < 2 || !flags.s_flag) {
                        if ((ch != '\n' && flags.b_flag) || (isFirstChar && flags.n_flag)) {
                            if (prev == '\n') {
                                printf("%6d\t", lineNumber);
                                lineNumber++;
                            } else if (isFirstChar && (flags.n_flag || flags.b_flag)) {
                                if (ch == '\n' && (flags.e_flag || flags.E_flag)) {
                                    if (!flags.b_flag) {
                                        printf("%6d\t", lineNumber);
                                        lineNumber++;
                                    }
                                } else if (isFirstChar && (flags.n_flag || flags.b_flag)) {
                                    if (flags.b_flag && ch == '\n') {
                                    } else {
                                        printf("%6d\t", lineNumber);
                                        lineNumber++;
                                    }
                                }
                            }
                        }
                        if (ch == '\n' && (flags.E_flag || flags.e_flag)) {
                            printf("$\n");
                        } else if (ch == '\t' && (flags.T_flag || flags.t_flag)) {
                            printf("^I");
                        } else {
                            displaySpecialSymbols(ch, flags, symbols);
                        }

                        if (ch == '\n' && flags.n_flag && !flags.b_flag && (!flags.E_flag || !flags.e_flag) &&
                            !flags.s_flag) {
                            handleLineNumberWithoutBlank(ch, file, &lineNumber);
                        }

                        if (ch == '\n' && flags.n_flag && !flags.b_flag && (!flags.E_flag || !flags.e_flag) &&
                            flags.s_flag && blankLinesCount < 2) {
                            handleLineNumberWithSqueeze(ch, file, &lineNumber, &blankLinesCount);
                        }
                    }
                }
                prev = ch;
                isFirstChar = 0;
            }
            fclose(file);
        } else {
            fprintf(stderr, "cat: Error opening file\n");
        }
    } else {
        fprintf(stderr, "usage: cat [-benstv] [file ...]\n");
    }
}

void displaySpecialSymbols(int currentChar, const struct FlagsStructure flags, char *symbols[]) {
    if (((currentChar >= 0 && currentChar <= 31 && currentChar != 9 && currentChar != 10) ||
         currentChar == 127) &&
        (flags.v_flag || flags.e_flag || flags.t_flag)) {
        if (currentChar == 127) {
            printf("^?");
        } else {
            printf("%s", symbols[currentChar]);
        }
    } else {
        putchar(currentChar);
    }
}

void handleLineNumberWithoutBlank(int currentChar, FILE *file, int *lineNumber) {
    currentChar = getc(file);
    if (currentChar == EOF) {
        // Do nothing
    } else {
        fseek(file, -1, SEEK_CUR);
        printf("%6d\t", *lineNumber);
        (*lineNumber)++;
    }
}

void handleLineNumberWithSqueeze(int currentChar, FILE *file, int *lineNumber, int *blankLinesCount) {
    int seekCount = 0;
    int isEnd = 0;

    while (currentChar != EOF) {
        currentChar = getc(file);
        if (currentChar != '\n' && currentChar != EOF) {
            isEnd = 1;
        }
        seekCount++;
    }

    if (!isEnd) {
        printf("%6d\t", *lineNumber);
        if (flags.e_flag) printf("$\n");
    } else {
        fseek(file, -seekCount, SEEK_CUR);
        currentChar = getc(file);
        printf("%6d\t", *lineNumber);
        (*lineNumber)++;
    }
}
