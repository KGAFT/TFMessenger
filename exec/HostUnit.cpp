#include <Sockets/ServerSocket.h>

int main() {
    ServerSocket socket(8090, APR_INET6);
    socket.serverListen();
    while(true) {
        socket.acceptClient();
    }
}