#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>


void main(void){
    char cadena[100]; /*Guardar Mensajes Recibidos*/
    int listend_fd, comm_fd; /*Para crear el socket*//*Recoger a los clientes de la lista de espera*/
    struct sockaddr_in servaddr;
    FILE *myf = fopen("conversacion_servidor.txt", "a");/*Guardar la conversacion en un txt*/
    /*Tiempo*/
    time_t t;
    struct tm *tm;
    char hora[100];
    char *tmp;
    char sendline[100] = "Usando el puerto 22000\n"; /*El mensaje en cuestion que se manda*/

    listend_fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(22000);

    bind(listend_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listend_fd, 10);
    comm_fd = accept(listend_fd, (struct sockaddr*) NULL, NULL);

    printf("\n\n\t\t-------- Se inicio eL chat------------\n\n");
    fputs("\n\n\t\t-------- Se inicio eL chat------------\n\n", myf);

    while (!strstr(cadena, "adios") && !strstr(sendline, "adios")){
        bzero(cadena, 100);
        t = time(NULL);
        tm = localtime(&t);
        strftime(hora, 100, "\n otro usuario (%H:%M) -> ", tm);

        read(comm_fd, cadena, 100);
        tmp = strcat(hora, cadena);
        printf("%s", tmp);
        fputs(tmp, myf);
        if(!strstr(cadena, "adios")){
            strftime(hora, 100, "\n yo (%H:%M) -> ", tm);
            printf("%s", hora);
            gets(sendline);
            tmp = strcat(hora, sendline);
            write(comm_fd, sendline, strlen(sendline)+1);
            fputs(tmp, myf);
        }

    }
    
    printf("\n\n Conversacion finalizada \n");
    printf("\n Se genero el archivo -> conversacion_servidor.txt");
    fclose(myf);
    
}





