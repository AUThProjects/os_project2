# OS Class
## Project 2

## Description

This project constitutes the 2nd assignment of the OS class of the CS department of Aristotle University of Thessaloniki (winter semester 2014-2015).

Goal of this project was to create a terminal in the GNU/Linux Operating System supporting the following functionality:

1. Parsing
    * cd
    * exit
    * PATH programs
    * relative/absolute path programs
    * arguments
    * pipelining
    * redirection
    * (optional) star operator
2. Execution
    * cd -> chdir
    * PATH programs -> execvp(program, args)
    * relative/absolute path programs -> exevp (program, args)
    * background running
3. Streams 
    * redirection -> dup2 system call
    * pipelining -> pipe, dup2 system calls
4. Scheduling
    * implemented with signals from each process
    * part of the same program
    * different main
    * different process forked by the parser when a process is to be run in the background


## What we accomplished

We accomplished all the functionality described above, except for the star operator, which is meant to be fulfilled.


## To be fixed

As the time to complete the project was truly limited, there are still some rough edges that needs attention.


* Fix inconsistency on redirection + pipelining combination
* Support for multi level pipelining
* Change the name of the forked processes [click here](http://www.uofr.net/~greg/processname.html)

## Team

* Stefanos Laskaridis (@stevelaskaridis)
* Antonios Anagnostou (@anagnoad)
* Pavlos Gogousis (@gogopavl)
