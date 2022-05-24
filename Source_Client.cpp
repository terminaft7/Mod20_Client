#include <iostream> 
#include "Message.h"
#include "User.h"
#include "Userpool.h"
#include <vector>
#include <map>
#include "chatpool.h"
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
using namespace std;


#define MESSAGE_BUFFER 4096 // Максимальный размер буфера для приема и передачи
#define PORT 7777 // Номер порта, который будем использовать для приема и передачи 


char buffer[MESSAGE_BUFFER];
//char message[MESSAGE_BUFFER];
int socket_descriptor;
struct sockaddr_in serveraddress;

void sendRequest(string message) {
    // Укажем адрес сервера
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Зададим номер порта для соединения с сервером
    serveraddress.sin_port = htons(PORT);
    // Используем IPv4
    serveraddress.sin_family = AF_INET;
    // Создадим сокет 
    socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    // Установим соединение с сервером
    if (connect(socket_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress)) < 0) {
        cout << endl << " Something went wrong Connection Failed" << endl;
        exit(1);
    }

    while (1) {
        cout << "Enter a message you want to send to the server: " << endl;
        //cin >> message;
        if (strcmp(message, "end") == 0) {
            sendto(socket_descriptor, message, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
            cout << "Client work is done.!" << endl;
            close(socket_descriptor);
            exit(0);
        }
        else {
            sendto(socket_descriptor, message, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
            cout << "Message sent successfully to the server: " << message << endl;
            cout << "Waiting for the Response from Server..." << endl;
        }
        cout << "Message Received From Server: " << endl;
        recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, nullptr, nullptr);
        cout << buffer << endl;
    }
    // закрываем сокет, завершаем соединение
    close(socket_descriptor);
}



int main()
{
    char action('a');
    Userpool Userpool_vect;
    chatpool chatsinmap;

    while (action != 'q')
    {
        cout << "Press 1 to login with your nickname and password " << endl;
        cout << "Press 2 to create new user and send message " << endl;
        cout << "Press 3 to see all users." << endl;
        cout << "Press q to quit" << endl;
        cin >> action;

        if (action == 'q')
            break;
        string nickname;
        string password;
        string name;
        string receiver;
        char message[100];
        int ind(0);
        
        switch (action)
        {
        case '1':
            if (chatsinmap.chatpool_vect.empty()) {
                cout << "There are no users or messages. Please create a new user." << endl;
                break;
            }
            cout << "Enter your nickname" << endl;
            cin >> nickname;
            cout << "Enter your password" << endl;
            cin >> password;

            ind = Userpool_vect.checkUser(nickname, password);
            if (ind == true) {
                cout << "Enter receiver" << endl;
                cin >> receiver;
               chatsinmap.showmap(receiver, nickname);
                cin.ignore();
                std::cin.getline(message, 4096);
                chatsinmap.sendmessage(receiver, nickname, message);
                cout << "SERVER IS LISTENING THROUGH THE PORT: " << PORT << " WITHIN A LOCAL SYSTEM" << endl;
                // Запускаем функцию обработки сообщений от клиентов и ответа на эти сообщения
                cout << "CLIENT IS ESTABLISHING A CONNECTION WITH SERVER THROUGH PORT: " << PORT << endl;
                sendRequest(message);
                                    
                break;
            }
            else break;
                       
        case '2': {

            cout << "Enter your nickname" << endl;
            cin >> nickname;
            if (nickname == "all") {
                cout << "This nickname is not allowed. Please try again." << endl;
                break;
            }
            cout << "Enter your password" << endl;
            cin >> password;

            Userpool_vect.addUser(User(nickname, password));
                 
            cout << "Type receiver, type enter, then type your message." << endl;
            cout << "Write all as a receiver to send message to all users." << endl;
            cout << "Enter receiver" << endl;
            cin >> receiver;
            cin.ignore();
            std::cin.getline(message, 100);           
            chatsinmap.sendmessage(receiver, nickname, message);
                                  
            break; }

        case '3':

            if (Userpool_vect.Userpool_vect.empty())
            {
                cout << "No users" << endl;
                break;
            }
            
            Userpool_vect.showUsers();
            break;
             
        default: break;
        }
    }
}