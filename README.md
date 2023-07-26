## this is a Naively-made shell 
* warning: don't even think of trying it out!!!
* this shell works both in interactive & non-interactive mode:
    - takes command(s), seperates them by a known delimiter (newline, and semi-colon). execute all of th commands entered.
    - ...etc?
* what does each file contains? :
    - `builtin.c` : this file handles shell's builtin commands (primitively). 
    - `shell`1-3`.c` : needs some work, but they're the core of the shell (you don't say!!!), from splitting a string, and trimmin it, upto forking & executing commands.....
    - `structCommand.c` : contains a struct, in which we'll store a command's data .
    