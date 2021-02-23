#include "shell.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>




int len_ptr ( char* s[]){
    int j=0;
    if(*s==NULL) return 0;
    while(s[j]!=NULL){
    	j++ ; 
    	 }
    return j-1 ; 
}


void affiche_cmd(char* argv[]){
	int i=0;
  	while(argv[i]){
    if(argv[i+1]==NULL){
      printf("%s",argv[i]);
    }else {
		 printf("%s|",argv[i]);
		}
    i++;

		}
	
	}






int parse_line(char* s,char** argv[]){
    int i =0;
    int k =0;
    
    if((*argv=(char**)malloc(100*sizeof(char*)))==NULL)error("malloc failed",EXIT_FAILURE);
    char* line_g;
    int j ; 
    if((s)[i]=='\0') return 0;
    while((s)[i]!='\0' && (s)[i]!='\n'){
      if((line_g=(char*)malloc(1))==NULL) error("parse_line line_g malloc",EXIT_FAILURE);
  	  j=0;
      while( (s)[i]==' ' && (s)[i]!='\0'  ) i++;
      while((s)[i]!=' ' && (s)[i]!='\0'){
	   	  line_g[j]=(s)[i];
		    j++;
		    i++;
		    if(((line_g)=(char*)realloc(line_g,j))==NULL) error("realloc failed part1.c-l46",EXIT_FAILURE);

		  
	   }
	   line_g[j]='\0';
	   if(((*argv)[k]=(char*)malloc(j*sizeof(char)))==NULL)	  error("parse_line : malloc failed ", EXIT_FAILURE);
     for(long i=0;i<=j;i++)  ((*argv)[k])[i]=line_g[i];	 
	   free(line_g);
	   k++;
	 }
   (*argv)[k]=NULL;
    if((s)[i]=='\n'){
    	 s=s+i;
    	return 1;
    }
   return 0;
}

void exec_bash(int fdesc){
     int nb;
     char buf[1024+1];
     char temp[1024+1];
     char** argv;
     
     while((nb=read(fdesc,buf,1024))>0){
     	if(nb<0){
        perror("read");
        exit(EXIT_FAILURE);
      }
     	buf[nb]='\0';
     	int i=0;
     	bool continu =true ; 
     	while(i<=nb && continu==true ){
     		if(buf[i]=='\n') {
     			continu=false;
 	  		}else{
 			  	temp[i]=buf[i];    	
     		}
     		i++;
     	}
      if(nb==0)return;
      temp[i]='\0';
      parse_line(temp,&argv);
      simple_cmd(argv);
      free_argv(&argv);     
      if(!continu)lseek(fdesc,-(nb-i),SEEK_CUR);
    }
}  


int parse_line_redir(char* s,char** argv[],char** in,char** out){
    int i =0;
    int k =0;
    
    bool is_chevout_alloc=false;
    bool is_chevin_alloc=false;
    bool need_chevin_alloc=false  ;
    bool need_chevout_alloc=false;
    bool jump = false;
   
    size_t str_len;
    if((*argv=(char**)malloc(100*sizeof(char*)))==NULL)error("malloc failed",EXIT_FAILURE);
    char* line_g;
    int j ; 
    if((s)[i]=='\0') return 0;
    while((s)[i]!='\0' && (s)[i]!='\n'){
      if((line_g=(char*)malloc(1024))==NULL) error("parse_line line_g malloc",EXIT_FAILURE);
      j=0;
      while((s)[i]==' ' && (s)[i]!='\0')i++;
      if(s[i]=='\0') jump=true;
      if(!jump){
        while((s)[i]!=' ' && (s)[i]!='\0'){
        line_g[j]=(s)[i];
        j++;
        i++;
        }
        line_g[j]='\0';
        if(((*argv)[k]=(char*)malloc(j*sizeof(char)))==NULL)  error("parse_line : malloc failed ", EXIT_FAILURE);
        for(long i=0;i<=j;i++)  ((*argv)[k])[i]=line_g[i];
        str_len= strlen((*argv)[k]);
        if(need_chevin_alloc==true && is_chevin_alloc==false){
          if((*in=(char*)malloc(str_len+1))==NULL)error("malloc failed",EXIT_FAILURE);     
           for(size_t i=0;i<=str_len;i++) {
           (*in)[i]=((*argv)[k])[i];
            }
           is_chevin_alloc=true;
        }
        if(need_chevout_alloc && is_chevout_alloc==false){
           *out=(char*)malloc(str_len+1);     
           for(size_t i=0;i<=str_len;i++) {
           (*out)[i]=((*argv)[k])[i];
           }
           is_chevout_alloc=true;
        }
      

       if( !(strcmp(line_g,">"))){
        if(is_chevout_alloc) {
          printf("Erreur: une redirection par chevron maximum \n");
          return -1;
         }
         if(k==0) {
          *in=NULL ;
          }else{
            need_chevout_alloc=true;
          }
        }
       if(!(strcmp(line_g,"<"))){
        if(is_chevin_alloc) {
          printf("Erreur: une redirection par chevron maximum \n");
          return -1;
         }
        if(k==0) { 
          *in=NULL;
        }
        else{
          need_chevin_alloc=true;
        } 
     }
      free(line_g);
      k++;
   }else{
     free(line_g);
     k++;
   }
  }
    (*argv)[k]=NULL; 
    if(!is_chevin_alloc) *in=NULL;
    if(!is_chevout_alloc)*out=NULL;
    return 1;
}

void scan_and_replace(char c,char** str){
    int i=0;
    int j=0;
    int k=0;

    
    size_t len=strlen((*str));
    while((*str)[i]!='\0'){

     
      while((*str)[i]!=c && (*str)[i]!='\0'){
        
        i++;
        if(i>0)j=i-1;
        k=i+1;
      }
      if((*str)[i]=='\0' || (*str)[i+1]=='\0'){
        return;
      }
      if(i==0){
        if(len==1)return;

        if((*str)[k]==' ')return;
        if((*str)[k]!=' '){
        len++;
        if((*str=(char*)realloc(*str,len))==NULL) error("malloc failed l-265 part1.c\n",EXIT_FAILURE);
        for(long compteur=len;compteur>k;compteur--)(*str)[compteur]=(*str)[compteur-1];
        (*str)[k]=' ';
        i++;
        } 

      }else{
        if((*str)[j]==' ' && (*str)[k]==' '){
             return;
        }
        if((*str)[j]!=' ' && (*str)[k]!=' '){
         len=len+2;
          if((*str=(char*)realloc(*str,len))==NULL) error("malloc failed l-265 part1.c\n",EXIT_FAILURE);       
          long compteur;
          for(compteur=len-1;compteur>k+1;compteur--){
            (*str)[compteur]=(*str)[compteur-2];
            }
          (*str)[k+1]=' ';
          (*str)[k]=(*str)[i];
          (*str)[i]=' ';      
          i+=2;            
          }
        if((*str)[j]!=' ' &&(*str)[k]==' '){
          len++;
          if((*str=(char*)realloc(*str,len))==NULL) error("malloc failed l-265 part1.c",EXIT_FAILURE);
          for(long compteur=len;compteur>j;compteur--)(*str)[compteur]=(*str)[compteur-1];
          (*str)[i]=' ';
          i++;
        }
        if((*str)[j]==' ' &&(*str)[k]!=' '){
          len++;
          if(((*str)=(char*)realloc(*str,len))==NULL) error("malloc failed l-265 part1.c",EXIT_FAILURE);
          for(long compteur=len-1;compteur>k;compteur--){
            (*str)[compteur]=(*str)[compteur-1];
          }
          (*str)[k]=' ';
          i++;  
        }


      }

 }   


}
int parse_line_pipes(char* s,char**** argv,char** in,char** out){
    int i =0;
    int k =0;
    bool is_chevout_alloc=false;
    bool is_chevin_alloc=false;
    bool need_chevin_alloc=false  ;
    bool need_chevout_alloc=false;
    bool jump = false;
    size_t str_len;
    int nb_pipe=0;

    bool pipe=true;
    

  

    if((*argv=(char***)malloc(100*sizeof(char**)))==NULL) error("parse_line line_g malloc",EXIT_FAILURE);
    if(((*argv)[0]=(char**)malloc(100*sizeof(char*)))==NULL) error("parse_line line_g malloc",EXIT_FAILURE);
    char* line_g;
    int j ; 
    if((s)[i]=='\0') return 0;
    while((s)[i]!='\0' && (s)[i]!='\n'){
      if(pipe){
        
          if(((*argv)[nb_pipe]=(char**)malloc(101*sizeof(char*)))==NULL) error("parse_line line_g malloc",EXIT_FAILURE);
          pipe=false;
       }
      
      if((line_g=(char*)malloc(1024))==NULL) error("parse_line line_g malloc",EXIT_FAILURE);
      j=0;
    

      while((s)[i]==' ' && (s)[i]!='\0')i++;
      
      if(s[i]=='\0') jump=true;
     if(!jump){
        while((s)[i]!=' ' && (s)[i]!='\0'){
          
           line_g[j]=(s)[i];
           
           j++;
           i++;
        }
        line_g[j]='\0';
        if(line_g[0]=='$') {
            long env_len=strlen(getenv(line_g+1))+1;
            strcpy(line_g,getenv(line_g+1));
            line_g[env_len]=0;
          }
      
        if((((*argv)[nb_pipe])[k]=(char*)malloc((1024)*sizeof(char)))==NULL)  error("parse_line : malloc failed ", EXIT_FAILURE);
        for(long i=0;i<=j;i++)  (((*argv)[nb_pipe])[k])[i]=line_g[i];
        
        str_len= strlen(((*argv)[nb_pipe])[k]);
        if(need_chevin_alloc==true && is_chevin_alloc==false){
      
          if((*in=(char*)malloc(str_len+1))==NULL) error(" : malloc failed ", EXIT_FAILURE);;     
           for(size_t i=0;i<=str_len;i++) {
           (*in)[i]=(((*argv)[nb_pipe])[k])[i];
            }
           is_chevin_alloc=true;
        }
        if(need_chevout_alloc && is_chevout_alloc==false){
           if((*out=(char*)malloc(str_len+1))==NULL)error(" : malloc failed ", EXIT_FAILURE);     
           for(size_t i=0;i<=str_len;i++) {
           (*out)[i]=(((*argv)[nb_pipe])[k])[i];
           }
           is_chevout_alloc=true;
        }
        
       if(!strcmp(line_g,"|")){
              ((*argv)[nb_pipe])[k]=NULL;
              k=0;
              nb_pipe++;
              pipe=true;
                        
        }
        if( !(strcmp(line_g,">"))){
         if(is_chevout_alloc) {
          printf("Erreur: une redirection par chevron maximum \n");
          return -1;
         }
         if(k==0) {
          *out=NULL ;
          }else{
            need_chevout_alloc=true;
          }
        }
        if( !(strcmp(line_g,"<"))){
        
         if(is_chevin_alloc) {
          printf("Erreur: une redirection par chevron maximum \n");
          return -1;
         }
         if(k==0) {
          
          *in=NULL;
         }
         else{
    
          need_chevin_alloc=true;
         } 
        }
     
        free(line_g);
        if(!pipe) k++;
    
     }else{
       free(line_g);
       if(!pipe)k++;
     
    }
 } 
 if(!is_chevin_alloc)*in=NULL;
 if(!is_chevout_alloc)*out=NULL;
 (*argv)[nb_pipe+1]=NULL;
 return 1;
}




void affiche_cmd_piped(char *** argv){
  int i=0;
  printf("{");
  while(argv[i]){
    printf("[");
    
    affiche_cmd(argv[i]);
    printf("]");
    i++;
  }
  printf("}\n");
}



void redir_cmd_1(char** argv, char*in, char* out){ 
    int fdout;
    int fdin;
    if(in==NULL && out==NULL){
      simple_cmd(argv);
    }else if(out!=NULL && in==NULL){
      fdout=open(out,O_WRONLY|O_CREAT,0644);
      int stdout=dup(STDOUT_FILENO);
      int i=0; 
      while(strcmp(argv[i],out))i++;
      char** temp= (char**)malloc(i*sizeof(char*));
      for(int j=0;j<=i-2;j++){
        temp[j]=argv[j];
      }
     dup2(fdout,STDOUT_FILENO);
     simple_cmd(temp);
     
     dup2(stdout,STDOUT_FILENO);  
    
     free(temp);
   }else if(in!=NULL && out==NULL){
   
    if((fdin=open(in,O_RDONLY,0644))<0){
      perror("open");
      return;
    }
   
    int stdin=dup(STDIN_FILENO);
    int i=0;
    while(strcmp(argv[i],in))i++;
    int j=i-2;
    char** temp= (char**)malloc(i*sizeof(char*));
    int k;
    for(k=0;k<=j;k++){
       temp[k]=argv[k];
    }
    temp[k]=0;
    dup2(fdin,STDIN_FILENO);
    affiche_cmd(temp);
    printf("\n");
    simple_cmd(temp);
    close(fdin);
    dup2(stdin,STDIN_FILENO);  
    free(temp);       
  }else {
    bool first_in=false;
    bool first_out=false;
    for(int i=0;argv[i]!=NULL && !first_out && !first_in;i++){
      if(!first_out && !strcmp(argv[i],">")) first_out=true ;
      if(!first_in && !strcmp(argv[i],"<")) first_in=true ;
     }   
    if(first_out){
      printf("redirection (... > ... < ...) inutile\n");
      return;
    }

    if((fdout=open(out,O_WRONLY| O_CREAT,0644))<0){
      perror("open");
      return;
    }
   if((fdin=open(in,O_RDONLY,0644))<0){
      perror("open");

      return;
    }
      int i=0; 
      int stdin= dup(STDIN_FILENO);
      int stdout= dup(STDOUT_FILENO);
      while(strcmp(argv[i],in))i++;
      char** temp= (char**)malloc(i*sizeof(char*));
      for(int j=0;j<=i-2;j++){
         temp[j]=argv[j];
      }
      dup2(fdin,STDIN_FILENO);
      dup2(fdout,STDOUT_FILENO);
      simple_cmd(temp);
      close(fdin);
      close(fdout);
      dup2(stdin,STDIN_FILENO);
      dup2(stdout,STDOUT_FILENO);
      free(temp);
 }
}
 

  
void simple_cmd(char* argv[] ){
  bool wake_up=false;
  if(!argv[0]) return;
	int k=0;
  bool continu=true;
  int inc_equal=0;
 
  long str_len=strlen(argv[0]);
  while((argv[0])[k]!='\0' && continu){
  
    if((argv[0])[k]=='='){
      inc_equal=k;
      continu=false;
    }
    k++;
  }
  if(!strcmp(argv[0],"fg") ||!strcmp(argv[0],"bg")){
    printf("ici\n");
     wake_up=true;                            
     int n;
     if(argv[1]==NULL){
        n=1;
     }else{
      n=atoi(argv[1]);
     } 
      char bufread[8192];
     
     int fdesc=open("pid_list",O_RDONLY,0644);
        
      int nbread;
      nbread=read(fdesc,bufread,8192);
      
    
      
      int j=0;
      int i=1;
      while(i<n && j<nbread){
        if(bufread[j]=='p')i++;
        j++;
      }
      char* pid_s=(char*)malloc(16);
      int k=0;
      while(bufread[j]!='p' &&k<16){
        pid_s[k]=bufread[j];
        j++;
        k++;
       }

      pid_s[k]='\0';
      printf("pids %s\n",pid_s);
      int pid_child=atoi(pid_s);
      free(pid_s);
      close(fdesc);
      int status2;
      
      if(!strcmp(argv[0],"fg")){
        kill(pid_child,SIGCONT);
        printf("ok\n");
        waitpid(pid_child,&status2,WUNTRACED);
        return;

       }    
      if(!strcmp(argv[0],"bg")){  
        printf("bg %d\n",pid_child);
          sigset_t mask;
          sigemptyset(&mask);
          sigaddset(&mask,SIGTTIN);
          sigprocmask(SIG_BLOCK,&mask,NULL);
         // kill(pid_child,SIGUSR1);
          kill(pid_child,SIGCONT);
          
          //kill(pid_child,SIGUSR1);
          //waitpid(pid_child,&status2,WNOHANG);
          return;
        }
      
  
  }
  if(inc_equal>0 && inc_equal<(str_len-1)){

    char str[64];
    char value[64];
    int compteur=0;
    for(int j=inc_equal+1;j<str_len;j++){
      value[compteur]=(argv[0])[j];
      compteur++;
    }
    value[compteur]=0;
    int i;
    for(i=0;i<inc_equal;i++)str[i]=(argv[0])[i];
    str[i]=0;
    setenv(str,value,1);
    return;
  }
  if(!strcmp(argv[0],"exit")){
    exit(EXIT_SUCCESS);
    
    }		
	 
    if(!strcmp(argv[0],"cd")){
    	chdir(argv[1]);
	}
	if(!wake_up){
  pid_t p = fork();
   int status;
	
  if(p){

		int pid_child =waitpid(-1,&status,WUNTRACED);
      if(WIFEXITED(status)){
      char buff[1024];
      char buff2[1024];
      char pid_child_s[16];
      sprintf(pid_child_s,"%d",pid_child);
      int fd1 =open("pid_list",O_RDONLY);
      int nb=read(fd1,buff,8192);   
      int i=0;
      int k=0;
      int z=0; 
      char* buffline=(char*)malloc(125);
      while(nb!=0  ){
          z=0;
          while(buff[i]!='p'){
            buffline[z]=buff[i];
            i++;
            nb--;
            z++;
          }
          i++;
          nb--;
      
          buffline[z]='\0';         
          if(strcmp(buffline,pid_child_s)){
             int temp=0;
             while(temp<z){
                 
                  buff2[k]=buffline[temp];
                  temp++;
                  k++;
                
              }
              buff2[k]='p';
              k++;
          
          }
      }
    free(buffline);
    buffline=NULL;
    if((fd1=open("pid_list",O_WRONLY|O_TRUNC))<0){
      perror("open");
      exit(EXIT_FAILURE);
    }
    if((write(fd1,buff2,k))<0){
      perror("write");
      exit(EXIT_FAILURE);
    }
    close(fd1);
		}
    return;
   
	}else{
        char* pid_s=(char*)malloc(10);
        sprintf(pid_s,"%dp",getpid());
        int len=0;
        while(pid_s[len]!='p')len++;
        len++;
        pid_s[len]=0;
        int fd;
        if(( fd= open("pid_list",O_WRONLY|O_APPEND|O_CREAT,0644))<0){
          perror("open");
          exit(EXIT_FAILURE);
        }
        int nb;
        lseek(fd,0,SEEK_END);
        if((nb=write(fd,pid_s,len))<0){
          perror("write");
          exit(EXIT_FAILURE);
        }
        close(fd);
        free(pid_s);
        execvp(argv[0],argv);    
        
  }
}
}

int len_ptr_2(char*** argv){
    int i=0;
    while(argv[i]!=NULL)i++;
    return i;

}

void redir_cmd(char *** argv, char* in,char* out){
   
    int pipe_nb = len_ptr_2(argv)-1;
    int fd[2];
    int fd_stock=0;
    
    if(pipe_nb==0){
      redir_cmd_1(argv[0],in,out);
     return;
    }
    int stdin = dup(STDIN_FILENO);
    int stdout = dup(STDOUT_FILENO);
    for(int i=0;i<=pipe_nb;i++){
           pipe(fd);


           if(fork()){
                wait(NULL);
                close(fd[1]);
                fd_stock=fd[0];


           }else{
            //je me suis ici un peu aider d'internet car je mélanger les différents dup
              dup2(fd_stock,STDIN_FILENO);
              if(i<pipe_nb)dup2(fd[1],STDOUT_FILENO);
              close(fd[0]);
              execvp((argv[i])[0],argv[i]);
              exit(1);
                   
 

           }

         }


   dup2(stdin,STDIN_FILENO);
   dup2(stdout,STDOUT_FILENO);

  return;

}





void free_argv_2(char **** argv){
  int i=0;

  while((*argv)[i]!=NULL){
    int j=0;
    
    while(((*argv)[i])[j]!=NULL){
      free(((*argv)[i])[j]);
      ((*argv)[i])[j]=NULL;
      j++;
    }
    (*argv)[i]=NULL;

  i++;
  }

  free(*argv);
  *argv=NULL;
}

void free_argv(char** argv[]){
	int i=0;
	while((*argv)[i]!=NULL){
		free((*argv)[i]);
        (*argv)[i]=NULL;
        i++;
	}
	free(*argv);
	argv=NULL;
}



	



