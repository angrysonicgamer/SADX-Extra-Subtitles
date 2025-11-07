#pragma once

enum Encoding
{
	Windows1252 = 1252,
	ShiftJIS = 932
};

const char* Encode(const std::string& text, int codepage);