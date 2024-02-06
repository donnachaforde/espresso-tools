////////////////////////////////////////////////////////////////////////////////
//
// Main.cxx
//
////////////////////////////////////////////////////////////////////////////////
//
// Developed by Donnacha Forde (@DonnachaForde)
//
// Copyright © 2005-2024, Donnacha Forde. All rights reserved.
//
// This software is provided 'as is' without warranty, expressed or implied.
// Donnacha Forde accepts no responsibility for the use or reliability of this software.
//
////////////////////////////////////////////////////////////////////////////////

// disable newer warnings
#ifdef WIN32
	#pragma message("note : CRT security warning (so we can use ol' fashioned 'C' calls)")
	#define _CRT_SECURE_NO_WARNINGS

	#pragma message("note : Suppress warnings about deprecated POSIX function names.")
	#pragma warning(disable : 4996)
#endif


// espresso lib
#include <espresso.hxx>
using namespace espresso;

// crt
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <cctype>
using namespace std;

#include <errno.h>

#ifdef WIN32
	#include <io.h>
#else
	#include <unistd.h>
#endif


// version number
static const char VERSION[] = "0.11.0-beta";


// prototypes
void execute(const Args& args);


/******************************************************************************\
 ******************************************************************************
 ******************************************************************************
 *********************************    MAIN    *********************************
 ******************************************************************************
 ******************************************************************************
\******************************************************************************/

int main(int argc, char* argv[], char* envp[])
{
	//
	// arg parsing
	// 

	Args args(argc, 
			  argv, 
			  "bin2txt", 
			  "Displays binary files in text format.", 
			  VERSION, 
			  "Donnacha Forde", 
			  "2005-2024", 
			  "@DonnachaForde");

	// pick up default args/switches
	args.addDefaults();

	// specify our switches & aliases
	args.add("columns",	Arg::INTEGER, false, "Number of hexadecimal columns to display on each line.", true);
	args.add("file",	Arg::STRING,  true,	 "Filename to modify.", true, "filename");
	args.add("text",	Arg::NOARG,	  false, "Display text only.");
	args.add("hex",		Arg::NOARG,	  false, "Display hexadecimal only.");

	args.addAlias("columns", 'c');
	args.addAlias("file", 'f');
	args.addAlias("text", 't');
	args.addAlias("hex", 'x');


	// create argmgr to handle default switches 	
	ArgManager argMgr = ArgManagerFactory::createInstance();
	int nRetVal = argMgr.parseAndProcessArgs(args);
	if (nRetVal != 0)
	{
		::exit(0);
	}


	// determine if we should proceed
	if (args.isTargetPresent() || args.isRequiredArgsPresent())
	{
		execute(args);
	}


	return 0;
}






//------------------------------------------------------------------------------
//
// Function       : execute and process the file 
//
// Return type    : void 
//
// Argument       : const Args& args
//
// Description    : 
//
// Notes          : 
//
//------------------------------------------------------------------------------
void execute(const Args& args)
{
	//
	// now use any args specified
	//

	bool isDisplayHexOnly = args.isPresent("hex");
	bool isDisplayTextOnly = args.isPresent("text"); 


	// default to 16 wide
	int nNumColumnsToDisplay = 16;
	if (args.isPresent("columns"))
	{
		nNumColumnsToDisplay = args.getNumericValue("columns");
	}


	// handle the file/target
	FILE* fdIn = NULL;
	if (args.isPresent("file") || args.isTargetPresent())
	{
		string strInputFilename;
		if (args.isPresent("file"))
		{
			strInputFilename = args.getStringValue("file");
		}
		else
		{
			strInputFilename = args.getTarget();
		}

		// check the filename
		if (!espresso::strings::isValidString(strInputFilename))
		{
			cout << "ERROR: Unable to process empty filename." << endl;
			::exit(-1);
		}


		// can we open the file for reading (and hence, does it even exist)...
		int nRetCode = ::access(strInputFilename.c_str(), 04);
		if (nRetCode == EACCES)
		{
			cout << "ERROR: Access denied. '" << strInputFilename << "' file permission doesn't allow access." << endl;
			::exit(-1);
		}
		else if (nRetCode == ENOENT)
		{
			cout << "ERROR: File '" << strInputFilename << "' does not exist." << endl;
			::exit(-1);
		}
		else if (nRetCode == EINVAL)
		{
			cout << "ERROR: An invalid parameter for checking access to file '" << strInputFilename << "' was provided." << endl;
			::exit(-1);
		}

		// open the file
		fdIn = ::fopen(strInputFilename.c_str(), "rt");
		assert(fdIn != NULL);

		if (fdIn == NULL)
		{
			cout << "ERROR: Failed to open file: '" << strInputFilename << "' for reading." << endl;
			::exit(-1);
		}
	}
	else
	{
		// use stdin
		fdIn = stdin;
	}




	//
	// main processing loop
	//


	//::setvbuf(stdin, NULL, _IOLBF, 0);
	//::setvbuf(stdout, NULL, _IOLBF, 0);

	int ch = 0;
	int nNumColumnsShown = 0;


	if (isDisplayTextOnly)
	{
		while ((ch = ::fgetc(fdIn)) != EOF)
		{
			// if we're showing text, we must handle CR/LF (for this we must read ahead by one char, hence the two similar 'else' clauses
			if ((char)ch == '\\')
			{
				// read next char
				ch = ::fgetc(fdIn);
				if ((char)ch == 'n')
				{
					::fprintf(stdout, "\n");
				}
				else
				{
					// if not a printable char, substitute with a dot
					if (isprint(ch))
					{
						::fprintf(stdout, "\\%c", (char)ch);
					}
					else
					{
						::fprintf(stdout, "\\.");
					}
				}
			}
			else
			{
				if (isprint(ch))
				{
					::fprintf(stdout, "%c", (char)ch);
				}
				else
				{
					::fprintf(stdout, ".");
				}

			}
		}
	}
	else
	{
		// create buffer for string representation
		char* szBuffer = new char[nNumColumnsToDisplay + 1];
		::memset(szBuffer, 0, (nNumColumnsToDisplay + 1));
		char* pch = szBuffer;

		while ((ch = ::fgetc(fdIn)) != EOF)
		{
			// build up string representation
			if (!isDisplayHexOnly)
			{
				// if not a printable char, substitute with a dot
				if (!isprint(ch))
				{
					ch = '.';
				}

				*pch++ = (char)ch;
			}

			// display the hex representation
			::fprintf(stdout, "%02x ", ch);
			
			// handle column display logic
			nNumColumnsShown++;
			if (nNumColumnsShown == nNumColumnsToDisplay)
			{
				assert(::strlen(szBuffer) <=  (size_t)nNumColumnsToDisplay);
				::fprintf(stdout, "    %s\n", szBuffer);
				nNumColumnsShown = 0;
				::memset(szBuffer, 0, (nNumColumnsToDisplay + 1));
				pch = szBuffer;
			}
		}

		// write blank columns at end where necessary
		for (int i = nNumColumnsToDisplay; i > nNumColumnsShown; i--)
		{
			::fprintf(stdout, "   ");
		}
		
		// deal with stragling buffer
		szBuffer[nNumColumnsShown] = '\0';
		::fprintf(stdout, "    %s\n", szBuffer);


		//
		// tidy up
		//

		delete [] szBuffer;

	}

	::fclose(fdIn);
	::fclose(stdout);

	return;
}


