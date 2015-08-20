# README - espresso tools 
Some simple CLI tools, built using standard C/C++ libraries and the espresso library.


Tools

* bin2txt     - writes binary file to stdout as text
* dec2hex     - writes a decimal integer to stdout in its raw hex equivalent format
* env         - same as Unix env
* memcheck    - checks how much heap memory can be allocated by a process (by actually trying to allocate it, quickly or slowly if required). 
* secs2time   - write a timestamp in seconds to stdout as 'human-readable' date and time (in the format HH:MM:SS DD/MM/YYYY)
* spaces2tabs - writes file text file to stdout, converting spaces to tabs
* tabs2spaces - writes file text file to stdout, converting tabs to spaces

The software was initially written to be used on Windows 32-bit but it has been successfully built and used on the platforms above.

Donnacha Forde
2008

