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
			  "tabs2spaces", 
			  "Expands tabs to spaces in text files.", 
			  "1.0 (Beta)", 
			  "Donnacha Forde", 
			  "2006-2007", 
			  "@DonnachaForde");

	// pick up default args/switches
	args.Initialize();

	// specify our switches & aliases
	args.Add("spaces",	Arg::INTEGER,	"Number of spaces to replace <TAB> with.", true);
	args.Add("file",	Arg::STRING,	"Filename to modify.", true, "filename");
	
	args.AddAlias("spaces", 's');
	args.AddAlias("file", 'f');

	// parse the arg list
	if (!args.Parse())
	{
		cout << "ERROR: Invalid option: '" << args.GetInvalidOption() << "'. Use --help for option information." << endl;
		::exit(-1);
	}


	// check for requests for help, usage and version
	if (args.IsPresent("help") || args.IsPresent('h') || args.IsPresent('?'))
	{
		cout															<< endl
			 << args.GetProgramDescription()							<< endl
																		<< endl 
			 << args.GetUsage()											<< endl
																		<< endl 
			 << args.GetOptionsDescriptions()							<< endl
																		<< endl
			 << args.GetCopyrightNotice()								<< endl
			 << args.GetBugReportingInstructions()						<< endl;

		::exit(0);
	}
	else if (args.IsPresent("usage"))
	{
		cout << args.GetUsage()											<< endl
																		<< endl 
			 << args.GetCopyrightNotice()								<< endl;
		::exit(0);
	}
	else if (args.IsPresent("version"))
	{
		cout << args.GetProgramName() << " " << args.GetVersion()		<< endl 
			 << args.GetCopyrightNotice()								<< endl;
		::exit(0);
	}


	//
	// now use any args specified
	//

	int nNumSpacesToWrite = 4;
	if (args.IsPresent("spaces"))
	{
		nNumSpacesToWrite = args.GetNumericValue("spaces");
	}

	
	FILE* fdIn = NULL;

	if (args.IsPresent("file") || args.IsTargetPresent())
	{
		//
		// open the input file
		//
		
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

	return 0;
}


