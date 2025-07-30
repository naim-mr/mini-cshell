#include "shell.h"
#include <stdbool.h>
#include <signal.h>


static void handler(int sig){
      
      int pid=atoi(getenv("pid_father"));
      pid_t pid_cur=getpid();
      
      if(pid==pid_cur)return;
      if(pid_cur!=pid){
        
        
        kill(pid_cur,SIGSTOP);
        

      }
      

    }
static void sigttin_hand(int sig){
    //printf("%d\n",getpid());
    kill(getpid(),SIGSTOP);
}

static void sigusr_hand(int sig){
       int fdnull =open("/dev/null",O_RDONLY);
       dup2(fdnull,STDIN_FILENO);

}

static void sigint_hand(int sig){
      int pid = atoi(getenv("pid_father"));
      pid_t pid_cur = getpid();
      if(pid==pid_cur)
        return; 
      if(pid_cur!=pid)
        kill(pid_cur,SIGKILL);
      
}

 
int main(int argc , char** argv){
  // struct sigaction back_stdin; 
  // back_stdin.sa_handler=sigttin_hand;
  // back_stdin.sa_flags=SA_RESTART|SA_NOCLDWAIT|SA_NOCLDSTOP;
  // sigaction(SIGTTIN,&back_stdin,NULL);
  // sigaction(SIGTTOU,&back_stdin,NULL);
 

 
  // if(getenv("pid_father")==NULL){
  //    int fd=open("pid_list",O_CREAT|O_TRUNC,0644);
  //    //printf("%d\n",fd);
  //    close(fd);

  // }
  // pid_t pid= getpid();
  // char s[10];
  // sprintf(s,"%d",pid);
  // setenv("pid_father",s,0);
  
  // struct sigaction background;
  // background.sa_handler=sigusr_hand;
  // background.sa_flags=SA_RESTART|SA_NOCLDWAIT|SA_NOCLDSTOP;
  // sigaction(SIGUSR1,&background,NULL);
  // struct sigaction ctrlz;
  // ctrlz.sa_handler=handler;
  // ctrlz.sa_flags =SA_RESTART|SA_NOCLDWAIT|SA_NOCLDSTOP;
  // sigaction(SIGTSTP,&ctrlz,NULL);

  struct sigaction ctrlc;
  ctrlc.sa_handler = sigint_hand;
  ctrlc.sa_flags = SA_RESTART|SA_NOCLDWAIT|SA_NOCLDSTOP;
  sigaction(SIGINT,&ctrlc,NULL);
  
 
  char* line;
  char* dir;
 
  if((line=(char*)malloc(1024)) == NULL)
    error("Malloc of var line failed",EXIT_FAILURE);
	if((dir=(char*)malloc(128)) == NULL)
    error("Malloc of var dir failed",EXIT_FAILURE);
  
  int nb; 
  int script;
  
  if((script=open(argv[1],O_RDONLY))) exec_bash(script);        
  
  char*** argv_2;

  while((nb=read(STDIN_FILENO,line,1024))>0){
      if(nb<0){
        perror("Read failure");
        exit(EXIT_FAILURE);
      }  
      // TODO: check output 
      printf("\n");
      char* in;
      char *out;
      if((in=(char*)malloc(100)) == NULL)
        error("malloc failed",EXIT_FAILURE);
      if((out=(char*)malloc(100)) == NULL)
        error("malloc failed",EXIT_FAILURE);
      
      getcwd(dir,128);

      // TODO: check output 
      printf(" \x1B[32m%s$:\x1B[0m\n ",dir);
      
      line[nb-1]=0;

      scan_and_replace('>',&line);
      scan_and_replace('<',&line);
      scan_and_replace('|',&line);

      parse_line_pipes(line,&argv_2,&in,&out);
      redir_cmd(argv_2,in,out);
      
      free(in); 
      free(out);
      free_argv_2(&argv_2);
     }
  free(dir);
  free(line);
  //unsetenv("pid_father");
  return EXIT_SUCCESS;
}


 