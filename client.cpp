#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fstream>
using namespace std;

#define PORT 12345
#define SERVER_IP "127.0.0.1"

ofstream chatLogFile;

void *receiveServerMessage(void *clientSocket)
{
    int clientSock = *(int *)clientSocket;
    char buffer[1024];
    string messageFromServer;
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSock, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0)
        {
            cout << "Server disconnected\n";
            close(clientSock);
            exit(EXIT_FAILURE);
        }
        messageFromServer = "Server: " + string(buffer);
        cout << messageFromServer << "\n";
        chatLogFile << messageFromServer << "\n";
    }
    pthread_exit(NULL);
}

void *sendServerMessage(void *clientSocket)
{
    int clientSock = *(int *)clientSocket;
    char buffer[1024];
    string messageToServer;
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        cin.getline(buffer, sizeof(buffer));
        messageToServer = "Client: " + string(buffer);
        chatLogFile << messageToServer << "\n";
        send(clientSock, (char *)&buffer, strlen(buffer), 0);
    }
    pthread_exit(NULL);
}

int main()
{
    int clientSocket;
    struct sockaddr_in serverAddr;
    pthread_t thread;
    chatLogFile.open("chat_log.txt", ios::app);
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        cout << "Error creating socket\n";     
        return -1;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0)
    {
        cout << "Invalid address\n";        
        close(clientSocket);
        return -1;
    }
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        cout << "Error connecting to server\n";      
        close(clientSocket);
        return -1;
    }
    if (pthread_create(&thread, NULL, receiveServerMessage, (void *)&clientSocket) != 0)
    {
        cout << "Error creating receiveServerMessage thread\n";        
        close(clientSocket);
        return -1;
    }
    if (pthread_create(&thread, NULL, sendServerMessage, (void *)&clientSocket) != 0)
    {
        cout << "Error creating sendServerMessage thread\n";        
        close(clientSocket);
        return -1;
    }
    while (true){}
    close(clientSocket);
    chatLogFile.close();
    return 0;
}
