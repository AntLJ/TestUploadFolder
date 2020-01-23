#include "stdafx.h"
#include "ctime.h"
#include <boost/format.hpp>

std::string GetTimeText(bool bUtc)
{
	SYSTEMTIME st;
	if (bUtc) GetSystemTime(&st);
	else GetLocalTime(&st);
	return boost::str(boost::format("%04d/%02d/%02d %02d:%02d:%02d")
		% st.wYear % st.wMonth % st.wDay % st.wHour % st.wMinute % st.wSecond);
}