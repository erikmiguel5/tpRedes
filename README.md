# tpRedes

Este código é uma implementação de um socket, que cria um cliente e um servidor, 
onde um cliente envia uma mensagem criptografada pela cifra de Cézar para o servidor, 
que retornar a mensagem original.

Para compilar o servidor utilizar gcc server.c -o server
Para rodar o servidor ./server

Para compilar o servidor utilizar gcc client.c -o client
Para rodar o servidor ./client MSG X

Onde msg, é a mensagem a ser enviada, e x o tamnaho da cifra de cezar
obs: a msg deve conter apenas carcteres minusculos de a-z.
