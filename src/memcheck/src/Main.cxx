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

	args.add("exec",	  Arg::NOARG,   true,  "Execute the memory check. (Warning: This will actually allocate real memory on your system.)");
	args.add("delay",	  Arg::INTEGER, false, "Number of seconds to delay inbetween allocation increments - defaults to 1 sec.");
	args.add("kilobytes", Arg::NOARG,	false, "Make increments in kilobytes - default is megabytes.");
	args.add("verbose",   Arg::NOARG,	false, "Provide detailed output, with delay in between increaments.");

	args.addAlias("delay", 'd');
	args.addAlias("kilobytes", 'k');

	// create an arg manager and parse the arg list
	ArgManager argMgr = ArgManagerFactory::createInstance();
	int nRetCode = argMgr.parseAndProcessArgs(args);

	if (nRetCode == -1)
	{
		cout << "ERROR: Invalid option: '" << args.getInvalidOption() << "'. Use --help for option information." << endl;
		::exit(-1);
	}
	else if (nRetCode == 1)
	{
		// a provided arg has been automatically handled
		::exit(0);
	}

	

	//
	// main processing loop
	//

	// check args passed
	bool isExecute = args.isPresent("exec");
	bool isVerboseMode = args.isPresent("verbose");

	if (isExecute)
	{


		// default allocation size is 1Mb
		int nSizeofAllocUnit = (1024 * 1024);
		string strUnit = "MB";
	
		// check if we're being asked to do it in Kb
		if (args.isPresent("kilobytes"))
		{
			nSizeofAllocUnit = (1 * 1024);
			strUnit = "KB";

		}

		int iCount = 0;
		for ( ; ; )
		{
			void* pv = ::malloc(nSizeofAllocUnit); 

			if (pv != NULL)
			{	
				iCount++;

				if (isVerboseMode)
				{
					cout << "INFO: Allocated " << iCount << strUnit << " (of dynamic memory)." << endl;

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
				if (isVerboseMode)
				{
					cout << "INFO: Stopped allocating memory at " << iCount << strUnit << "." << endl;
				}
				else
				{
					cout << iCount << strUnit << endl;

				}
				break; 
			}
		}
	}
	else
	{
		cout << "INFO: You need to explicitly use the --exec argument to invoke memcheck." << endl;
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
