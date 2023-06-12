#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>
#include <strings.h>

  
#define PORT 5000
#define MAXLINE 1000

/*
Do lado do cliente, usamos funções compostas de nomes maisculos seguidos dos dados para essas funções, se multiplos, são separados por um char ';'
Detalhamento de funções
para as funções, mandamos o nome da função depois os dados
//cadastrar um novo perfil utilizando o email como identificador;
CREATE email; name; surname; residence; course; year; skills;
// listar todas as pessoas (email e nome) formadas em um determinado curso;
COURSE n_course
// listar todas as pessoas (email e nome) que possuam uma determinada habilidade;
SKILL sub_skill
// listar todas as pessoas (email, nome e curso) formadas em um determinado ano;
YEAR n_year
// listar todas as informações de todos os perfis;
ALL
// dado o email de um perfil, retornar suas informações;
GET n_email
// remover um perfil a partir de seu identificador (email);
REMOVE n_email

*/



void func(int sockfd, struct sockaddr_in servaddr)
{
	char buffer[MAXLINE];
	int n;
    printf("Enter your command:\n");
	printf("commands: register\nlist_course\nlist_skills\nlist_year\nlist_all\nget_email\nremove_email\n");
	for (;;) {
		bzero(buffer, sizeof(buffer));
		n = 0;
		while ((buffer[n++] = getchar()) != '\n')
			;
		// request to send datagram
		// no need to specify server address in sendto
		// connect stores the peers IP and port
		sendto(sockfd, buffer, MAXLINE, 0, (struct sockaddr*)NULL, sizeof(servaddr));
		bzero(buffer, sizeof(buffer));

		// waiting for response
		recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
		puts(buffer);
		// if (strcmp(buffer, "exit")== 0) {
		// 	printf("Client Exit....\n");
		// 	break;
		// }
	}
}

// Driver code
int main()
{   
    int sockfd, n;
    struct sockaddr_in servaddr;
      
    // clear servaddr
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET;
      
    // create datagram socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
      
    // connect to server
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        exit(0);
    }
  
	// function for chat
	func(sockfd, servaddr);
  
    // close the descriptor
    close(sockfd);
}