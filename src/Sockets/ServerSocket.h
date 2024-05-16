//
// Created by kgaft on 5/16/24.
//

#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <apr_pools.h>
#include <apr_network_io.h>
#include <apr_thread_proc.h>
#include <stdexcept>
#include <Memory/MemoryPoolManager.h>

class ServerSocket {
public:
    ServerSocket(uint32_t port, apr_int32_t family) {
        memPool = MemoryPoolManager::acquireMemoryPool();

        apr_threadattr_create(&thd_attr, memPool);

        apr_status_t retStatus = apr_sockaddr_info_get(&sa, nullptr, family, port, 0, memPool);
        if (retStatus != APR_SUCCESS) {
            printAPRError(retStatus);
        }

        retStatus = apr_socket_create(&listenSocket, sa->family, SOCK_STREAM, APR_PROTO_TCP, memPool);

        if (retStatus != APR_SUCCESS) {
            printAPRError(retStatus);
        }

        apr_socket_opt_set(listenSocket, APR_SO_NONBLOCK, 0);
        apr_socket_timeout_set(listenSocket, -1);
        apr_socket_opt_set(listenSocket, APR_SO_REUSEADDR, 1);

        retStatus = apr_socket_bind(listenSocket, sa);
        if (retStatus != APR_SUCCESS) {
            printAPRError(retStatus);
        }
    }

private:
    apr_socket_t *listenSocket = nullptr;
    apr_pool_t *memPool = nullptr;
    apr_threadattr_t *thd_attr = nullptr;
    apr_sockaddr_t *sa = nullptr;

public:
    void serverListen() {
        apr_status_t retStatus = apr_socket_listen(listenSocket, SOMAXCONN);
        if (retStatus != APR_SUCCESS) {
            printAPRError(retStatus);
        }
    }

    void acceptClient() {
        apr_socket_t *ns; /* accepted socket */

        apr_status_t retStatus = apr_socket_accept(&ns, listenSocket, memPool);
        if (retStatus != APR_SUCCESS) {
            printAPRError(retStatus);
        }

        apr_socket_opt_set(ns, APR_SO_NONBLOCK, 0);
        apr_socket_timeout_set(ns, -1);

        //Create the new thread
        apr_thread_t *thd_obj;
        retStatus = apr_thread_create(&thd_obj, NULL, processConnection, ns, memPool);

        if (retStatus != APR_SUCCESS) {
            printAPRError(retStatus);
        }
    }
    ~ServerSocket() {
        apr_socket_close(listenSocket);
        MemoryPoolManager::poolRelease(memPool);
    }

private:
    static void * APR_THREAD_FUNC processConnection(apr_thread_t *thd, void *data) {
        apr_socket_t *sock = (apr_socket_t *) data;

        while (true) {
            char buf[4096];
            apr_size_t len = sizeof(buf) - 1; /* -1 for a null-terminated */

            apr_status_t rv = apr_socket_recv(sock, buf, &len);

            if (rv == APR_EOF || len == 0) {
                printf("Socket Closed\n");
                apr_socket_close(sock);
                apr_thread_exit(thd, 0);
            }

            if (len > 0) {
                printf("Read: %s\n", buf);
            }

            buf[len] = '\0'; /* apr_socket_recv() doesn't return a null-terminated string */
        }
    }

    static void printAPRError(apr_status_t status) {
        char errbuf[256];
        apr_strerror(status, errbuf, sizeof(errbuf));
        printf("error: %d, %s\n", status, errbuf);
    }
};


#endif //SERVERSOCKET_H
