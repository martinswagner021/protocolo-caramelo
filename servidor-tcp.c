#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include "caramelo.c"

/* 
 * GLOSSARIO
 * sockaddr_in: struct para IP e porta, e outras coisas mais
 * PF_INET: protocolo ipv4, SOCK_STREAM: TCP
 * fflush força saída da mensagem no arquivo passado (no caso está o stdout)
 * AF_INET: basicamente mesma coisa que o PF_INET
 * INADDR_ANY: é uma constante especial que significa: "Aceitar conexões em qualquer interface de rede local".
 * htonl, htons: transforman dados do "host" para big endian(tipo de dados usados na rede)
 * bind: Associa um socket a um endereço e porta local. 
 * */

/*
 *ETAPAS Servidor
 
 criar socket
 esperar mensagem
     receber
     - processar
      - conexão?
      - atualiza a lista + broadcast
      - processar
      - desconexão? 
      - atualiza a lista + broadcast
 mandar mensagem broadcast
 * */

/*
 * ETAPAS cliente TODO
 * criar socket
 * 
 * */

Cliente* clientes[50];

volatile sig_atomic_t signal_received = 0;
int sock;                   /* Socket */
int socket_cliente;         /* Socket de conexão com o cliente */

void sig_handler(int sig) {
    printf("\nServidor encerrado pelo usuário.\n");
    if (socket_cliente > 0) {
        close(socket_cliente);
    }
    if (sock > 0) {
        close(sock);
    }
    exit(0);
}

int main(){

    int resultado;              /* Resultado das funções */
    char mensagem[TAM_MAX_MENSAGEM]; /* Buffer para a recepção da string de echo */
    #ifdef WIN
        WORD wPackedValues;
        WSADATA  SocketInfo;
        int      nLastError,
                nVersionMinor = 1,
                nVersionMajor = 1;
        wPackedValues = (WORD)(((WORD)nVersionMinor)<< 8)|(WORD)nVersionMajor;
        nLastError = WSAStartup(wPackedValues, &SocketInfo);
    #endif

    sock = criar_socket(PORTA_SERVIDOR);
    if (sock < 0)
    {
        printf("\nErro na criação do socket!\n");
        return(1);
    }

    signal(SIGINT, sig_handler);

    while(!signal_received) /* Loop eterno */
    {
        /* Aguarda por uma conexão e a aceita criando o socket de contato com o cliente */
        socket_cliente = aceitar_conexao(sock);
        if (socket_cliente == 0)
        {
            printf("\nErro na conexao do socket!\n");
            return(1);
        }

        /* Recebe a mensagem do cliente */
        resultado = receber_mensagem(mensagem,socket_cliente);
        if (resultado < 0)
        {
            printf("\nErro no recebimento da mensagem\n");
            return(1);
        }

        printf("Resultado: %s", resultado);

        /* Devolve o conte�do da mensagem para o cliente */
        resultado = enviar_mensagem("", 'A',socket_cliente);
        if (resultado < 0)
        {
            printf("\nErro no envio da mensagem\n");
            return(1);
        }

           /* Fecha o socket do cliente */
        close(socket_cliente);
    }
}

