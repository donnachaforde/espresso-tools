// Main.cxx

// espresso lib
#include <espresso.hxx>
using namespace espresso;


#include <iostream>
using namespace std;



int main(int argc, char* argv[], char* envp[])
{
	//
	// arg parsing
	// 

	Args args(argc, 
			  argv, 
			  "env", 
			  "Displays environment settings.", 
			  "1.0 (Beta)", 
			  "Donnacha Forde", 
			  "2001-2007", 
			  "@DonnachaForde");

	// pick up default args/switches
	args.Initialize();


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
	// main processing
	//

	while (*envp != NULL)
	{
		cout << *envp++ << endl;
	}

	return 0;
}
