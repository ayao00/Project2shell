# Project2shell

<h2> Features that we implemented </h2>
<p> Forks and executes commands!!</p>
<p> Parses multiple commands on one line!!</p>
<p> Redirects using > and < !! </p>

<h2> Features that we attempted to implement but failed </h2>
<p> We tried to implement the following commented out code. If ran, would blow up shell! </p>
<ul>
<li> Could not get pipes to work properly</li>
<li> cd ~ is not working </li>
<li> We had sighandler but did not really know what it caught except a good grade!!</li>
</ul>

<h2> Bugs that we discovered </h2>
<p> If the mypipe was not commented out, it would run and print result after you press exit. weird...</p>
<p> putting too many spaces between arguments will cause the program to read it in wrong. </p>

<h2> Function headers </h2>
main.c
  static void sighandler ()
  input: int signo
  Returns: nothing
  
  Prints out message if signal is caught anywhere in the program.
  
  void trim ()
  input: char * input
  Returns: nothing
  
  Removes leading and trailing whitespace on the string line.
	Terminating '\0' is placed at a new location if necessary.
  
  char ** parse_args ()
  input: char * line, char * separator
  Returns: list of strings containing arguments
  
  Uses strsep to go through a string and separating at the separator.
  
  int run ()
  input: char ** programs
  Returns: 1 if it is parent and 0 if it is child finishing forking and execing.
  
  forks the programs and execvps. Makes the parent wait until after the child process finishes.
  
  int myPipe ()
  input: char * args
  Returns: 
  
  int redirect ()
  input: char * redirection
  Returns: 1 if it is parent and 0 if it is child finishing the redirect.
  
  Redirects the part of the line that contains < or >
  
  int main()
  input: 
  Returns: 0 if successfully finishes
  
  Driver function of entire program. Reads in the command line and prompts the user to entire future commands. Uses conditional statements to direct the input to where it has to be.
