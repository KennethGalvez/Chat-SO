#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include "chat.pb-c.h"

void main(void){
    char cadena[100]; /*Guardar Mensajes Recibidos*/
    int sockfd; /*Para crear el socket*//*Recoger a los clientes de la lista de espera*/
    struct sockaddr_in servaddr;
    FILE *myf = fopen("conversacion_cliente.txt", "a");/*Guardar la conversacion en un txt*/
    /*Tiempo*/
    time_t t;
    struct tm *tm;
    char hora[100];
    char *tmp;
    char sendline[100] = "Usando el puerto 22000\n"; /*El mensaje en cuestion que se manda*/

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(22000);

    inet_pton(AF_INET, "127.0.0.1", &(servaddr.sin_addr));
    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    printf("\n\n\t\t-------- Se inicio eL chat------------\n\n");
    fputs("\n\n\t\t-------- Se inicio eL chat------------\n\n", myf);

    while (!strstr(cadena, "adios") && !strstr(sendline, "adios")){
        bzero(cadena, 100);
        t = time(NULL);
        tm = localtime(&t);
        strftime(hora, 100, "\n yo (%H:%M) -> ", tm);
        printf("%s", hora);
        gets(sendline);
        tmp = strcat(hora, sendline);
        fputs(tmp, myf);
        write(sockfd, sendline, strlen(sendline)+1);

        if(!strstr(cadena, "adios")){
            strftime(hora, 100, "\n otro usuario (%H:%M) -> ", tm);
            read(sockfd, cadena, 100);
            tmp = strcat(hora, cadena);
            printf("%s", tmp);
            fputs(tmp, myf);
        }

    }
    printf("\n\n Conversacion finalizada \n");
    printf("\n Se genero el archivo -> conversacion_cliente.txt");
    fclose(myf);
}
