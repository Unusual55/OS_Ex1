#include "shell.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

char *get_prompt();
int input_check(char *input);
char *firstTrims(char *input);
int caseExit();
int caseEcho(char *token);
int caseDir();
int caseCd(char *token);
int caseCopy(char *token);
int caseDelete(char *token);

char path[MAX_LENGTH];
int mod = 1; // mod 1 for shell, mod 2 for server stdout
int sockfd = -1;
int connfd=-1;
int outfd = 1;
int errfd = 2;

char *get_prompt()
{
    return PROMPT;
}

void reset_path()
{
    getcwd(path, sizeof(path));
}

char *get_path()
{
    return path;
}

char *firstTrims(char *input)
{
    char *token = strtok(input, "\n");
    token = strtok(token, " ");
    return token;
}

int caseExit()
{
    return 1; // code for exit the program
}

int caseEcho(char *token)
{
    while (token != null)
    {
        printf("%s ", token);
        token = strtok(null, " ");
    }
    printf("\n");
    return 0; // code for function completed successfully
}

int caseDir()
{
    DIR *directory;
    struct dirent *entry;
    int files = 0;
    directory = opendir(".");
    if (directory == null)
    {
        puts("cannot open the directory");
        // sendLastLine();
        return 1;
    }
    else
    {
        puts("Directory is opened!\n");
        // if (mod == 2)
        // {
        //     fflush(stdout);
        // }
        files++;
        while (entry = readdir(directory))
        {
            printf("%d: %s\n", files, entry->d_name);
            if (mod == 2)
                // {
                //     fflush(stdout);
                // }
                files++;
        }
        closedir(directory);
        // sendLastLine();
        return 0;
    }
}

int caseCd(char *token)
{
    if (!strcmp(token, ".."))
    {
        chdir("..");
    }
    else
    {
        if (chdir(token) != 0)
        {
            puts("Cannot enter this directory\n");
        }
        if (!strcmp(token, "/tmp"))
        {
            puts("Cannot enter this directory\n");
        }
        if (!strcmp(token, "/error"))
        {
            puts("Cannot enter this directory\n");
        }
    }
    // sendLastLine();
    return 0;
}

int caseCopy(char *token)
{
    char cTemp;
    chdir(getenv("HOME"));
    FILE *fpSource = fopen(token, "r");
    if (fpSource == null)
    {
        puts("Source file cannot be opened!");
        return 0;
    }
    token = strtok(null, " ");
    FILE *fpDest = fopen(token, "w");
    if (fpDest == null)
    {
        puts("Destination file cannot be opened!");

        fclose(fpSource);
        return 0;
    }
    // Copy the file from source to destination
    while (fread(&cTemp, 1, 1, fpSource) == 1)
    {
        fwrite(&cTemp, 1, 1, fpDest);
    }
    fclose(fpSource);
    fclose(fpDest);
    chdir(path);
    // sendLastLine();
    return 2; // 2 code for reset the directory
}

int caseDelete(char *token)
{
    chdir(getenv("HOME"));
    int out = unlink(token);
    if (out == -1)
    {
        puts("An error occured");
    }
    else
    {
        puts("File deleted successfully!");
    }
    // sendLastLine();
    chdir(path);
    return 2;
}
char *system_calls[25];
int input_check(char *input)
{
    char *token = firstTrims(input);
    int code = 0;
    if (!strcmp(token, EXIT))
    {
        return caseExit();
    }
    else if (!strcmp(token, ECHO))
    {
        token = strtok(null, " ");
        return caseEcho(token);
    }
    else if (!strcmp(token, dir))
    {
        return caseDir();
    }
    else if (!strcmp(token, "CD"))
    {
        token = strtok(null, " ");
        return caseCd(token);
    }
    else if (!strcmp(token, "COPY"))
    {
        token = strtok(null, " ");
        return caseCopy(token);
    }
    else if (!strcmp(token, "DELETE"))
    {
        token = strtok(null, " ");
        return caseDelete(token);
    }
    else if (!strcmp(token, "TCP"))
    {
        token = strtok(null, " ");
        if (!strcmp(token, "PORT"))
        {
            return caseTCP(token);
        }
        else
        {
            return 0;
        }
    }
    else if (!strcmp(token, "LOCAL"))
    {
        return caseLocal();
    }
    else
    {
        // system(input); // run any other command using system
        bzero(system_calls, 50);
        setArguments(input);
        int pid=fork();
        if(pid<0){
            exit(1);
        }
        else if(pid==0){
            execvp(system_calls[0], system_calls);
        }
        else{
            wait(null);
        } 
    }
    bzero(system_calls, 50);
    return 0;
}

int caseTCP(char *token)
{
    if (mod == 2)
    {
        return 0;
    }
    mod = 2;
    int sockfd, connfd;

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
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    connfd=connect(sockfd, (SA *)&servaddr, sizeof(servaddr));
    if (connfd != 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
    outfd = dup(1);
    errfd = dup(2);
    dup2(sockfd, STDOUT_FILENO);
    dup2(sockfd, STDERR_FILENO);
    return 0;
}

int caseLocal()
{
    if (mod == 1)
    {
        return 0;
    }
    else
    {
        printf("LOCAL");
        int pid=fork();
        if(pid<0){
            exit(1);
        }
        if(pid==0){
            write(sockfd, "LOCAL", 5);
            sleep(2);
            exit(1);
        }
        wait(NULL);
        mod = 1;
        dup2(outfd, STDOUT_FILENO);
        dup2(errfd, STDERR_FILENO);
        close(outfd);
        close(errfd);
        close(sockfd);
        close(connfd);
        return 0;
    }
}

void resetArguments(){
    int i=0;
    for(i=0;i<50;i++){
        bzero(system_calls[i], strlen(system_calls[i]));
    }
}

void setArguments(char* input){
    resetArguments();
    char* token= strtok(input, " ");
    system_calls[0]=token;
    int k=0;
    while(token!=null){
        system_calls[k]=token;
        k++;
        token=strtok(null, " ");
    }
    system_calls[k]=null;
}

int main()
{
    reset_path();
    char *input;
    int code;
    size_t size = 10;
    size_t input_size;
    mod = 1;
    while (1)
    {
        printf("%s %s\n", get_path(), get_prompt());
        while (1)
        {
            input = (char *)malloc(size);
            if (input == NULL)
            {
                exit(1);
            }
            input_size = getline(&input, &size, stdin);
            if (input_size == -1)
            {
                free(input);
                exit(1);
            }
            code = input_check(input);
            if (code == 1)
            {
                free(input);
                exit(0);
            }
            code = 0;
            free(input);
            break;
        }
    }
}