#pragma once

#include <string>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <shobjidl.h> 
#include <sstream>
#include <iostream>
#include <locale>
#include <codecvt>

class FileDialog
{
public:

	static std::string OpenOld();
	static std::string Open();
	static std::string Save();
	static std::string Export();
private:


};

