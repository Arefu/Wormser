#pragma once

using std::string;

class Loader
{
public:
	//Load New DLL
	Loader(string dllName, string funcName);
	//Load Already Imported DLL
	Loader(string dllName, string funcName, HINSTANCE__ *loadedLibrary);
	~Loader();
	//Import Library
	void Load();
	//Return Function
	FARPROC Execute();
	//Get Handle of Imported DLL
	HINSTANCE__ *GetHandleForDll();

private:
	std::string _dllName;
	std::string _funcName;
	HINSTANCE__  *_library;
	FARPROC _function;
};

