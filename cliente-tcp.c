#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "caramelo.c"

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
            break;
        }
        close(m_socket);
    }while (opcoes != 0);
    return 0;
}

/*
 * TODO talvez quando o usuário escolher pra mandar mensagem, mostrar a lista de usuarios no chat, com um indice,
o usuário escolhe outro cliente e ja chama automaticamente as funções de conexão e de envio de mensagem
 * TODO talvez renomear as variaveis da main pra minusculo pra n confundir com variaveis globai
*/
