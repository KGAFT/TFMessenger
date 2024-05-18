//
// Created by kgaft on 5/16/24.
//

#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <apr_network_io.h>
#include <stdexcept>
#include <Memory/MemoryPoolManager.h>
#include <Logging/Logger.h>
#include "Threads/Thread.h"


class ServerSocket {
public:
    ServerSocket(uint32_t port, apr_int32_t family) {
        memPool = MemoryPoolManager::acquireMemoryPool();

        apr_status_t retStatus = apr_sockaddr_info_get(&sa, nullptr, family, port, 0, memPool);
        if (retStatus != APR_SUCCESS) {
            Logger::logAPRError(retStatus);
            throw std::runtime_error("Failed to create server");
        }

        retStatus = apr_socket_create(&listenSocket, sa->family, SOCK_STREAM, APR_PROTO_TCP, memPool);

        if (retStatus != APR_SUCCESS) {
            Logger::logAPRError(retStatus);
            throw std::runtime_error("Failed to create server");

        }

        apr_socket_opt_set(listenSocket, APR_SO_NONBLOCK, 0);
        apr_socket_timeout_set(listenSocket, -1);
        apr_socket_opt_set(listenSocket, APR_SO_REUSEADDR, 1);

        retStatus = apr_socket_bind(listenSocket, sa);
        if (retStatus != APR_SUCCESS) {
            Logger::logAPRError(retStatus);
            throw std::runtime_error("Failed to create server");
        }

        Logger::logMessage(new Message{std::string("Successfully created server on port: ")+std::to_string(port), "TFMessengerServer", Logger::currentTime(), MESSAGE_VERBOSE});
    }

private:
    apr_socket_t *listenSocket = nullptr;
    apr_pool_t *memPool = nullptr;
    apr_sockaddr_t *sa = nullptr;

public:
    apr_status_t serverListen() {
        apr_status_t retStatus = apr_socket_listen(listenSocket, SOMAXCONN);
        if (retStatus != APR_SUCCESS) {
            Logger::logAPRError(retStatus);
        }
        return retStatus;
    }

    apr_status_t acceptClient() {
        apr_socket_t *ns;
        apr_status_t retStatus = apr_socket_accept(&ns, listenSocket, memPool);
        if (retStatus != APR_SUCCESS) {
            Logger::logAPRError(retStatus);
            return retStatus;
        }

        apr_socket_opt_set(ns, APR_SO_NONBLOCK, 0);
        apr_socket_timeout_set(ns, -1);
        Thread thread;
        thread.run(processConnection, ns);

        return retStatus;
    }

    ~ServerSocket() {
        apr_socket_close(listenSocket);
        MemoryPoolManager::poolRelease(memPool);
    }

private:
    static void processConnection(void *data) {
        apr_socket_t *sock = (apr_socket_t *) data;

        while (true) {
            char buf[4096];
            apr_size_t len = sizeof(buf) - 1; /* -1 for a null-terminated */

            apr_status_t rv = apr_socket_recv(sock, buf, &len);

            if (rv != APR_SUCCESS) {
                Logger::logAPRError(rv);
                Logger::logMessage(new Message{std::string("Client disconnected")+std::to_string(rv), "TFMessengerServer", Logger::currentTime(), MESSAGE_VERBOSE});
                apr_socket_close(sock);
                return;
            }

            if (len > 0) {
                printf("Read: %s\n", buf);
            }

            buf[len] = '\0'; /* apr_socket_recv() doesn't return a null-terminated string */
        }
    }


};


#endif //SERVERSOCKET_H
