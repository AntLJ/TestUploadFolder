#include "stdafx.h"
#include "StdOutput.h"

using namespace std;

void StdOutput::writeRoadInfoMsg(const CString& msg)
{
	CString outMsg;
	outMsg.Format(_T("%s %s..."), GetNow(), msg);
	wcout << outMsg.GetString();
}

void StdOutput::writeErrMsg(const CString& msg)
{
	writeConsole(msg + _T("\nˆÙíI—¹"));
}

void StdOutput::writeConsole(const CString& msg)
{
	wcerr << msg.GetString() << endl;
}