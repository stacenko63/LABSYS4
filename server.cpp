#include "common.h"
char buffer[100];
sockaddr_in server_address;
int listening_socket;
pthread_t array[100];
int process_count = 0;
pthread_mutex_t mutex;
int sockets[100];
int sockets_count = 0;
void* work(void* arg) {
    pair<int, sockaddr_in> arguments = pair<int, sockaddr_in>(*(pair<int, sockaddr_in>*)arg);
    int connected_socket = arguments.first;
    sockaddr_in connected_address = arguments.second;
    cout << "Connected from " << connected_address << endl;
    string message = "Welcome to server!\n1 - Begin game\n0 - Exit\nEnter your choice: ";
    send(connected_socket, message.c_str(), message.size() + 1, MSG_WAITALL);
    int min_value = 1, max_value = 20;
    while (true) {
        if (Recv(connected_socket, buffer, sizeof(buffer), 0) == 0) {
            break;
        }
        cout << connected_address << " send a message : " << buffer << endl;
        if (buffer[0] == '1') {
            int trying_count = 1;
            int result = min_value + rand() % (max_value - min_value + 1);
            cout << "Begin game for user " << connected_address << endl << "Guessed number: " << result << "\n\n\n";
            message = "I made a number from " + to_string(min_value) + " to " + to_string(max_value) + ". Enter the number: ";
            send(connected_socket, message.c_str(), message.size() + 1, MSG_WAITALL);
            while (true) {
                if (Recv(connected_socket, buffer, sizeof(buffer), 0) == 0) {
                    break;
                }
                string trying(buffer);
                cout << "User: " << connected_address << "\nGuessed number: " << result << "\nTrying number: " << trying_count << "\nTrying value: " << stoi(trying) << "\n";
                if (stoi(trying) == result) {
                    cout << "Trying result: ACCESS!\n\n\n";
                    message = "You guessed the number!\n1 - Begin new game\n0 - Exit\nEnter your choice: ";
                    send(connected_socket, message.c_str(), message.size() + 1, MSG_WAITALL);
                    break;
                }
                else {
                    cout << "Trying result: FAIL!\n\n\n";
                    trying_count++;
                    message = "You didn't guess the number. Try again: ";
                    send(connected_socket, message.c_str(), message.size() + 1, MSG_WAITALL);
                }

            }
        }
        else if (buffer[0] == '0') {
            cout << "Disconnected from " << connected_address << endl;
        }
    }

    return NULL;
}
int main() {
    srand(time(NULL));
    server_address = local_addr(SERVER_PORT);
    listening_socket = check(make_socket(SOCKET_TYPE));
    int connected_socket = 0;
    sockaddr_in connected_address{};
    pthread_mutex_init(&mutex, NULL);
    socklen_t addrlen;
    check(bind(listening_socket, (sockaddr*)&server_address, sizeof(server_address)));
    check(listen(listening_socket, 2));
    while (true) {
        addrlen = sizeof(connected_socket);
        connected_socket = check(accept(listening_socket, (sockaddr*)&connected_address,&addrlen));
        pthread_mutex_lock(&mutex);
        sockets[sockets_count++] = connected_socket;
        process_count++;
        pthread_mutex_unlock(&mutex);
        pair<int, sockaddr_in> p = pair<int, sockaddr_in>(connected_socket, connected_address);
        pthread_create(&array[process_count-1], NULL, &work, &p);

    }
    pthread_mutex_destroy(&mutex);
}
//g++ -std=c++17 server.cpp -o server