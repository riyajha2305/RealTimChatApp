## A realtime chatting system using Sockets & Threads
- The server and client will be talking to each other in real time for an infinite period until one of them is closed (by pressing Ctrl+C on the terminal).
- Threads is used for communication between server and client.
- Also, all the chat messages from the server and client are labeled and stored into a file for historical reference.
- Threads are used for saving data!

## To run the project

#### - to run server.cpp
```
g++ -std=c++20 -pthread -o server server.cpp
./server
```

#### - to run client.cpp
```
g++ -std=c++20 -pthread -o client client.cpp
./client
```
