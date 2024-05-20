//
// Created by kgaft on 5/16/24.
//
#include <apr-1/apr_general.h>
#include <apr-1/apr_pools.h>
#include <apr-1/apr_file_io.h>
#include <apr-1/apr_strings.h>
#include <apr-1/apr_network_io.h>
#include <apr-1/apr_thread_proc.h>


int main() {
    apr_initialize();
    apr_pool_initialize();
    apr_pool_t* pool;
    apr_pool_create(&pool, NULL);

    apr_socket_t* client_sock;
    apr_socket_create(&client_sock, APR_INET6, SOCK_STREAM, APR_PROTO_TCP, pool);

    apr_sockaddr_t* server_addr;
    apr_sockaddr_info_get(&server_addr, "localhost", APR_INET6, 8090, 0, pool);

    apr_socket_connect(client_sock, server_addr);

    const char* msg = "Hello, world!";
    apr_size_t len = strlen(msg);
    apr_socket_send(client_sock, msg, &len);
    while(true);
    apr_socket_close(client_sock);

    apr_pool_destroy(pool);
    apr_terminate();
    return 0;
}