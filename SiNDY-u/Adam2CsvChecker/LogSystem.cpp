/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/


#include "StlAux.h"
#include "LogSystem.h"

#include <stdarg.h>

#include <string>
#include <locale>
#include <iostream>
#include <sstream>
using namespace std;

#if !defined(FOREGROUND_BLUE)
#  define FOREGROUND_BLUE      0x0001
#  define FOREGROUND_GREEN     0x0002
#  define FOREGROUND_RED       0x0004
#  define FOREGROUND_INTENSITY 0x0008
#  define BACKGROUND_BLUE      0x0010
#  define BACKGROUND_GREEN     0x0020
#  define BACKGROUND_RED       0x0040
#  define BACKGROUND_INTENSITY 0x0080
#endif

enum CONSOLE_COLOR {
	FORE_EMPHASIS   = FOREGROUND_INTENSITY,
	FORE_RED        = FOREGROUND_RED,
	FORE_GREEN      = FOREGROUND_GREEN,
	FORE_BLUE       = FOREGROUND_BLUE,
	FORE_CYAN       = FOREGROUND_GREEN | FOREGROUND_BLUE,
	FORE_MAGENTA    = FOREGROUND_BLUE  | FOREGROUND_RED,
	FORE_BLACK      = 0,
	FORE_YELLOW     = FOREGROUND_RED   | FOREGROUND_GREEN,
	FORE_WHITE      = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE,
	BACK_EMPHASIS   = BACKGROUND_INTENSITY,
	BACK_RED        = BACKGROUND_RED,
	BACK_GREEN      = BACKGROUND_GREEN,
	BACK_BLUE       = BACKGROUND_BLUE,
	BACK_CYAN       = BACKGROUND_GREEN | BACKGROUND_BLUE,
	BACK_MAGENTA    = BACKGROUND_BLUE  | BACKGROUND_RED,
	BACK_YELLOW     = BACKGROUND_RED   | BACKGROUND_GREEN,
	BACK_WHITE      = BACKGROUND_RED   | BACKGROUND_GREEN | BACKGROUND_BLUE,
	BACK_BLACK      = 0,

	FORE_E_RED      = FOREGROUND_INTENSITY | FOREGROUND_RED,
	FORE_E_GREEN    = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
	FORE_E_BLUE     = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
	FORE_E_CYAN     = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
	FORE_E_MAGENTA  = FOREGROUND_INTENSITY | FOREGROUND_BLUE  | FOREGROUND_RED,
	FORE_E_YELLOW   = FOREGROUND_INTENSITY | FOREGROUND_RED   | FOREGROUND_GREEN,
	FORE_E_WHITE    = FOREGROUND_INTENSITY | FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE,
	BACK_E_RED      = BACKGROUND_INTENSITY | BACKGROUND_RED,
	BACK_E_GREEN    = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
	BACK_E_BLUE     = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
	BACK_E_CYAN     = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
	BACK_E_MAGENTA  = BACKGROUND_INTENSITY | BACKGROUND_BLUE  | BACKGROUND_RED,
	BACK_E_YELLOW   = BACKGROUND_INTENSITY | BACKGROUND_RED   | BACKGROUND_GREEN,
	BACK_E_WHITE    = BACKGROUND_INTENSITY | BACKGROUND_RED   | BACKGROUND_GREEN | BACKGROUND_BLUE,
};


#pragma warning(disable: 4996)

#define _VA_UPDATE(flg)                 \
  string msg;                           \
  va_list ap;                           \
  va_start(ap, format);                 \
  msg = vform(format, ap);              \
  va_end(ap);                           \
  bool ret = update(flg, msg.c_str());  \
  return;

//  printf ("[%d]abc\n", int(flg)); \
//  printf ("%s\n", msg.c_str()); \


void LogSystem::f(const char* format, ...) { _VA_UPDATE(eLOG_FATAL);        }
void LogSystem::e(const char* format, ...) { _VA_UPDATE(eLOG_ERROR);        }
void LogSystem::w(const char* format, ...) { _VA_UPDATE(eLOG_WARNING);      }
void LogSystem::n(const char* format, ...) { _VA_UPDATE(eLOG_NOTICE);       }
void LogSystem::i(const char* format, ...) { _VA_UPDATE(eLOG_INFO);         }
#if defined(_DEBUG)
void LogSystem::d(const char* format, ...) { _VA_UPDATE(eLOG_DEVELOP);      }
void LogSystem::t(const char* format, ...) { _VA_UPDATE(eLOG_TRACE);        }
#endif

bool LogSystem::update(const int flag, const char* msg)
{
#if defined(_DEBUG)
	if (flag < 0 || eLOG_DEVELOP < flag) { return false; }
	if (! msg) { return false; }
#endif

	static const char* kATTR[] = {
		"致命",     // FATAL
		"異常",     // ERROR
		"警告",     // WARNING
		"注意",     // NOTICE
		"情報",     // INFORMATION
		"開発",     // DEVELOP
		"追跡",     // TRACE

		NULL
	};
	static const unsigned int kCOLOR[] = {
		FORE_E_RED    | BACK_BLACK,     // FATAL
		FORE_E_RED    | BACK_BLACK,     // ERROR
		FORE_E_YELLOW | BACK_BLACK,     // WARNING
		FORE_E_GREEN  | BACK_BLACK,     // NOTICE
		FORE_E_WHITE  | BACK_BLACK,     // INFORMATION
		FORE_CYAN     | BACK_BLACK,     // DEVELOP
		FORE_MAGENTA  | BACK_BLACK,     // TRACE

		FORE_E_WHITE,
	};

	{
		std::lock_guard<std::mutex> lock(m_cMutex);
		if (m_pStream)
		{
#if defined(_MSC_VER)
		    string str = form("%s\t%s\t%s", getnow().c_str(), kATTR[flag], msg);
#else
		    //ostringstream oss;
		    //oss << getnow() << "	" << kATTR[flag] << "	" << msg;
		    //string str = oss.str();
		    string str = form("%s\t%s\t%s", getnow().c_str(), kATTR[flag], msg);
#endif

			fprintf(m_pStream, "%s\n", str.c_str());
#if defined(_MSC_VER)
			_fflush_nolock(m_pStream);
#else
            fflush_unlocked(m_pStream);
#endif
		}
	}

#if !defined(_DISABLE_CONSOLE_OUTPUT_MESSAGE_)
	chg(kCOLOR[flag]);
	if (m_pConsole) { *m_pConsole << getnow().c_str() << " " << msg << endl; }
#endif // _DISABLE_CONSOLE_OUTPUT_MESSAGE_

#if defined(_MSC_VER) && defined(_DEBUG)
	OutputDebugStringA((getnow() + " " + msg + "\r\n").c_str());
#endif
	return true;
}

void LogSystem::chg(const unsigned int color)
{
#if defined(_MSC_VER)
	if (m_hConsole) { ::SetConsoleTextAttribute(m_hConsole, WORD(color)); }
#else
	{ printf("\033[0m"); }
	if (color & FORE_EMPHASIS) { printf("\033[1m");  }
//	if (color & FORE_BLACK   ) { printf("\033[30m"); }
	switch (color & FORE_WHITE) {
	case FORE_RED:      printf("\033[31m"); break;
	case FORE_GREEN:    printf("\033[32m"); break;
	case FORE_YELLOW:   printf("\033[33m"); break;
	case FORE_BLUE:     printf("\033[34m"); break;
	case FORE_MAGENTA:  printf("\033[35m"); break;
	case FORE_CYAN:     printf("\033[36m"); break;
	case FORE_WHITE:    printf("\033[37m"); break;
	}
/*/
	if (color & FORE_WHITE   ) { printf("\033[37m"); }
	if (color & FORE_RED     ) { printf("\033[31m"); }
	if (color & FORE_GREEN   ) { printf("\033[32m"); }
	if (color & FORE_YELLOW  ) { printf("\033[33m"); }
	if (color & FORE_BLUE    ) { printf("\033[34m"); }
	if (color & FORE_MAGENTA ) { printf("\033[35m"); }
	if (color & FORE_CYAN    ) { printf("\033[36m"); }
//*/
#endif
}

LogSystem::LogSystem(void)
{
	m_pStream = NULL;

#if !defined(_DISABLE_CONSOLE_OUTPUT_MESSAGE_)
	// ↓cerrを変更する場合は 出力ハンドラも合わせて変更すること!!
	m_pConsole = &cerr;
#  if defined(_MSC_VER)
	m_hConsole = ::GetStdHandle(STD_ERROR_HANDLE);
#  endif
#endif

}

LogSystem::~LogSystem(void)
{
	close();
}

bool LogSystem::create(const char* fname)
{
	if (m_pStream) { return false; }

	string name;
	if (! fname)
	{
		name = "./Log";
#if defined(_DEBUG)
		name += getnow(-2) + "d.txt";
#else
		name += getnow(11) + ".txt";
#endif
		fname = name.c_str();
	}

	m_pStream = ::fopen(fname, "a");
	if (! m_pStream) { return false; }

	i("==================== START ====================");
#if defined(_DEBUG)
	t("DEBUG");
#endif
	return true;
}

void LogSystem::close()
{
	i("====================  END  ====================");
	if (m_pStream)
	{
		::fprintf(m_pStream, "\n");
		::fclose(m_pStream);
		m_pStream = NULL;
	}
#if !defined(_DISABLE_CONSOLE_OUTPUT_MESSAGE_)
	chg(FORE_E_WHITE);
#endif
}

#if !defined(_DISABLE_CONSOLE_OUTPUT_MESSAGE_)
ostream* LogSystem::setConsoleStream(ostream* os)
{
	ostream* old_os = m_pConsole;
	m_pConsole = os;
	return old_os;
}
#endif
