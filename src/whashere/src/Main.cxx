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

// c++rt
#include <iostream>
//#include <filesystem>
using namespace std; 

// windows
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
	int nNumWordDocs;
	int nNumSpreadsheets;
	int nNumTextFiles;
};



void execute(Args& args);
void parseDirectory(const wstring& szwDirectoryName, bool isVerbose, DirectoryCounter& dirCount);
void checkFileType(const wstring& wstrFilename, DirectoryCounter& dirCounter);


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
	args.add("detail", Arg::NOARG, false, "Displays number of certain file types.", false);


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

	// initialize counters
	DirectoryCounter dirCounter;
	dirCounter.nNumDirs = 0; 
	dirCounter.nNumFiles = 0;
	dirCounter.nNumWordDocs = 0;
	dirCounter.nNumSpreadsheets = 0;
	dirCounter.nNumTextFiles = 0;

	// convert string to WIN32 friendly format
	wstring wstrDirectoryName = wstring(strTargetDirectory.begin(), strTargetDirectory.end());
	parseDirectory(wstrDirectoryName, args.isPresent("verbose"), dirCounter);

	// write out results
	cout << dirCounter.nNumDirs << " directories, " << dirCounter.nNumFiles << " files" << endl;

	// if asked, provide details
	if (args.isPresent("detail"))
	{
		cout << dirCounter.nNumWordDocs << " Word docs"				<< endl
			 << dirCounter.nNumSpreadsheets << " Spreadsheets"		<< endl
			 << dirCounter.nNumTextFiles << " Text files"			<< endl;
	}

	return;
}




void parseDirectory(const wstring& wstrDirectoryName, bool isVerbose, DirectoryCounter& dirCounter)
{
	// need to add wildchar for call to FindFirstFile
	wstring strwDirectoryPath = wstrDirectoryName;
	strwDirectoryPath += L"\\*";
	
	HANDLE hFind;
	WIN32_FIND_DATA findData;
	hFind = ::FindFirstFile(strwDirectoryPath.c_str(), &findData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			// construct full path name
			wstring wstrPathName = wstrDirectoryName;
			wstrPathName += L"\\";
			wstrPathName += findData.cFileName;

			// check for directories
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (isVerbose)
				{
					::wprintf(L"<DIR>  %s\n", wstrPathName.c_str());
				}

				// skip self and parent dirs - i.e. '.' and '..'
				if (::wcscmp(L".", findData.cFileName) != 0 && ::wcscmp(L"..", findData.cFileName) != 0)
				{
					dirCounter.nNumDirs++;

					// parse subdirectory - recursively
					wstring wstrSubDirectoryName = wstrDirectoryName;
					wstrSubDirectoryName += L"\\";
					wstrSubDirectoryName += findData.cFileName;
					parseDirectory(wstrSubDirectoryName, isVerbose, dirCounter);
				}
			}
			else 
			{
				dirCounter.nNumFiles++;
				checkFileType(wstrPathName, dirCounter);

				if (isVerbose)
				{
					::wprintf(L"<FILE> %s\n", wstrPathName.c_str());
				}
			}

		} while (::FindNextFile(hFind, &findData));

		::FindClose(hFind);
	}

	return;
}


void checkFileType(const wstring& wstrFilename, DirectoryCounter& dirCounter)
{
	// determine the file extension
	wstring extn; 
	size_t dot = wstrFilename.find_last_of(L".");
	if (dot != string::npos)
	{
		extn = wstrFilename.substr(dot, (wstrFilename.size() - dot));
	}

	if (!extn.empty())
	{
		if (extn == L".xlsx")
		{
			dirCounter.nNumSpreadsheets++;
		}
		else if (extn == L".docx")
		{
			dirCounter.nNumWordDocs++;
		}
		else if (extn == L".txt")
		{
			dirCounter.nNumTextFiles++;
		}
	}

	return;
}



