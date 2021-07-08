# README - watch
A watcher tool that monitors a process and prompts you when it terminates. 

***
## Introduction

'watch' is a monitoring tool that waits until the program you specify terminates and pops up a message box to grab your attention. It's purpose is to overcome programs silently terminating without you realizing it. 




***
## How to use
	$ watch --exec program.exe -l --switch

Or use a full path:

	$ watch --exec C:\myfolder\program.exe --flags


__Note:__ Everything following the `--exec` switch is passed to target process as it's command-line arguments.

To keep the target process running continuously - i.e. automatically restart after it terminates - use the 'forever' switch, as follows:

	$ watch --forever --exec C:\myfolder\program.exe --flags

If you really want to keep the process running indefinitely, then you probably don't want to see the messagebox that pops up when it terminates. In that case, use the `--silent` argument. 

	$ watch --forever --silent --exec C:\myfolder\program.exe --flags


To see all options, type:

	$ watch --help


## What it does

It will kick-off the target executable you specify (as a child process), reporting the Process ID (PID) of that process.

e.g.

	INFO: Process 'program.exe' started as PID=12345.




When the target executable terminates, you'll get a message popup to inform you, reporting the exit code. You'll also see a log message at the console:

e.g.

	INFO: Process 'program.exe' (PID=12345) completed .


If you've specified the `--forever` it will simply launch the command again. 

__Note:__ If you wish to terminate the target process, you must first terminate 'watch' to stop it automatically restarting your target process.

***
## Supported Platforms


What platforms are supported?

* Windows 10 (Visual C++ 2019)


The tool utilizes Win32 calls and is only for Windows platforms. 

***
## Build 

The project was built using Visual Studio 2019. 


***
## Dependencies

'watch' uses [espresso](https://github.com/donnachaforde/espresso) lib for command-line argument parsing. 

***
Donnacha Forde

July, 2021

