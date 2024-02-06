# README - espresso tools 
Simple CLI tools that help users of the terminal. 

***
## Introduction

Espresso tools consists of a small number of useful CLI tools that are (mostly) portable across Windows, UNIX, Linux and macOS. 

* They have all been built using the standard C/C++ libraries and the [espresso library](https://github.com/donnachaforde/espresso) for handling command-line args. 
* The tools indicated as **Windows only** also leverage the Windows API. 
 
***
## Contents

Here's what's included in the repo:

| Utility        | Desciption           | Platforms  |
| ------------- |-------------| -----|
| bin2txt	    | writes binary file to stdout as text ||
| dec2hex		| writes a decimal integer to stdout in its raw hex equivalent format||
| cores			| Reports the number of logical cores on the host  | Windows only|
| env         	| same as Unix env ||
| memcheck    	| checks how much heap memory can be allocated by a process (by actually trying to allocate it, quickly or slowly if required). ||
| secs2time   	| write a timestamp in seconds to stdout as 'human-readable' date and time (in the format HH:MM:SS DD/MM/YYYY)||
| spaces2tabs 	| writes file text file to stdout, converting spaces to tabs	||
| tabs2spaces 	| writes file text file to stdout, converting tabs to spaces	||
| watch			| kicks off and monitors the specified processes, reporting when terminated	| Windows only|
| whashere	    | traverses the specified directory and summarizes the contents (See --usage for options)	| Windows only|




***
## Supported Platforms


What platforms are supported?

* Windows 10 x64 (Visual C++ 2019)

What platforms are under development?

* macOS (clang)
* Ubuntu Linux (gcc)


> Note: At one point, I built most of these tools for various UNIX/Linux platforms using the primitive makefiles.  

* Solaris SunOS
* AIX
* HP-UX
* Red Hat Linux


## Build 

Build from the source code using Visual Studio 2019 or download a copy of the Release binaries from the releases section. 



***
## Dependencies

The espresso-tools are dependent on:

* [espresso-lib](https://github.com/donnachaforde/espresso) - Clone the repo and build the library. If you clone this project at the same root directory, everything should compile _out of the box_. 

* [Windows API](https://learn.microsoft.com/en-us/windows/win32/api/) - This is likely installed along with your Visual Studio instance. 

## Releases
See the [Releases](https://github.com/donnachaforde/espresso-tools/releases) page. 

## History
The original visual studio file were created Visual Studio 6.0 (which gives an indication as to the point when this 'tinkering' began) but were later ported to Visual Studio 2003, then Visual Studio 2005 and later Visual Studio 2008. There followed a fallow period before I again became re-acquainted with DevStudio using Visual Studio 2014 and then Visual Studio 2017. Most recently, I've ported to Visual Studio 2019. 


The tool were initially build for Windows 32-bit but was successfully ported and deployed to UNIX/Linux environments. Later, the code was successfully built for Windows x64 without issue. While the `makefile` is primitive, it should still work for UNIX/Linux environments. Most recently, it has been revised to build on macOS environments. I will eventually get round to supporting CMake. 


***
Donnacha Forde

February 2024

