// Main.cxx


// espresso lib
#include <espresso.hxx>
using namespace espresso;

// crt
#include <iostream>
#include <ctime>
#include <cassert>
using namespace std;




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
			  "1.0 (Beta)", 
			  "Donnacha Forde", 
			  "2009", 
			  "@DonnachaForde");

	// pick up default args/switches
	args.Initialize();

	// specify our switches
//	args.Add("date", Arg::NOARG, "Show date only.");
//	args.AddAlias("date", 'd');


	// create argmgr to handle default switches
	ArgMgrCLI argMgr;

	int nRetVal = argMgr.ParseAndProcessArgs(args);
	if (nRetVal != 0)
	{
		::exit(0);
	}



	//
	// main processing
	//

	if (args.IsTargetPresent())
	{
		//
		// check range - devnote: time_t is defined as a long, not an unsigned long so use one to convert & perform the test
		// 

		unsigned long ul = ::strtoul(args.GetTarget().c_str(), 0, 10);

		 // time_t is a long, but time is measure in secs since epoch
		if ((ul > 2147483647) || (ul < 0))
		{
			cout << "ERROR: Value is out of range (0..2147483647 - i.e. 2,147,483,647 or 2^31)" << endl;
			return -2;
		}


		//
		// Otherwise, carry on - it's safe to do the conversion
		//

		int nDec = ::atoi(args.GetTarget().c_str());


		char szBuffer[19 + 1] = "";
		::sprintf(szBuffer, "%x", nDec);

		cout << szBuffer << endl;
	}
	else
	{
		cout << args.GetUsage()											<< endl
																		<< endl 
			 << args.GetCopyrightNotice()								<< endl;
	}

	return 0;
}



