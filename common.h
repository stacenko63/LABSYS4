#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <pthread.h>
#include "check.hpp"
using namespace std;
constexpr unsigned short SERVER_PORT = 60002;
constexpr int SOCKET_TYPE = SOCK_STREAM;
ssize_t Recv(int sock_fd, void* buffer, size_t n, int flags) {
    ssize_t size = recv(sock_fd, buffer, n, flags);
    check(size);
    return size;
}
inline ostream& operator<<(ostream& s, const sockaddr_in& addr) {
    union {
        in_addr_t x;
        char c[sizeof(in_addr)];
    }t{};
    t.x = addr.sin_addr.s_addr;
    return s << to_string(int(t.c[0])) << "." << to_string(int(t.c[1])) << "." << to_string(int(t.c[2])) << "." << to_string(int(t.c[3]))
             << ":" << to_string(ntohs(addr.sin_port));
}
inline int make_socket(int type) {
    switch (type) {
        case SOCK_STREAM:
            return socket(AF_INET, SOCK_STREAM, 0);
        case SOCK_SEQPACKET:
            return check(socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP)); // analogue to SOCK_SEQPACKET
        default:
            errno = EINVAL;
            return -1;
    }
}
inline sockaddr_in local_addr(unsigned short port) {
    sockaddr_in addr{};
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);//127.0.0.1
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    return addr;
}

