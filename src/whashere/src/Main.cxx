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


//bool isDirectory(const char* szDirectoryName)
//{
//	struct stat directoryInfo;
//	::stat(szDirectoryName, &directoryInfo);
//
//	return S_ISDIR(directoryInfo.st_mode);
//}

void parseDirectory(LPCWSTR szDirectoryName)
{
	// prep dir name for processing with FindFirstFile
	wchar_t szwDirectory[MAX_PATH];
	::wcscpy_s(szwDirectory, szDirectoryName);
	::wcscat_s(szwDirectory, L"\\*");


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
					::wcscpy_s(szwSubDirectory, szDirectoryName);
					::wcscat_s(szwSubDirectory, L"\\");
					::wcscat_s(szwSubDirectory, findData.cFileName);

					parseDirectory(szwSubDirectory);
				}
			}
			else
			{
				::wprintf(L"<FILE> %s\n", findData.cFileName);
			}

		} 
		while (::FindNextFile(hFind, &findData));
		
		::FindClose(hFind);
	}

}


int main(int argc, char* argv[], char* envp[])
{
	cout << "whashere parsing directories..." << endl;
	parseDirectory(L"D:/tmp");
}