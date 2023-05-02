#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <dirent.h> // loop in directories
#define MAX 80000
#define PORT 8081
#define SA struct sockaddr

int CREATE_PROFILE(char *data){
    char *token = strtok(data, ";");
    char email[100],name[100],surname[100],residence[100],course[100],year[100],skills[600];
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
        fgets(buffer, 100, fp);
        sprintf(formatted_string, "Email: %s", buffer);
        strcat(profile, formatted_string);
        fgets(buffer, 100, fp);
        buffer[strlen(buffer)-1] = ' ';
        sprintf(formatted_string, "Nome: %sSobrenome: ", buffer);
        strcat(profile, formatted_string);
        fgets(buffer, 100, fp);
        sprintf(formatted_string, "%s", buffer);
        strcat(profile, formatted_string);
        fgets(buffer, 100, fp);
        sprintf(formatted_string, "Residência: %s", buffer);
        strcat(profile, formatted_string);
        fgets(buffer, 100, fp);
        sprintf(formatted_string, "Formação Acadêmica: %s", buffer);
        strcat(profile, formatted_string);
        fgets(buffer, 100, fp);
        sprintf(formatted_string, "Ano de Formatura: %s", buffer);
        strcat(profile, formatted_string);
        fgets(buffer, 100, fp);
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
            char c, buffer[100];

            FILE* fp;
            fp = fopen(path,"r");
            fgets(buffer, 100, fp);
            char n_email[100];
            strcpy(n_email,buffer);
            char *newline = strchr(n_email, '\n');
            if (newline != NULL) {
                // Replace the newline character with a null character
                *newline = '\0';
            }
            fgets(buffer, 100, fp);
            buffer[strlen(buffer)-1] = ' ';
            char name[100];
            strcpy(name,buffer);
            newline = strchr(n_email, '\n');
            if (newline != NULL) {
                // Replace the newline character with a null character
                *newline = '\0';
            }
            // printf("Nome: %sSobrenome: ", buffer);
            fgets(buffer, 100, fp);
            // printf("%s", buffer);
            fgets(buffer, 100, fp);
            // printf("Residência: %s", buffer);
            fgets(buffer, 100, fp);
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
            char c, buffer[100];

            FILE* fp;
            fp = fopen(path,"r");
            fgets(buffer, 100, fp);
            char n_email[100];
            strcpy(n_email,buffer);
            char *newline = strchr(n_email, '\n');
            if (newline != NULL) {
                // Replace the newline character with a null character
                *newline = '\0';
            }
            fgets(buffer, 100, fp);
            buffer[strlen(buffer)-1] = ' ';
            char name[100];
            strcpy(name,buffer);
            newline = strchr(name, '\n');
            if (newline != NULL) {
                // Replace the newline character with a null character
                *newline = '\0';
            }
            // printf("Nome: %sSobrenome: ", buffer);

            fgets(buffer, 100, fp);
            // printf("Residência: %s", buffer);
            fgets(buffer, 100, fp);
            // printf("Formação Acadêmica: %s", buffer);
            fgets(buffer, 100, fp);
            // printf("Ano de Formatura: %s", buffer);
            fgets(buffer, 100, fp);
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
            char c, buffer[100];
            char *suffix = ".txt";
            char *last_occurrence = strrchr(path, *suffix);
            int prefix_length = last_occurrence - path;
            char prefix[100];
            strncpy(prefix, path, prefix_length);
            prefix[prefix_length] = '\0';

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
            char c, buffer[100];
            FILE* fp;
            fp = fopen(path,"r");
            fgets(buffer, 100, fp);
            char n_email[100];
            strcpy(n_email,buffer);
            char *newline = strchr(n_email, '\n');
            if (newline != NULL) {
                // Replace the newline character with a null character
                *newline = '\0';
            }
            fgets(buffer, 100, fp);
            buffer[strlen(buffer)-1] = ' ';
            char name[100];
            strcpy(name,buffer);
            newline = strchr(n_email, '\n');
            if (newline != NULL) {
                // Replace the newline character with a null character
                *newline = '\0';
            }
            // printf("Nome: %sSobrenome: ", buffer);
            fgets(buffer, 100, fp);
            // printf("%s", buffer);
            fgets(buffer, 100, fp);
            // printf("Residência: %s", buffer);
            fgets(buffer, 100, fp);
            // printf("Formação Acadêmica: %s", buffer);
            fgets(buffer, 100, fp);
            // printf("Ano de Formatura: %s", buffer);
            fgets(buffer, 100, fp);
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

int main(){
    char data[] = "enrico@gmail.com;enrico;fernandes;casa do chapéu;Engenharia;2023;codar,andar,correr,pular,marretar.";
    CREATE_PROFILE(data);
    char n_email[] = "enrico@gmail.com";
    char data2[] = "jhonjhe@gmail.com;vitor;bap;quadras da med;Engenharia;2023;jogar fut, correr, chapelar, codar, amar";
    char email2[] = "jhonjhe@gmail.com";
    CREATE_PROFILE(data2);
    printf("%s\n", GET_PROFILE(n_email));
    printf("%s", GET_PROFILE(email2));
    printf("---------------");
    REMOVE_PROFILE(n_email);
    printf("%s", GET_PROFILE(n_email));
    printf("\n------###--------\n");
    printf("%s\n",LIST_COURSE("Engenharia"));
    printf("\n------###--------\n");
    printf("%s\n",LIST_YEAR(2023));
    printf("\n------###--------\n");
    printf("%s\n",LIST_ALL());
    printf("\n------&&&&&&&&&&&&&&&&-------\n");
    printf("%s\n",LIST_SKILL("codar"));
    return 0;
}