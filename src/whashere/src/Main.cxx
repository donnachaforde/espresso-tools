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




void execute(Args& args);
void parseDirectory(LPCWSTR szDirectoryName);
//bool isDirectory(const char* szDirectoryName);




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

	//parseDirectory(L"D:/tmp");
	//parseDirectory(szwDirectory);

	// convert string to WIN32 friendly format
	wstring strwDirectoryName = wstring(strTargetDirectory.begin(), strTargetDirectory.end());
	//wchar_t szwDirectory[MAX_PATH];
	//::wcscpy_s(szwDirectory, strwDirectoryName.c_str());
	//::wcscat_s(szwDirectory, L"\\*");

	parseDirectory(strwDirectoryName.c_str());



}





void parseDirectory(LPCWSTR szDirectoryName)
{
	wchar_t szwDirectory[MAX_PATH];
	::wcscpy_s(szwDirectory, szDirectoryName);
	::wcscat_s(szwDirectory, L"\\*");


	// prep dir name for processing with FindFirstFile
	HANDLE hFind;
	WIN32_FIND_DATA findData;
	hFind = ::FindFirstFile(szwDirectory, &findData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				::wprintf(L"<DIR> %s\n", findData.cFileName);

				// skip self and parent dirs
				if (wcscmp(L".", findData.cFileName) != 0 && wcscmp(L"..", findData.cFileName) != 0)
				{
					wchar_t szwSubDirectory[MAX_PATH];
					::wcscpy_s(szwSubDirectory, szwDirectory);
					::wcscat_s(szwSubDirectory, L"\\");
					::wcscat_s(szwSubDirectory, findData.cFileName);

					parseDirectory(szwSubDirectory);
				}
			}
			else
			{
				::wprintf(L"<FILE> %s\n", findData.cFileName);
			}

		} while (::FindNextFile(hFind, &findData));

		::FindClose(hFind);
	}

}

//bool isDirectory(const char* szDirectoryName)
//{
//	struct stat directoryInfo;
//	::stat(szDirectoryName, &directoryInfo);
//
//	return S_ISDIR(directoryInfo.st_mode);
//}
