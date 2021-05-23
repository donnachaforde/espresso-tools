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
			  "0.2.0-beta", 
			  "Donnacha Forde", 
			  "2001-2007", 
			  "@DonnachaForde");

	// pick up default args/switches
	args.addDefaults();

	// create an arg manager to parse the args
	ArgManager argMgr = ArgManagerFactory::createInstance();
	if (argMgr.parseAndProcessArgs(args) == -1)
	{
		cout << "ERROR: Invalid option: '" << args.getInvalidOption() << "'. Use --help for option information." << endl;
		::exit(-1);
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
