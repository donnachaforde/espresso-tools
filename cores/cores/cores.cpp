// cores.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// win32 API
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>


int main()
{

	SYSTEM_INFO systemInfo;
	::GetSystemInfo(&systemInfo);

	::printf("%i\n", systemInfo.dwNumberOfProcessors);

    return 0;
}

