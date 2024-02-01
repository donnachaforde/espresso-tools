////////////////////////////////////////////////////////////////////////////////
//
// Main.cxx
//
////////////////////////////////////////////////////////////////////////////////
//
// Developed by Donnacha Forde (@DonnachaForde)
//
// Copyright © 2001-2024, Donnacha Forde. All rights reserved.
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
static const char VERSION[] = "0.1.0-beta";


// format time into a timestamp understood by a database (uses a static buffer)
char* GenerateTimestamp(time_t nTimeInSecs);



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
			  "secs2time", 
			  "Converts a timestamp in seconds to human-readable date and time (in the format HH:MM:SS DD/MM/YYYY).", 
			  VERSION, 
			  "Donnacha Forde", 
			  "2001-2024", 
			  "@DonnachaForde");

	// pick up default args/switches
	args.addDefaults();
	args.requireTarget();

	// specify our switches
	args.add("date-only", Arg::NOARG, false, "Show date only.");
	args.add("time-only", Arg::NOARG, false, "Show time only.");
	args.addAlias("date-only", 'd');
	args.addAlias("time-only", 't');

	// create an arg manager to parse the args
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

		// get the target value & turn time_t into something useful & format the time structure (e.g. 15:45:00 16/11/2001)
		time_t nTimeInSecs = ::atol(args.getTarget().c_str());
		struct tm* UTCTime = ::gmtime(&nTimeInSecs);


		// convert the timestamp format
		char szBuffer[19 + 1] = "";
		if (args.isPresent("date-only") && args.isPresent("time-only"))
		{
			::sprintf(szBuffer, "%02u:%02u:%02u %02u/%02u/%04u", UTCTime->tm_hour,
																 UTCTime->tm_min,
																 UTCTime->tm_sec,
																 UTCTime->tm_mday,			
										   						 (UTCTime->tm_mon + 1),			// months count from 0
																 (UTCTime->tm_year + 1900));	// years since 1900
		}
		else if (args.isPresent("date-only"))
		{
			::sprintf(szBuffer, "%02u/%02u/%04u", UTCTime->tm_mday,				
										   		  (UTCTime->tm_mon + 1),		// months count from 0
												  (UTCTime->tm_year + 1900));	// years since 1900

		}
		else if (args.isPresent("time-time"))
		{
			::sprintf(szBuffer, "%02u:%02u:%02u", UTCTime->tm_hour,
											      UTCTime->tm_min,
												  UTCTime->tm_sec);
		}
		else 
		{
			::sprintf(szBuffer, "%02u:%02u:%02u %02u/%02u/%04u", UTCTime->tm_hour,
																 UTCTime->tm_min,
																 UTCTime->tm_sec,
																 UTCTime->tm_mday,			
										   						 (UTCTime->tm_mon + 1),			// months count from 0
																 (UTCTime->tm_year + 1900));	// years since 1900
		}

		cout << szBuffer << endl;
	}


	return 0;
}




char* GenerateTimestamp(time_t nTimeInSecs)
{
	static char szStaticBuffer[19 + 1];

	assert(nTimeInSecs >= 0);

	// reset the buffer
	::strcpy(szStaticBuffer, "");

	// turn time_t into something useful
	struct tm* UTCTime = ::gmtime(&nTimeInSecs);

	// Format the time structure (e.g. 2001/11/16 15:45:00)
	::sprintf(szStaticBuffer, "%04u/%02u/%02u %02u:%02u:%02u",	(UTCTime->tm_year + 1900),	// years since 1900
											   					(UTCTime->tm_mon + 1),		// months count from 0
																UTCTime->tm_mday,
																UTCTime->tm_hour,
																UTCTime->tm_min,
																UTCTime->tm_sec);

	return szStaticBuffer;
}

