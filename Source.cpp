#include <Windows.h>
#include <iostream>

#include "Share.h"
#include "Loader.h" //Dynamic Load of DLL
#include "stdafx.h"

//Set Prototypes for Functions we're loading.

//CopyFile https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-copyfile
typedef bool(__stdcall *Copy)(LPCTSTR lpExistingFileName, LPCTSTR  lpNewFileName, BOOL bFailIfExists);

//GetModuleFileNameA https://docs.microsoft.com/en-us/windows/desktop/api/libloaderapi/nf-libloaderapi-getmodulefilenamea
typedef DWORD(__stdcall *GetCwd)(HMODULE hModule, LPSTR lpFilename, DWORD nSize);



HMODULE GetCurrentModule()
{ // NB: XP+ solution!
	HMODULE hModule = NULL;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		(LPCTSTR)GetCurrentModule,
		&hModule);

	return hModule;
}



int main(void)
{
	//Load "CopyFile" Function From Kernel32.dll
	Loader CopyFileLoader = Loader("Kernel32.dll", "CopyFile");
	//Load "GetModuleFileName" From the Handle in Kernel32.dll
	Loader GetModuleFileNameLoader = Loader("Kernel32.dll", "GetModuleFileName", CopyFileLoader.GetHandleForDll());
	//Load Other Functions
	
	//Set and Handle For Functions
	CopyFileLoader.Load();
	GetModuleFileNameLoader.Load();

	//Get Function Handle For Functions.
	Copy copyFile = (Copy)(CopyFileLoader.Execute());
	GetCwd getModuleFileName = (GetCwd)(GetModuleFileNameLoader.Execute());

	TCHAR actualpath[2048];
	auto Mod = GetCurrentModule();
	getModuleFileName(Mod, actualpath, 2048);
	//GetLastError();

}