////////////////////////////////////////////////////////////////////////////////
//
// Main.cxx
//
////////////////////////////////////////////////////////////////////////////////
//
// Developed by Donnacha Forde (@DonnachaForde)
//
// Copyright © 2021, Donnacha Forde. All rights reserved.
//
// This software is provided 'as is' without warranty, expressed or implied.
// Donnacha Forde accepts no responsibility for the use or reliability of this software.
//
////////////////////////////////////////////////////////////////////////////////



// espresso lib
#include <espresso.hxx>
using namespace espresso;


#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iostream>
using namespace std;


#include <Windows.h>


// declaration
void execAndMonitorProcess(char* szCommand); 



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
		"watch",
		"Monitors the process you provides and reports when it terminates.",
		"0.1.0-beta",
		"Donnacha Forde",
		"2021",
		"@DonnachaForde");

	// pick up default args/switches
	args.addDefaults();

	args.add("exec", Arg::STRING, "The command to execute and monitor.", true);

	// create an arg manager to parse the args
	ArgManager argMgr = ArgManagerFactory::createInstance();
	int nRetCode = argMgr.parseAndProcessArgs(args);

	// proceed if a default switch hasn't been triggered
	if (nRetCode != 1)
	{
		// check we have required arg
		if (!args.isPresent("exec"))
		{
			cout << "ERROR: You must specify what command to execute. Use --help for option information." << endl;
			::exit(-1);
		}


		//
		// everything after the --exec switch is the command (and args) we need to execute
		//

		// search for '--exec' in the arg list
		int iIndex = 1;
		bool isFound = false;

		while (!isFound && iIndex < argc)
		{
			if (::strcmp(argv[iIndex], "--exec") == 0)
			{
				isFound = true;

				// bump the index so we refer to the command after the --exec switch
				iIndex++;
			}
			else
			{
				iIndex++;
			}
		}

		// target command is everything after --exec - 	create the command string from the rest of the args
		assert(isFound);
		char szTargetCommand[256] = "";

		for (int i = iIndex; i < argc; i++)
		{
			::strcat(szTargetCommand, argv[i]);

			// if we have more args, add a space
			if ((i + 1) < argc)
			{
				::strcat(szTargetCommand, " ");
			}
		}


		//
		// kick off and watch child process
		//

		execAndMonitorProcess(szTargetCommand);
	}
	
	::exit(0);
}


void execAndMonitorProcess(char* szCommand)
{
	assert(szCommand != NULL); 

	// convert to widestring for Win32
	wchar_t szwCommand[512];
	::mbstowcs(szwCommand, szCommand, (::strlen(szCommand) + 1));


	// kick off child process
	STARTUPINFO startInfo = { sizeof(startInfo) };
	PROCESS_INFORMATION procInfo;
	bool isSuccess = false;
	isSuccess = ::CreateProcess(NULL, 
								szwCommand, 
								NULL, 
								NULL, 
								TRUE, 
								0,			/* was using CREATE_NEW_CONSOLE | NORMAL_PRIORITY_CLASS here but want to keep console apps in same window */
								NULL, 
								NULL, 
								&startInfo, 
								&procInfo);

	if (isSuccess)
	{
		//
		// wait until the child procdess terminates & then notify
		//

		cout << "INFO: Process '" << szCommand << "' started as PID=" << procInfo.dwProcessId << "." << endl;
		::WaitForSingleObject(procInfo.hProcess, INFINITE);


		//
		// now, let user know the child process has terminated
		// 

		// get the child process exit code
		DWORD exitCode = 0;
		::GetExitCodeProcess(procInfo.hProcess, &exitCode);

		// pop up a message box to let the user know the child process has terminated
		wchar_t szwStatusMessage[512];
		::swprintf(szwStatusMessage, 256, L"'%s' \n\n(PID=%d) has terminated with exit code %d.", szwCommand, procInfo.dwProcessId, exitCode);
		::MessageBox(NULL, szwStatusMessage, L"watch: Monitored Process Terminated", MB_OK | MB_ICONEXCLAMATION);

		cout << "INFO: Process '" << szCommand << "' (PID=" << procInfo.dwProcessId << ") completed." << endl;

	}
	else
	{
		DWORD dwError = ::GetLastError();

		LPVOID lpMessageBuffer = NULL;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
						NULL,
						dwError,
						LANG_NEUTRAL,
						(LPWSTR)&lpMessageBuffer,
						0,
						NULL);

		cout << "ERROR: " << lpMessageBuffer << endl;
	}
	
	return;

}