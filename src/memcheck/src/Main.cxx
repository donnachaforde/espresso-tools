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


// espresso lib
#include <espresso.hxx>
using namespace espresso;


// crt
#include <iostream>
using namespace std; 

#ifdef WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif


// version number
static const char VERSION[] = "0.1.0-beta";



void waitabit(unsigned int ui = 1);



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
			  "memcheck", 
			  "Checks how much heap memory a process can allocate.", 
			  VERSION, 
			  "Donnacha Forde", 
			  "2005-2024", 
			  "@DonnachaForde");

	// pick up default args/switches
	args.addDefaults();
	
	args.add("delay",	  Arg::INTEGER, false, "Number of seconds to delay in between allocation increments - defaults to 1.");
	args.add("kilobytes", Arg::NOARG,	false, "Make increments in kilobytes - default is megabytes.");
	args.add("verbose",   Arg::NOARG,	false, "Provide detailed output, with delay in between increaments.");

	args.addAlias("delay", 'd');
	args.addAlias("kilobytes", 'k');
	args.addAlias("spaces", 's');
	args.addAlias("file", 'f');

	// create an arg manager and parse the arg list
	ArgManager argMgr = ArgManagerFactory::createInstance();
	if (argMgr.parseAndProcessArgs(args) == -1)
	{
		cout << "ERROR: Invalid option: '" << args.getInvalidOption() << "'. Use --help for option information." << endl;
		::exit(-1);
	}

	
	if (args.isPresent("info"))
	{
		cout																	<< endl
			 << args.getProgramName() << " " << args.getVersion()				<< endl 
																				<< endl
			 << "Compiled: " << __TIME__ << " " << __DATE__						<< endl

#ifdef _DEBUG
			 << "Mode: Debug"													<< endl
#else
			 << "Mode: Release"													<< endl
#endif
			 << "Built with the espresso library."								<< endl
																				<< endl
			 << args.getCopyrightNotice()										<< endl
			 << args.getBugReportingInstructions()								<< endl;

		::exit(0);
	}


	//
	// examine the switches specified
	// 

	bool IsVerbose = args.isPresent("verbose");



	//
	// main processing loop
	//

	// default allocation size is 1Mb
	int nSizeofAllocUnit = (1024 * 1024);
	string strUnit = "Mb";
	
	// check if we're being asked to do it in Kb
	if (args.isPresent("kilobytes"))
	{
		nSizeofAllocUnit = (1 * 1024);
		strUnit = "Kb";

	}

	int iCount = 0;
	for ( ; ; )
	{
		void* pv = ::malloc(nSizeofAllocUnit); 

		if (pv != NULL)
		{	
			iCount++;

			if (IsVerbose)
			{
				cout << "Allocated " << iCount << strUnit << " (of dynamic memory)." << endl;

				if (args.isPresent("delay"))
				{
					if (args.isValueSupplied("delay"))
					{
						waitabit(args.getNumericValue("delay"));
					}
					else
					{
						waitabit();
					}
				}
			}
		}
		else
		{
			// we've reached our limit
			if (IsVerbose)
			{
				cout << "Stopped allocating memory at " << iCount << strUnit << "." << endl;
			}
			else
			{
				cout << iCount << strUnit << endl;

			}
			break; 
		}
	}

	return 0; 
}



void waitabit(unsigned int ui /* = 1 */)
{

#ifdef WIN32

	Sleep(ui * 1000);

#else
	
	sleep(ui);

#endif

}
