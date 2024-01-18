////////////////////////////////////////////////////////////////////////////////
//
// Main.cxx
//
////////////////////////////////////////////////////////////////////////////////
//
// Developed by Donnacha Forde (@DonnachaForde)
//
// Copyright © 2006-2024, Donnacha Forde. All rights reserved.
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
using namespace std;


#include <errno.h>

#ifdef WIN32
	#include <io.h>
#else
	#include <unistd.h>
#endif





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
			  "spaces2tabs", 
			  "Replaces spaces with tabs in text files.", 
			  "1.0 (Beta)", 
			  "Donnacha Forde", 
			  "2006-2024", 
			  "@DonnachaForde");

	// pick up default args/switches
	args.addDefaults();

	// specify our switches & aliases
	args.add("spaces",	Arg::INTEGER,	"Number of spaces to replace <TAB> with.", true);
	args.add("file",	Arg::STRING,	"Filename to modify.", true, "filename");
	
	args.addAlias("spaces", 's');
	args.addAlias("file", 'f');

	// create an arg manager to parse the args
	ArgManager argMgr = ArgManagerFactory::createInstance();
	if (argMgr.parseAndProcessArgs(args) == -1)
	{
		cout << "ERROR: Invalid option: '" << args.getInvalidOption() << "'. Use --help for option information." << endl;
		::exit(-1);
	}




	//
	// now use any args specified
	//

	int nNumSpacesToReplace = 4;
	if (args.isPresent("spaces"))
	{
		nNumSpacesToReplace = args.getNumericValue("spaces");
	}

	
	FILE* fdIn = NULL;

	if (args.isPresent("file") || args.isTargetPresent())
	{
		//
		// open the input file
		//
		
		const char* szInputFilename = NULL; 

		if (args.isPresent("file"))
		{
			szInputFilename = args.getStringValue("file").c_str(); 
		}
		else
		{
			szInputFilename = args.getTarget().c_str(); 
		}

		// check the filename
		if (!espresso::strings::isValidString(szInputFilename))
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
		fdIn = ::fopen(szInputFilename, "rt");
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
	int nNumSpacesRead = 0;
	while ((ch = ::fgetc(fdIn)) != EOF)
	{

		if (char(ch) == ' ')
		{
			nNumSpacesRead++;
		}
		else
		{
			// maybe we've just read a series of spaces, but not enough to replace with a <TAB>
			if (nNumSpacesRead > 0)
			{
				for (int i = 0; i < nNumSpacesRead; i++)
				{
					::fputc(' ', stdout);
				}
				nNumSpacesRead = 0;
			}
		}

		if (nNumSpacesRead == 0)
		{
			::fputc(ch, stdout);
		}
		else if (nNumSpacesRead == nNumSpacesToReplace)
		{
			::fputc(0x09, stdout);		// <TAB>
			nNumSpacesRead = 0;
		}
	}


	//
	// tidy up
	//

	::fclose(fdIn);
	::fclose(stdout);

	return 0;
}


