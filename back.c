#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <msgpack.h>
#include <pthread.h>



struct cliente {
    int socket_id;
    char username[100];
    int pid;
    int pipefd[2];
    int active;
    int logueado;
    int numpeticiones;
    int numrespuestas;
    int numpreguntas;
    char **historial;
    int status; // Add a "status" member to the struct
};

cliente allClients[10] = {};


void *handler(void *arg)
{
    pthread_t thisThread = pthread_self();
    int slot = -1;
    int socketInt = *(int *)arg;

    printf("Un cliete conectado\n");

    char buffer[2048] = {0};
    int readPid;
    bool flagLive = true;
    bool *reading = (bool *)mmap(NULL, sizeof(bool), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    bool *dieT = (bool *)mmap(NULL, sizeof(bool), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int flags = fcntl(socketInt, F_GETFL, 0);
    fcntl(socketInt, F_SETFL, flags | O_NONBLOCK);

    while(flagLive){
        memset(buffer, 0, 2048);
    int dieT = 0;
    int reading = 1;
    int pid = fork();
    int timeInactive = 0;

    if (pid == 0)
    {
        timeInactive = 0;
        while (reading)
        {
            sleep(1);
            timeInactive += 1;

            if (timeInactive >= 180)
            {
                close(socketInt);
                dieT = 1;
                break;
            }
        }
            return 0;
        }
        
        
        else if (pid > 0)
        {
            readPid = -1;
            int *dieT;

            while (readPid == -1 && !*dieT)
            {
                readPid = read(socketInt, buffer, 2048);
            }

            if (readPid != -1)
            {
                // Message was received successfully
                // Process the message here
            }
            int *reading;
            *reading = false;
            wait(NULL);
            close(socketInt);
            exit(0);
        }
        
        if (int *dieT)
{
    if (slot != -1)
    {
        cliente->status = 1; // Set the "status" member to 1
    }

    break;
}

memset(buffer, 0, 2048);
readPid = recv(socketInt, buffer, 2048, 0);

    if (readPid < 0)
    {
        printf("Error: cant read pid client socket\n");
        break;
    }
    else if (readPid == 0)
    {
        printf("Client disconnected\n");

        if (slot != -1)
        {
            strncpy(nuevo_cliente->username, "", sizeof(nuevo_cliente->username))
            allClients[slot].ip = "";
            allClients[slot].socket = 0;
            allClients[slot].status = 0;
        }
        break;
    }
    if (newRequest.option() == 1)
{
    printf("New Request from user: REGISTER\n");
    printf("User:%s\n", newRequest.mutable_newuser()->username().c_str());
    printf("IP:%s\n", newRequest.mutable_newuser()->ip().c_str());

    chat::ServerResponse newResponse;
    newResponse.set_option(1);
    newResponse.set_code(400);
    newResponse.set_servermessage("Error registering user");

    bool canRegister = true;

    for (int i = 0; i < 10; i++)
    {
        if (strcmp(allClients[i].username, newRequest.mutable_newuser()->username().c_str()) == 0)
        {
            printf("We already have a user with the username: %s\n", newRequest.mutable_newuser()->username().c_str());
            newResponse.set_servermessage("IP already registered");
            canRegister = false;
        }
    }

    if (canRegister)
{
    bool flagExist = false;
    for (int i = 0; i < 10; i++)
    {
        printf("Check Slot: %d\n", i);
        if (strcmp(allClients[i].username, "") == 0)
        {
            printf("New client in server client slot number: %d\n", i);
            slot = i;
            flagExist = true;
            break;
        }
    }

    if (!flagExist)
    {
        printf("Unable to set new client due to don't have enough slot, sorry :(\n", thisThread);
        flagLive = false;
    }
    else
    {
        struct cliente *nuevo_cliente = malloc(sizeof(struct cliente) + strlen(username) + 1);
        strcpy(allClients[slot].username, newRequest->newuser().username().c_str());
        strcpy(allClients[slot].ip, newRequest->newuser().ip().c_str());
        allClients[slot].socket = socketInt;
        allClients[slot].status = 1;
        printf("New client in server client slot number: %d\n", slot);
        printf("Register a new user successfully:\n");
        printf("Username: %s\n", allClients[slot].username);
        printf("IP: %s\n", allClients[slot].ip);
    }
    newResponse->set_code(200);
    newResponse->set_servermessage("User registered successfully");
}
char responseString[2048];
    size_t responseStringLen;

    if (newRequest.option() == 1)
    {
        printf("New Request from user: REGISTER\n");
        printf("User:%s\n", newRequest.mutable_newuser()->username().c_str());
        printf("IP:%s\n", newRequest.mutable_newuser()->ip().c_str());

        chat::ServerResponse newResponse;
        newResponse.set_option(1);
        newResponse.set_code(400);
        newResponse.set_servermessage("Error registering user");

        bool canRegister = true;

        for (int i = 0; i < 10; i++)
        {
            if (strcmp(allClients[i].username, newRequest.mutable_newuser()->username().c_str()) == 0)
            {
                printf("We alredy have a user whit the username: %s\n", newRequest.mutable_newuser()->username().c_str());
                newResponse.set_servermessage("IP already register");
                canRegister = false;
            }
        }

        if (canRegister)
        {
            bool flagExist = false;
            for (int i = 0; i < 10; i++)
            {
                printf("Check Slot: %d\n", i);
                if (strcmp(allClients[i].username, "") == 0)
                {
                    printf("New client in server client slot number: %d\n", i);
                    slot = i;
                    flagExist = true;
                    i = 10;
                }
            }

            if (!flagExist)
            {
                printf("Unable to set new client due to dont have enoght slot, sorry :(\n", thisThread);
                flagLive = false;
            }
            else
            {
                strcpy(allClients[slot].username, newRequest.mutable_newuser()->username().c_str());
                strcpy(allClients[slot].ip, newRequest.mutable_newuser()->ip().c_str());
                allClients[slot].socket = socketInt;
                allClients[slot].status = 1;
                printf("New client in server client slot number: %d\n", slot);
                printf("Register a new user sucesfully:\n");
                printf("Username: %s\n", allClients[slot].username);
                printf("IP: %s\n", allClients[slot].ip);
            }
            newResponse.set_code(200);
            newResponse.set_servermessage("User registered sucesfully");
        }

        newResponse.SerializeToArray(responseString, 2048);
        responseStringLen = newResponse.ByteSizeLong();

        send(socketInt, responseString, responseStringLen, 0);
    }
    else if (newRequest.option() == 2)
    {
        bool type_request = newRequest.mutable_inforequest()->type_request();
        printf("%d", type_request);
    }
    if (type_request == false)
{
    printf("New Request from user: GET SINGLE USER\n");
    printf("User:%s\n", newRequest.newuser().username().c_str());
    printf("IP:%s\n", newRequest.newuser().ip().c_str());

    const char* dataTemp = newRequest.inforequest().user().c_str();
    int dataTempLen = strlen(dataTemp);

    chatserver_response newResponse = CHATSERVER_RESPONSEINIT;
    chatuser_info newUserInfo = CHATUSER_INFOINIT;

    newResponse.option = 2;
    newResponse.code = 200;

    bool flagGetUsers = false;
    for (int i = 0; i < 10; i++)
    {
        if (strcmp(allClients[i].username.c_str(), dataTemp) == 0)
        {
            flagGetUsers = true;

            chatuser_infoinit(&newUserInfo);
            newUserInfo.username = allClients[i].username.c_str();
            newUserInfo.ip = allClients[i].ip.c_str();
            newUserInfo.status = allClients[i].status;

            newResponse.connectedusers = malloc(sizeof(chatuser_info) 1);
            newResponse.connectedusers[0] = &newUserInfo;
            newResponse.n_connectedusers = 1;
        }
    }

    if (flagGetUsers)
    {
        newResponse.servermessage = "Users found";
    }
    else
    {
        newResponse.servermessage = "No users found";
        newResponse.code = 400;
    }

    void* buffer = NULL;
    unsigned int length = chatserver_responseget_packed_size(&newResponse);
    buffer = malloc(length);
    chatserver_responsepack(&newResponse, buffer);

    send(socketInt, buffer, length, 0);

    chatserver_response__free_unpacked(&newResponse, NULL);
    free(buffer);
}
else if (type_request == false)
{
    printf("New Request from user: GET SINGLE USER\n");
    printf("User:%s\n", newRequest.newuser().username().c_str());
    printf("IP:%s\n", newRequest.newuser().ip().c_str());

    const char* dataTemp = newRequest.inforequest().user().c_str();
    int dataTempLen = strlen(dataTemp);

    chatserver_response newResponse = CHATSERVER_RESPONSEINIT;
    chatuser_info newUserInfo = CHATUSER_INFOINIT;

    newResponse.option = 2;
    newResponse.code = 200;

    bool flagGetUsers = false;
    for (int i = 0; i < 10; i++)
    {
        if (strcmp(allClients[i].username.c_str(), dataTemp) == 0)
        {
            flagGetUsers = true;

            chatuser_infoinit(&newUserInfo);
            newUserInfo.username = allClients[i].username.c_str();
            newUserInfo.ip = allClients[i].ip.c_str();
            newUserInfo.status = allClients[i].status;

            newResponse.connectedusers = malloc(sizeof(chatuser_info) 1);
            newResponse.connectedusers[0] = &newUserInfo;
            newResponse.n_connectedusers = 1;
        }
    }

    if (flagGetUsers)
    {
        newResponse.servermessage = "Users found";
    }
    else
    {
        newResponse.servermessage = "No users found";
        newResponse.code = 400;
    }

    void* buffer = NULL;
    unsigned int length = chatserver_responseget_packed_size(&newResponse);
    buffer = malloc(length);
    chatserver_responsepack(&newResponse, buffer);

    send(socketInt, buffer, length, 0);

    chatserver_response__free_unpacked(&newResponse, NULL);
    free(buffer);
}

if (flagGetUsers) {
    newResponse.servermessage = "Users found";
} else {
    newResponse.servermessage = "No users found";
    newResponse.code = 400;
}

char responseString[MAX_RESPONSE_SIZE];
size_t responseStringLength = 0;
chat_ServerResponse_serializeToString(&newResponse, responseString, MAX_RESPONSE_SIZE, &responseStringLength);
send(socketInt, responseString, responseStringLength, 0);

if (newRequest.option == 3) {
    printf("New Request from user: CHANGE STATUS\n");
    printf("User:%s\n", newRequest.newuser.username);
    printf("IP:%s\n", newRequest.newuser.ip);

    chat_ServerResponse newResponse;
    newResponse.option = 3;
    newResponse.code = 400;
    newResponse.servermessage = "Status failed to change";

    const char* dataTemp = newRequest.inforequest.user;
    for (int i = 0; i < 10; i++) {
        if (strcmp(allClients[i].username, dataTemp) == 0) {
            allClients[i].status = newRequest.status.newstatus;
            printf("Thread %lu: User %s changed status to %d\n", thisThread, newRequest.status.username, newRequest.status.newstatus);
            newResponse.code = 200;
            newResponse.servermessage = "Status changed";
            flagLive = false;
        }
    }

    char responseString[MAX_RESPONSE_SIZE];
    size_t responseStringLength = 0;
    chat_ServerResponse_serializeToString(&newResponse, responseString, MAX_RESPONSE_SIZE, &responseStringLength);
    send(socketInt, responseString, responseStringLength, 0);
}
else if (newRequest.option == 4 && newRequest.mutable_message->message_type == true)
{
    printf("New Request from user: NEW MESSAGE ALL\n");
    printf("User:%s\n", newRequest.mutable_newuser->username);
    printf("IP:%s\n", newRequest.mutable_newuser->ip);
    printf("Message:%s\n", newRequest.mutable_message->message);

    chat_ServerResponse newResponse;
    newResponse.option = 4;
    newResponse.code = 200;
    strcpy(newResponse.servermessage, "Sending message");

    char responseString[MAX_BUFFER_SIZE];
    int responseStringLen = serialize_response(&newResponse, responseString);

    send(socketInt, responseString, responseStringLen, 0);
    memset(buffer, 0, sizeof(buffer));

    chat_ServerResponse sentMessage;
    sentMessage.option = 4;
    sentMessage.code = 200;
    strcpy(sentMessage.servermessage, "New message");
    sentMessage.mutable_message = newRequest.mutable_message;
    char sentMsg[MAX_BUFFER_SIZE];
    int sentMsgLen = serialize_response(&sentMessage, sentMsg);
    char* sender = newRequest.mutable_message->sender;

    for (int i = 0; i < 10; i++)
    {
        bool notSameUser = true;
        bool isUserOnline = false;
        if (strcmp(allClients[i].username, sender) == 0)
        {
            notSameUser = false;
        }
        if (allClients[i].status != 0)
        {
            isUserOnline = true;
        }
        if (!notSameUser && isUserOnline)
        {
            send(allClients[i].socket, sentMsg, sentMsgLen, 0);
            newRequest.mutable_message->sender;
        }
    }
}else if (newRequest.option() == 4 && newRequest.mutable_message()->message_type() == false)
{
    printf("New Request from user: NEW MESSAGE PRIVATE\n");
    printf("User:%s\n", newRequest.mutable_newuser()->username().c_str());
    printf("IP:%s\n", newRequest.mutable_newuser()->ip().c_str());
    printf("Message:%s\n", newRequest.mutable_message()->message());
    printf("Para:%s\n", newRequest.mutable_message()->recipient());

    chat_ServerResponse newResponse = chat_ServerResponse_init_zero;
    newResponse.option = 4;
    newResponse.code = 200;
    newResponse.servermessage = "Message sent";

    size_t responseString_size;
    uint8_t* responseString_buffer;

    flatbuffers::FlatBufferBuilder builder;
    auto response_offset = chat_CreateServerResponse(builder, newResponse);
    builder.Finish(response_offset);
    responseString_buffer = builder.GetBufferPointer();
    responseString_size = builder.GetSize();

    send(socketInt, responseString_buffer, responseString_size, 0);

    memset(buffer, 0, sizeof(buffer));

    chat_ServerResponse sentMessage = chat_ServerResponse_init_zero;
    sentMessage.option = 4;
    sentMessage.code = 200;
    sentMessage.servermessage = "New message";
    chat_Message message = chat_Message_init_zero;
    message.sender = newRequest.mutable_message()->sender()->c_str();
    message.message = newRequest.mutable_message()->message()->c_str();
    message.recipient = newRequest.mutable_message()->recipient()->c_str();
    sentMessage.message = &message;

    size_t sentMsg_size;
    uint8_t* sentMsg_buffer;

    flatbuffers::FlatBufferBuilder builder2;
    auto sentMessage_offset = chat_CreateServerResponse(builder2, sentMessage);
    builder2.Finish(sentMessage_offset);
    sentMsg_buffer = builder2.GetBufferPointer();
    sentMsg_size = builder2.GetSize();

    for (int i = 0; i < 100; i++)
    {
        if (strcmp(allClients[i].username, newRequest.mutable_message()->recipient()->c_str()) == 0)
        {
            if (allClients[i].status != 0)
            {
                send(allClients[i].socket, sentMsg_buffer, sentMsg_size, 0);
                flagLive = false;
            }
            else
            {
                printf("Request failed, the destinatary is offline\n");
                printf("Para:%s\n", newRequest.mutable_message()->recipient()->c_str());

                chat_ServerResponse newResponse;
                newResponse.option = 4;
                newResponse.code = 400;
                newResponse.servermessage = "User offline";

                size_t responseString_size;
                uint8_t* responseString_buffer;

                flatbuffers::FlatBufferBuilder builder3;
                auto response_offset = chat_CreateServerResponse(builder3, newResponse);
                builder3.Finish(response_offset);
                responseString_buffer = builder3.GetBufferPointer();
                responseString_size = builder3.GetSize();

                send(socketInt, responseString_buffer, responseString_size, 0);
            }
        }
    }
}

else if (newRequest.option == 5) {
    // Heartbeat
    printf("New Request from user: HEARTBEAT\n");
    printf("User:%s\n", newRequest.newuser.username);
    printf("IP:%s\n", newRequest.newuser.ip);

    chat_ServerResponse newResponse;
    newResponse.option = 5;
    newResponse.code = 200;
    strcpy(newResponse.servermessage, "Heartbeat ok");

    char responseString[MAX_RESPONSE_SIZE];
    size_t responseStringLen = 0;

    // Serialize the response message
    if (!serialize_chat_ServerResponse(&newResponse, responseString, &responseStringLen)) {
        fprintf(stderr, "Failed to serialize response message\n");
        return;
    }

    // Send the response message back to the client
    if (send(socketInt, responseString, responseStringLen, 0) < 0) {
        fprintf(stderr, "Failed to send response message\n");
    }
} else {
    printf("New Request from user: INVALID OPTION\n");
    printf("User:%s\n", newRequest.newuser.username);
    printf("IP:%s\n", newRequest.newuser.ip);
}

    }

    munmap(&reading, sizeof(bool));
    munmap(&dieT, sizeof(bool));

    pthread_exit(NULL);



}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }
    int usagePort = atoi(argv[1]);

    struct sockaddr_in serverAddress;
    int socket_server;
    int optval = 1;

    socket_server = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_server < 0)
    {
        perror("Error creating socket");
        return 1;
    }

    if (setsockopt(socket_server, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        perror("Error setting socket options");
        close(socket_server);
        return 1;
    }

    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(usagePort);

    if (bind(socket_server, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("Error binding socket");
        close(socket_server);
        return 1;
    }

    if (listen(socket_server, MAX_PENDING_CONNECTIONS) < 0)
    {
        perror("Error listening on socket");
        close(socket_server);
        return 1;
    }

    printf("-------------------\n");
    printf("Server connected!\n");
    printf("-------------------\n");

    while (1)
    {
        struct sockaddr_in clientAddress;
        socklen_t clientSize = sizeof(clientAddress);
        int socketAccept = accept(socket_server, (struct sockaddr *)&clientAddress, &clientSize);

        if (socketAccept < 0)
        {
            perror("Error accepting connection");
            close(socket_server);
            return 1;
        }

        printf("-------------------\n");
        printf("Client connected from %s:%d\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
        printf("-------------------\n");

        pthread_t thread;
        if (pthread_create(&thread, NULL, handler, (void *)&socketAccept) != 0)
        {
            perror("Error creating thread");
            close(socket_server);
            return 1;
        }
    }

}


    close(socket_server);
    return 0;
}