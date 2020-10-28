////////////////////////////////////////////////////////////////////////////////
//
// Main.cxx
//
////////////////////////////////////////////////////////////////////////////////
//
// Developed by Donnacha Forde (@DonnachaForde)
//
// Copyright © 2006-2008, Donnacha Forde. All rights reserved.
//
// This software is provided 'as is' without warranty, expressed or implied.
// Donnacha Forde accepts no responsibility for the use or reliability of this software.
//
////////////////////////////////////////////////////////////////////////////////

// disable newer warnings
#ifdef WIN32
	#pragma message("note : CRT security warning (so we can use ol'fashion 'C' calls)")
	#define _CRT_SECURE_NO_WARNINGS
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


// prototypes
void ProcessFile(const Args& args);


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
			  "1.0 (Beta)", 
			  "Donnacha Forde", 
			  "2005-2007", 
			  "@DonnachaForde");

	// pick up default args/switches
	args.Initialize();

	// specify our switches & aliases
	args.Add("columns",	Arg::INTEGER,	"Number of hexadecimal columns to display on each line.", true);
	args.Add("file",	Arg::STRING,	"Filename to modify.", true, "filename");
	args.Add("text",	Arg::NOARG,		"Display text only.");
	args.Add("hex",		Arg::NOARG,		"Display hexadecimal only.");

	args.AddAlias("columns", 'c');
	args.AddAlias("file", 'f');
	args.AddAlias("text", 't');
	args.AddAlias("hex", 'x');


	// create argmgr to handle default switches
	ArgMgrCLI argMgr;
	if (!argMgr.ParseAndProcessArgs(args))
	{
		::exit(0);
	}


	// check display preferences
	if (args.IsPresent("hex") && args.IsPresent("text"))
	{
		cout << "ERROR: Cannot display only hex and only text at once." << endl;
		argMgr.OnArgError(args);
		::exit(0);
	}


	if (args.IsPresent("text") && args.IsPresent("columns"))
	{
		cout << "ERROR: Specifying number of columns in not applicable when displaying text only." << endl;
		argMgr.OnArgError(args);
		::exit(0);
	}

	ProcessFile(args);

	return 0;
}






//------------------------------------------------------------------------------
//
// Function       : ProcessFile
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
void ProcessFile(const Args& args)
{
	//
	// now use any args specified
	//


	bool IsDisplayHexOnly = args.IsPresent("hex");
	bool IsDisplayTextOnly = args.IsPresent("text"); 




	// default to 16 wide
	int nNumColumnsToDisplay = 16;
	if (args.IsPresent("columns"))
	{
		nNumColumnsToDisplay = args.GetNumericValue("columns");
	}


	// handle the file/target
	FILE* fdIn = NULL;
	if (args.IsPresent("file") || args.IsTargetPresent())
	{
		const char* szInputFilename = NULL; 

		if (args.IsPresent("file"))
		{
			szInputFilename = args.GetStringValue("file").c_str(); 
		}
		else
		{
			szInputFilename = args.GetTarget().c_str(); 
		}

		// check the filename
		if (!espresso::strings::IsValidString(szInputFilename))
		{
			cout << "ERROR: Unable to process empty filename." << endl;
			::exit(-1);
		}


		// can we open the file for reading (and hence, does it even exist)...
		int nRetCode = ::access(szInputFilename, 04);
		if ((nRetCode == EACCES) || (nRetCode == ENOENT))
		{
			cout << "ERROR: Either the file '" << szInputFilename << "' does not exist or is not available for reading purposes." << endl;
			::exit(-1);
		}

		// open the file
		fdIn = ::fopen(szInputFilename, "rb");
		assert(fdIn != NULL);

		if (fdIn == NULL)
		{
			cout << "ERROR: Failed to open file: '" << szInputFilename << "' for reading." << endl;
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


	::setvbuf(stdin, NULL, _IOLBF, 0);
	::setvbuf(stdout, NULL, _IOLBF, 0);

	int ch = 0;
	int nNumColumnsShown = 0;


	if (IsDisplayTextOnly)
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
			if (!IsDisplayHexOnly)
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
				assert(::strlen(szBuffer) <=  nNumColumnsToDisplay);
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


