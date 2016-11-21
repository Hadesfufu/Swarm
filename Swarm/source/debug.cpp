
#include "debug.h"
#include <cstdio>
#include <windows.h>
#include <ctype.h>
#include <stdlib.h>
#include <malloc.h>

#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

void GfxDbgMessageBox( const char *pMessage );

struct TGfxDbgStrBuffer
{
	TGfxDbgStrBuffer() { string[ 0 ] = '\0'; }
	operator const char * () const { return string; }
	char string[ 1024 ];
};
struct TGfxDbgStrStream
{
	TGfxDbgStrBuffer buffer;

	void Append( int value )
	{
		sprintf_s( buffer.string, "%s%d", buffer.string, value );
	}
	void Append( const char * value )
	{
		sprintf_s( buffer.string, "%s%s", buffer.string, value );
	}
	TGfxDbgStrBuffer GetBuffer()
	{
		return buffer;
	}
	static TGfxDbgStrStream & GetInstance()
	{
		static TGfxDbgStrStream str_stream;
		str_stream.buffer = TGfxDbgStrBuffer();
		return str_stream;
	}
};

template<typename T> TGfxDbgStrStream & operator,(TGfxDbgStrStream & stream, const T & value)
{
	stream.Append( value );
	return stream;
}

#define GfxDbgStrFormat( ... ) ( TGfxDbgStrStream::GetInstance(), __VA_ARGS__ ).GetBuffer()

void GfxDbgMsgHelper( const char * prefix, const char * text, const char * expr, const char * file, int line )
{
	TGfxDbgStrBuffer message;

	if( text != 0 && text[ 0 ] != 0 )
	{
		message = GfxDbgStrFormat( prefix, text, "\nFile : ", file, "\nLine : ", line );
	}
	else
	{
		message = GfxDbgStrFormat( prefix, expr, "\nFile : ", file, "\nLine : ", line  );
	}

	GfxDbgPrintf( "%s\n", message.string );
	GfxDbgMessageBox( message.string );
}

#define GfxDbgError() GfxDbgErrorInfo( "" )
#define GfxDbgErrorInfo( ... ) \
{ \
	TGfxDbgStrBuffer tMessage = GfxDbgStrFormat( __VA_ARGS__ ); \
	GfxDbgMsgHelper( "Critical error : ", tMessage.string, "???", __FILE__, __LINE__ ); \
	GfxDbgBreak(); \
}

#define GfxDbgBreak() if( IsDebuggerPresent() ) { __asm { _emit 0xF1 }; } else DebugBreak()

wchar_t *convertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}

void GfxDbgMessageBox(const char *pMessage)
{
	MessageBox(0, convertCharArrayToLPCWSTR(pMessage), 0, MB_OK | MB_ICONERROR);
}

int GfxDbgPrintf(const char *pFormat, ...)
{
	va_list args;
	va_start(args, pFormat);

	int count = _vscprintf(pFormat, args);
	char * buffer = (char *)_malloca(count + 1);
	vsnprintf_s(buffer, count + 1, _TRUNCATE, pFormat, args);
	OutputDebugString(convertCharArrayToLPCWSTR(buffer));

	CharToOem(convertCharArrayToLPCWSTR(buffer), buffer);
	fputs(buffer, stdout);

	_freea(buffer);
	return count;
	return 0;
}

void GfxDbgAssert( bool bExpectedTrue, const char * pMessage )
{
	if( !bExpectedTrue )
	{
		// if your application breaks here, go back one level in the callstack
		GfxDbgErrorInfo( !pMessage ? "assertion failure" : pMessage );
	}
}

#define GfxDbgAssert( expr ) GfxDbgAssertInfo( expr, "" )
#define GfxDbgAssertInfo( expr, ... ) \
for(bool value = (expr); !value; value = true) \
{ \
	TGfxDbgStrBuffer tMessage = GfxDbgStrFormat( __VA_ARGS__ ); \
	GfxDbgMsgHelper( "Assertion failed : ", tMessage.string, #expr, __FILE__, __LINE__ ); \
	GfxDbgBreak(); \
}