#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
// добавить обработку заглавных букв
// сравнить с работой cat

// fd = STDIN_FILENO; //= 1 если программа вызывается без аргументов, она будет читать данные из stdin

typedef struct {
    bool number_empty;  // b
    bool number_all;    // n
    bool show_end;      // e
    bool squeeze;
    bool show_tabs;

} catFlags;

bool catParseArg(catFlags *flags, char *argv[], char *name) {  //
    ++argv;  // инкрементируем так как первый элемент это "-" ??????????/
    if (*argv == '-') {
        ++argv;
        if (strcmp(argv, "number-nonblank") ==
            0) {  // сравнивает строчки когда будет другой символ- он  выведет -1 ё
            flags->number_empty = true;
        } else if (strcmp(argv, "number") == 0) {
            flags->number_all = true;
        } else if (strcmp(argv, "squeeze-blank") == 0) {
            flags->squeeze = true;  // добавить обработку заглавных букв
        } else {
            dprintf(STDERR_FILENO, "%s:  illegal option '-- %s'\n", name, argv);
        }

        return true;
    }
    for (char *it = argv; *it != '\0'; it++) {
        // первый аргумент это будет какой-то наш флаг
        switch (*it) {
            case 'b':
                flags->number_empty = true;
                break;
            case 'e':
                flags->show_end = true;
                break;
            case 'n':
                flags->number_all = true;
                break;
            case 's':
                flags->squeeze = true;
                break;
            case 't':
                flags->show_tabs = true;
                break;
            default:
                dprintf(STDERR_FILENO, "%s: %s '%s'\n", name, "illegal option --", argv);
                return false;
        }
    }
    return true;
}

void catNoArgs(int fd, char *name) {
    char buffer[4096];
    int bytes_read;
    if (fd == -1) {
        perror(name);  // что это такое?
        return;
    }
    bytes_read =
        read(fd, buffer,
             4096);  // откуда читаем, куда записать, размер буффера - возвращает, сколько мы прочитали байтов
    while (bytes_read > 0) {                 //
        printf("%.*s", bytes_read, buffer);  // строка будет ок прочитана если с %s
        bytes_read =
            read(fd, buffer,
                 4096);  // позволяет программе пошагово читать и выводить данные из файла блоками до тех пор,
                         // пока не будет достигнут конец файла или не произойдет ошибка чтения.
    }
}

// "-ne" "-e" "-n"
bool catWithAgrs(int argc, char *argv[]) {
    catFlags flags = {0, 0, 0, 0, 0};
    for (int i = 1; i != argc; i++) {
        if (*argv[i] == '-')
            if (!catParseArg(&flags, argv[i], argv[0])) {  // почему 0?
                return false;
            }  // &flags передаем адрес структуре
    }
    if (!(flags.number_empty + flags.number_all + flags.show_end + flags.squeeze + flags.show_tabs))
        for (int i = 1; i != argc; i++) {
            if (*argv[i] != '-') {
                catNoArgs(open(argv[i], O_RDONLY), argv[0]);  // почему 0?

            }  // &flags передаем адрес структуре
        }

    // catNoArgs(open(argv[1], O_RDONLY));
    return true;
}

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc == 1)
        catNoArgs(STDIN_FILENO, argv[0]);
    else {
        if (catWithAgrs(argc, argv)) return 1;
    }

    return 0;
}

/* void catWithAgrs(int argc, char *argv[]){
    char buffer[4096];
    int fd = open(argv[1], O_RDONLY); // only reading
    int bytes_read;
    bytes_read=read(fd,buffer, 4096);
    while (bytes_read != -1){
        printf("%.*s",bytes_read,buffer);
        bytes_read=read(fd, buffer, 4096);
    }
}
*/