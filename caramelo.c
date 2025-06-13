#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define TAM_MENSAGEM 255
#define MAXPEDING 5
#define PORTA_SERVIDOR 9999

int criar_socket(int porta){

    int sock;
    struct sockaddr_in endereco;
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) { 
        printf("\n ERRO NA CRIAÇÃO DO SOCKET \n");fflush(stdout);
        return -1;
    }

    if (porta > 0) {
        memset(&endereco,0, sizeof(endereco));
        endereco.sin_family = AF_INET; 
        endereco.sin_addr.s_addr = htonl(INADDR_ANY);
        endereco.sin_port = htons(porta);

        if (bind(sock, (struct sockaddr *) &endereco, sizeof(endereco)) < 0) {
            printf("\n ERRO NO BIND! \n");fflush(stdout);
            return -1;
        }

        if (listen(sock, MAXPEDING)) {
            printf("\n ERRO NO LISTEN! \n");fflush(stdout);
            return -1;
        }
    }
    return(sock);//sempre retornar o socket
}

int aceitar_conexao(int sock){
    int socket_cliente;
    struct sockaddr_in endereco;
    int tamanho_endereco;

    tamanho_endereco = sizeof(endereco);

    if ((socket_cliente = accept(sock, (struct sockaddr *) &endereco,\
                                 (unsigned int *) &tamanho_endereco)) < 0) {
        printf("\n ERRO NO ACCEPT! \n");fflush(stdout);
        return 0;
    }
    return socket_cliente;
}

int conectar_com_outro_dispositivo(int sock, char* IP, short porta){

    struct sockaddr_in endereco;
    memset(&endereco, 0, sizeof(endereco));
    endereco.sin_family = AF_INET;
    endereco.sin_addr.s_addr = inet_addr(IP);
    endereco.sin_port = htons(porta);

    if (connect(sock, (struct sockaddr *) &endereco, sizeof(endereco)) < 0) {
        printf("\n ERRO NA CONEXÃO \n");
        return -1;
    }
    return 0;
}

int receber_mensagem(char* mensagem, int sock){
    memset((void *) mensagem, (int) NULL, TAM_MENSAGEM);
    
    if (recv(sock, mensagem, TAM_MENSAGEM, 0) < 0) {
        printf("\n ERRO NA RECEPÇÃO DA MENSAGEM! \n");fflush(stdout);
        return -1;
    }
    printf("\n (TCP SERVER) MENSAGEM RECEBIDA: %s \n", mensagem); fflush(stdout);
    return 0;
}

int enviar_mensagem(char* mensagem, int sock) //TODO rastrear o erro de quem não pode ter cedebido num broadcast opr exemplo
{
    if (send(sock, mensagem, strlen(mensagem), 0) != strlen(mensagem)) {
        printf("\n (TCP SERVER) ERRO NO ENVIO DA MENSAGEM! \n");fflush(stdout);
        return -1;
    }

    printf("\n (TCP SERVER) ENVIADO: %s \n", mensagem);fflush(stdout);

    return 0;
}