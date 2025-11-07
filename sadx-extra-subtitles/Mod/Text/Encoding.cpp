#include "pch.h"
#include "Encoding.h"
#include "TextConv.hpp"


const char* Encode(const std::string& text, int codepage)
{
	return UTF8toCodepage(text.c_str(), codepage);
}