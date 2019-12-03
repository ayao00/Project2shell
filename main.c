#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>

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

int main(int argc, char * argv[]){
  char * s = malloc(256);
  if(argc <= 1){
    printf("Enter the command line invocation: ");
    fgets(s, 256, stdin);
    s[strlen(s)-1]=0;
  }else{
    s = argv[1];
  }
  char ** args = parse_args(s);
  execvp(args[0] , args);
  return 0;
}
