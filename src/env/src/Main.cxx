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


// espresso lib
#include <espresso.hxx>
using namespace espresso;


#include <iostream>
using namespace std;

// version number
static const char VERSION[] = "0.3.0-beta";


// print list of env vars to stdout
void execute(char* envp[]);





int main(int argc, char* argv[], char* envp[])
{
	//
	// arg parsing
	// 

	Args args(argc, 
			  argv, 
			  "env", 
			  "Displays environment settings.", 
			  VERSION, 
			  "Donnacha Forde", 
			  "2001-2024", 
			  "@DonnachaForde");

	// pick up default args/switches
	args.addDefaults();

	// parse args and if all okay, diplay env vars
	ArgManager argMgr = ArgManagerFactory::createInstance();
	int nRetCode = argMgr.parseAndProcessArgs(args);

	// env has no target arg so complain if we find one
	if (args.isTargetPresent())
	{
		argMgr.onRequestUsage(args);
	}
	else
	{
		if (nRetCode == 0)
		{
			execute(envp);
		}
	}



	::exit(0);
}



// print the list of env variables to stdout
void execute(char* envp[])
{
	while (*envp != NULL)
	{
		cout << *envp++ << endl;
	}

	return;
}
