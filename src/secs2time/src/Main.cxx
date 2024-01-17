// Main.cxx


// espresso lib
#include <espresso.hxx>
using namespace espresso;

// crt
#include <iostream>
#include <ctime>
#include <cassert>
using namespace std;


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
			  "1.0 (Beta)", 
			  "Donnacha Forde", 
			  "2001-2007", 
			  "@DonnachaForde");

	// pick up default args/switches
	args.addDefaults();

	// specify our switches
	args.add("date", Arg::NOARG, "Show date only.");
	args.add("time", Arg::NOARG, "Show time only.");
	args.addAlias("date", 'd');
	args.addAlias("time", 't');

	// create an arg manager to parse the args
	ArgManager argMgr = ArgManagerFactory::createInstance();
	if (argMgr.parseAndProcessArgs(args) == -1)
	{
		cout << "ERROR: Invalid option: '" << args.getInvalidOption() << "'. Use --help for option information." << endl;
		::exit(-1);
	}


	// check for requests for help, usage and version
	if (args.isPresent("help") || args.isPresent('h') || args.isPresent('?'))
	{
		cout															<< endl
			 << args.getProgramDescription()							<< endl
																		<< endl 
			 << args.getUsage()											<< endl
																		<< endl 
			 << args.getOptionsDescriptions()							<< endl
																		<< endl
			 << args.getCopyrightNotice()								<< endl
			 << args.getBugReportingInstructions()						<< endl;

		::exit(0);
	}
	else if (args.isPresent("usage"))
	{
		cout << args.getUsage()											<< endl
																		<< endl 
			 << args.getCopyrightNotice()								<< endl;
		::exit(0);
	}
	else if (args.isPresent("version"))
	{
		cout << args.getProgramName() << " " << args.getVersion()		<< endl 
			 << args.getCopyrightNotice()								<< endl;
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
		if (args.isPresent("date") && args.isPresent("time"))
		{
			::sprintf(szBuffer, "%02u:%02u:%02u %02u/%02u/%04u", UTCTime->tm_hour,
																 UTCTime->tm_min,
																 UTCTime->tm_sec,
																 UTCTime->tm_mday,			
										   						 (UTCTime->tm_mon + 1),			// months count from 0
																 (UTCTime->tm_year + 1900));	// years since 1900
		}
		else if (args.isPresent("date"))
		{
			::sprintf(szBuffer, "%02u/%02u/%04u", UTCTime->tm_mday,				
										   		  (UTCTime->tm_mon + 1),		// months count from 0
												  (UTCTime->tm_year + 1900));	// years since 1900

		}
		else if (args.isPresent("time"))
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
	else
	{
		cout << args.getUsage()											<< endl
																		<< endl 
			 << args.getCopyrightNotice()								<< endl;
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

