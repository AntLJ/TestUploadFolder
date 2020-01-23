#include "stdafx.h"
#include "CStdOutput.h"

using namespace std;

void CStdOutput::writeRoadInfoMsg(const CString& msg)
{
	CString outMsg;
	outMsg.Format(_T("%s %s"), GetNow(), msg);
	wcout << outMsg.GetString() << endl;
}

void CStdOutput::writeErrMsg(const CString& msg)
{
	writeConsole(msg + _T("\nˆÙíI—¹"));
}

void CStdOutput::writeConsole( const CString& msg )
{
	wcerr << msg.GetString() << endl;
}