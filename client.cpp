#include "common.h"
char buffer[100];
void input_correctly_number(int& aa) {
    while (!(cin >> aa) || cin.peek() != '\n') {
        cin.clear();
        cin.ignore(cin.rdbuf()->in_avail());
        while(cin.get() != '\n') {}
        cout << "An incorrect value was entered. Please try again: ";
    }
}
void Limitations(int min, int max, int& value) {
    while (value < min || value > max) {
        cout << "The entered number does not correspond to the interval. Repeat the input: ";
        input_correctly_number(value);
    }
}
int main() {
    auto dest_address = local_addr(SERVER_PORT);
    int sock_fd = check(make_socket(SOCKET_TYPE));
    check(connect(sock_fd, (sockaddr*)&dest_address, sizeof(dest_address)));
    Recv(sock_fd, buffer, sizeof(buffer), 0);
    cout << buffer;
    while (true) {
        int menu = 0;
        input_correctly_number(menu);
        Limitations(0, 1, menu);
        if (menu == 0) {
            break;
        }
        else if (menu == 1) {
            send(sock_fd, to_string(menu).c_str(), to_string(menu).size() + 1, MSG_WAITALL);
            Recv(sock_fd, buffer, sizeof(buffer), 0);
            int value = 0;
            while (true) {
                cout << buffer;
                input_correctly_number(value);
                send(sock_fd, to_string(value).c_str(), to_string(value).size() + 1, MSG_WAITALL);
                Recv(sock_fd, buffer, sizeof(buffer), 0);
                if (string(buffer).find("You guessed the number!") != string::npos) {
                    cout << string(buffer);
                    break;
                }
            }
        }
    }
    shutdown(sock_fd, SHUT_RDWR);
    close(sock_fd);
}
//g++ -std=c++17 client.cpp -o client
