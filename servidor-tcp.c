#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
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

typedef struct {
    char nome[30];
    int IP;
    short porta;
}clientes;


int main(){
    
    int sock;                   /* Socket */
    int socket_cliente;         /* Socket de conexão com o cliente */
    int resultado;              /* Resultado das funções */
    char mensagem[TAM_MENSAGEM]; /* Buffer para a recepção da string de echo */
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

    for (;;) /* Loop eterno */
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

        /* Devolve o conte�do da mensagem para o cliente */
        resultado = enviar_mensagem(mensagem,socket_cliente);
        if (resultado < 0)
        {
            printf("\nErro no envio da mensagem\n");
            return(1);
        }

        close(socket_cliente);    /* Fecha o socket do cliente */
    }
    /*n�o passa por aqui */

}

