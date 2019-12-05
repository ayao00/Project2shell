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

char ** parse_args( char * line ){
  char ** parsed_args = malloc(256);
  char * current;
  int i = 0;
  while(current){
    current = strsep(&line," ");
    parsed_args[i] = current;
    i++;
  }
  return parsed_args;
}

int main(){
  char * s = malloc(256);
  char * programs = malloc(256);
  char * line = malloc(256);
  char * parsed = malloc(256);
  char * current = malloc(256);
  while(1){
    printf("%s",getcwd(current, 256));
    printf("$ ");
    fgets(s, 256, stdin);
    s[strlen(s)-1]=0;
    if (strcmp(s,"exit") == 0){
    return 0;
    }
  }
  int f, child,status;
  f = fork();
  if(f){
    char ** args = parse_args(s);
    execvp(args[0] , args);
    child = wait(&status);
    printf("parent. pid: %d f: %d parent :%d\n", getpid(), f, getppid());
  }else{
    printf("child. pid: %d f: %d parent :%d\n", getpid(), f, getppid());
  }
  char ** args = parse_args(s);
  execvp(args[0] , args);
  return 0;
}
