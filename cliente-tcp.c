#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORTA_SERVIDOR 9999
#define TAM_MENSAGEM 255
#define MAXPEDING 5
//entrar no chat --> conecta com o servidor, manda mensagem de conexão e recebe ack
//conectar ao servidor
//desconectar do servidor
//

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

int conctar_com_outro_dispositivo(int sock, char* IP, short porta){

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
    int opcoes;
    char IP_DESTINATARIO[15];
    int m_socket;
    unsigned short PORTA_LOCAL;
    do {
        puts("Primeiro, digite a sua porta para recepçao de mensagens");
        scanf("%hu", &PORTA_LOCAL);
        int m_socket = cria_socket(PORTA_LOCAL);
    }while (!m_socket);
    //TODO ver função de clear do stdin/stdout
    puts("ESCOLHA UMA OPÇÃO");
    puts("1. Participar do chat");
    puts("2. Mandar mendagem");
    puts("Mandar broadcast");
    

    scanf("%d", &opcoes);

    do {
        switch (opcoes) {
            case 1:
                puts("Participar do chat");
                // entrar_no_chat(int sock, IP_SERVIDOR, PORTA_SERVIDOR, porta_de_escuta_cliente?, nome); 
            case 2:
                char* delim = ":";
                puts("IP e porta do destinatario (exemplo: 192.168.255.255:6969)");
                fgets(IP_DESTINATARIO, sizeof(IP_DESTINATARIO), stdin);
                IP_DESTINATARIO[strcspn(IP_DESTINATARIO, "\n")] = '\0';
                char* IP_TOK= strtok(IP_DESTINATARIO, delim);
                char* PORTA = strtok(NULL, delim);//TODO test this
                short is_PORTA = (short)atoi(PORTA);
                conctar_com_outro_dispositivo(m_socket, IP_DESTINATARIO, is_PORTA);
                // enviar_mensagem()
            // goto? pra poder redesenhar a tela
            case 3:
            // loop de enviar_mensagem()
            case 4:
            break
        }
        close(m_socket);
    }while (opcoes != );
    return 0;
}

/*
 * TODO talvez quando o usuário escolher pra mandar mensagem, mostrar a lista de usuarios no chat, com um indice,
o usuário escolhe outro cliente e ja chama automaticamente as funções de conexão e de envio de mensagem
 * TODO talvez renomear as variaveis da main pra minusculo pra n confundir com variaveis globai
*/
