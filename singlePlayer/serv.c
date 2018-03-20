#include<stdio.h>
#include<string.h>   
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h>    
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define MAXLEN 1024

int main(int argc , char *argv[])
{

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;


    int socket_desc , client_sock , c , read_size, i, j, score, numofq;
    struct sockaddr_in server , client;
    char client_message[2000], data[MAXLEN];
    
    fp = fopen("questions.txt", "r");
    if(fp == NULL){
        perror("Error in loading quistions");
        return 0;
    }

    socket_desc = socket(AF_INET , SOCK_STREAM , 0);

    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
	
	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");
   
    numofq = 3;
    score = 0;
    int res, cnt;
    while(1){
        for (i=0; i<5; i++){
            read = getline(&line, &len, fp); //reading from file
            
            if(read == -1){
                perror("Error while reading from file");
                return 0;
            }
            
            cnt++;
            write(client_sock, line, read);
        }

        read_size = recv(client_sock , client_message , strlen(client_message) , 0);
        //recieving response from client

        if(read_size == 0)
        {
            puts("Client disconnected");
            fflush(stdout);
        }
        else if(read_size == -1)
        {
            perror("recv failed");
        }

        puts("CLIENT RESPONSE IS: ");
        puts(client_message);

        read = getline(&line, &len, fp); //reading correct answer from file
        puts(line);
        if (line[0] == client_message[0]){
            write(client_sock, "correct", 10);
            score++;

        }
        else{
            write(client_sock, "WRONG", 200);
        }
    }

    char result[12];
    sprintf(result, "%d", score);
    write(client_sock, result, 200);

    return 0;
}



/*
    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {

        write(client_sock , client_message , strlen(client_message));
        memset(&client_message[0], 0, sizeof(client_message));
    
    }
*/
/*

    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
        while(res = read(fd1, &data, MAXLEN)){
        if(res == -1){
            perror("Error while reading from file");
            close(fd1);
            return 0;
        }
        write(client_sock, &data, res);
    
    }
        write(client_sock , client_message , strlen(client_message));
        memset(&client_message[0], 0, sizeof(client_message));
    }

*/