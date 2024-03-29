////////////////////////////////////////////////////////////////////////////////
//
// Main.cxx
//
////////////////////////////////////////////////////////////////////////////////
//
// Developed by Donnacha Forde (@DonnachaForde)
//
// Copyright © 2020-2024, Donnacha Forde. All rights reserved.
//
// This software is provided 'as is' without warranty, expressed or implied.
// Donnacha Forde accepts no responsibility for the use or reliability of this software.
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

// WIN32
#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#define VC_EXTRALEAN
	#include <windows.h>
#endif

// espresso lib
#include <espresso.hxx>
using namespace espresso;

// version number
static const char VERSION[] = "0.11.0-beta";


void execute(Args& args);



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

	Args args(	argc,
				argv,
				"cores",
				"Reports the number of core processors on the host.",
				VERSION,
				"Donnacha Forde",
				"2020-2024",
				"@DonnachaForde");

	// pick up default args/switches
	args.addDefaults();

	// create an arg manager to parse the args
	ArgManager argMgr = ArgManagerFactory::createInstance();
	int nRetVal = argMgr.parseAndProcessArgs(args);
	if (nRetVal != 0)
	{
		::exit(0);
	}

	execute(args);

    ::exit(0);
}


void execute(Args& args)
{
	SYSTEM_INFO systemInfo;
	::GetSystemInfo(&systemInfo);

	::printf("%i\n", systemInfo.dwNumberOfProcessors);

	return;
}

