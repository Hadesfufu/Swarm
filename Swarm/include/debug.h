#ifndef __DEBUG_H_INCLUDED__
#define __DEBUG_H_INCLUDED__

#ifdef ZERATEAM_DEBUG_DLL_EXPORTS
#define ZERATEAM_DEBUG_DLL_API __declspec(dllexport) 
#else
#define ZERATEAM_DEBUG_DLL_API __declspec(dllimport)
#endif

//	custom assert to display file and line abord
//	also display custom message in alertBox
void ZERATEAM_DEBUG_DLL_API GfxDbgAssert(bool bExpectedTrue, const char * pMessage = 0);

//	custom printf to get display in consoleoutput
int ZERATEAM_DEBUG_DLL_API GfxDbgPrintf(const char * pFormat, ...);

#endif