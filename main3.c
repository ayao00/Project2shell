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
#include <fcntl.h>

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

void myPipe(char * args){

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
      printf("Type exit to exit shell. Or type a command u bozo\n");
    }
    return 0;
  }
}

char ** redirect(char * redirection){
  printf("REDIRECTED!!! %s\n", redirection);
  int fdnew;
  char * s = malloc(256);
  char ** parsed = malloc(256);
  char ** programs = malloc(256);
  int redirectin = 0;
  int backup;
  if(strchr(redirection,'<')){
      parsed = parse_args(redirection,"<");
      fdnew = open(parsed[1], O_RDONLY, 0664);
      backup = dup(STDIN_FILENO);
      dup2(fdnew, STDIN_FILENO);
  }
  else{
    parsed = parse_args(redirection,">");
    fdnew = open(parsed[1], O_CREAT | O_WRONLY, 0664);
    backup = dup(STDOUT_FILENO);
    dup2(fdnew, STDOUT_FILENO);
    redirectin = 1;
    printf("sending it to the file.");
  }
  if(strchr(parsed[1],'|')){
    myPipe(parsed[1]);
  }
  if(redirectin){
    dup2(backup, STDOUT_FILENO);
  }
  else{
    dup2(backup, STDIN_FILENO);
  }
  programs = parse_args(parsed[0], " ");
  return programs;
}

int main(){
  signal(SIGINT,sighandler);
  char * currentdirectory = malloc(256);
  char * s = malloc(256);
  char * current = malloc(256);
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
      printf("(ENTIRE COMMAND:) %s\n", args[i]);
      strcpy(current, args[i]);
      programs = parse_args(args[i], " ");
      if(strcmp("exit", programs[0]) == 0){
        return 0;
      }
      else if(strcmp("cd", programs[0])== 0){
        if(programs[1]){
          chdir(programs[1]);
        }
        else{
          chdir("~");
        }
      }
      else{
        if(strchr(current,'<') || strchr(current,'>')){
          programs = redirect(current);
        }
        if (run(programs) == 0){
          return 0;
        }
      }
      i++;
    }
  }
  return 0;
}
