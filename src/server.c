// biblioteca e arquivo .c
#include "server.h"

// Criando string HTTP.
char webpage[] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=UTF-8\r\n\r\n"
    "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta "
    "http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta "
    "name=\"viewport\" content=\"width=device-width, "
    "initial-scale=1.0\"><title>InCcure-Api</title><script "
    "src=\"https://cdn.tailwindcss.com\"></script></head><body><div "
    "class=\"min-h-screen bg-gray-50 py-6 flex flex-col justify-center "
    "relative overflow-hidden sm:py-12\"><img src=\"/img/beams.jpg\" alt=\"\" "
    "class=\"absolute top-1/2 left-1/2 -translate-x-1/2 -translate-y-1/2 "
    "max-w-none\" width=\"1308\" /><div class=\"absolute inset-0 "
    "bg-[url(/img/grid.svg)] bg-center "
    "[mask-image:linear-gradient(180deg,white,rgba(255,255,255,0))]\"></"
    "div><div class=\"relative px-6 pt-10 pb-8 bg-white shadow-xl ring-1 "
    "ring-gray-900/5 sm:max-w-lg sm:mx-auto sm:rounded-lg sm:px-10\"><div "
    "class=\"max-w-md mx-auto\"><div class=\"py-8 text-base leading-5 "
    "space-y-0 text-gray-600\"><div class=\"w-80 ...\" ><p>IFSC - Instituto "
    "Federal De Santa Catarina</p></div><div class=\"divide-y "
    "divide-gray-300/50\"><div class=\"py-8 text-base leading-7 space-y-6 "
    "text-gray-600 	\"><p>Alunos: Arthur Cadore e Gabriel Espindola</p><ul "
    "class=\"space-y-4\"><code class=\"text-sm font-bold "
    "text-gray-900\">Descrição do Projeto: </code><ul class=\"space-y-4\"><li "
    "class=\"flex items-center\">O objetivo deste projeto será desenvolver um "
    "servidor HTTP a partir da linguagem “C”. </li><ul class=\"space-y-4\"><li "
    "class=\"flex items-center\"> O servidor irá implementar apenas o método "
    "“GET” do protocolo HTTP (versão 1.1), dessa forma, irá apenas coletar o "
    "body dos endpoints cadastrados no servidor. </li><li class=\"flex "
    "items-center\"> O servidor operará a partir da abertura de sockets TCP "
    "para envio e recebimento de dados entre cliente e servidor, dessa "
    "maneira, o servidor deve: </li><li class=\"flex items-center\"><svg "
    "class=\"w-6 h-6 flex-none fill-sky-100 stroke-sky-700 stroke-2\" "
    "stroke-linecap=\"round\" stroke-linejoin=\"round\"><circle cx=\"12\" "
    "cy=\"12\" r=\"5\" /></svg><p class=\"ml-4\">Abrir o socket.</p></li><li "
    "class=\"flex items-center\"><svg class=\"w-6 h-6 flex-none fill-sky-100 "
    "stroke-sky-700 stroke-2\" stroke-linecap=\"round\" "
    "stroke-linejoin=\"round\"><circle cx=\"12\" cy=\"12\" r=\"5\" /></svg><p "
    "class=\"ml-4\">Aguardar por comunicação do cliente/servidor. </p></li><li "
    "class=\"flex items-center\"><svg class=\"w-6 h-6 flex-none fill-sky-100 "
    "stroke-sky-700 stroke-2\" stroke-linecap=\"round\" "
    "stroke-linejoin=\"round\"><circle cx=\"12\" cy=\"12\" r=\"5\" /></svg><p "
    "class=\"ml-4\">Transmitir mensagens HTTP entre ambos. </p></li><li "
    "class=\"flex items-center\"><svg class=\"w-6 h-6 flex-none fill-sky-100 "
    "stroke-sky-700 stroke-2\" stroke-linecap=\"round\" "
    "stroke-linejoin=\"round\"><circle cx=\"12\" cy=\"12\" r=\"5\" /></svg><p "
    "class=\"ml-4\">Fechar o socket.</p></li><p>O servidor também irá gravar "
    "os dados coletados nas mensagens HTTP de teste (body), e armazená-los em "
    "arquivos. </p><p>O servidor irá operar com autenticação coleta de dados "
    "através do método "
    "“GET”</p></div></div></div></div></div></div></body></html>";

// Estabelece um socket TCP a partir do protocolo IPV4 (prototipo).
int create_ipv4_socket() { return socket(AF_INET, SOCK_STREAM, 0); }

int main(int argc, char const *argv[]) {
    // printar a inicialização do servidor
    log_event("EVENT", "Inicializando servidor");

    // Cria uma estrutura para recebimento do IPv4 do servidor e do cliente.
    struct sockaddr_in server_addr, client_addr;

    // armazena o tamanho da "string" do IP do cliente
    socklen_t sin_len = sizeof(client_addr);
    int fd_server, fd_client;
    char buf[2048];
    int on = 1;

    // Executa função de criação do socket e retorna o file descriptor do
    // socket.
    fd_server = create_ipv4_socket();

    // Verifica se o file descritor é menor que '0', e nesse caso, printa erro
    // na abertura.
    if (fd_server < 0) {
        log_event("ERROR", "Erro ao criar o socket");
        perror("socket");
        exit(1);
    }

    // caso a função acima não seja executada, é retornado como socket aberto.
    log_event("EVENT", "Socket criado");

    // Define os parâmetros da conexão TCP (file descriptor, socket level,
    // reutilização do socket, ponteiro dos dados, tamanho dos dados)
    setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

    // Define os parâmetros de conexão do servidor (IPv4 e porta)
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);

    // Seta as possiveis conexões, no caso qualquer endereço IP.
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Estabele o socket TCP (dentro do server) na porta definida.
    if (bind(fd_server, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
        -1) {
        log_event("ERROR", "Erro ao vincular o socket à porta TCP");
        perror("bind");
        close(fd_server);
        exit(1);
    }
    log_event("EVENT", "Socket vinculado à porta TCP");

    // Verifica se o servidor está em modo de escuta para recebimento de
    // solicitações do cliente.
    if (listen(fd_server, 10) == -1) {
        log_event("ERROR", "Erro ao habilitar modo de escuta do socket");
        perror("listen");
        close(fd_server);
        exit(1);
    }
    log_event("EVENT", "Modo de escuta do socket habilitado");

    // loop de recebimento de conexões (será sempre válido)
    while (1) {
        //  Estabelece a conexão entre cliente e servidor.
        fd_client =
            accept(fd_server, (struct sockaddr *)&client_addr, &sin_len);

        // se o file descriptor do socket retornar -1, ocorreu erro na
        // vinculação do socket.
        if (fd_client == -1) {
            log_event("ERROR", "Erro na conexão com o cliente");

            perror("Connection failed...\n");
            continue;
        }
        log_event("EVENT", "Conexão com o cliente aberta");

        printf("Got client connection...\n");

        // Faz uma cópia do processo aberto.
        if (!fork()) {
            // Finaliza o socket TCP (pelo lado do servidor), para o primeiro
            // processo.
            close(fd_server);

            // seta o buf (no caso 2048) para o valor '0'
            memset(buf, 0, 2048);

            // sobrescreve toda a string com o valor do file descriptor (no caso
            // 1, para sucesso), mantendo o final da string em 0 (null
            // terminated)
            read(fd_client, buf, 2047);

            // printa a solicitação do navegador (cliente) na CLI.
            printf("valor do buf = %s\n", buf);

            // Envia a string HTTP para o cliente.
            write(fd_client, webpage, sizeof(webpage) - 1);
            log_event("EVENT", "Página enviada ao cliente");

            // Fecha o socket (pelo lado do cliente), para o primeiro processo.
            close(fd_client);
            log_event("EVENT", "Conexão com o cliente encerrada");

            printf("Closing...\n");
            exit(0);
        }
        // Fecha o socket (pelo lado do cliente), para o segundo processo.
        close(fd_client);
    }

    return 0;
}
