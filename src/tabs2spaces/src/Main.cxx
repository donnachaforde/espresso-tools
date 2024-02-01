////////////////////////////////////////////////////////////////////////////////
//
// Main.cxx
//
////////////////////////////////////////////////////////////////////////////////
//
// Developed by Donnacha Forde (@DonnachaForde)
//
// Copyright © 2006, Donnacha Forde. All rights reserved.
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
	//#pragma warning(suppress : 4996)
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

// version number
static const char VERSION[] = "0.1.0-beta";

void execute(Args& args);



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
		"tabs2spaces",
		"Expands tabs to spaces in text files.",
		VERSION,
		"Donnacha Forde",
		"2006-2024",
		"@DonnachaForde");

	// pick up default args/switches
	args.addDefaults();
	//args.requireTarget();

	// specify our switches & aliases
	args.add("spaces", Arg::INTEGER, false, "Number of spaces to replace <TAB> with. (Default=4)", true);
	args.add("file", Arg::STRING, false, "Filename to modify.", true, "filename");

	args.addAlias("spaces", 's');
	args.addAlias("file", 'f');

	// create an arg manager to parse the args
	ArgManager argMgr = ArgManagerFactory::createInstance();
	int nRetVal = argMgr.parseAndProcessArgs(args);
	if (nRetVal != 0)
	{
		::exit(0);
	}

	if (args.isTargetPresent() || args.isRequiredArgsPresent())
	{
		execute(args);
	}

	return 0;
}



	//
	// main processing
	//

void execute(Args& args)
{ 
	if (args.isTargetPresent() || args.isRequiredArgsPresent())
	{
		int nNumSpacesToWrite = 4;
		if (args.isPresent("spaces"))
		{
			nNumSpacesToWrite = args.getNumericValue("spaces");
		}


		FILE* fdIn = NULL;

		if (args.isPresent("file") || args.isTargetPresent())
		{
			//
			// open the input file
			//

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
		while ((ch = ::fgetc(fdIn)) != EOF)
		{
			if (ch == 0x09)		// is this a <TAB>
			{
				for (int i = 0; i < nNumSpacesToWrite; i++)
				{
					::fputc(' ', stdout);
				}
			}
			else
			{
				::fputc(ch, stdout);
			}
		}


		//
		// tidy up
		//

		::fclose(fdIn);
		::fclose(stdout);
	}

	return;
}


