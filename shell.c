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

////////////////////////////////////Needed variables

char path[MAX_LENGTH];
int mod = 1; // mod 1 for shell, mod 2 for server stdout
int sockfd = -1;
int connfd = -1;
int outfd = 1;
int errfd = 2;

//This function returns the prompt of the shell

char *get_prompt()
{
    return PROMPT;
}

//This function reset the char array that contains the path to the current work directory

void reset_path()
{
    getcwd(path, sizeof(path));
}

//This function returns the path of the current work directory

char *get_path()
{
    bzero(path, 256);
    getcwd(path, sizeof(path));
    return path;
}

//This function trim the \n character in the end of the string and
//then split it by ' ', and return the result

char *firstTrims(char *input)
{
    char *token = strtok(input, "\n");
    token = strtok(token, " ");
    return token;
}

/**
 * @brief Handler of EXIT command
 * This function does not do much, but return a return code 1, which will cause the shell
 * to exit
 * @return int The return code 
 */

int caseExit()
{
    return 1; // code for exit the program
}

/**
 * @brief Handler of ECHO command
 * This function split the input by the delimiter ' ' and print the rest of the 
 * input string that after the "ECHO" command
 * @param token the token that contains the string splitted by the delimiter ' '
 * @return int return code 0, which means the command worked successfully
 */

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

/**
 * @brief Handler of DIR command
 * This function print all of the names of the files in the current work directory
 * @return int return code 1-exit if an error occured, otherwise return code 0- success
 */

int caseDir()
{
    //Setting the directory and the struct of the entries
    DIR *directory;
    struct dirent *entry;
    //Open the directory
    directory = opendir(".");
    if (directory == null)
    {
        puts("cannot open the directory");
        return 1;
    }
    else
    {
        puts("Directory is opened!\n");
        //Read the names of the entries in the directory
        while (entry = readdir(directory))
        {
            printf("%s\n", entry->d_name);
        }
        //Close the directory
        closedir(directory);
        return 0;
    }
}

/**
 * @brief Handler of CD command
 * This function try to change the current work directory using the input token
 * chdir is a system call which is used to change the current working directory.
 * @param token the directory we want to change to
 * @return int return code 0, it will not exit
 */

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
    return 0;
}

/**
 * @brief 
 * This function uses the input token to parse to files absolute path, and copy the
 * file from the first file to the second location.
 * At the beginning of the function, we will set the cwd to HOME in order to allow to copy
 * files that are not in the cwd.
 * At the end of the function, we will reset the path to the original path before this change.
 * Answers for part 10:
 * In our implementation we used the following functions in the copy operation:
 * fopen - library function
 * fclose - library function
 * fread - library function
 * fwrite - library function
 * @param token the names of the source and destination files
 * @return int return code 2 to inform that the path changed
 */

int caseCopy(char *token)
{
    char cTemp;
    //Set the cwd to home/ in order to allow any valid path
    chdir(getenv("HOME"));
    //Try to open for reading the source file
    FILE *fpSource = fopen(token, "r");
    if (fpSource == null)
    {
        //Stop the operation in case of failure
        puts("Source file cannot be opened!");
        return 0;
    }
    token = strtok(null, " ");
    //Try to open the destination file, create it if its not exist
    FILE *fpDest = fopen(token, "w");
    if (fpDest == null)
    {
        //Stop the opetation in case of failure
        puts("Destination file cannot be opened!");
        //Close the source file which opened earlier
        fclose(fpSource);
        return 0;
    }
    // Copy the file from source to destination
    while (fread(&cTemp, 1, 1, fpSource) == 1)
    {
        fwrite(&cTemp, 1, 1, fpDest);
    }
    //Close the source and destination files
    fclose(fpSource);
    fclose(fpDest);
    //reset the path
    chdir(path);
    return 2; // 2 code for reset the directory
}

/**
 * @brief Handler of DELETE command
 * This command deletes the input file.
 * At the beginning of the function, we will set the cwd to HOME in order to allow to delete
 * files that are not in the cwd.
 * At the end of the function, we will reset the path to the original path before this change.
 * Answer for Part 11: 
 * unlink is a system call, we used it in out implementation.
 * @param token the name of the file we want to delete
 * @return int return code 2 to inform the path has changed
 */

int caseDelete(char *token)
{
    //Set the cwd to \home
    chdir(getenv("HOME"));
    //Try to delete the file
    int out = unlink(token);
    if (out == -1)
    {
        puts("An error occured");
    }
    else
    {
        puts("File deleted successfully!");
    }
    //Reset the path
    chdir(path);
    return 2;
}

/**
 * @brief Handling of commands
 * This function decides which support function is needed to handle the input command
 * @param input input from the user
 * @return int return code
 */

int input_check(char *input)
{
    //Remove the \n and split by ' '
    char *token = firstTrims(input);
    int code = 0;
    //Handle EXIT commands
    if (!strcmp(token, EXIT))
    {
        return caseExit();
    }
    //Handle ECHO commands
    else if (!strcmp(token, ECHO))
    {
        token = strtok(null, " ");
        return caseEcho(token);
    }
    //Handle DIR commands
    else if (!strcmp(token, dir))
    {
        return caseDir();
    }
    //Handle CD commands
    else if (!strcmp(token, "CD"))
    {
        token = strtok(null, " ");
        return caseCd(token);
    }
    //Handle COPY commands
    else if (!strcmp(token, "COPY"))
    {
        token = strtok(null, " ");
        return caseCopy(token);
    }
    //Handle DELETE commands
    else if (!strcmp(token, "DELETE"))
    {
        token = strtok(null, " ");
        return caseDelete(token);
    }
    //Handle the TCP PORT commands
    //if the first token is "TCP"
    else if (!strcmp(token, "TCP"))
    {
        //If the second token is "PORT"
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
    //Handle LOCAL commands
    else if (!strcmp(token, "LOCAL"))
    {
        return caseLocal();
    }
    //Handly any other command
    else
    {
        /**
         * @brief Using system to send unknown commands
         * system is a library function, not a system call
         */
        // system(input);
        //create an array of strings and set it
        char **sys_calls = setArguments(input);
        //Try to create a child process
        int pid = fork();
        if (pid < 0)
        {
            exit(1);
        }
        else if (pid == 0)
        {
            //Execute the command using the child process
            execvp(sys_calls[0], sys_calls);
        }
        else
        {
            //Wait till the child process will die
            wait(null);
            //free the pointer to the array
            free(sys_calls);
        }
    }
    return 0;
}

/**
 * @brief Handle TCP PORT commands
 * This function will do nothing if the mod is 2->The socket is already connected,
 * otherwise, it will connect to the server using TCP socket.
 * The function will create duplicates of STDOUT, STDERR, and will redirect both
 * stdout and stderr to the socket
 * @param token 
 * @return int 
 */

int caseTCP(char *token)
{
    if (mod == 2)
    {
        return 0;
    }

    /**
     * @brief Credit
     * We took this implementation from Geeks for Geeks and adapt it to our needs
     */

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
    connfd = connect(sockfd, (SA *)&servaddr, sizeof(servaddr));
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

/**
 * @brief Handler of LOCAL commands 
 * This function will do nothing if the mod is 1->socket is closed,
 * otherwise it will close the socket at the server side using child process, and after the
 * child process died, it will close the socket and the connection on the client side.
 * The function will redirect stdout and stderr to their original file descriptors
 * @return int 
 */

int caseLocal()
{
    if (mod == 1)
    {
        return 0;
    }
    else
    {
        //send a command to close the socket at the server side
        printf("LOCAL");
        int pid = fork();
        if (pid < 0)
        {
            exit(1);
        }
        if (pid == 0)
        {
            write(sockfd, "LOCAL", 5);
            sleep(2);
            exit(1);
        }
        //Wait till the child process will close the child process will die, which
        //means the socket at the server side is closed now
        wait(NULL);
        //reset the mod
        mod = 1;
        //redirect the stdout and stderr
        dup2(outfd, STDOUT_FILENO);
        dup2(errfd, STDERR_FILENO);
        //close the duplicates file descriptors of stdout and stderr
        close(outfd);
        close(errfd);
        //close the file descriptors of the socket
        close(sockfd);
        close(connfd);
        return 0;
    }
}

/**
 * @brief Set the Arguments object
 * This function get an input and return an array of string that contains the input
 * splitted by the delimeter ' '
 * @param input 
 * @return char** 
 */

char **setArguments(char *input)
{
    char *token = strtok(input, " ");
    int k = 0;
    //Get the size of the output array
    while (token != null)
    {
        k++;
        token = strtok(null, " ");
    }
    //Add 1 in for the null at the last cell
    k++;
    token = strtok(input, " ");
    //Create and allocate the array
    char **sys_calls;
    sys_calls = (char **)malloc(k * sizeof(char *));
    //Set the first cell in the arry
    sys_calls[0] = token;
    k = 1;
    while (token != null)
    {
        //Set the values in the next cells
        token = strtok(null, " ");
        sys_calls[k] = token;
        k++;
    }
    //Set null at the last cell
    sys_calls[k] = null;
    return sys_calls;
}

/**
 * @brief Argument Handler
 * This function will deal with any arguments that the user entered
 * @param argc number of arguments
 * @param argv arguments values
 */

void ArgumentHandler(int argc, char **argv)
{
    //Create and set the string array
    char **sys_call;
    sys_call = (char **)malloc(argc * sizeof(char *));
    for (int i = 0; i < argc - 1; i++)
    {
        sys_call[i] = argv[i + 1];
    }
    sys_call[argc - 1] = null;
    //Create a child process in order to execute the command
    int pid = fork();
    if (pid < 0)
    {
        exit(1);
    }
    else if (pid == 0)
    {
        execvp(sys_call[0], sys_call);
    }
    else
    {
        //Wait till the child process will die
        wait(null);
        //free the allocated memory
        free(sys_call);
    }
}