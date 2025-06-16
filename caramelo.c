#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define TAM_MAX_MENSAGEM 255
#define MAXPEDING 5
#define PORTA_SERVIDOR 8888

typedef struct cliente {
    char nome[30];
    char IP[22]; // Representa o IP como um inteiro //de jg: as funções do leandro lidam com IP sendo char, e dps ele faz o casting pros big indian/little indian da vida, ent pra manter a conveniencia mudei pra char
    short porta;
} Cliente;

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

int enviar_mensagem(char* mensagem, char tipo, int sock) //TODO rastrear o erro de quem não pode ter cedebido num broadcast opr exemplo
{
    char payload[TAM_MAX_MENSAGEM];
    if (strlen(mensagem) > TAM_MAX_MENSAGEM - 4) {
        printf("\n (PID %d) MENSAGEM MUITO GRANDE! \n", getpid());fflush(stdout);
        return -1;
    }
    snprintf(payload, sizeof payload, "%03d%c%s", (int) strlen(mensagem), tipo, mensagem);
    if (send(sock, payload, strlen(payload), 0) != strlen(payload)) {
        printf("\n (PID %d) ERRO NO ENVIO DA MENSAGEM! \n", getpid());fflush(stdout);
        return -1;
    }

    printf("\n (PID %d) ENVIADO: %s \n", getpid(), payload);fflush(stdout);

    return 0;
}

int entrar_no_chat(int sock, char* IP, short porta, Cliente cliente){
    
    struct sockaddr_in endereco;
    memset(&endereco, 0, sizeof(endereco));
    endereco.sin_family = AF_INET;
    endereco.sin_addr.s_addr = inet_addr(IP);
    endereco.sin_port = htons(porta);

    if (connect(sock, (struct sockaddr *) &endereco, sizeof(endereco)) < 0) {
        printf("\n ERRO NA CONEXÃO \n");
        return -1;
    }
    
    char mensagem_de_conexao[TAM_MAX_MENSAGEM];
    snprintf(mensagem_de_conexao, sizeof mensagem_de_conexao, "%s|%hd|%s", IP, porta, cliente.nome); // TODO ver como botar o tamanho da mensagem
    
    enviar_mensagem(mensagem_de_conexao, 'R', sock);

    return 0;
}

int receber_mensagem(char* mensagem, int sock){
    memset((void *) mensagem, (int) NULL, TAM_MAX_MENSAGEM);
    
    if (recv(sock, mensagem, TAM_MAX_MENSAGEM, 0) < 0) {
        printf("\n ERRO NA RECEPÇÃO DA MENSAGEM! \n");fflush(stdout);
        return -1;
    }

    printf("\n (PID %d) MENSAGEM RECEBIDA: %s \n", getpid(), mensagem); fflush(stdout);

    char buffer[4], tam[3], tipo;
    strncpy(tam, mensagem, 3);
    return 0;
}