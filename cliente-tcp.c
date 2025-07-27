#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "caramelo.c"

void clear_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void manda_mensagem_cliente(int socket, int tam_IP, int tam_port, char IP_DESTINATARIO[], char PORTA_DESTINATARIO[]){
    puts("Digite o IP do destinatario (exemplo: 192.168.255.255)");
    fgets(IP_DESTINATARIO, tam_IP, stdin);
    IP_DESTINATARIO[strcspn(IP_DESTINATARIO, "\n")] = '\0';
    clear_stdin();
    puts("Digite a porta do destinatario");
    fgets(PORTA_DESTINATARIO, tam_port, stdin);
    clear_stdin();
    PORTA_DESTINATARIO[strcspn(PORTA_DESTINATARIO, "\n")] = '\0';
    if (strlen(IP_DESTINATARIO) == 0) {
        puts("\n FORMATO INVALIDO (IP vazio)\n");
    }
    if (strlen(PORTA_DESTINATARIO) == 0) {
        puts("\n FORMATO INVALIDO (porta vazia)\n");
    }
    int is_PORTA = atoi(PORTA_DESTINATARIO);
    if (conectar_com_outro_dispositivo(socket, IP_DESTINATARIO, is_PORTA) != 0) {
        puts("FALHA AO CONECTAR");
    }
    puts("Agora digite uma mensagem");
    char mensagem[TAM_MAX_MENSAGEM];
    scanf("%244s", mensagem);
    enviar_mensagem(mensagem, '\0', socket);
    // isso é pra supostamente ser feito em outra thread ent dps de enviar teria que fechar o socket
}

int main(){
    int opcoes;
    char* IP_SERVIDOR = "127.0.0.1"; //só pra testar numa maquina local, deve ser mudado posteriormente
    int tam_IP = 22;
    int tam_port = 6;
    char IP_DESTINATARIO[tam_IP];
    char PORTA_DESTINATARIO[tam_port];
    int m_socket;
    int PORTA_LOCAL;
    char* NOME_DO_CLIENTE;
    Cliente cliente = {
        .nome  = "vascodagama",
        .IP    = "192.168.169.169",
        .porta = 6969
};
    do {
        puts("Primeiro, digite a sua porta para recepçao de mensagens");
        scanf("%d", &PORTA_LOCAL);
        //criar_thread_socket() -> criação thread de escuta
        //int m_socket = criar_socket(PORTA_LOCAL); //TODO vai ter que usar threads pra criar uma thread ouvindo
        clear_stdin();
        if ((m_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) { 
            printf("\n ERRO NA CRIAÇÃO DO SOCKET \n");fflush(stdout);
        return -1;
    }
    }while (!m_socket);
    do {
        puts(" ");
        puts("ESCOLHA UMA OPÇÃO");
        puts("1. Participar do chat");
        puts("2. Mandar mensagem");
        puts("3. Mandar broadcast");
        puts("4. Sair");
        

        if (scanf("%d", &opcoes) != 1) {
            opcoes = 4;
        }
        clear_stdin();
        switch (opcoes) {
            case 1:
                puts("Participar do chat");
                entrar_no_chat(m_socket, IP_SERVIDOR, PORTA_SERVIDOR, cliente); //fazer conexão com servidor
                break;
            case 2:
                manda_mensagem_cliente(m_socket, tam_IP, tam_port, IP_DESTINATARIO, PORTA_DESTINATARIO);
                break;
            case 3:
            // loop de enviar_mensagem()
            case 4:
                close(m_socket);
            break;
        }
        close(m_socket);
    }while (opcoes != 4);
    return 0;
}

/*
 * TODO talvez quando o usuário escolher pra mandar mensagem, mostrar a lista de usuarios no chat, com um indice,
o usuário escolhe outro cliente e ja chama automaticamente as funções de conexão e de envio de mensagem
 * TODO talvez renomear as variaveis da main pra minusculo pra n confundir com variaveis globai
*/
