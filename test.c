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

int CREATE_PROFILE(char *data)
{
    printf("DATA:");
    printf(data);
    char *token = strtok(data, ";");
    char email[100],name[100],surname[100],residence[100],course[100],year[100],skills[100];
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
    printf("\nPassou\n");
    printf(email);
    printf("\n");
    FILE* fp;
    char savepath[100] = "./data/";
    strcat(savepath,email);
    strcat(savepath,".txt");
    printf(savepath);
    printf("\n");
    fp = fopen(savepath,"w");
}

int main(){
    char data[] = "enrico@gmail.com;enrico;fernandes;casa do chapéu;Engenharia;2023;codar,andar,correr,pular,marretar.";
    CREATE_PROFILE(data);
}