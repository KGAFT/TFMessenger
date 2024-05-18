//
// Created by easy on 5/19/2024.
//

#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <apr_network_io.h>
#include <cstdint>

#include "Logging/Logger.h"

class ClientSocket {
private:
    ClientSocket(apr_socket_t *socket, apr_interface_e type)
        : socket(socket), type(type) {
    }

private:
    apr_socket_t *socket;
    bool connected = true;
    apr_interface_e type;
public:
    bool readData(char* buffer, size_t* pSize) {
        apr_status_t res = apr_socket_recv(socket, buffer, pSize);
        if(res!=APR_SUCCESS) {
            connected = false;
            Logger::logAPRError(res);
        }
        return connected;
    }
    bool writeData(char* data, size_t dataSize) {
        if(connected) {
            apr_status_t res = apr_socket_send(socket, data, &dataSize);
            connected = false;
            Logger::logAPRError(res);
        }
        return connected;
    }
    apr_sockaddr_t* getSocketInfo() {
        apr_sockaddr_t* data;
        apr_socket_addr_get(&data, type, socket);
        return data;
    }
};



#endif //CLIENTSOCKET_H
