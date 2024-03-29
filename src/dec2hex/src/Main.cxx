////////////////////////////////////////////////////////////////////////////////
//
// Main.cxx
//
////////////////////////////////////////////////////////////////////////////////
//
// Developed by Donnacha Forde (@DonnachaForde)
//
// Copyright � 20059-2024, Donnacha Forde. All rights reserved.
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
#include <iostream>
#include <ctime>
#include <cassert>
using namespace std;

// version number
static const char VERSION[] = "0.11.0-beta";





/******************************************************************************\
 ******************************************************************************
 ******************************************************************************
 *********************************    MAIN    *********************************
 ******************************************************************************
 ******************************************************************************
\******************************************************************************/

int main(int argc, char* argv[])
{
	//
	// arg parsing
	// 

	Args args(argc, 
			  argv, 
			  "dec2hex", 
			  "Converts a decimal integer into its raw hex equivalent.", 
			  VERSION, 
			  "Donnacha Forde", 
			  "2009-2024", 
			  "@DonnachaForde");

	// pick up default args/switches
	args.addDefaults();
	args.requireTarget();


	// create argmgr to handle default switches
	ArgManager argMgr = ArgManagerFactory::createInstance();
	int nRetVal = argMgr.parseAndProcessArgs(args);
	if (nRetVal != 0)
	{
		::exit(0);
	}



	//
	// main processing
	//

	if (args.isTargetPresent())
	{
		//
		// check range - devnote: time_t is defined as a long, not an unsigned long so use one to convert & perform the test
		// 

		unsigned long ul = ::strtoul(args.getTarget().c_str(), 0, 10);

		 // time_t is a long, but time is measure in secs since epoch
		if ((ul > 2147483647) || (ul < 0))
		{
			cout << "ERROR: Value is out of range (0..2147483647 - i.e. 2,147,483,647 or 2^31)" << endl;
			return -2;
		}


		//
		// Otherwise, carry on - it's safe to do the conversion
		//

		int nDec = ::atoi(args.getTarget().c_str());


		char szBuffer[19 + 1] = "";
		::sprintf(szBuffer, "%x", nDec);

		cout << szBuffer << endl;
	}

	return 0;
}



