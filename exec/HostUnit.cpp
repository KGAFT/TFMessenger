#include <Sockets/ServerSocket.h>

#include "Logging/Logger.h"

int main() {
    apr_initialize();
    Logger::initializeLogger("serverlog.txt");

    ServerSocket socket(8090, APR_INET6);
    socket.serverListen();
    while(true) {
        socket.acceptClient();
    }
}
