# README - espresso tools 
Some simple CLI tools, built using standard C/C++ libraries and the espresso library.

***
## Introduction

Espresso Tools consists of a handful of useful CLI tools that are portable across Windows, Unix, Linux and macOS. They all utilize the [Espresso Lib](https://github.com/donnachaforde/espresso) to handle the management of command line arguments and provide a consist help, info and versioning output.

### Tools

* bin2txt     - writes binary file to stdout as text
* dec2hex     - writes a decimal integer to stdout in its raw hex equivalent format
* env         - same as Unix env
* memcheck    - checks how much heap memory can be allocated by a process (by actually trying to allocate it, quickly or slowly if required). 
* secs2time   - write a timestamp in seconds to stdout as 'human-readable' date and time (in the format HH:MM:SS DD/MM/YYYY)
* spaces2tabs - writes file text file to stdout, converting spaces to tabs
* tabs2spaces - writes file text file to stdout, converting tabs to spaces

The software was initially written to be used on Windows 32-bit but it has been successfully built and used on the platforms above.



***
## Contents

Whats included?

* Espresso Tools - Useful CLI tools. 
* VS2019 Project Files and solution files - For development on Windows


***
## Supported Platforms


What platforms are supported?

* Windows 10 (Visual C++ 2019)
* macOS (gcc)


Previously, the following platforms were supported for *gcc*. They haven't been exercised in recent years but there's every chance the makefile will still work. 

* Solaris SunOS
* AIX
* HP-UX
* Red Hat Linux

The software was originally developed on Windows 32-bit environments but was successfully ported and deployed to Unix/Linux environments. Later, the code was successfully built for x64 environments without issue. While the `makefile` is primitive, it should still work for Unix/Linux environments. Most recently, it has been revised to build on macOS environments. 


***
## Build Environments

The original visual studio file were created Visual Studio 6.0 (which gives an indication as to the point when this 'tinkering' began) but were later ported to Visual Studio 2003, then Visual Studio 2005 and later Visual Studio 2008. There followed a fallow period before I again became re-acquainted with DevStudio using Visual Studio 2014 and then Visual Studio 2017. Most recently, I've ported to Visual Studio 2019. 

The makefile is pretty trivial but works. Plans are afoot to adopt CMake. 

***
Donnacha Forde

2020

