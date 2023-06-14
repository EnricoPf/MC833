#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>
#include <strings.h>

  
#define PORT 8081
#define MAXLINE 1024

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
// dado o nome de uma imagem, faz download dela na pasta local
GET_IMAGE n_image
*/



void func(int sockfd, struct sockaddr_in servaddr)
{
	char buffer[MAXLINE];
    FILE *file;
	int n;
	int bytesRead;
    printf("Enter your command:\n");
	printf("commands: register\nlist_course\nlist_skills\nlist_year\nlist_all\nget_email\nremove_email\nget_image\n------------\n");
	for (;;) {
		bzero(buffer, sizeof(buffer));
		n = 0;
		while ((buffer[n++] = getchar()) != '\n')
			;
		char *newline = strchr(buffer, '\n');
		if (newline != NULL) {
			// Replace the newline character with a null character
			*newline = '\0';
		}
		printf("----%s----\n", buffer);
		sendto(sockfd, buffer, MAXLINE, 0, (struct sockaddr*)NULL, sizeof(servaddr));
		if (strcmp("get_image", buffer) == 0){
			// send "get_image" and receive "Enter ID:"
			recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
			// Send the id of the image
			printf("%s\n",buffer);
			bzero(buffer, sizeof(buffer));
			n = 0;
			while ((buffer[n++] = getchar()) != '\n')
				;
			char *newline = strchr(buffer, '\n');
			if (newline != NULL) {
				// Replace the newline character with a null character
				*newline = '\0';
			}
			char* filepath;
			sprintf(filepath, "./downloads/%s.jpg",buffer);
			FILE* received_image = fopen(filepath, "wb");

			// send id and receive packets
			sendto(sockfd, buffer, MAXLINE, 0, (struct sockaddr*)NULL, sizeof(servaddr));

			// RECEIVE number_of_packets
			recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
			newline = strchr(buffer, '\n');
			if (newline != NULL) {
				// Replace the newline character with a null character
				*newline = '\0';
			}
			int number_of_packets = atoi(buffer);
			printf("NÚMERO DE PACOTES -> %s\n", buffer);

			bzero(buffer, sizeof(buffer));
			// RECEIVE FIRST PACKET
			bytesRead = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
			newline = strchr(buffer, '\n');
			if (newline != NULL) {
				// Replace the newline character with a null character
				*newline = '\0';
			}
			printf("PACOTE RECEBIDO\n");


			// write data in a file
        	fwrite(buffer, 1, bytesRead, received_image);
			bzero(buffer, sizeof(buffer));
			for(int i=1; i<number_of_packets;i++){
				bytesRead = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
				printf("PACOTE %d RECEBIDO\n",i);
				newline = strchr(buffer, '\n');
				if (newline != NULL) {
					// Replace the newline character with a null character
					newline = '\0';
				}
        		fwrite(buffer, 1, bytesRead, received_image);
				bzero(buffer, sizeof(buffer));
			}
			continue;
		}
		// request to send datagram
		// no need to specify server address in sendto
		// connect stores the peers IP and port
		if (strcmp("exit", buffer) == 0){
			printf("Client Exit...\n");
			break;
		}
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