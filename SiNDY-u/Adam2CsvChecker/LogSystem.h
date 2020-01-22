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


#pragma once

#include <mutex>

//#define _DISABLE_CONSOLE_OUTPUT_MESSAGE_
#if !defined(_DISABLE_CONSOLE_OUTPUT_MESSAGE_)
#  include <ostream>
#endif // _DISABLE_CONSOLE_OUTPUT_MESSAGE_

#if defined(_MSC_VER)
#  define WIN32_LEAN_AND_MEAN
#  include <Windows.h>
#else
#  include <time.h>
#  include <sys/time.h>
#endif
#include <stdio.h>

enum enumLOG_TYPE {
	eLOG_UNKNOWN    = -1,
	eLOG_FATAL      = 0,
	eLOG_ERROR,
	eLOG_WARNING,
	eLOG_NOTICE,
	eLOG_INFO,
	eLOG_DEVELOP,
	eLOG_TRACE,
};

class LogSystem {
// Singleton ModelÇ…Çƒé¿ëï
private:
	~LogSystem(void);
	LogSystem (void);

	LogSystem(const LogSystem& );
	LogSystem& operator = (const LogSystem& );

public:
	static LogSystem* GetInstance(void) {
		static LogSystem kInstance;
		return &kInstance;
	}

/*/
	static LogSystem* CreateInstance(void) {
		return new LogSystem();
	}

	static void ReleaseInstance(LogSystem* ptr) {
		if (ptr && ptr != GetInstance()) { delete ptr; }
	}
//*/

public:
	// Method
	void f(const char* format, ...);  // fatal
	void e(const char* format, ...);  // error
	void w(const char* format, ...);  // warning
	void n(const char* format, ...);  // notice
	void i(const char* format, ...);  // info
#if defined(_DEBUG)
	void d(const char* format, ...);
	void t(const char* format, ...);
#else
	inline void d(const char*, ...) { }
	inline void t(const char*, ...) { }
#endif

#if !defined(_DISABLE_CONSOLE_OUTPUT_MESSAGE_)
	std::ostream* setConsoleStream(std::ostream* os);
#endif // _DISABLE_CONSOLE_OUTPUT_MESSAGE_

public:
	bool create(const char* fname);
	void close ();

private:
	bool update(const int/*enumLOG_TYPE*/ flag, const char* msg);
	void chg   (const unsigned int color);

private:
	FILE*               m_pStream;
	//CRITICAL_SECTION    m_Section;
	std::mutex          m_cMutex;

#if !defined(_DISABLE_CONSOLE_OUTPUT_MESSAGE_)
	std::ostream*       m_pConsole;
#  if defined(_MSC_VER)
	HANDLE              m_hConsole;
#endif
#endif // _DISABLE_CONSOLE_OUTPUT_MESSAGE_
};

#define LOG_CREATE()        LogSystem::GetInstance()->create(NULL)
#define LOG_CREATE2(fname)  LogSystem::GetInstance()->create(fname)
#define LOG_CLOSE()         LogSystem::GetInstance()->close()
#define LOG_F(format, ...)  LogSystem::GetInstance()->e(format, __VA_ARGS__)
#define LOG_E(format, ...)  LogSystem::GetInstance()->e(format, __VA_ARGS__)
#define LOG_W(format, ...)  LogSystem::GetInstance()->w(format, __VA_ARGS__)
#define LOG_N(format, ...)  LogSystem::GetInstance()->n(format, __VA_ARGS__)
#define LOG_I(format, ...)  LogSystem::GetInstance()->i(format, __VA_ARGS__)
#define LOG_D(format, ...)  LogSystem::GetInstance()->d(format, __VA_ARGS__)
#define LOG_T(format, ...)  LogSystem::GetInstance()->t(format, __VA_ARGS__)

class CFunctionLog {
public:
	CFunctionLog (const char* f_) : f(f_) { LOG_I("+%s()", f); }
	~CFunctionLog() { LOG_I("-%s()", f); }

	void msg(const char* str) const { LOG_I("*%s() %s", f, str); }

protected:
	const char* f;
};

#if defined(_MSC_VER)
class CFunctionTimeLog : public CFunctionLog {
public:
	CFunctionTimeLog(const char* name) : CFunctionLog(name)
	{
		::QueryPerformanceCounter(&liTime);
	}
	~CFunctionTimeLog()
	{
		LARGE_INTEGER liCount, liFreq;
		::QueryPerformanceCounter(&liCount);
		::QueryPerformanceFrequency(&liFreq);

		double d = double(liCount.QuadPart - liTime.QuadPart) / double(liFreq.QuadPart);
		LOG_I("*%s() èàóùéûä‘: %.5f [sec]", f, d);
	}

private:
	LARGE_INTEGER liTime;
};
#else
class CFunctionTimeLog : public CFunctionLog {
public:
	CFunctionTimeLog(const char* name) : CFunctionLog(name)
	{
		gettimeofday(&st_timeval1, NULL);
	}
	~CFunctionTimeLog()
	{
	    struct timeval st_timeval2;
		gettimeofday(&st_timeval2, NULL);

        long    sec = st_timeval2.tv_sec  - st_timeval1.tv_sec;
        double usec = (st_timeval2.tv_usec - st_timeval1.tv_usec) / 1.0e+6;
		LOG_I("*%s() èàóùéûä‘: %.5f [sec]", f, double(sec + usec));
	}

private:
	 struct timeval st_timeval1;
};
#endif

#define LOG_FUNCTION()     CFunctionLog  cfl___(__FUNCTION__)


inline void putLogArguments(int argc, char** argv)
{
	std::string str;
	for (int i=0 ; i<argc ; i++)
	{
		if (i) { str += " "; }
		str += argv[i];
	}
	LOG_I("Arguments [%s]", str.c_str());
}
