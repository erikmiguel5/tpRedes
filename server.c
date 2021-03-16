#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>


/* funcao para descriptografar a mensagem */
void cifra(char* msg, int x){

    /* armazena a quantidade de caracteres */
    int msgSize = (int)strlen(msg);

    /* tratar valores maiores que a quantidade de letras */
    if(x > 26){
        while(x > 26){
            x = x - 26;
        }
    }

    /* laco para percorrer a mensagem */
    for(int i = 0; i < msgSize; i++){
        if((int)msg[i] - x >= 97){
            msg[i] = (int)msg[i] - x;
        }
        else{
            msg[i] = 123 + (((int)msg[i] - 97) - x);
        }
    }
    printf("\nmsg: %s\n", msg);
}


/* parte 1:funcao principal */
int main(int argc, char* argv){
    short port = 8686;

    /* parte 2: declarar variaveis impportantes */
    int ssocket, csocket, err, nbytes, caddr_size, X;
    struct sockaddr_in saddr, caddr;
    char msg[100];
    caddr_size = sizeof(caddr);

    /* struct para o timeout */
    struct timeval timeout;      
    timeout.tv_sec = 15;
    timeout.tv_usec = 0;

    /* parte3: criacao do socket */
    ssocket = socket(AF_INET, SOCK_STREAM, 0);
    if(ssocket < 0){
        printf("\nErro ao criar socekt\n");
        exit(1);
    }

    /* parte 4: configurar endereco do servidor*/
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

    /* parte 5: associar o endereco do sevidor */
    err = bind(ssocket, (struct sockaddr*) &saddr, sizeof(saddr));
    if(err < 0){
        printf("\nErro ao associar endereco ao servidor\n");
        exit(1);
    }

    /* parte 6: configurar a conexao local passiva(listen) */
    err = listen(ssocket, 1);
    if(err < 0){
        printf("\nErro ao marcar o socket para escuta\n");
        exit(1);
    }

    while(1){
        printf("\nEsperando conexao com um cliente");
        
        /* parte 7: aceitar proxima conexao de cliente */
        csocket = accept(ssocket, (struct sockaddr*) &caddr, &caddr_size);
        if(csocket < 0){
            printf("\nErro ao aceitar uma nova conexao de cliente");
            exit(1);
        }

        /* parte 8: receber tamanho do x, para a cifra de cesar do cliente */
        nbytes = recv(csocket, msg, sizeof(msg), 0);
        if(nbytes < 0){
            printf("\nErro ao receber cifra de cesar do cliente\n");
        }

        /* timeout 15s */
        if (setsockopt (ssocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0){
            printf("\nErro, timeout exedido\n");
        }

        /* converte char do parametro de entrada para int */
        X = atoi(msg);
        printf("\nValor da cifra: %d\n", X);
        
        /* parte 8: receber mensagem do cliente */
        nbytes = recv(csocket, msg, sizeof(msg), 0);
        if(nbytes < 0){
            printf("\nErro ao receber mensagem do cliente\n");
        }

        /* timeout 15s */
        if (setsockopt (ssocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0){
            printf("\nErro, timeout exedido\n");
        }

        /* parte 9: processar a mensagem criptografada*/ 
        cifra(msg, X);

        /* parte 10: enviar mensagem descriptografada para o cliente */
        nbytes = send(csocket, msg, sizeof(msg), 0);
        if(nbytes < 0){
            printf("nErro ao enviar mensagem para o cliente");
            exit(1);
        }

        /* parte 11: fechar o socket de conexao com o cliente */
        close(csocket);
    }
    /* parte 12: fechar o socket local do servidor */ 
    close(ssocket);


    return 0;
}