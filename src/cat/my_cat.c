#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> // для макроса
#include<stdbool.h>


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

void catWithAgrs(int argc, char *argv[]){
    catFlags flags = {0, 0, 0, 0, 0};
    
    catNoArgs(open(argv[1], O_RDONLY));
}



int main(int argc, char *argv[]){
    (void)argv;
    if(argc == 1)
        catNoArgs(STDIN_FILENO);
    else
        catWithAgrs(argc,argv);
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