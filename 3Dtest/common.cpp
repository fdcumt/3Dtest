#include "common.h"
#include <Windows.h>
#include <stdio.h> 

void utility::OutPutDebugPrintf(const char* strFormat, ...)
{
	char strBuffer[4096] = {0};
	va_list vlArgs;
	va_start(vlArgs, strFormat);
	vsnprintf(strBuffer, sizeof(strBuffer)-1, strFormat, vlArgs );
	va_end(vlArgs);
	OutputDebugString(strBuffer);
}


