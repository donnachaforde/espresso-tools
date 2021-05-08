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
	args.addDefaults();


	// parse the arg list
	if (!args.parse())
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

	while (*envp != NULL)
	{
		cout << *envp++ << endl;
	}

	return 0;
}
