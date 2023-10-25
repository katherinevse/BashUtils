
void my_parser(const int argc, char *argv[]) {
    struct opt options;
    options.b = 0;  // Нумеровать только непустые строки, отменяет -n
    options.e = 0;  // Выводить символ $ в конце каждой строки
    options.E = 0;  // Выводить символ $ в конце каждой строки (переопределяет -e)
    options.n = 0;  // Нумеровать все строки вывода
    options.s = 0;  // Подавлять повторяющиеся пустые строки
    options.t = 0;  // Отображать символы TAB как ^I
    options.T = 0;  // Отображать символы TAB как ^I (переопределяет -t)
    options.v = 0;  // Отображать непечатные символы, за исключением новых строк и символов табуляции
    options.err = 0;  // Флаг для обозначения ошибок в аргументах командной строки

    int isFile =
        0;  // чтобы понять это файл или опция   <<<< isFile++;: Это увеличивает счетчик файлов (isFile). Этот
            // счетчик используется, чтобы отслеживать, началась ли уже обработка файлов в командной строке.
            // Если isFile был равен 0, это означает, что программа еще не начинала обработку файлов.

    for (int i = 1; i < argc; i++) {  // i это аргумент строки нашей
        if (argv[i][0] == '-' && argv[i][1] != '-' &&
            isFile ==
                0) {  // проверяем что i строка c нулевым символом является "-" и второй аргумент не является
                      // "-" (типа файл) и file = 0 =программа еще не начала обрабатывать файлы.
            for (int j = 1; j < (int)strlen(argv[i]);
                 j++) {  // Внутри этого цикла обрабатываются символы после первого дефиса '-'
                         // в текущем аргументе командной строки.
                // j будет использоваться для итерации по символам внутри текущего аргумента командной строки.
                //(int)strlen(argv[i]) используется для получения длины строки и приведения ее к типу int.
                switch (argv[i][j]) {  // как он тут понимает, что первое это строка а второй это символы
                    case 'b':
                        options.b = 1;
                        break;
                    case 'e':
                        options.e = 1;
                        break;
                    case 'n':
                        options.n = 1;
                        break;
                    case 's':
                        options.s = 1;
                        break;
                    case 't':
                        options.t = 1;
                        break;
                    case 'v':
                        options.v = 1;
                        break;
                    case 'E':
                        options.E = 1;
                        break;
                    case 'T':
                        options.T = 1;
                        break;
                    default:
                        options.err = 1;
                        break;
                }
            }

            // Оператор switch в C поддерживает только целочисленные значения (и символы), и не позволяет
            // использовать строки в качестве кейсов.
        } else if (argv[i][0] == '-' && argv[i][1] == '-' && isFile == 0) {  // проверка на '- -'
            if (strcmp(argv[i], "--number-nonblank"))  // сравниваем аргументы
                                                       // if (strcmp(argv[i], "--number-nonblank") == 0)
                options.b = 1;
            else if (strcmp(argv[i], "--number"))
                options.n = 1;
            else if (strcmp(argv[i], "--squeeze-blank"))
                options.s = 1;
            else
                options.err = 1;
        } else {
            isFile++;
            // FILE *readFile;
            // printf("%s", argv[i]);
            // output(flags, argv[i], &readFile);
        }
    }
}