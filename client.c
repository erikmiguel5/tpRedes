#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>

/* parte 1: funcao main e argumentos */
int main(int argc, char** argv){

    /* parte 2: declarar variaveis importantes */
    int csocket, err, nbytes;
    short port = 8686;
    struct sockaddr_in saddr;
    char host[] = "127.0.0.1";
    char* msg = argv[1];

    /* struct para o timeout */
    struct timeval timeout;      
    timeout.tv_sec = 15;
    timeout.tv_usec = 0;

    /* parte 3: criar socket do cliente */
    csocket = socket(AF_INET, SOCK_STREAM, 0);
    if(csocket < 0){
        printf("\nErro ao criar socket do cliente\n");
        exit(1);
    }

    /* parte 4: criar endereco do servidor */
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = inet_addr(host);

    /* parte 5: conectar com o servidor */
    err = connect(csocket, (struct sockaddr*) &saddr, sizeof(saddr));
    if(err < 0){
        printf("\nErro ao conectar com o servidor\n");
        exit(1);
    }

    //sleep(10);

    /* parte 6: enviar cifra de cesar para o servidor */
    nbytes = send(csocket, argv[2], strlen(argv[2]), 0);
    if(nbytes < 0){
        printf("\nErro ao enviar mensagem para o servidor\n");
        exit(1);
    }

    /* parte 6: enviar mensagem para o servidor */
    nbytes = send(csocket, msg, strlen(msg), 0);
    if(nbytes < 0){
        printf("\nErro ao enviar mensagem para o servidor\n");
        exit(1);
    }

    printf("\nClient send: %s\n", argv[1]);

    /* parte 7: receber mensagem de resposta do servidor */
    nbytes = recv(csocket, msg,  strlen(msg), 0);
    if(nbytes < 0){
        printf("\nErro ao receber mensagem para o servidor\n");
        exit(1);
    }

    /* timeout 15s */
    if (setsockopt (csocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0){
            printf("\nErro, timeout exedido\n");
            exit(1);
    }

    printf("\nMensagem do servidor: %s\n", msg);

    /* parte 8: fechar conexao com o servidor */
    close(csocket);

    return 0;
}