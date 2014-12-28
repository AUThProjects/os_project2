1. Parsing
    * cd
    * exit
    * PATH programs
    * relative/absolute path programs
    * arguments
    * pipelining
    * redirection
    * (optional) star operator
2. Execution (Pavlos)
    * cd -> chdir
    * PATH programs -> execvp(program, args)
    * relative/absolute path programs -> exevp (program, args)
    * background running
3. Streams (Antonis)
    * redirection -> dup2 system call
    * pipelining -> pipe, dup2 system calls
4. Scheduling (Stefanos)
    * implemented with signals from each process
    * part of the same program
    * different main
    * different process forked by the parser when a process is to be run in the background

