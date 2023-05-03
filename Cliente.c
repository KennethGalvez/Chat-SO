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


    //Aqui se crea el socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0)
    {
        perror("Error al creando el socket");
        exit(EXIT_FAILURE);
    }

    //server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    //connection
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection error");
        exit(EXIT_FAILURE);
    }

    printf("Conectado!\n");


    //Thread created to hande incoming messages
    if (pthread_create(&thread_id, NULL, receive_handler, (void *)&client_fd) != 0) {
        perror("Error al crear el hilo de recepción");
        exit(EXIT_FAILURE);
    }

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
            printf("escriba 1 para toda la lista y 2 para un usuario especifico");
            int list_option;
            scanf("%d", &list_option);

            if (list_option == 1)
            {
                user_list.list = true;
            }
            else if (list_option == 2)
            {
                user_list.list = false;
                printf("Nombre del usuario: ");
                scanf("%s", input);
                user_list.user_name = input;
            }
            else
            {
                printf("Opcion invalida.\n");
            }

            user_option.userlist = &user_list;
            
        }
        else if (op == 3) //cambiar estatus
        {
            user_option.op = 3;
            ChatSistOS__Status *status = malloc(sizeof(ChatSistOS__Status));
            chat_sist_os__status__init(status);
            
            printf("Ingrese su usuario: ");
            scanf("%s", input);
            status->user_name = strdup(input);

            printf("Escriba 1 para en linea, 2 ocupado, 3 para desconectado: ");
            int new_status;
            scanf("%d", &new_status);
            status->user_state = new_status;

            user_option.status = status;
            
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
