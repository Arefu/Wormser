#include <Windows.h>
#include <string>

#include "stdafx.h"
#include "Loader.h"
#include <assert.h>

using std::string;

Loader::Loader(string dllName, string funcName)
{
	this->_dllName = dllName;
	this->_funcName = funcName;
	this->_library = LoadLibrary(_dllName.c_str());
	if (this->_library == NULL)
	{
		assert("Uh-Oh!");
	}
}

Loader::Loader(string dllName, string funcName, HINSTANCE__ *loadedLibrary)
{
	this->_dllName = dllName;
	this->_funcName = funcName;
	this->_library = GetModuleHandle(dllName.c_str());
}

Loader::~Loader()
{
	FreeLibrary(this->_library);
}

void Loader::Load()
{
	this->_function = GetProcAddress(this->_library, _funcName.c_str()); //Really Dunno Aye.... (I Don't Care If It Fails #MadLads)
}

FARPROC Loader::Execute()
{
	return this->_function;
}

HINSTANCE__ *Loader::GetHandleForDll()
{
	return this->_library;
}