#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sysinfoapi.h>
#include <windows.h>
#include "prototypes.h"


int main()
{
    char input[100];    // allocate memory to store user input (arbitrary value)
    printf("\nWelcome to the homemade shell:\n");
    menu();    // print shell menu 
    gets(input);    // retrieve user input
    while (input[0] == ' ' || strcmp(input, "") == 0) {    // check input validity
        printf("Error: invalid command\n");
        menu();
        gets(input);
    }
    char *token = strtok(input, " ");    // store first "string" as token
    while (strcmp(token, "quit") != 0) {    // while user hasn't entered "quit", keep looping the shell
        if (strcmp(token, "calc") == 0)    // prefix calculation
            calc(token);
        else if (strcmp(token, "time") == 0)    // print current time and date
            local_tmd();
        else if (strcmp(token, "path") == 0)    // print current working directory
            print_cwd();
        else if (strcmp(token, "sys") == 0)    // print OS and CPU info
            sys();
        else if (strcmp(token, "put") == 0)    // copy files to new/existing directory
            put_file(token);
        else if (strcmp(token, "get") == 0)    // print file contents to screen
            print_file(token);
        else 
            printf("\nError: invalid command\n");      
        // loop again
        fflush(stdout);
        menu();
        gets(input);
        while (input[0] == ' ' || strcmp(input, "") == 0) {
            printf("Error: invalid command\n");
            menu();
            gets(input);
        }
        token = strtok(input, " ");
    }       
    return 0;
}