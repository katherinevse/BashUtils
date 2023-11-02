#include "s21_grep.h"

#include <string.h>

int main(int argc, char *argv[]) {
  regex_t regex;  // скомпилированный паттерн лежит
  (void)argv;
  if (argc == 1) {
    fprintf(stderr,
            "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
            "[-C[num]]\n");
    fprintf(stderr,
            "\t[-e pattern] [-f file] [--binary-files=value] [--color=when]\n");
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
  int reg_flag = 0;  // установки флагов регулярных выражений, для i, для е jndtxftn pf hfcibhtybz 
  char pattern[1000];  // для хранения шаблона регулярного выражения. При
                       // обработке опции -e, значение этой опции (которое
                       // передается через optarg
  while ((flag = getopt_long(argc, argv, "e:ivclnsoh", NULL, NULL)) !=
         (-1)) {  // почему именно такая запись? e:ivclnsoh
    switch (flag) {
      case 'i':
        options->i = 1;
        reg_flag = REG_ICASE;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'c':
        options->c = 1;
        break;
      case 'l':
        options->l = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 'h':
        options->h = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'e':
        options->e = 1;
        strcat(pattern,
               optarg);  // optarg- аргумент(шаблон) в данный момент // optind -
                         // индекс  // указатель просто указывает на элемент
                         // массива argv[]).   // pattern = argument(name)
        strcat(pattern,
               "|");  //чтобы создать шаблон, который будет включать в себя
                      //несколько подшаблонов, разделенных символом |.
        if (reg_flag == 0) {
          reg_flag = 1;  // REG_EXTENDED_FLAG
        }
        pattern[strlen(pattern) - 1] = '\0';
        break;
    }
  }
  if (options->e) {
    regcomp(regex, pattern, reg_flag);
    // regex: Это указатель на структуру regex_t, в которую будет сохранено
    // скомпилированное регулярное выражение. pattern: Это строка, содержащая
    // само регулярное выражение, которое нужно скомпилировать
  } else {
    regcomp(regex, argv[optind],
            reg_flag);  //Если опция -e не указана, программа просто берет
                        //следующий аргумент из командной строки (argv[optind])
                        //и компилирует его как регулярное выражение.
    optind++;  // для перехода к следующему аргументу
  }
  read_file(argc, argv, options, regex);
}

void read_file(int argc, char *argv[], opt *options, regex_t *regex) {
  FILE *file;
  char *line = NULL;
  size_t len = 0;
  int read = 0;  // возвращает количество считанных символов
  int search_reg = 0;
  int str_count = 0;
  int str_count_c = 0;
  int num_files =
      argc - optind;  // Определяем количество переданных файлов для поиска

  while (optind < argc) {
    file = fopen(argv[optind], "r");
    if (file) {
      int over = 0;  //было ли уже выведено имя файла
      while ((read = getline(&line, &len, file)) != EOF) {
        search_reg =
            regexec(regex, line, 0, NULL,0);  //строка выполняет поиск регулярного выражения в строке если regexes возвращает 0 значит строка соответсвует шаблону
        str_count++;
        // проверка на должны ли быть выведены результаты поиска (например,
        // строки из файла) в зависимости от условий поиска и установленных
        // флагов. Если условия выполняются, то выводится информация о файле или
        // строке.
        if ((options->v && search_reg == REG_NOMATCH) ||
            (search_reg == 0 && (options->v == 0 || options->e))) {
          //проверка на должны ли быть выведены результаты поиска
          if (num_files > 1 && options->h == 0 && options->c == 0 &&
              options->l == 0) {
            printf("%s:", argv[optind]);
          }
          //отработка флага n
          if (options->n && options->c == 0 && options->l == 0) {
            printf("%d:%s", str_count, line);
            if (line[strlen(line) - 1] != '\n') {
              printf("\n");
            }
          }
          //отработка флага с
          //флаг l
          else if (options->l && over == 0) {
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
      // выводит количество строк совпадения в каждом файле, а при наличии
      // нескольких файлов также выводит их имена.
      if (options->c) {
        if (num_files > 1 && options->h == 0) {
          printf("%s:%d\n", argv[optind], str_count_c);
        } else {
          printf("%d\n", str_count_c);
        }
        str_count_c =
            0;  // Сбрасывает счетчик совпадающих строк для следующего файла.
      }
    } else {
      if (options->s == 0) {
        fprintf(stderr, "s21_grep: %s: No such file or directory\n",
                argv[optind]);
      }
    }
    optind++;
  }
  fclose(file);
  exit(0);
  free(line);
  regfree(regex);
}

// v выводит строки, которые НЕ содержат указанный шаблон.
//-h исключит отображение имени файла перед каждой строкой совпадения.
//-с используется для вывода только количества строк, содержащих совпадения, а
//не самих строк
//  -n в команде grep добавляет номера строк, в каких строках файла были
//  обнаружены совпадения с заданным шаблоном
//-l  выводить только имена файлов где есть шаблон


//шаблон нам нужен для каких флагов? 