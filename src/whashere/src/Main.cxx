////////////////////////////////////////////////////////////////////////////////
//
// Main.cxx
//
////////////////////////////////////////////////////////////////////////////////
//
// Developed by Donnacha Forde (@DonnachaForde)
//
// Copyright © 2023-2024, Donnacha Forde. All rights reserved.
//
// This software is provided 'as is' without warranty, expressed or implied.
// Donnacha Forde accepts no responsibility for the use or reliability of this software.
//
////////////////////////////////////////////////////////////////////////////////


//#include <sys/stat.h>
//#include <sys/types.h>
//#include <sys/mkdev.h>
//#include <dirent.h>
//#include <fstream.h>		// ifstream, iostream

#include <iostream>
using namespace std; 

//#include <winbase.h>
#include <windows.h>

// espresso lib
#include <espresso.hxx>
using namespace espresso;



// version number
static const char VERSION[] = "0.11.0-beta";


// shared struct
struct DirectoryCounter
{
	int nNumFiles;
	int nNumDirs;
};



void execute(Args& args);
void parseDirectory(const wchar_t* szwDirectoryName, bool isVerbose, DirectoryCounter& dirCount);


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
		"whashere",
		"Examines what elements are in the given directory.",
		VERSION,
		"Donnacha Forde",
		"2023-2024",
		"@DonnachaForde");

	// pick up default args/switches
	args.addDefaults();
	args.requireTarget();

	// specify our switches & aliases
	args.add("verbose", Arg::NOARG, false, "Display directory details to stdout.", false);


	// create an arg manager to parse the args
	ArgManager argMgr = ArgManagerFactory::createInstance();
	int nRetVal = argMgr.parseAndProcessArgs(args);
	if (nRetVal != 0)
	{
		::exit(0);
	}

	if (args.isTargetPresent())
	{
		execute(args);
	}


	return 0;
}


void execute(Args& args)
{
	// get and check the target direct
	string strTargetDirectory = args.getTarget();
	if (!espresso::strings::isValidString(strTargetDirectory))
	{
		cout << "ERROR: Invalid target director." << endl;
		::exit(-1);
	}


	bool isVerbose = args.isPresent("versbose");



	DirectoryCounter dirCounter;
	dirCounter.nNumDirs = 0; 
	dirCounter.nNumFiles = 0;

	// convert string to WIN32 friendly format
	wstring strwDirectoryName = wstring(strTargetDirectory.begin(), strTargetDirectory.end());
	parseDirectory(strwDirectoryName.c_str(), isVerbose, dirCounter);

	cout << dirCounter.nNumDirs << " directories, " << dirCounter.nNumFiles << " files" << endl;

	return;
}




void parseDirectory(const wchar_t* szwDirectoryName, bool isVerbose, DirectoryCounter& dirCount)
{
	// need to add wildchar 
	wstring strwDirectoryPath = szwDirectoryName;
	strwDirectoryPath += L"\\*";
	
	HANDLE hFind;
	WIN32_FIND_DATA findData;
	hFind = ::FindFirstFile(strwDirectoryPath.c_str(), &findData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{

			wstring strwPath = szwDirectoryName;
			strwPath += L"\\";
			strwPath += findData.cFileName;

			// check for directories
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				::wprintf(L"<DIR>  %s\n", strwPath.c_str());

				// skip self and parent dirs - i.e. '.' and '..'
				if (::wcscmp(L".", findData.cFileName) != 0 && ::wcscmp(L"..", findData.cFileName) != 0)
				{
					dirCount.nNumDirs++;

					// parse subdirectory - recursively
					wstring strwSubDirectory = szwDirectoryName;
					strwSubDirectory += L"\\";
					strwSubDirectory += findData.cFileName;
					parseDirectory(strwSubDirectory.c_str(), isVerbose, dirCount);
				}
			}
			else 
			{
				dirCount.nNumFiles++;
				::wprintf(L"<FILE> %s\n", findData.cFileName);
			}

		} while (::FindNextFile(hFind, &findData));

		::FindClose(hFind);
	}

	return;
}



