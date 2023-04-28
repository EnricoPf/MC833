#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define PORT 8081
#define SA struct sockaddr

/*
Do lado do servidor, usaremos arquivos .txt numa pasta para armazenar perfils
Detalhamento de funções
para as funções, mandamos o nome da função depois os dados
//cadastrar um novo perfil utilizando o email como identificador;
CREATE_PROFILE(string data) CREATE email; name; surname; residence; course; year; skills;
 string data = email; name; surname; residence; course; year; skills;
// listar todas as pessoas (email e nome) formadas em um determinado curso;
LIST_COURSE(string n_course) COURSE n_course
só checar se string==curso
// listar todas as pessoas (email e nome) que possuam uma determinada habilidade;
LIST_SKILL(string sub_skill) SKILL sub_skill
checar se a string sub_skill é substring da string skills de todos
// listar todas as pessoas (email, nome e curso) formadas em um determinado ano;
LIST_YEAR(int n_year) YEAR n_year
checar se n_year==year dos perfis
// listar todas as informações de todos os perfis;
LIST_ALL() ALL
// dado o email de um perfil, retornar suas informações;
GET_PROFILE(string n_email) GET n_email
 só checar os nomes dos arquivos,veja abaixo
// remover um perfil a partir de seu identificador (email);
REMOVE_PROFILE(string n_email) REMOVE n_email
 salvamos os arquivos com a email(ID) como nome pra ficar mais facil de deletar
*/



void func(int sockfd)
{
	char buff[MAX];
	int n;
    printf("Enter your command:\n");
	for (;;) {
		bzero(buff, sizeof(buff));
		n = 0;
		while ((buff[n++] = getchar()) != '\n')
			;
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("%s",buff);
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit....\n");
			break;
		}
	}
}

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created...\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
		!= 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("Connected to the server...\n");

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);
}
