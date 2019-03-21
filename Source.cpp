#include <Windows.h>
#include <iostream>

#include "Share.h"
#include "Loader.h" //Dynamic Load of DLL
#include "stdafx.h"

//Set Prototypes for Functions we're loading.

//CopyFile https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-copyfile
typedef bool(__stdcall *Copy)(LPCTSTR lpExistingFileName, LPCTSTR  lpNewFileName, BOOL bFailIfExists);
//GetModuleFileNameA https://docs.microsoft.com/en-us/windows/desktop/api/libloaderapi/nf-libloaderapi-getmodulefilenamea
typedef DWORD(__stdcall *GetModuleFile)(HMODULE hModule, LPSTR lpFilename, DWORD nSize);
//GetFileVersionInfoSizeA https://docs.microsoft.com/en-us/windows/desktop/api/winver/nf-winver-getfileversioninfosizea
typedef DWORD(__stdcall *GetFileVersionSize)(LPCSTR  lptstrFilename, LPDWORD lpdwHandle);
//GetFileVersionInfoA https://docs.microsoft.com/en-us/windows/desktop/api/winver/nf-winver-getfileversioninfoa#requirements
typedef DWORD(__stdcall *GetFileVersionInformation)(LPCSTR lptstrFilename, DWORD  dwHandle, DWORD  dwLen, LPVOID lpData);

int main(void)
{
	//Load "CopyFile" From Kernel32.dll
	Loader CopyFileLoader = Loader("Kernel32.dll", "CopyFileA");
	//Load "GetModuleFileName" From Kernel32.dll
	Loader GetModuleFileNameLoader = Loader("Kernel32.dll", "GetModuleFileNameA", CopyFileLoader.GetHandleForDll());
	//Load "GetFileVersionInfoSizeA" From Api-ms-win-core-version-l1-1-0.dll
	Loader GetFileVersionInfoSizeLoader = Loader("Api-ms-win-core-version-l1-1-0.dll", "GetFileVersionInfoSizeA");
	//Load "GetFileVersionInfoA" From Api-ms-win-core-version-l1-1-0.dll
	Loader GetFileVersionInfoLoader = Loader("Api-ms-win-core-version-l1-1-0.dll", "GetFileVersionInfoA");

	//Set and Handle For Functions
	CopyFileLoader.Load();
	GetModuleFileNameLoader.Load();
	GetFileVersionInfoSizeLoader.Load();
	GetFileVersionInfoLoader.Load();

	//Get Function Handle For Functions.
	Copy copyFile = (Copy)(CopyFileLoader.Execute());
	GetModuleFile getModuleFileName = (GetModuleFile)(GetModuleFileNameLoader.Execute());
	GetFileVersionSize getFileVersionInfoSize = (GetFileVersionSize)(GetFileVersionInfoSizeLoader.Execute());
	GetFileVersionInformation getFileVersionInfo = (GetFileVersionInformation)(GetFileVersionInfoLoader.Execute());

	//Get Current EXE Path.
	TCHAR CurrentModulePath[MAX_PATH];
	getModuleFileName(NULL, CurrentModulePath, MAX_PATH);

	//Random List Of Copy Folders (Choose One)
	copyFile(CurrentModulePath, "C:\\Users\\Johnathon\\Johnathon.exe", false);

	DWORD
}