#pragma once

class DebugMessage
{
public:
	static void Success(const char* language);
	static void Error(const char* language);
	static void Loading();
	static void Finished();
};