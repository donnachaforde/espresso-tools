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

To see all options, type:

	$ watch --help

## What it does

It will kick-off the target executable you specify (as a child process), reporting the Process ID (PID) of that process.

e.g.

	INFO: Process 'program.exe' started as PID=12345.




When the target executable terminates, you'll get a message popup to inform you, reporting the exit code. You'll also see a log message at the console:

e.g.

	INFO: Process 'program.exe' (PID=12345) completed .



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

