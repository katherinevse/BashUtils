#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> // для макроса
#include <stdbool.h>
#include <string.h>
// добавить обработку заглавных букв 



// fd = STDIN_FILENO; //= 1 если программа вызывается без аргументов, она будет читать данные из stdin


typedef struct 
{
    bool number_empty; //b
    bool number_all; // n
    bool show_end; // e
    bool squeeze;
    bool show_tabs;

} catFlags;


void catNoArgs(int fd){
    char buffer[4096];
     STDIN_FILENO; //= 1 если программа вызывается без аргументов, она будет читать данные из stdin
    int bytes_read;
    bytes_read=read(fd,buffer, 4096); //откуда читаем, куда записать, размер буффера - возвращает, сколько мы прочитали байтов
    while (bytes_read > 0){ // 
        printf("%.*s",bytes_read,buffer) ;// строка будет ок прочитана если с %s
        bytes_read=read(fd, buffer, 4096); // позволяет программе пошагово читать и выводить данные из файла блоками до тех пор, пока не будет достигнут конец файла или не произойдет ошибка чтения.
    }
}




bool catParseArg(catFlags *flags, char *argv[]){ // 
    ++argv; //инкрементируем так как первый элемент это "-" ??????????/
    if(*argv == "-"){
        ++argv;
        if(strcmp(argv, "number-nonblank") == 0) {//сравнивает строчки когда будет другой символ- он  выведет -1 ё
            flags->number_empty = true;
        }
        else if(strcmp(argv, "number") == 0){
            flags->number_all = true;
        }
        else if(strcmp(argv, "squeeze-blank") == 0){
            flags->squeeze = true;  // добавить обработку заглавных букв 
        else
        {
            /* code */
        }
        
        return 1;
    }
    for (char *it = argv; *it != '\0' ; it++)
    {
        //первый аргумент это будет какой-то наш флаг
        switch(*it){
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
            return false;

        }
    }
    
}
 // "-ne" "-e" "-n"
bool catWithAgrs(int argc, char *argv[]){
    catFlags flags = {0, 0, 0, 0, 0};
    for (int i = 1; i != argc; i++)
    {
        if(*argv [i] == '-')
        if(!catParseArg( &flags, argv[i]) ){
            return false;
        } // &flags передаем адрес структуре 
    }
    

    //catNoArgs(open(argv[1], O_RDONLY));
}



int main(int argc, char *argv[]){
    (void)argv;
    if(argc == 1)
        catNoArgs(STDIN_FILENO);
    else{
        if (catWithAgrs(argc,argv));
            return 1;
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