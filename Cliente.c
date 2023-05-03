#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include "chat.pb-c.h"


#define PORT 4000
#define BUFFER_SIZE 2048

void receive_handler(void *arg)
{
    // Cast argument to integer and extract client file descriptor
    int client_fd = *(int *)arg;

    // Define buffer and bytes_received variables
    char buffer[BUFFER_SIZE];
    int bytes_received;

    // Keep receiving messages until client disconnects or an error occurs
    while ((bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0)
    {
        // Unpack received message into ChatSistOSAnswer struct
        ChatSistOS__Answer *answer = chat_sist_os__answer__unpack(NULL, bytes_received, buffer);

        // Handle decoding errors
        if (answer == NULL)
        {
            perror("Error al decodificar el mensaje");
            break;
        }

        // Handle different types of messages
        switch (answer->op)
        {
            case 1: // Crear nuevo usuario
            case 2: // Ver usuarios conectados
            case 3: // Cambiar estado de usuario
                printf("%s\n", answer->response_message);
                break;
            case 4: // Recibir mensaje
                printf("[%s]: %s\n", answer->message->message_sender, answer->message->message_content);
                break;
        }

        // Liberar memoria
        chat_sist_os__answer__free_unpacked(answer, NULL);
    }

    // Return NULL when done
    return NULL;
}


int main()
{
    int client_fd;
    struct sockaddr_in server_addr;
    pthread_t thread_id;


    //Se crea el socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    //server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    //connection
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error al conectar al servidor");
        exit(EXIT_FAILURE);
    }

    printf("Conectado!\n");


    //Crear hilo para recibir mensajes
    if (pthread_create(&thread_id, NULL, receive_handler, (void *)&client_fd) != 0) {
        perror("Error al crear el hilo de recepcion");
        exit(EXIT_FAILURE);
    }

    // Esperar por la entrada del usuario y enviar mensajes al servidor
    char input[256];
    ChatSistOS__UserOption user_option;
    char buffer[BUFFER_SIZE];
    int bytes_sent;
    int op;

    while (1)
    {
        printf("Ingrese la opción que desea realizar: \n");
        printf("1. Crear un usuario nuevo\n");
        printf("2. Ver usuarios\n");
        printf("3. Cambiar status\n");
        printf("4. Enviar mensaje publico\n");
        printf("5. Enviar mensaje privado\n");
        printf("6. Salir\n");
        
        scanf("%d", &op);

        //initialize user option
        chat_sist_os__user_option__init(&user_option);

        if (op == 1) //usuario nuevo
        {
            user_option.op = 1;
            ChatSistOS__NewUser new_user = CHAT_SIST_OS__NEW_USER__INIT;
            printf("Ingrese el nombre de usuario: ");
            scanf("%s", input);
            new_user.username = input;
            user_option.createuser = &new_user;

            
        }
        else if (op == 2) //lista de usuarios
        
        {
            user_option.op = 2;

            ChatSistOS__UserList user_list = CHAT_SIST_OS__USER_LIST__INIT;
            printf("Escriba el numero:\n");
            printf("1. Ver lista completa de usuarios\n");
            printf("2. Buscar un usuario específico\n");

            int list_option;
            scanf("%d", &list_option);

            if (list_option == 1) {
                user_list.list = true;
            }
            else if (list_option == 2) {
                user_list.list = false;
                printf("Ingrese el nombre del usuario: ");
                scanf("%s", input);
                user_list.user_name = input;
            }
            else {
                printf("Opción inválida.\n");
                continue; // Restart the loop
            }

            user_option.userlist = &user_list;
            
        }
        else if (op == 3) //cambiar status
        {
            user_option.op = 3;
            ChatSistOS__Status status = CHAT_SIST_OS__STATUS__INIT;

            printf("Ingrese su usuario: ");
            scanf("%s", input);
            status.user_name = input;

            printf("Elija su status:\n");
            printf("1. En linea\n");
            printf("2. Ocupado\n");
            printf("3. Desconectado\n");
            int new_status;
            scanf("%d", &new_status);
            status.user_state = new_status;

            user_option.status = &status;

            
        }

        
            
        else if (op == 4) //mandar mensaje publico
        {
            user_option.op = 4;
            ChatSistOS__Message message = CHAT_SIST_OS__MESSAGE__INIT;
            printf("Escriba su mensaje publico\n");
            
            printf("Mensaje: ");
            scanf(" %[^\n]", input);
            message.message_content = input;
            user_option.message = &message;
            
        }
        else if (op == 5) //mensaje privado
        {
            ChatSistOS__Message message = CHAT_SIST_OS__MESSAGE__INIT;
            user_option.op = 4;
            printf("Nombre del usuario a quien va dirigido el mensaje: \n");
            scanf("%s", input);
            message.message_destination = strdup(input);

            printf("Mensaje: ");
            scanf(" %[^\n]", input);
            message.message_content = input;
            user_option.message = &message;
            
        }   
        else if (op == 6){
            printf("Cerrando sesion!\n");
            close(client_fd);
            break;
        } 
        else
        {
            printf("Elija una opcion valida\n");
            continue;
        }

        int user_option_size = chat_sist_os__user_option__get_packed_size(&user_option);
        chat_sist_os__user_option__pack(&user_option, buffer);

        bytes_sent = send(client_fd, buffer, user_option_size, 0);
        if (bytes_sent < 0)
        {
            perror("Error");
        }
    }

    return 0;
}