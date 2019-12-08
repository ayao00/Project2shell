#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

static void sighandler(int signo){
  printf("Type exit to exit shell. Or type a command u bozo\n");
}

char ** parse_args( char * line , char * separator){
  char ** parsed_args = malloc(256);
  char * current;
  int i = 0;
  //while you can continue to strsep, continue to strsep.
  while((current = strsep(&line, separator))){
    parsed_args[i] = current;
    i++;
  }
  //when you have all the pieces, return the array of pieces.
  return parsed_args;
}

int main(){
  signal(SIGINT,sighandler);
  char * currentdirectory = malloc(256);
  char * s = malloc(256);
  char ** args;
  int f;
  int * status;
  while(1){
    printf("%s",getcwd(currentdirectory, 256));
    printf("$ ");
    fgets(s, 256, stdin);
    s[strlen(s)-1]=0;
    args = parse_args(s, " ");
    printf("%s\n", s);
    printf("%s\n", args[0]);
    if(strncmp("exit",args[0],4) == 0){
      return 0;
    }else if(strncmp("cd",s)== 0){
      printf("in this part\n");
      chdir("..");
    }else{
      f = fork();
      if(!f){
        execvp(args[0], args);
      }else{
        waitpid(f, status, 0);
      }
      printf("failed;\n");
    }
  }
  return 0;
}
