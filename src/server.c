#include "server.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char webpage[] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=UTF-8\r\n\r\n"
    "<!DOCTYPE html:\r\n>"
    "<html><head><title>C-Web-Server</title>\r\n"
    "<style> body { background-color: #FF0 } </style>\r\n"
    "<body><h1>Hello World!</h1><p>this is a test</p>\r\n"
    "</body>\r\n";

int create_ipv4_socket() { return socket(AF_INET, SOCK_STREAM, 0); }

int main(int argc, char const *argv[]) {
    log_event("EVENT", "Inicializando servidor");

    struct sockaddr_in server_addr, client_addr;

    socklen_t sin_len = sizeof(client_addr);
    int fd_server, fd_client;
    char buf[2048];
    int on = 1;

    fd_server = create_ipv4_socket();
    if (fd_server < 0) {
        log_event("ERROR", "Erro ao criar o socket");
        perror("socket");
        exit(1);
    }
    log_event("EVENT", "Socket criado");

    setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(fd_server, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
        -1) {
        log_event("ERROR", "Erro ao vincular o socket à porta TCP");
        perror("bind");
        close(fd_server);
        exit(1);
    }
    log_event("EVENT", "Socket vinculado à porta TCP");

    if (listen(fd_server, 10) == -1) {
        log_event("ERROR", "Erro ao ativar modo de escuta do socket");
        perror("listen");
        close(fd_server);
        exit(1);
    }
    log_event("EVENT", "Modo de escuta do socket ativado");

    while (1) {
        fd_client =
            accept(fd_server, (struct sockaddr *)&client_addr, &sin_len);

        if (fd_client == -1) {
            log_event("ERROR", "Erro na conexão com o cliente");

            perror("Connection failed...\n");
            continue;
        }
        log_event("EVENT", "Conexão com o cliente aberta");

        printf("Got client connection...\n");

        if (!fork()) {
            close(fd_server);
            memset(buf, 0, 2048);
            read(fd_client, buf, 2047);

            printf("%s\n", buf);

            write(fd_client, webpage, sizeof(webpage) - 1);
            log_event("EVENT", "Página enviada ao cliente");

            close(fd_client);
            log_event("EVENT", "Conexão com o cliente encerrada");

            printf("Closing...\n");
            exit(0);
        }
        close(fd_client);
    }

    return 0;
}
