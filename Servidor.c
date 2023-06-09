#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <pthread.h>
#include "chat.pb-c.h"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define PORT 8080

typedef struct
{
    int client_fd;
    char username[BUFFER_SIZE];
    int user_state;
    pthread_t thread_id;
} client_t;

client_t clients[MAX_CLIENTS];
int num_clients = 0;

void *client_handler(void *arg)
{
    client_t *client = (client_t *)arg;
    char buffer[BUFFER_SIZE];
    int bytes_received, bytes_sent;
    int client_fd = client->client_fd;

    ChatSistOS__Answer answer = CHAT_SIST_OS__ANSWER__INIT;
    int answer_size;
        
    // Recibimos mensajes del cliente
    while ((bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0)
    {
        // Procesamos el mensaje
        ChatSistOS__UserOption *user_option = chat_sist_os__user_option__unpack(NULL, bytes_received, buffer);
        if (user_option == NULL)
        {
            perror("Error");
            break;
        }

        int ope = user_option->op;
        if (ope == 1)
        {
            // Crear nuevo usuario
            strcpy(client->username, user_option->createuser->username);
            client->user_state = 1;

            // Creamos lo que le pasaremos al cliente
            ChatSistOS__Answer answer = CHAT_SIST_OS__ANSWER__INIT;
            answer.response_message = "Usuario creado";
            answer.op = 1;

            uint8_t buffer[BUFFER_SIZE];
            int answer_size = chat_sist_os__answer__get_packed_size(&answer);
            chat_sist_os__answer__pack(&answer, buffer);
            int bytes_sent = send(client->client_fd, buffer, answer_size, 0);
            if (bytes_sent < 0)
            {
                perror("Error");
            }

            size_t username_length = strlen(client->username);

            printf("Usuario %s creado \n", client->username);
   
        }
        else if (ope == 2)
        {
            // Verificar opción 
            if (user_option->userlist->list)
            {
                // Mostrar lista 
                printf("Lista de usuarios conectados:\n");
                for (int i = 0; i < num_clients; i++)
                {
                    if (clients[i].thread_id != 0)
                    {
                        printf("- %s\n", clients[i].username);
                    }
                }
            }
            else
            {
                //Usuario solo
                const char *target_username = user_option->userlist->user_name;
                bool user_found = false;

                printf("Información de %s:\n", target_username);

                for (int i = 0; i < num_clients; i++)
                {
                    if (strcmp(clients[i].username, target_username) == 0)
                    {
                        printf("- Estado: ");
                        switch (clients[i].user_state)
                        {
                            case 1:
                                printf("En linea\n");
                                break;
                            case 2:
                                printf("Ocupado\n");
                                break;
                            case 3:
                                printf("Desconectado\n");
                                break;
                            default:
                                printf("No se encontro\n");
                                break;
                        }
                        user_found = true;
                        break;
                    }
                }

                if (!user_found)
                {
                    printf("El usuario %s no esta conectado.\n", target_username);
                }
            }

            // Respuesta al cliente
            ChatSistOS__Answer answer = CHAT_SIST_OS__ANSWER__INIT;
            answer.op = 2;
            answer.response_status_code = 200;
            size_t answer_size = chat_sist_os__answer__get_packed_size(&answer);
            uint8_t *buffer = malloc(answer_size);
            chat_sist_os__answer__pack(&answer, buffer);
            ssize_t bytes_sent = send(client_fd, buffer, answer_size, 0);
            if (bytes_sent < 0)
            {
                perror("Error");
            }
            free(buffer);

        }

        else if (ope == 3)
        {
            if (user_option->status != NULL && strlen(user_option->status->user_name) > 0)
            {
                // Obtener el nombre de usuario
                const char *target_username = user_option->status->user_name;

                // Buscar el usuario en la lista de clientes
                bool user_found = false;
                for (int i = 0; i < num_clients; i++)
                {
                    if (strcmp(clients[i].username, target_username) == 0)
                    {
                        // Mostrar estado del usuario
                        char estado[20] = "";
                        if (clients[i].user_state == 1)
                        {
                            strcpy(estado, "En linea");
                        }
                        else if (clients[i].user_state == 2)
                        {
                            strcpy(estado, "Ocupado");
                        }
                        else
                        {
                            strcpy(estado, "Desconectado");
                        }

                        printf("User %s status: %s\n", clients[i].username, estado);

                        // Crear mensaje de respuesta para el cliente
                        ChatSistOS__Answer answer = CHAT_SIST_OS__ANSWER__INIT;
                        answer.op = 3;
                        answer.response_status_code = 200;
                        answer.response_message = estado;

                        // Empaquetar la respuesta y enviarla al cliente
                        size_t answer_size = chat_sist_os__answer__get_packed_size(&answer);
                        uint8_t *buffer = malloc(answer_size);
                        chat_sist_os__answer__pack(&answer, buffer);
                        ssize_t bytes_sent = send(client_fd, buffer, answer_size, 0);
                        if (bytes_sent < 0)
                        {
                            perror("Error");
                        }

                        free(buffer);

                        user_found = true;
                        break;
                    }
                }

                if (!user_found)
                {
                    // Si el usuario no se encuentra, se envía un mensaje de error al cliente
                    ChatSistOS__Answer answer = CHAT_SIST_OS__ANSWER__INIT;
                    answer.op = 3;
                    answer.response_status_code = 404;
                    answer.response_message = "Usuario no encontrado";

                    size_t answer_size = chat_sist_os__answer__get_packed_size(&answer);
                    uint8_t *buffer = malloc(answer_size);
                    chat_sist_os__answer__pack(&answer, buffer);
                    ssize_t bytes_sent = send(client_fd, buffer, answer_size, 0);
                    if (bytes_sent < 0)
                    {
                        perror("Error");
                    }

                    free(buffer);
                }
            }
            else
                {
                    // Cambiar el estado del usuario
                    client->user_state = user_option->status->user_state;

                    // Crear mensaje de respuesta para el cliente
                    ChatSistOS__Answer answer = CHAT_SIST_OS__ANSWER__INIT;
                    answer.op = 3;
                    answer.response_status_code = 200;
                    answer.response_message = "Estado cambiado";

                    size_t answer_size = chat_sist_os__answer__get_packed_size(&answer);
                    uint8_t *buffer = malloc(answer_size);
                    chat_sist_os__answer__pack(&answer, buffer);
                    ssize_t bytes_sent = send(client_fd, buffer, answer_size, 0);
                    if (bytes_sent < 0)
                    {
                        perror("Error");
                    }

                    free(buffer);

                    printf("Se cambió el estado a %d\n", client->user_state);
                }
        }
        else if (ope == 4)
        {
                // Send message
            if (user_option->message->message_private == 1)
            {
                printf("Mensaje Privado de %s para %s: %s\n", client->username, user_option->message->message_destination, user_option->message->message_content);
                
                // Private message
                for (int i = 0; i < num_clients; i++)
                {
                    if (strcmp(clients[i].username, user_option->message->message_destination) == 0)
                    {
                        answer.op = 4;
                        answer.response_status_code = 200;
                        answer.message = user_option->message;
                        answer_size = chat_sist_os__answer__get_packed_size(&answer);
                        chat_sist_os__answer__pack(&answer, buffer);

                        bytes_sent = send(clients[i].client_fd, buffer, answer_size, 0);
                        if (bytes_sent < 0)
                        {
                            perror("Error");
                        }
                        break;
                    }
                }
            }
            else
            {
                printf("Mensaje Global de %s: %s\n", client->username, user_option->message->message_content);
                
                // Global message
                for (int i = 0; i < num_clients; i++)
                {
                    if (clients[i].thread_id != 0 && clients[i].client_fd != client->client_fd)
                    {
                        answer.op = 4;
                        answer.response_status_code = 200;
                        answer.message = user_option->message;
                        answer_size = chat_sist_os__answer__get_packed_size(&answer);
                        chat_sist_os__answer__pack(&answer, buffer);

                        bytes_sent = send(clients[i].client_fd, buffer, answer_size, 0);
                        if (bytes_sent < 0)
                        {
                            perror("Error");
                        }
                    }
                }
            }      
        }
        chat_sist_os__user_option__free_unpacked(user_option, NULL);
    }

    // El cliente se desconectó
    printf("Usuario %s se desconectó\n", client->username);
    close(client_fd);
    memset(client->username, 0, BUFFER_SIZE);
    client->thread_id = 0;
    pthread_exit(NULL);
}

void add_client(int client_fd) {
    if (num_clients >= MAX_CLIENTS) {
        printf("Máximo número de clientes\n");
        close(client_fd);
        return;
    }

    clients[num_clients++] = (client_t) { .client_fd = client_fd };

    printf("Nuevo usuario conectado\n");
}

void remove_client(int index) {
    client_t *client = &clients[index];
    close(client->client_fd);
    memset(client, 0, sizeof(client_t));
    num_clients--;

    printf("Usuario desconectado\n");

    if (index < num_clients) {
        *client = clients[num_clients];
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <puerto>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Error");
        return 1;
    }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error");
        return 1;
    }

    if (listen(server_fd, MAX_CLIENTS) == -1) {
        perror("Error");
        return 1;
    }

    printf("Iniciando servidor\nEsperando conexion de un cliente\n");

    while (1) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd == -1) {
            perror("Error");
            continue;
        }

        printf("Conexion detectada\n");

        add_client(client_fd);

        client_t *client = &clients[num_clients - 1];
        if (pthread_create(&client->thread_id, NULL, client_handler, (void *)client) != 0) {
            perror("Error");
            remove_client(num_clients - 1);
        }
    }

    close(server_fd);
    return 0;
}