//- Name: prototypes.h
#ifndef PROTOTYPES_H
#define PROTOTYPES_H
//-- Function prototypes.---
void menu();
void local_tmd();
void sys();
void print_cwd();
bool quit_shell(char *input);
void file_copy(int num_files, char *dirname, char *array[]);
void put_file(char *token);
void print_file(char *token);
void calc(char *token);
int prefix_calc(int *curr_index, int last_index, char *array[]);
#endif 
