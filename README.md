# Command Shell
Command line shell coded in C using Cygwin. The shell accepts 7 commands in total and executes in an endless loop until the user enters the 'quit' command.

#   Requirements 
	* Cygwin x64
	* gcc compiler

#   Compilation

In a cygwin terminal window, nagivate to the ./cygdrive/c directory, and from there navigate to the location of the project folder. Alternatively, drap and drop the .exe file into the cygwin terminal window and press enter for the program to start.

# Execution
A main menu will be visible after every command log. An example of the menu and available commands are shown below.

  Shell commands:
  1. Print the result of the prefix expression 'exp':      < calc 'exp' >
  2. Print out current local time and date:                < time >
  3. Print current working directory:                      < path >
  4. Print name and version of the OS and CPU type:        < sys >
  5. Put files 'filenames' in the directory 'dirname':     < put 'dirname' 'filename(s)' [-f] >
  6. Print the content of the file 'filename':             < get 'filename' >
  7. End the program:                                      < quit >
  ---

Command 5. can optionally include the [-f] flag. This flag overwrites an existing folder with the same name as 'dirname'. If the user does not enter the flag and the folder already exists, an error message will be shown.
