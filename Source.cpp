#include <Windows.h>
#include <iostream>

#include "Share.h"
#include "Loader.h" //Dynamic Load of DLL
#include "stdafx.h"

//Set Prototypes for Functions we're loading.

//CopyFile https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-copyfile
typedef bool(__stdcall *Copy)(LPCTSTR lpExistingFileName, LPCTSTR  lpNewFileName, BOOL bFailIfExists);

int main(void)
{
	//Load "CopyFile" Function From Kernel32.dll
	Loader CopyFileLoader = Loader("Kernel32.dll", "CopyFile");
	//Load Other Functions
	
	//Set and Handle For Functions
	CopyFileLoader.Load();	//Get Function Handle For Functions.
	Copy copyFile = (Copy)(CopyFileLoader.Execute());
	CopyFile()
}