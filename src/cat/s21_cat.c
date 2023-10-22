#include "s21_cat.h"

int main(const int argc, char **const argv) {
  getFlags(argc, argv);
  return 0;
}

void getFlags(const int argc, char **const argv) {
  struct FlagsStructure flags;
  flags.b_flag = 0;    // Нумеровать только непустые строки, отменяет -n
  flags.e_flag = 0;    // Выводить символ $ в конце каждой строки
  flags.E_flag = 0;    // Выводить символ $ в конце каждой строки (переопределяет -e)
  flags.n_flag = 0;    // Нумеровать все строки вывода
  flags.s_flag = 0;    // Подавлять повторяющиеся пустые строки
  flags.t_flag = 0;    // Отображать символы TAB как ^I
  flags.T_flag = 0;    // Отображать символы TAB как ^I (переопределяет -t)
  flags.v_flag = 0;    // Отображать непечатные символы, за исключением новых строк и символов табуляции
  flags.err_flag = 0;  // Флаг для обозначения ошибок в аргументах командной строки


  int isFile = 0; // чтобы понять это файл или опция   <<<< isFile++;: Это увеличивает счетчик файлов (isFile). Этот счетчик используется, чтобы отслеживать, началась ли уже обработка файлов в командной строке. Если isFile был равен 0, это означает, что программа еще не начинала обработку файлов.

  for (int i = 1; i < argc; i++) { // i это аргумент строки нашей
    if (argv[i][0] == '-' && argv[i][1] != '-' && isFile == 0) { // проверяем что i строка c нулевым символом является "-" и второй аргумент не является "-" (типа файл) и file = 0 =программа еще не начала обрабатывать файлы.
      for (int j = 1; j < (int)strlen(argv[i]); j++) { // Внутри этого цикла обрабатываются символы после первого дефиса '-'
    // в текущем аргументе командной строки.
    //j будет использоваться для итерации по символам внутри текущего аргумента командной строки.
    //(int)strlen(argv[i]) используется для получения длины строки и приведения ее к типу int.
        switch (argv[i][j]) { // как он тут понимает, что первое это строка а второй это символы 
          case 'b':
            flags.b_flag = 1;
            break;
          case 'e':
            flags.e_flag = 1;
            break;
          case 'n':
            flags.n_flag = 1;
            break;
          case 's':
            flags.s_flag = 1;
            break;
          case 't':
            flags.t_flag = 1;
            break;
          case 'v':
            flags.v_flag = 1;
            break;
          case 'E':
            flags.E_flag = 1;
            break;
          case 'T':
            flags.T_flag = 1;
            break;
          default:
            flags.err_flag = 1;
            break;
        }
      }

    // Оператор switch в C поддерживает только целочисленные значения (и символы), и не позволяет использовать строки в качестве кейсов.
    } else if (argv[i][0] == '-' && argv[i][1] == '-' && isFile == 0) { // проверка на '- -' 
      if (strcmp(argv[i], "--number-nonblank")) // сравниваем аргументы  
      // if (strcmp(argv[i], "--number-nonblank") == 0)
        flags.b_flag = 1;
      else if (strcmp(argv[i], "--number"))
        flags.n_flag = 1;
      else if (strcmp(argv[i], "--squeeze-blank"))
        flags.s_flag = 1;
      else
        flags.err_flag = 1;
    } else {
      isFile++;
      FILE *readFile;
      //printf("%s", argv[i]);
      output(flags, argv[i], &readFile);
    }
  }
}

void output(const struct FlagsStructure flags, char *fileName, FILE **file) { // FILE **file почему тут два указателя? на что эти указатели?

  *file = fopen(fileName, "r"); // указатель на файл, почему не сам файл? 
  int ch; // будет храниться текущий символ, считанный из файла.
  int prev; // для хранения предыдущего символа, чтобы обнаруживать изменения строк.
  int blank_lines = 0; // отслеживает количество пустых строк
  char *symbols[33] = {"^@", "^A", "^B", "^C", "^D",  "^E", "^F", "^G",
                       "^H", "^I", "$",  "^K", "^L",  "^M", "^N", "^O",
                       "^P", "^Q", "^R", "^S", "^T",  "^U", "^V", "^W", 
                       "^X", "^Y", "^Z", "^[", "^\\", "^]", "^^", "^_"}; 
                       //массив строк, представляющих специальные символы, которые могут встречаться в текст
  int is_first_ch = 1; // Флаг, указывающий, является ли текущий символ первым в файле.

  if (!flags.err_flag) { // нет ошибок во флагах
    if (*file != NULL) { // успешно открылся файл
      int counter = 1; // нумерации строк при выводе на экран
      while (!feof(*file) && !ferror(*file)) {   // feof(*file) - это функция, которая проверяет, достигнут ли конец файла
      // ferror(*file)  Отсутствие файла, любые нарушения
        ch = getc(*file); //  у нас цикл, В каждой итерации цикла вызывается getc(*file), что означает получение очередного символа из файла, на который указывает указатель file. Полученный символ сохраняется в переменной ch.
        if (ch != EOF) { // символ не конец строки
          if ((ch == '\n' && prev == '\n') || // не новая строка и потом не новая строка 
              (is_first_ch == 1 && ch == '\n')) {

                //HELLO h e
            blank_lines++; // либо 0 либо 1
          } else {
            blank_lines = 0;
          }
        // blank_lines = 0
          if (blank_lines < 2 || !flags.s_flag) { // squeeze и подряд нет строк
            if ((ch != '\n' && flags.b_flag) || (is_first_ch && flags.n_flag)) { ///// stop
              if (prev == '\n') {
                printf("%6d\t", counter);
                counter++;
              } else if (is_first_ch && (flags.n_flag || flags.b_flag)) {
                if (ch == '\n' && (flags.e_flag || flags.E_flag)) {
                  if (!flags.b_flag) {
                    printf("%6d\t", counter);
                    counter++;
                  }
                } else if (is_first_ch && (flags.n_flag || flags.b_flag)) {
                  if (flags.b_flag && ch == '\n') {
                  } else {
                    printf("%6d\t", counter);
                    counter++;
                  }
                }
              }
            }
            if (ch == '\n' && (flags.E_flag || flags.e_flag)) {
              printf("$\n");
            } else if (ch == '\t' && (flags.T_flag || flags.t_flag)) {
              printf("^I");
            } else {
              if (((ch >= 0 && ch <= 31 && ch != 9 && ch != 10) || ch == 127) &&
                  (flags.v_flag || flags.e_flag || flags.t_flag))
                if (ch == 127)
                  printf("^?");
                else
                  printf("%s", symbols[ch]);
              else {
                putchar(ch);
              }
            }

            if (ch == '\n' && flags.n_flag && !flags.b_flag &&
                (!flags.E_flag || !flags.e_flag) && !flags.s_flag) {
              ch = getc(*file);
              if (ch == EOF) {
              } else {
                fseek(*file, -1, SEEK_CUR);
                printf("%6d\t", counter);
                counter++;
              }
            }

            if (ch == '\n' && flags.n_flag && !flags.b_flag &&
                (!flags.E_flag || !flags.e_flag) && flags.s_flag &&
                blank_lines < 2) {
              // ch = getc(*file);
              // if (ch == EOF) {
              // } else {
              //   fseek(*file, -1, SEEK_CUR);
              //   printf("%6d\t", counter);
              //   counter++;
              // }
              int seek_count = 0;
              int is_end = 0;
              while (ch != EOF) {
                ch = getc(*file);
                if (ch != '\n' && ch != EOF) {
                  is_end = 1;
                }
                seek_count++;
              }
              if (!is_end) {
                printf("%6d\t", counter);
                if (flags.e_flag) printf("$\n");
              } else {
                fseek(*file, -seek_count, SEEK_CUR);
                ch = getc(*file);
                printf("%6d\t", counter);
                counter++;
              }
            }
          }
        }
        prev = ch;
        is_first_ch = 0;
      }
      fclose(*file);
    } else {
      fprintf(stderr, "cat: %s: No such file or directory\n", fileName);
    }

  } else {
    fprintf(stderr, "usage: cat [-benstv] [file ...]\n");
  }
}