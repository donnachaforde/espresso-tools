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
#include <iostream>
using namespace std; 

#ifdef WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif



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
			  "1.0 (Beta)", 
			  "Donnacha Forde", 
			  "2005-2006", 
			  "@DonnachaForde");

	// pick up default args/switches
	args.Initialize();
	
	args.Add("delay",	  Arg::INTEGER, "Number of seconds to delay in between allocation increments - defaults to 1.", true);
	args.Add("kilobytes", Arg::NOARG,	"Make increments in kilobytes - default is megabytes.");
	args.Add("verbose",   Arg::NOARG,	"Provide detailed output, with delay in between increaments.");

	args.AddAlias("delay", 'd');
	args.AddAlias("kilobytes", 'k');
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
	else if (args.IsPresent("info"))
	{
		cout																	<< endl
			 << args.GetProgramName() << " " << args.GetVersion()				<< endl 
																				<< endl
			 << "Compiled: " << __TIME__ << " " << __DATE__						<< endl

#ifdef _DEBUG
			 << "Mode: Debug"													<< endl
#else
			 << "Mode: Release"													<< endl
#endif
			 << "Built with the espresso library."								<< endl
			 << "Built with the symapi++ library."								<< endl
																				<< endl
			 << args.GetCopyrightNotice()										<< endl
			 << args.GetBugReportingInstructions()								<< endl;

		::exit(0);
	}


	//
	// examine the switches specified
	// 

	bool IsVerbose = args.IsPresent("verbose");



	//
	// main processing loop
	//

	// default allocation size is 1Mb
	int nSizeofAllocUnit = (1024 * 1024);
	string strUnit = "Mb";
	
	// check if we're being asked to do it in Kb
	if (args.IsPresent("kilobytes"))
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

				if (args.IsPresent("delay"))
				{
					if (args.IsValueSupplied("delay"))
					{
						waitabit(args.GetNumericValue("delay"));
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
