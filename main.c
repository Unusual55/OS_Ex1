#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SHELL_OUT 1
#define SERVER_OUT 2
#define TO_SERVER "TCP PORT"
#define TO_SHELL "LOCAL"
#define ECHO "ECHO"
#define PROMPT "I can do this all day"
#define MAX_LENGTH 256
#define EXIT "EXIT"
#define dir "DIR\n"
#define null NULL
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include "shell.h"
#include <fcntl.h>

// int input_check(char * input);

// int main()
// {
//     char *prompt = PROMPT;
//     char *input;
//     int size=10;
//     int input_size;
//     int code=0;
//     char path[MAX_LENGTH];
//     int pid;
//     // char * token;
//     while (1)
//     {
//         getcwd(path, sizeof(path));
//         if(path==null){
//             exit(1);
//         }
//         printf("%s %s\n",path, prompt);
//         while (1)
//         {
//             input = (char *) malloc (size);
//             if(input==NULL){
//                 exit(1);
//             }
//             input_size=getline(&input, &size, stdin);
//             if(input_size==-1){
//                 free(input);
//                 exit(1);
//             }
//             // puts(input)
//             system(input);
//             pid=fork();
//             if(pid==-1){
//                 free(input);
//                 puts("Cannot create a child proccess");
//                 exit(0);
//             }
//             if(pid==0){
//                 code=input_check(input);
//                 free(input);
//                 exit(0);
//             }
//             wait(null);
//             free(input);
//             if(code==1){
//                 exit(0);
//             }
//             else if (code==2)
//             {
//                 chdir(path);
//             }            
//             break;
//         }
//     }
// }

// int input_check(char * input){
//     const char regex[2] = " ";
//     const char pregex[2] = "\n";
//     char * token;
//     token=strtok(input,pregex);
//     token = strtok(input, regex);
//     if(token==NULL){
//         return 0;
//     }
//     if(!strcmp(token, EXIT) || !strcmp(token, "exit")){
//         return 1; //1 for exit code
//     }
//     else if(!strcmp(token, ECHO) || !strcmp(token, "echo")){
//         token=strtok(null,regex);
//         while(token!=null){
//             printf("%s ", token);
//             token=strtok(null,regex);
//         }
//         puts("\n");
//         return 0;//0 for continue code
//     }
//     else if(!strcmp(token, dir) || !strcmp(token, "dir")){
//         DIR * directory;
//         struct dirent *entry;
//         int files=0;
//         directory=opendir(".");
//         if(directory==null){
//             puts("cannot open the directory");
//             return 1;
//         }
//         else{
//             puts("Directory is opened!\n");
//             files++;
//             while(entry=readdir(directory)){
//                 printf("%d: %s\n", files, entry->d_name);
//                 files++;
//             }
//             closedir(directory);
//             return 0;
//         }
//     }
//     else if(!strcmp(token, "CD") || !strcmp(token, "cd")){
//         token=strtok(null, regex);
//         if(!strcmp(token, "..")){
//             chdir("..");
//             return 0;
//         }
//         else{
//             if(chdir(token)!=0){
//                 puts("Cannot enter this directory\n");
//                 return 0;
//             }
//             if(!strcmp(token, "/tmp")){
//                 puts("Cannot enter this directory\n");
//                 return 0;
//             }
//             if(!strcmp(token, "/error")){
//                 puts("Cannot enter this directory\n");
//                 return 0;
//             }
//         }
//     }
//     else if(!strcmp(token, "COPY") || !strcmp(token, "copy")){
//         //Definition and validation check
//         token=strtok(null, regex);
//         char cTemp;
//         chdir(getenv("HOME"));
//         FILE *fpSource = fopen(token, "r");
//         if(fpSource==null){
//             puts("Source file cannot be opened!");
//             return 0;
//         }
//         token=strtok(null, regex);
//         FILE *fpDest=fopen(token,"w");
//             if(fpDest==null){
//             puts("Destination file cannot be opened!");
//             fclose(fpSource);
//             return 0;
//         }
//         //Copy the file from source to destination
//         while(fread(&cTemp, 1, 1, fpSource)==1){
//             fwrite(&cTemp, 1, 1, fpDest);
//         }
//         fclose(fpSource);
//         fclose(fpDest);
//         return 2;//2 code for reset the directory
//     }
//     else if(!strcmp(token, "DELETE") || !strcmp(token,"delete")){
//         chdir(getenv("HOME"));
//         token = strtok(null, regex);
//         int out=unlink(token);
//         if(out==-1){
//             puts("An error occured");
//         }
//         else{
//             puts("File deleted successfully!");
//         }
//     }
//     return 0;
// }
