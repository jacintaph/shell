#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sysinfoapi.h>
#include <windows.h>

int prefix_calc(int *curr_index, int last_index, char *array[])
{
    if (*curr_index > last_index)    // check working within range
        return 0xFFFFFFF;
    if (strlen(array[*curr_index]) == 1 && (*(array[*curr_index]) == '+')) {    // perform operand addition    
        (*curr_index)++; //added
        int operand1 = prefix_calc(curr_index, last_index, array);
        int operand2 = prefix_calc(curr_index, last_index, array);
        return operand1 + operand2;
    }
    else if (strlen(array[*curr_index]) == 1 && (*(array[*curr_index]) == '-')) {    // perform operand subtraction
        (*curr_index)++; //added
        int operand1 = prefix_calc(curr_index, last_index, array);
        int operand2 = prefix_calc(curr_index, last_index, array);
        return operand1 - operand2;
    }
    else {  
        int result = atoi(array[(*curr_index)++]);    // array element is integer
        return result;
    }
}

void menu()    // print shell menu (command list) to screen
{
    printf("\nShell commands: \n");
    printf("1. Print the result of the prefix expression 'exp': \t < calc 'exp' >\n");
    printf("2. Print out current local time and date: \t\t < time >\n");
    printf("3. Print current working directory: \t\t\t < path >\n");
    printf("4. Print name and version of the OS and CPU type: \t < sys >\n");
    printf("5. Put files 'filenames' in the directory 'dirname': \t < put 'dirname' 'filename(s)' [-f] >\n");
    printf("6. Print the content of the file 'filename': \t\t < get 'filename' >\n");
    printf("7. End the program: \t\t\t\t\t < quit >\n---\n");
}

bool quit_shell(char *input) 
{
    char *token = strtok(input, " ");
    if (strcmp(input, "quit") == 0)    // if user enters quit
        return true;    // quit shell
    return false;
}

void calc(char *token)
{
    int i = 0;    // track array index
    int num = 0;    // track size of prefix expression
    char *array[50];    // arbitrary value to pre-allocate array memory (num of inputs)
    token = strtok(NULL, " ");    // move to next "string" from the input
    while (token != NULL) {    // while not yet at the end of the input
        array[i] = (char*)malloc(strlen(token) * sizeof(char));    // allocate memory for 2D array
        strcpy(array[i], token);    // save token in array
        token = strtok(NULL, " ");
        num++;
        i++;
    }    
    if (num < 2)    
        printf("\nError: invalid command\n");    // invalid expression
    else {    // calc prefix expression
        const int first_arg = 0;       
        int curr_pos = first_arg;
        int value = prefix_calc(&curr_pos, num, array);    // call func to calculate expression   
        printf("Answer = %d\n", value);    // print answer
    }
}

void local_tmd()
{
    time_t local_time_date;
    struct tm *local_td;
    time(&local_time_date);
    local_td = localtime( &local_time_date );    // call localtime func to retrieve time and date info
    printf("Current local time and date: %s", asctime(local_td));
}

void sys()    // print CPU and OS info
{
    struct utsname uts;    // create instance of structs for accessing OS and CPU info
    uname(&uts);
    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo);    // to find num of processors
    DWORD dwVersion = 0;
    DWORD dwMajorVersion = 0;
    DWORD dwMinorVersion = 0;
    DWORD dwBuild = 0;
    dwVersion = GetVersion();    // Windows version.
    if (dwVersion < 0x80000000)    // build num
        dwBuild = (DWORD)(HIWORD(dwVersion));
    
    dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
    dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
     
    printf("\nOS Name is: %s\n", uts.sysname);
    printf("GetVersion() : %d.%d (%d)  %s\n", dwMajorVersion, dwMinorVersion, dwBuild, uts.version);
    printf("CPU Type: %s\n", uts.machine);
    printf("Number of processors: %u\n", siSysInfo.dwNumberOfProcessors);    // num of CPU processors
}

void print_cwd()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)    // call getcwd func to retrieve current working directory
        printf("Current working directory: %s\n", cwd);
    else {
       printf("Error retrieving current working directory");
    }
}

void file_copy(int num_files, char *dirname, char *array[])
{
    char cwd[PATH_MAX];
    char *fname, ch;
    getcwd(cwd, sizeof(cwd));    // find cwd to create path for copying files to
    char* src_dir = strcat(cwd, "\\"); 
    char *dst_path = strcat(src_dir, dirname);    // creating variable for destination directory                 
    dst_path = strcat(dst_path, "\\");                   
    int count = 2;    // track where the file names start in user input array
    for (int j = 0; j < num_files; j++) {    // for each file name entered 
        fname = array[count];
        FILE *fptr1, *fptr2;
        fptr1 = fopen(fname, "r");    // open and read file if it exists
        if (fptr1 == NULL) {
            printf("\nError: File '%s' does not exist\n", fname);
        }
        else {
            dst_path = strcat(dst_path, fname);    // add file name to dir path                                                                 
            fptr2 = fopen(dst_path, "w");    // create and write to new file in dest fir (i.e. copying file across)
            if (fptr2 == NULL) 
                printf("\nError: File creation error\n");
            else {
                while ((ch = fgetc(fptr1) ) != EOF)   // writing to file
                fputc(ch, fptr2);                                 
            }     
            fclose(fptr1);
            fclose(fptr2);
            int size = strlen(dst_path);
            dst_path[size - strlen(fname)] = '\0';    // remove file name from dir path, so next file can be processed                                                  
        } 
        count++;   
    }               
}

void put_file(char *token)
{
    int i = 0, num = 0, count = 0;  
    char ch;
    char *array[50], *dirname, *fname;
    while (token != NULL) {
        array[i] = (char*)malloc(strlen(token) * sizeof(char));    // allocate memory for file names
        strcpy(array[i], token);    // save file names into array
        token = strtok(NULL, " ");
        num++;
        i++;
    } 
    if (num < 3)    // need at least the command, dir name, and 1 file name 
        printf("\nError: invalid command\n");
    else {
        dirname = array[1];    // save directory name
        if (strcmp(array[num-1], "-f") == 0) {    // overwrite existing dir/create new dir
            DIR* dir = opendir(dirname);
            if (dir) {    // Directory exists
                struct dirent *next_file;
                char filepath[256];
                while ((next_file = readdir(dir)) != NULL) {    // while there are still files in the directory
                    sprintf(filepath, "%s/%s", dirname, next_file->d_name);    // creating file path
                    remove(filepath);    // remove the file
                }                 
                closedir(dir);
            }
            else    // dir doesn't exist, create new
                mkdir(dirname, 0755);    // make new dir with required permissions
            int num_files = num - 3;    // number of files to copy
            file_copy(num_files, dirname, array);    // copy files 
        }
        else {    // no -f entered
            DIR* dir = opendir(dirname); 
            if (dir) {    // first check if dir exists
                printf("\nError: '%s' already exists. Add < -f > to command to overwrite the directory.\n", dirname);
                closedir(dir);
            }
            else {    // dir doesn't exist, create it
                mkdir(dirname, 0755);
                int num_files = num - 2;    // number of files to copy
                file_copy(num_files, dirname, array);    // copy the files
                closedir(dir);
            }
        }        
    }          
}

void print_file(char *token)
{
    printf("\n");
    int i = 0;
    int num = 0;
    char *fname, *array[3];
    while (token != NULL) {
        array[i] = (char*)malloc(strlen(token) * sizeof(char));    // allocate memory for input
        strcpy(array[i], token);
        token = strtok(NULL, " ");
        num++;
        i++;
    } 
    if (num < 2)    // need at least the command, and file name
        printf("\nError: invalid command\n");
    else {
        fname = array[1];       
        const int lines = 40;    // number of lines to print at once
        FILE *fptr;
        char s;
        fptr = fopen(fname, "r");    // open file to read
        if (fptr == NULL) 
            printf("\nError: File '%s' does not exist\n", fname);
        else {       
            int line_num = 0;
            s = fgetc(fptr);
            while (s != EOF) {    // while not at end of file
                printf("%c", s);    // print the line to screen
                if (s == '\n')    // count the number of lines printed
                    line_num++;
                s = fgetc(fptr);
                if (line_num == 40) {    // if reached 40 line cut-off
                    line_num = 0;
                    char c;
                    fflush(stdout);
                    getchar();    // wait for key press before printing next 40 lines
                }
            }
        }
        fclose(fptr);
    }
    printf("\n");
}