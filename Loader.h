#pragma once

using std::string;

class Loader
{
public:
	
	Loader(string dllName, string funcName);
	Loader(string dllName, string funcName, HINSTANCE__ *loadedLibrary);
	~Loader();
	void Load();
	FARPROC Execute();
	HINSTANCE__ *GetHandleForDll();

private:
	std::string _dllName;
	std::string _funcName;
	HINSTANCE__  *_library;
	FARPROC _function;


};

