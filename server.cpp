#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fstream>
using namespace std;

#define PORT 12345
#define MAX_CLIENTS 10

ofstream chatLogFile;

void *receiveClientMessage(void *clientSocket)
{
    int clientSock = *(int *)clientSocket;
    char buffer[1024];
    string data;
    string messageFromClient;
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSock, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0)
        {
            cout << "Client disconnected\n";
            close(clientSock);
            exit(EXIT_FAILURE);
        }
        messageFromClient = "Client: " + string(buffer);
        cout << messageFromClient << "\n";
        chatLogFile << messageFromClient << "\n";
    }
    pthread_exit(NULL);
}

void *sendClientMessage(void *clientSocket)
{
    int clientSock = *(int *)clientSocket;
    char buffer[1024];
    string messageToClient;
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        cin.getline(buffer, sizeof(buffer));
        messageToClient = "Server: " + string(buffer);
        chatLogFile << buffer << "\n";
        send(clientSock, (char *)&buffer, strlen(buffer), 0);
    }
    pthread_exit(NULL);
}

int main()
{
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    pthread_t thread;
    chatLogFile.open("chat_log.txt", ios::app);
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        cout << "Error creating socket\n";
        return -1;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    if (::bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        cout << "Error binding socket\n";
        close(serverSocket);
        return -1;
    }
    listen(serverSocket, MAX_CLIENTS);
    cout << "Server listening on port " << PORT << "\n";
    while (true)
    {
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientLen);
        if (clientSocket == -1)
        {
            cout << "Error accepting connection\n";
            continue;
        }
        if (pthread_create(&thread, NULL, receiveClientMessage, (void *)&clientSocket) != 0)
        {
        if (pthread_create(&thread, NULL, receiveClientMessage, (void *)&clientSocket) != 0)
            cout << "Error creating receiveClientMessage thread\n";
            close(clientSocket);
        }
        if (pthread_create(&thread, NULL, sendClientMessage, (void *)&clientSocket) != 0)
        {
            cout << "Error creating sendClientMessage thread\n";
            close(clientSocket);
        }
    }
    chatLogFile.close();
    close(serverSocket);
    return 0;
}
