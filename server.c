#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define PORT 8081
#define SA struct sockaddr

/*
Do lado do servidor, usaremos arquivos .txt numa pasta para armazenar perfils
Detalhamento de funções
//cadastrar um novo perfil utilizando o email como identificador;
CREATE_PROFILE(string email)
 dps de pegar o email,
 precisa pegar tbm curso,
 ano de formação
 habilidades e residencia
 string course, skills, residence
 int year
// listar todas as pessoas (email e nome) formadas em um determinado curso;
LIST_COUSE(string n_course)
só checar se string==curso
// listar todas as pessoas (email e nome) que possuam uma determinada habilidade;
LIST_SKILL(string sub_skill)
checar se a string sub_skill é substring da string skills de todos
// listar todas as pessoas (email, nome e curso) formadas em um determinado ano;
LIST_YEAR(int n_year)
checar se n_year==year dos perfis
// listar todas as informações de todos os perfis;
LIST_ALL()
// dado o email de um perfil, retornar suas informações;
GET_PROFILE(string email)
 só checar os nomes dos arquivos,veja abaixo
// remover um perfil a partir de seu identificador (email);
REMOVE_PROFILE(string email)
 salvamos os arquivos com a email(ID) como nome pra ficar mais facil de deletar
*/


struct profile{
    char* email;
    char* name;
    char* residence;
    char* course;
    char* graduation_year;
    char* skills;
};

// Function designed for chat between client and server.
void func(int connfd)
{
	char buff[MAX];
    int n;
    // infinite loop for chat
    for (;;) {
        bzero(buff, MAX);
   
        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
        // print buffer which contains the client contents
        printf("From client: %s\n", buff);

        if (strncmp("1", buff, 1) == 0) {
            char* message = "Registration Process started...\n Enter e-mail: \n";
            n = 0;
            while ((buff[n] = message[n]) && (message[n++] != '\n'))
                ;
            write(connfd, buff, sizeof(buff));
            read(connfd, buff, sizeof(buff));
            printf("sent email:%s\n",buff);

            
            char* message = "Enter e-mail: \n";
            n = 0;
            while ((buff[n] = message[n]) && (message[n++] != '\n'))
                ;
            write(connfd, buff, sizeof(buff));
        }
        else{
            bzero(buff, MAX);
            // and send that buffer to client
            write(connfd, buff, sizeof(buff));
        }
        
        /* commands:
         register
         list_course
         list_skills
         list_year
         list_all
         get_email
         remove_email
        */
           
        // if msg contains "Exit" then server exit and chat ended.
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
	}
}

// Driver function
int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server accept failed...\n");
		exit(0);
	}
	else
		printf("server accept the client...\n");

	// Function for chatting between client and server
	func(connfd);

	// After chatting close the socket
	close(sockfd);
}
