////////////////////////////////////////////////////////////////////////////////
//
// Main.cxx
//
////////////////////////////////////////////////////////////////////////////////
//
// Developed by Donnacha Forde (@DonnachaForde)
//
// Copyright © 2021-2024, Donnacha Forde. All rights reserved.
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
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iostream>
using namespace std;

#ifdef WIN32
	#include <io.h>
#else
	#include <unistd.h>
#endif

// windows
#include <Windows.h>


// version number
static const char VERSION[] = "0.11.0-beta";



struct ProcessInfo
{
	wchar_t szwCommand[256];
	DWORD dwExitCode;
	time_t nStopTime;
	DWORD dwPID;
};


// declarations
void execAndMonitorProcess(const char* szCommand, /*inout*/ ProcessInfo& processInfo); 
void reportTerminationToUI(const ProcessInfo processInfo);

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

	Args args(argc,
		argv,
		"watch",
		"Starts and monitors the command you provide and reports when it terminates.",
		VERSION,
		"Donnacha Forde",
		"2021-2024",
		"@DonnachaForde");

	// pick up default args/switches
	args.addDefaults();

	args.add("exec", Arg::STRING, true, "The command to execute and monitor.", true);
	args.add("forever", Arg::NOARG, false, "Will keep restarting the command should it terminate.", false);
	args.add("silent", Arg::NOARG, false, "Do not report terminations to the user. Used in conjunction with --forever to keep restarting the command.", false);
	args.add("limit", Arg::INTEGER, false, "Used in conjuction with --forever to set a limit on the number of restarts.", true);

	// create an arg manager to parse the args
	ArgManager argMgr = ArgManagerFactory::createInstance();
	int nRetCode = argMgr.parseAndProcessArgs(args);
	if (nRetCode != 0)
	{
		::exit(0);
	}


	if (args.isPresent("limit") && !args.isPresent("forever"))
	{
		cout << "ERROR: Cannot specify --limit without also specifying --forever option." << endl;
		::exit(-1);
	}

	if (args.isTargetPresent() || args.isRequiredArgsPresent())
	{
		execute(args);
	}

	::exit(0);
}

void execute(Args & args)
{
	// unless a limit has been specified, this is enough to keep it going
	long nRestartLimit = 1;
	if (args.isPresent("limit"))
	{
		nRestartLimit = args.getNumericValue("limit");
		if (nRestartLimit <= 0)
		{
			cout << "ERROR: The --limit value must be > 0." << endl;
			::exit(-1);
		}
	}


	// extract the target command
	char szTargetCommand[256] = "";
	::strcpy(szTargetCommand, args.getStringValue("exec").c_str());
	assert(strings::isValidString(szTargetCommand));

	// check that the executable file actually exists
	int nRetCode = ::access(szTargetCommand, 00);
	if (nRetCode != 0)
	{
		if (nRetCode == EACCES)
		{
			cout << "ERROR: Unable to access '" << szTargetCommand << "' (file permissions)." << endl;
		}
		else if (nRetCode == ENOENT)
		{
			cout << "ERROR: Executable file '" << szTargetCommand << "' does not exist." << endl;
		}
		else if (nRetCode == EINVAL)
		{
			cout << "ERROR: An invalid parameter for checking access to file '" << szTargetCommand << "' was provided." << endl;
		}
		else if (nRetCode == -1)
		{
			cout << "ERROR: Executable file '" << szTargetCommand << "' does not exist." << endl;
		}

		::exit(-1);
	}


	//
	// kick off and watch child process
	//

	do
	{
		ProcessInfo processInfo;
		execAndMonitorProcess(szTargetCommand, processInfo);

		if (!args.isPresent("silent"))
		{
			reportTerminationToUI(processInfo);
		}

		// decrement our limit counter
		if (args.isPresent("limit"))
		{
			nRestartLimit--;
		}

	} while (args.isPresent("forever") && (nRestartLimit > 0));


	return;
}




void execAndMonitorProcess(const char* szCommand, /*inout*/ ProcessInfo& processInfo)
{
	assert(szCommand != NULL); 
	assert(strings::isValidString(szCommand));

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
								CREATE_NEW_CONSOLE | NORMAL_PRIORITY_CLASS | CREATE_PRESERVE_CODE_AUTHZ_LEVEL,
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
		DWORD dwExitCode = 0;
		::GetExitCodeProcess(procInfo.hProcess, &dwExitCode);

		// populate process info 
		processInfo.dwExitCode = dwExitCode;
		processInfo.dwPID = procInfo.dwProcessId; 
		::wcscpy(processInfo.szwCommand, szwCommand);
		time_t nRawTimeNow;
		::time(&nRawTimeNow);
		processInfo.nStopTime = nRawTimeNow; 

		cout << "INFO: Process '" << szCommand << "' (PID=" << procInfo.dwProcessId << ") completed." << endl;
	}
	else
	{
		// report why we weren't able to start the child process
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

		// free the allocated buffer
		::LocalFree(lpMessageBuffer);
	}
	
	return;

}


void reportTerminationToUI(const ProcessInfo processInfo)
{
	struct tm* timeInfo = ::localtime(&processInfo.nStopTime);
	char* szStopTime = ::asctime(timeInfo); 
	wchar_t szwStopTime[64];
	::mbstowcs(szwStopTime, szStopTime, (::strlen(szStopTime) + 1));


	// pop up a message box to let the user know the child process has terminated
	wchar_t szwStatusMessage[512];
	::swprintf(szwStatusMessage, 256, L"%s\n'%s' (PID=%d) has terminated with exit code %d.", szwStopTime, processInfo.szwCommand, processInfo.dwPID, processInfo.dwExitCode);
	
	::MessageBox(NULL, szwStatusMessage, L"watch: Monitored Process Terminated", MB_OK | MB_ICONEXCLAMATION | MB_SYSTEMMODAL | MB_SETFOREGROUND);

	return;
}