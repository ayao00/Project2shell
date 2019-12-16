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
#include <ctype.h>

static void sighandler(int signo){
  printf("Type exit to exit shell. Or type a command.\n");
}

void trim(char * input){
  char * last;
  int index = 0;
  int i = 0;
  // trims off the leading whitespaces
  while(isspace(input[index])){
    index++;
  }
  while(input[i + index] != '\0'){
    input[i] = input[i+index];
    i++;
  }
  input[i] = '\0';
  //trims off the trailing whitespaces
  last = input + strlen(input) - 1;
  while(last > input && isspace(*last)){
    last--;
  }
  *(last+1) = 0;
}

char ** parse_args( char * line , char * separator){
  //This function separates a line with a provided separator
  char ** parsed_args = malloc(256);
  char * current;
  int i = 0;
  //while you can continue to strsep, continue to strsep.
  while((current = strsep(&line, separator))){
    trim(current);
    parsed_args[i] = current;
    i++;
  }
  //when you have all the pieces, return the array of pieces.
  return parsed_args;
}

int run(char ** programs){
  //  Forks the following programs and then execvp within the shell as the parent waits
  int f;
  int status;
  int child;
  f = fork();
  signal(SIGINT,sighandler);
  if(f){
    waitpid(f, &status, 0);
    return 1;
  }else{
    signal(SIGINT,sighandler);
    if(execvp(programs[0], programs) < 0){
      printf("Type exit to exit shell. Or type a command u bozo\n");
    }
    return 0;
  }
}

int myPipe(char * args){
  char ** parsed = parse_args(args, "|");
  char ** read = parse_args(parsed[0], " ");
  char ** write = parse_args(parsed[1], " ");

  int fds[2];

  pipe(fds);
  char line[100];

  int f = fork();
  if(f){
    close(fds[1]);
    int backup = dup(STDIN_FILENO);
    dup2(fds[0], STDIN_FILENO);
    run(write);
    dup2(backup, STDIN_FILENO);
    return 1;
  }
  else{
    close(fds[0]);
    int backup = dup(STDOUT_FILENO);
    dup2(fds[1], STDOUT_FILENO);
    run(read);
    dup2(backup, STDOUT_FILENO);
    return 0;
  }
}

int redirect(char * redirection){
  // redirects the user input for both direction.
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
  }
  if(strchr(parsed[1],'|')){
    myPipe(parsed[1]);
  }
  else{
    programs = parse_args(parsed[0], " ");
    if(run(programs)==0){
      return 0;
    }
  }
  if(redirectin){
    dup2(backup, STDOUT_FILENO);
  }
  else{
    dup2(backup, STDIN_FILENO);
  }
  return 1;
}

int main(){
  signal(SIGINT,sighandler);
  char * currentdirectory = malloc(256);
  char * s = malloc(256);
  char * current = malloc(256);
  char ** args;
  char ** programs;
  int i;
  int * status;
  printf("\n\nThis is the Alvin and Bernard shell!\n");
  printf("Requirements:\n");
  printf("\t-Separate commands with a single space in between words.\n");
<<<<<<< HEAD
  printf("\t-Limit piping to a single pipe.\n");
=======
  printf("\t-Avoid double redirection.\n");
>>>>>>> 7d77060a520674601aed27eb44ce326e71fa59d8
  printf("Thanks!!\n\n\n");
  while(1){
    printf("%s",getcwd(currentdirectory, 256));
    printf("$ ");
    fgets(s, 256, stdin);
    s[strlen(s)-1]=0;
    args = parse_args(s, ";");
    i = 0;
    while(args[i]){
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
          if (redirect(current) == 0){
            return 0;
          }
        }
        else if(strchr(current,'|')){
          if(myPipe(current) == 0){
            return 0;
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
