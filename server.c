#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#include <dirent.h> // loop in directories
#define MAX 80000
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

struct profile
{
    char *email;
    char *name;
    char *residence;
    char *course;
    char *graduation_year;
    char *skills;
};

// int CREATE_PROFILE(char *data)
// {
//     char *token = strtok(data, ";");
//     // token[0] = email; 1-name; 2-surname; 3-residence; 4-course; 5-year; 6-skills;
//     int counter = 0;
//     while (token[counter] != NULL)
//     {
//         counter++;
//     }
//     if (counter != 7)
//     {
//         //missing parameter
//         return 1;
//     }
//     FILE* fp;
//     fp = fopen('./data/%C.txt',token[0]);
// }
int CREATE_PROFILE(char *data)
{
    char *token = strtok(data, ";");
    char email[100],name[100],surname[100],residence[100],course[100],year[4],skills[600];
    // token[0] = email; 1-name; 2-surname; 3-residence; 4-course; 5-year; 6-skills;
    int counter = 0;
    while (token != NULL)
    {
        switch (counter)
        {
        case (0):
            strcpy(email,token);
            break;
        case (1):
            strcpy(name,token);
            break;
        case (2):
            strcpy(surname,token);
            break;
        case (3):
            strcpy(residence,token);
            break;
        case (4):
            strcpy(course,token);
            break;
        case (5):
            strcpy(year,token);
            break;
        case (6):
            strcpy(skills,token);
            break;
        }
        token = strtok(NULL, ";");
        counter++;
    }
    FILE* fp;
    char savepath[100] = "./data/";
    strcat(savepath,email);
    strcat(savepath,".txt");
    fp = fopen(savepath,"w");
    fputs(email,fp);
    fputc('\n',fp);
    fputs(name,fp);
    fputc('\n',fp);
    fputs(surname,fp);
    fputc('\n',fp);
    fputs(residence,fp);
    fputc('\n',fp);
    fputs(course,fp);
    fputc('\n',fp);
    fputs(year,fp);
    fputc('\n',fp);
    fputs(skills,fp);
    fclose(fp);
    return 1;
}

int REMOVE_PROFILE(char* n_email){
    char filepath[100] = "./data/";
    strcat(filepath,n_email);
    strcat(filepath,".txt");
    int ret = remove(filepath);
    if (ret == 0){
        return 0;
    }else{
        printf("Falhou\n");
        return 1;
    }
}

char* GET_PROFILE(char* n_email){
    char filepath[100] = "./data/";
    char *profile = malloc(1000 * sizeof(char));
    strcpy(profile, "");
    char formatted_string[1000];
    strcat(filepath,n_email);
    strcat(filepath,".txt");   
    if (access(filepath, F_OK) == 0) {
        printf("Arquivo existe!\n");
        char c, buffer[100];
        FILE* fp;
        fp = fopen(filepath,"r");
        sprintf(formatted_string, "-----------------------------\n");
        strcat(profile, formatted_string);
        fgets(buffer, 50, fp);
        sprintf(formatted_string, "Email: %s", buffer);
        strcat(profile, formatted_string);
        fgets(buffer, 50, fp);
        buffer[strlen(buffer)-1] = ' ';
        sprintf(formatted_string, "Nome: %sSobrenome: ", buffer);
        strcat(profile, formatted_string);
        fgets(buffer, 50, fp);
        sprintf(formatted_string, "%s", buffer);
        strcat(profile, formatted_string);
        fgets(buffer, 50, fp);
        sprintf(formatted_string, "Residência: %s", buffer);
        strcat(profile, formatted_string);
        fgets(buffer, 50, fp);
        sprintf(formatted_string, "Formação Acadêmica: %s", buffer);
        strcat(profile, formatted_string);
        fgets(buffer, 50, fp);
        sprintf(formatted_string, "Ano de Formatura: %s", buffer);
        strcat(profile, formatted_string);
        fgets(buffer, 50, fp);
        sprintf(formatted_string, "Habilidades: %s\n", buffer);
        strcat(profile, formatted_string);
        sprintf(formatted_string, "------------------------------\n");
        strcat(profile, formatted_string);
        return profile;
    } else {
        printf("Arquivo (%s) NAO existe!\n", filepath);
    // file doesn't exist
    }
    return profile;
}
char* LIST_COURSE(char *n_course){

    char *profile = malloc(1000 * sizeof(char));
    strcpy(profile, "");
    char formatted_string[100];

    DIR *dir;
    struct dirent *entry;
    char path[100];
    // Open the directory
    dir = opendir("./data/");
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    int cont = 0;
    // Loop through all the entries in the directory
    while ((entry = readdir(dir)) != NULL) {
        // Check if the entry is a regular file and has a ".txt" extension
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".txt") != NULL) {
            // Construct the path to the file
            char path[200] = "./data/";
            strcat(path, entry->d_name);
            // snprintf(path, sizeof(path), "/data/%s", entry->d_name);
            char c, buffer[100];

            // Process the file
            // printf("Processing file: %s\n", path);
            // TODO: Add your code to process the file here
            FILE* fp;
            fp = fopen(path,"r");
            fgets(buffer, 50, fp);
            // printf("Email: %s", buffer);
            char n_email[100];
            strcpy(n_email,buffer);
            char *newline = strchr(n_email, '\n');
            if (newline != NULL) {
                // Replace the newline character with a null character
                *newline = '\0';
            }
            fgets(buffer, 50, fp);
            buffer[strlen(buffer)-1] = ' ';
            char name[100];
            strcpy(name,buffer);
            newline = strchr(n_email, '\n');
            if (newline != NULL) {
                // Replace the newline character with a null character
                *newline = '\0';
            }
            // printf("Nome: %sSobrenome: ", buffer);
            fgets(buffer, 50, fp);
            // printf("%s", buffer);
            fgets(buffer, 50, fp);
            // printf("Residência: %s", buffer);
            fgets(buffer, 50, fp);
            // printf("Formação Acadêmica: %s", buffer);
            newline = strchr(buffer, '\n');
            if (newline != NULL) {
                // Replace the newline character with a null character
                *newline = '\0';
            }
            if(!strcmp(buffer, n_course)){
                snprintf(formatted_string, 1000, "%s ", n_email);
                strcat(profile, formatted_string);
                snprintf(formatted_string, 1000, "%s\n", name);
                strcat(profile, formatted_string);
                cont ++;
            }
        }
    }
    // Close the directory
    closedir(dir);
    if (cont == 0){
        return "Nenhum perfil se formou nesse curso\n";
    }
    return profile;


};

char* LIST_YEAR(int year){

    char *profile = malloc(1000 * sizeof(char));
    strcpy(profile, "");
    char formatted_string[100];

    DIR *dir;
    struct dirent *entry;
    char path[100];
    // Open the directory
    dir = opendir("./data/");
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    int cont = 0;
    // Loop through all the entries in the directory
    while ((entry = readdir(dir)) != NULL) {
        // Check if the entry is a regular file and has a ".txt" extension
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".txt") != NULL) {
            // Construct the path to the file
            char path[200] = "./data/";
            strcat(path, entry->d_name);
            // snprintf(path, sizeof(path), "/data/%s", entry->d_name);
            char c, buffer[100];

            // Process the file
            // printf("Processing file: %s\n", path);
            // TODO: Add your code to process the file here
            FILE* fp;
            fp = fopen(path,"r");
            fgets(buffer, 50, fp);
            // printf("Email: %s", buffer);
            char n_email[100];
            strcpy(n_email,buffer);
            char *newline = strchr(n_email, '\n');
            if (newline != NULL) {
                // Replace the newline character with a null character
                *newline = '\0';
            }
            fgets(buffer, 50, fp);
            buffer[strlen(buffer)-1] = ' ';
            char name[100];
            strcpy(name,buffer);
            newline = strchr(name, '\n');
            if (newline != NULL) {
                // Replace the newline character with a null character
                *newline = '\0';
            }
            // printf("Nome: %sSobrenome: ", buffer);

            fgets(buffer, 50, fp);
            // printf("Residência: %s", buffer);
            fgets(buffer, 50, fp);
            // printf("Formação Acadêmica: %s", buffer);
            fgets(buffer, 50, fp);
            // printf("Ano de Formatura: %s", buffer);
            fgets(buffer, 50, fp);
            int graduation_year = atoi(buffer);
            if(graduation_year == year){
                snprintf(formatted_string, 1000, "%s %s\n", n_email, name);
                strcat(profile, formatted_string);
                cont ++;
            }
        }
    }
    // Close the directory
    closedir(dir);
    if (cont == 0){
        return "Não há nenhum usuário cadastrado do ano\n";
    }
    return profile;


};

char* LIST_ALL(){

    char *profile = malloc(10000 * sizeof(char));
    strcpy(profile, "");
    char formatted_string[10000];

    DIR *dir;
    struct dirent *entry;
    char path[100];
    // Open the directory
    dir = opendir("./data/");
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    int cont = 0;
    // Loop through all the entries in the directory
    while ((entry = readdir(dir)) != NULL) {
        // Check if the entry is a regular file and has a ".txt" extension
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".txt") != NULL) {
            // Construct the path to the file
            char path[200] = "";
            strcat(path, entry->d_name);
            // snprintf(path, sizeof(path), "/data/%s", entry->d_name);
            char c, buffer[100];
            char *suffix = ".txt";
            char *last_occurrence = strrchr(path, *suffix);
            int prefix_length = last_occurrence - path;
            char prefix[100];
            strncpy(prefix, path, prefix_length);
            prefix[prefix_length] = '\0';
            // Process the file
            // printf("Processing file: %s\n", path);
            // TODO: Add your code to process the file here

            strcpy(formatted_string,GET_PROFILE(prefix));
            strcat(profile, formatted_string);
            cont ++;
        }
    }
    // Close the directory
    closedir(dir);
    if (cont == 0){
        return "Não há nenhum usuário cadastrado\n";
    }
    return profile;


};

char* LIST_SKILL(char *sub_skill){

    char *profile = malloc(1000 * sizeof(char));
    strcpy(profile, "");
    char formatted_string[1000];

    DIR *dir;
    struct dirent *entry;
    char path[100];
    // Open the directory
    dir = opendir("./data/");
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    int cont = 0;
    // Loop through all the entries in the directory
    while ((entry = readdir(dir)) != NULL) {
        // Check if the entry is a regular file and has a ".txt" extension
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".txt") != NULL) {
            // Construct the path to the file
            char path[200] = "./data/";
            strcat(path, entry->d_name);
            // snprintf(path, sizeof(path), "/data/%s", entry->d_name);
            char c, buffer[100];

            // Process the file
            // printf("Processing file: %s\n", path);
            // TODO: Add your code to process the file here
            FILE* fp;
            fp = fopen(path,"r");
            fgets(buffer, 50, fp);
            // printf("Email: %s", buffer);
            char n_email[100];
            strcpy(n_email,buffer);
            char *newline = strchr(n_email, '\n');
            if (newline != NULL) {
                // Replace the newline character with a null character
                *newline = '\0';
            }
            fgets(buffer, 50, fp);
            buffer[strlen(buffer)-1] = ' ';
            char name[100];
            strcpy(name,buffer);
            newline = strchr(n_email, '\n');
            if (newline != NULL) {
                // Replace the newline character with a null character
                *newline = '\0';
            }
            // printf("Nome: %sSobrenome: ", buffer);
            fgets(buffer, 50, fp);
            // printf("%s", buffer);
            fgets(buffer, 50, fp);
            // printf("Residência: %s", buffer);
            fgets(buffer, 50, fp);
            // printf("Formação Acadêmica: %s", buffer);
            fgets(buffer, 50, fp);
            // printf("Ano de Formatura: %s", buffer);
            fgets(buffer, 50, fp);
            // printf("Habilidades: %s\n", buffer);
            newline = strchr(buffer, '\n');
            if (newline != NULL) {
                // Replace the newline character with a null character
                *newline = '\0';
            }
            if(strstr(buffer, sub_skill)){
                snprintf(formatted_string, 1000, "%s %s\n", n_email, name);
                strcat(profile, formatted_string);
                cont ++;
            }
        }
    }
    // Close the directory
    closedir(dir);
    if (cont == 0){
        return "Não há nenhum usuário cadastrado com essa habilidade\n";
    }
    return profile;


};

// Function designed for chat between client and server.
void func(int connfd)
{
    char buff[MAX];
    int n;
    // infinite loop for chat
    for (;;)
    {
        bzero(buff, MAX);
        int cont = 0;
        char * message;

        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
        // print buffer which contains the client contents
        printf("From client: %s\n", buff);
        char *newline = strchr(buff, '\n');
        if (newline != NULL) {
            // Replace the newline character with a null character
            *newline = '\0';
        }
        printf("buffer = --%s--\n", buff);
        if (strcmp("list_all", buff) == 0){

            message = LIST_ALL();
            strcpy(buff, message);
            write(connfd, buff, sizeof(buff));
            continue;
        }
        if (strcmp("get_email", buff) == 0){
            message = "Enter email:\n";
            strcpy(buff, message);
            write(connfd, buff, sizeof(buff));
            read(connfd, buff, sizeof(buff));
            printf("Email: %s\n", buff);
            char *newline = strchr(buff, '\n');
            if (newline != NULL) {
                // Replace the newline character with a null character
                *newline = '\0';
            }
            message = GET_PROFILE(buff);
            strcpy(buff, message);
            write(connfd, buff, sizeof(buff));
            continue;
        }
        if (strcmp("remove_email", buff) == 0){
            message = "Enter email:\n";
            strcpy(buff, message);
            write(connfd, buff, sizeof(buff));
            read(connfd, buff, sizeof(buff));
            printf("Email: %s\n", buff);
            char *newline = strchr(buff, '\n');
            if (newline != NULL) {
                // Replace the newline character with a null character
                *newline = '\0';
            }
            REMOVE_PROFILE(buff);
            message = "Profile removed!";
            strcpy(buff, message);
            write(connfd, buff, sizeof(buff));
            continue;
        }
        if (strcmp("list_course", buff) == 0){
            message = "Enter course:\n";
            strcpy(buff, message);
            write(connfd, buff, sizeof(buff));
            read(connfd, buff, sizeof(buff));
            printf("Course: %s\n", buff);
            char *newline = strchr(buff, '\n');
            if (newline != NULL) {
                // Replace the newline character with a null character
                *newline = '\0';
            }
            message = LIST_COURSE(buff);
            strcpy(buff, message);
            write(connfd, buff, sizeof(buff));
            continue;
        }
        if (strcmp("list_year", buff) == 0){
            message = "Enter year:\n";
            strcpy(buff, message);
            write(connfd, buff, sizeof(buff));
            read(connfd, buff, sizeof(buff));
            printf("year: %s\n", buff);
            char *newline = strchr(buff, '\n');
            if (newline != NULL) {
                // Replace the newline character with a null character
                *newline = '\0';
            }
            message = LIST_YEAR(atoi(buff));
            strcpy(buff, message);
            write(connfd, buff, sizeof(buff));
            continue;
        }
        if (strcmp("list_skills", buff) == 0){
            message = "Enter skill:\n";
            strcpy(buff, message);
            write(connfd, buff, sizeof(buff));
            read(connfd, buff, sizeof(buff));
            printf("skill: %s\n", buff);
            char *newline = strchr(buff, '\n');
            if (newline != NULL) {
                // Replace the newline character with a null character
                *newline = '\0';
            }
            message = LIST_SKILL(buff);
            strcpy(buff, message);
            write(connfd, buff, sizeof(buff));
            continue;
        }
        // if (strncmp("1", buff, 1) == 0)
        // {
        //     char *message = "Registration Process started...\n Enter e-mail: \n";
        //     n = 0;
        //     while ((buff[n] = message[n]) && (message[n++] != '\n'))
        //         ;
        //     write(connfd, buff, sizeof(buff));
        //     read(connfd, buff, sizeof(buff));
        //     printf("sent email:%s\n", buff);

        //     message = "Enter e-mail: \n";
        //     n = 0;
        //     while ((buff[n] = message[n]) && (message[n++] != '\n'))
        //         ;
        //     write(connfd, buff, sizeof(buff));
        // }
        // else
        // {
        //     bzero(buff, MAX);
        //     // and send that buffer to client
        //     write(connfd, buff, sizeof(buff));
        // }

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
        if (strncmp("exit", buff, 4) == 0)
        {
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
    if (sockfd == -1)
    {
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
    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA *)&cli, &len);
    if (connfd < 0)
    {
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
