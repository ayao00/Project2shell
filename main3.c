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
  printf("Type exit to exit shell. Or type a command.\n");
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

int run(char ** programs){
  int f;
  int status;
  int child;
  f = fork();
  signal(SIGINT,sighandler);
  if(f){
    waitpid(f, &status, 0);
    printf("Wait returned: %d status: %d return value: %d\n", child, status, WEXITSTATUS(status));
    return 1;
  }else{
    if(execvp(programs[0], programs) < 0){
      printf("Type exit to exit shell. Or type a command.\n");
    }
    return 0;
  }
}

void redirect(char * redirection){
  printf("REDIRECTED!!!!\n");
  printf("%s\n", redirection);
}

int main(){
  signal(SIGINT,sighandler);
  char * currentdirectory = malloc(256);
  char * s = malloc(256);
  char ** args;
  char ** programs;
  int f;
  int * status;
  int i;
  while(1){
    printf("%s",getcwd(currentdirectory, 256));
    printf("$ ");
    fgets(s, 256, stdin);
    s[strlen(s)-1]=0;
    args = parse_args(s, ";");
    i = 0;
    while(args[i]){
      if(strncmp("exit", args[i], 4) == 0){
        return 0;
      }
      else if(strchr(args[i],'<')||strchr(args[i],'>')){
        redirect(args[i]);
      }
      else{
        printf("%s\n",args[i]);
        programs = parse_args(args[i], " ");
        if(strncmp("cd", programs[0], 2)== 0){
          if(programs[1]){
            chdir(programs[1]);
          }
          else{
            //this still does not work btw
            chdir("~");
          }
        }
        else if (run(programs)  == 0){
          return 0;
        }
      }
      i++;
    }
  }
  return 0;
}
