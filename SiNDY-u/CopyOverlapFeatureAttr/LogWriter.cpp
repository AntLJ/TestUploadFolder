#include "stdafx.h"
#include "LogWriter.h"
#include "Message.h"
#include <windows.h>
#include <winlib/versioninfo.h>
#include <TDC/useful_headers/str_util.h>
#include <ArcHelperEx/AheglobalsMisc.h>
#include <iomanip>
#include <codecvt>

using namespace boost::program_options;

namespace
{
	const TCHAR* log_header = _T("#FREESTYLELOG\nFLAG\tLAYER\tOID\tERRLEVEL\tERRCODE\tERRMSG\tLON\tLAT\tMESH\tSRCOID\tNOTE");   //!< �G���[���O�̃w�b�_
	const TCHAR* log_format = _T("%d\t%s\t%d\t%s\t%d\t%s\t%.15lf\t%.15lf\t%u\t%d\t%s");   //!< �G���[���O�̃t�H�[�}�b�g
}

// �R���X�g���N�^
LogWriter::LogWriter(void)
{
	m_startTime = time(nullptr);
}

// �f�X�g���N�^
LogWriter::~LogWriter(void)
{
	closeLogFiles();
}

// ���O�t�@�C���X�g���[�����m�ۂ���
bool LogWriter::openLogFiles(const CString& runLogFileName, const CString& errLogFileName, const Arguments& args)
{
	CString message;
	// ���s���O
	m_runLogStream.open(runLogFileName, std::ios::out);
	if( !m_runLogStream.is_open() )
	{
		message = uh::str_util::format( message::getMsg(message::iCannotOpen), runLogFileName );
		writeRunLog(message);
		return false;
	}

	// BOM�Ȃ�UTF-8�ŏo�͂���
	std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
	m_runLogStream.imbue(utf8_locale);
	startLogFile( args );

	// �G���[���O
	m_errLogStream.open(errLogFileName, std::ios::out);
	if( !m_errLogStream.is_open() )
	{
		message = uh::str_util::format( message::getMsg(message::iCannotOpen), errLogFileName );
		writeRunLog(message);
		return false;
	}

	// BOM�Ȃ�UTF-8�ŏo�͂���
	m_errLogStream.imbue(utf8_locale);
	writeErrLog(log_header, false);

	return true;
}

// ���s���O�t�@�C���Ƀ��O����������
void LogWriter::writeRunLog(LPCTSTR message, bool putTime, bool disp)
{
	uh::tstring logMessage;

	if(putTime)
		logMessage = (uh::tformat(_T("%s\t%s")) % message % getNowTime()).str();
	else
		logMessage = message;

	if( m_runLogStream.is_open() )
		m_runLogStream << uh::toWStr(logMessage) << std::endl;

	if( disp )
		std::cerr << uh::toStr(logMessage) << std::endl;
}

// �G���[���O�t�@�C���Ƀ��O����������
void LogWriter::writeErrLog(LPCTSTR message, bool disp )
{
	if( m_errLogStream.is_open() )
		m_errLogStream << uh::toWStr(message) << std::endl;

	if( disp )
		std::cerr << uh::toStr(message) << std::endl;
}

// �G���[���O�t�@�C���Ƀ��O����������
void LogWriter::writeErrLog(ErrItem cErrItem, bool disp )
{
	CString strErr;
	strErr.Format( log_format, cErrItem.nFlag, cErrItem.strLayer, cErrItem.nOID, cErrItem.strErrLevel, cErrItem.nErrCode, cErrItem.strErrMsg, 
		cErrItem.dLon, cErrItem.dLat, cErrItem.nMeshCode, cErrItem.nSrcOID, cErrItem.strNote );

	if( m_errLogStream.is_open() )
		m_errLogStream << uh::toWStr((LPCTSTR)strErr) << std::endl;

	if( disp )
		std::cerr << uh::toStr((LPCTSTR)strErr) << std::endl;
}

// ���O�t�@�C�����N���[�Y����
void LogWriter::closeLogFiles()
{
	// ���s���O
	if( m_runLogStream.is_open() )
	{
		endLogFile();
		m_runLogStream.close();
	}

	// �G���[���O
	if( m_errLogStream.is_open() )
	{
		m_errLogStream.close();
	}
}

// ���O�t�@�C���Ƀv���O�����J�n�Ƃ��ăc�[�����A�I�v�V�����������o�͂���
void LogWriter::startLogFile(const Arguments& args)
{
	writeFileVersion();
	time( &m_startTime );

	CString message = uh::str_util::format( message::getMsg(message::iStartTime) );
	writeRunLog( message );
}

// ���s���O�t�@�C���Ƀv���O�����I�������Ə������ʂ��o�͂���
void LogWriter::endLogFile()
{
	time_t endTime;
	time( &endTime );
	long lTime = long(difftime( endTime, m_startTime ));
	int lHour = lTime / 3600;
	int lMinute = (lTime - lHour * 3600 ) / 60;
	int lSecond = lTime % 60;

	CString message;
	message = uh::str_util::format( message::getMsg(message::iEndTime) );
	writeRunLog( message );

	message = uh::str_util::format( message::getMsg(message::iExecTime), lHour, lMinute, lSecond );
	writeRunLog( message, false, true );
}

// ���s���O�t�@�C���Ƀc�[�����ƃo�[�W�������o�͂���
void LogWriter::writeFileVersion()
{
	CVersion version;
	CString fileDescription = version.GetInternalName();
	CString fileVersion     = version.GetFileVersion();
	CString productVersion  = version.GetProductVersion();

	writeRunLog( _T("# ") + fileDescription + _T(".exe F:") + fileVersion + _T(" P:") + productVersion, false, false );
}

// ���ݎ������擾����
uh::tstring LogWriter::getNowTime() const
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	uh::tstring strNowTime = (uh::tformat(_T("%d/%02d/%02d %02d:%02d:%02d"))
		% st.wYear % st.wMonth % st.wDay % st.wHour % st.wMinute % st.wSecond).str();
	return strNowTime;
}

// �G���[���b�Z�[�W������擾
CString LogWriter::getErrCodeMsg(err_code::ECode errCode)
{
	using namespace err_code;
	switch(errCode)
	{
	case errOverBuffer:                 return message::getMsg(message::iErrOverBuffer);
	case errSrcNotExist:                return message::getMsg(message::iErrSrcNotExist);
	case errOverlaySrcNotExist:         return message::getMsg(message::iErrOverlaySrcNotExist);
	case errScaleSrcNotExist:           return message::getMsg(message::iErrScaleSrcNotExist);
	case errBothSrcNotExist:            return message::getMsg(message::iErrBothSrcNotExist);
	case errMulitSrcSkipMode:           return message::getMsg(message::iErrMulitSrcSkipMode);
	case errMulitSrcAttrNoMatch:        return message::getMsg(message::iErrMulitSrcAttrNoMatch);
	case errAttrNotInAttrList:          return message::getMsg(message::iErrAttrNotInAttrList);
	case errNotSameField:               return message::getMsg(message::iErrNotSameField);
	case errNotSameSpRef:               return message::getMsg(message::iErrNotSameSpRef);
	case errPutValueFailed:             return message::getMsg(message::iErrPutValueFailed);
	case errSuccessUpdate:               return message::getMsg(message::iErrSuccessUpdate);
	case errUpdateFailed:                return message::getMsg(message::iErrUpdateFailed);
	case errFailureOverlay:             return message::getMsg(message::iErrFailureOverlay);
	case errFailureScale:               return message::getMsg(message::iErrFailureScale);
	default:                            return message::getMsg(message::iErrUnkownError);
	}
}

// �G���[���x��������擾
CString LogWriter::getErrLevelMsg(err_level::ECode errLevel)
{
	using namespace err_level;
	switch(errLevel)
	{
	case levelError:                return message::getMsg(message::iLevelError);
	case levelWarning:              return message::getMsg(message::iLevelWarning);
	case levelInfo:                 return message::getMsg(message::iLevelInfo);
	default:                        return message::getMsg(message::iLevelInfo);
	}
}