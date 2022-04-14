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

int main(int argc, char *argv[])
{
    //Reset the path
    reset_path();
    char *input;
    int code;
    size_t size = 10;
    size_t input_size;
    //Check if the used enterd a command in the argument
    if (argc > 1)
    {
        //Execute the command
        ArgumentHandler(argc, argv);
    }
    while (1)
    {
        /**
         * @brief Changes of Part 2
         * the printf row was the one we were asked to change in part 2 of the assignment
         */
        //printf("%s\n", get_prompt());
        //Print the path and the prompt
        printf("%s %s\n", get_path(), get_prompt());
        while (1)
        {
            input = (char *)malloc(size);
            if (input == NULL)
            {
                exit(1);
            }
            //Get an input from the user
            input_size = getline(&input, &size, stdin);
            if (input_size == -1)
            {
                free(input);
                exit(1);
            }
            //Execute the command
            code = input_check(input);
            //Exit if the code is 1
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