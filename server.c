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

// Criando resposta HTTP para GET / (formato text/html)
char webpage[] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=UTF-8\r\n\r\n"
    "<!DOCTYPE html:\r\n>"
    "<html><head><title>C-Web-Server</title>\r\n"
    "<style> body { background-color: #FF0 } </style>\r\n"
    "<body><h1>Hello World!</h1><p>this is a test</p>\r\n"
    "<img src=\"doctest.jpg\"></body>\r\n";

int main(int argc, char const *argv[]) {
    // Criando estruturas para armazenamento das informações de endereço
    struct sockaddr_in server_addr, client_addr;

    //
    socklen_t sin_len = sizeof(client_addr);
    // 'fd' significa file descriptor pode ser READ, WRITE, READ/WRITE
    // sempre será um inteiro positivo
    int fd_server, fd_client;
    //
    char buf[2048];
    //
    int fdimg;
    //
    int on = 1;

    // Cria um endpoint para estabelecer uma comunicação
    fd_server = socket(AF_INET, SOCK_STREAM, 0);
    // Função socket retorna -1 em caso de erro, verificamos se houve,
    // caso sim encerra-se o programa
    if (fd_server < 0) {
        perror("socket");
        exit(1);
    }

    // Permite a reutilização do socket para multiplas chamadas TCP
    setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

    // Define protocolo IP para o servidor (AF_NET = IPv4)
    server_addr.sin_family = AF_INET;
    // Define endereços permitidos pra estabelecer conexão (INADDR_ANY =
    // qualquer endereço)
    server_addr.sin_addr.s_addr = INADDR_ANY;
    // Define porta de comunicação do servidor
    server_addr.sin_port = htons(8080);

    // O bind vincula a porta com o socket criado
    if (bind(fd_server, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
        -1) {
        perror("bind");
        close(fd_server);
        exit(1);
    }

    // Ativa o socket para receber conexões
    if (listen(fd_server, 10) == -1) {
        perror("listen");
        close(fd_server);
        exit(1);
    }

    // Loop de escuta
    while (1) {
        // Quando houver requisição estabelece conexão requisitada pelo client
        fd_client =
            accept(fd_server, (struct sockaddr *)&client_addr, &sin_len);

        if (fd_client == -1) {
            perror("Connection failed...\n");
            continue;
        }

        printf("Got client connection...\n");

        // Se fork retornar 0 executa o if

        // pai = fd socket -> fecha a porta do cliente -> sobra a porta do
        //// servidor (aguarda nova conexão) filho = copia do fd socket -> fecha
        /// a
        // porta do pai -> sobra a porta do cliente
        ////-> fecha filho e encerra

        if (!fork()) {
            // Child process
            close(fd_server);
            //
            memset(buf, 0, 2048);
            read(fd_client, buf, 2047);

            printf("%s\n", buf);

            // Escreve na stream do socket a página
            write(fd_client, webpage, sizeof(webpage) - 1);

            // Fecha o socket
            close(fd_client);
            printf("Closing...\n");
            // Encerra o processo filho
            exit(0);
        }
        // Parent process
        // Encerra a comunicação com o cliente
        close(fd_client);
    }

    return 0;
}
