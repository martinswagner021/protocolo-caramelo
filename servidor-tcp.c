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


int cria_socket(int porta){

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

int aceita_conexao(int sock){
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

int receber_mensagem(char* mensagem, int sock){
    memset((void *) mensagem, NULL, TAM_MENSAGEM);
    
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

int main(){
    


}

