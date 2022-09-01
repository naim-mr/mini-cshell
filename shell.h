#ifndef  _217898_part_
#define  _217898_part_
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdbool.h>

#define error(mess,code) {fprintf(stderr,"err: %s\n",mess); exit(code);}







void simple_cmd(char** argv);
void display_cmd(char* argv[]);
int parse_line_redir(char* s,char** argv[],char** in,char** out);
int parse_line(char* s,char*** argv);
void free_argv(char** argv[]);
void exec_bash(int fdesc);
void redir_cmd_1(char** argv, char*in, char* out);
void redir_cmd(char *** argv, char* in,char* out);
void display_cmd_piped(char *** argv);
int parse_line_pipes(char* s,char*** argv[],char** in,char** out);
void free_argv_2(char **** argv);

void scan_and_replace(char c,char** str);
#endif