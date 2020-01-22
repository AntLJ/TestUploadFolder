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

/**
 *	@file	cmn_error.h
 *	@brief	<b>エラー情報をハンドリングする</b>\n
 *	@author	Fusatoshi Abe
 *	$ID$
 */
#ifndef	_CMN_ERROR_H_
#define	_CMN_ERROR_H_

#include "cmn.h"

/**
 *	@brief	<b>エラーメッセージを設定する</b>\n
 *	@note	設定されたメッセージはget_error_message()またはGetErrorInfo()により取得できます。
 *	@param	lpFormatString [in] 書式
 *	@param	... [in] 書式にあわせた引数
 *	@retval	true 成功
 *	@retval false 失敗
 */
inline bool PUT_ERROR_INFO(LPCTSTR lpFormatString, ...)
{
	ICreateErrorInfoPtr ipCreateErrorInfo = 0;
	if (FAILED(CreateErrorInfo(&ipCreateErrorInfo)))
		return false;

	va_list arg_list;
	va_start(arg_list, lpFormatString);
	CString msg;
	msg.FormatV(lpFormatString, arg_list);
	va_end(arg_list);

	if (FAILED(ipCreateErrorInfo->SetDescription(CComBSTR(msg))))
		return false;
	if (FAILED(SetErrorInfo(0, (IErrorInfoPtr)ipCreateErrorInfo)))
		return false;

	return true;
}

/**
 *	@brief	<b>エラーメッセージの取り出し</b>\n
 *	@note	GetErrorInfo()により取得されるIErrorInfoのDescriptionの内容を返します。
 *			Descriptionが空の場合には、空の文字列(長さ0の文字列)を返します。
 *	@return	CString(エラー文字列)
 */
inline CString GET_ERROR_INFO()
{
	CString Message = _T("");

	IErrorInfoPtr ipErrorInfo = 0;
	if (FAILED(GetErrorInfo(0, &ipErrorInfo)))
		return Message;

	if (ipErrorInfo == 0)
		return Message;

	CComBSTR bstrMessage;
	if (FAILED(ipErrorInfo->GetDescription(&bstrMessage)))
		return Message;

	Message = CString(bstrMessage);

	return Message;
}
/**
 *	@brief	<b>エラーメッセージの取り出し</b>\n
 *	@note	GetLastError()により取得されるエラー文字列を返します。
 *			エラー文字列が空の場合には、空の文字列(長さ0の文字列)を返します。
 *	@return	CString(エラー文字列)
 */
inline CString GET_LAST_ERROR()
{
	CString Message = _T("");
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);
	Message = CString((LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
	return Message;
}

/**
 *	@brief	<b>ログ出力環境</b>\n
 *	@note	デフォルトは標準出力です。
 */
extern std::ostream* g_ostream;

/**
 *	@brief	<b>ログの出力先設定</b>\n
 *	@note	
 *	@param	stream [in] 出力先
 */
inline void SET_FILE(std::ostream& stream)
{
	g_ostream = &stream;
}

/**
 *	@brief	<b>ログの出力</b>\n
 *	@note	
 *	@param	lpFormatString [in] フォーマット
 *	@param	可変
 */
inline void LOG(LPCTSTR lpFormatString, ...)
{
	CString msg;
	{
		va_list arg_list;
		va_start(arg_list, lpFormatString);
		msg.FormatV(lpFormatString, arg_list);
		va_end(arg_list);
	}
	USES_CONVERSION;
	*g_ostream << T2CA(msg);
}

#if defined UNICODE || defined _UNICODE
#define	__FUNC__(x) const LPCTSTR __func__=L#x
#else
#define	__FUNC__(x) const LPCTSTR __func__=#x
#endif

#define	_WIN_MSG	_T("この操作を正しく終了しました")

class Exception : public std::exception
{
	CString m_message;
public:
	Exception() : m_message(_T("")) { }
	Exception(LPCTSTR msg) : m_message(_T("")) { set_Message(msg); }
	virtual ~Exception() { }
	void set_Message(LPCTSTR msg) { m_message = msg ? msg : _T(""); }
	const char* what() const { USES_CONVERSION; return T2CA(m_message); }
};

static Exception g_exception;

#define	ERRCHECK(expr)	{ \
	if (! expr) \
	{ \
		CString msg = GET_ERROR_INFO(); \
		if (! msg.GetLength()) \
		{ \
			msg = GET_LAST_ERROR(); \
			if (! msg.GetLength() || ! msg.Left(lstrlen(_WIN_MSG)).CompareNoCase(_WIN_MSG)) \
				msg = _T("Error occurred."); \
		} \
		CString file(__FILE__); \
		file = file.Right(file.GetLength() - file.ReverseFind(_T('\\')) - 1); \
		LOG(_T("ERROR : %s [%s (%d) %s]\n"), msg, file, __LINE__, __func__); \
		g_exception.set_Message(msg); \
		throw &g_exception; \
	} \
	else { \
	}\
}

#define	ERRHANDLE(msg, ret)	{ \
	if (! msg) \
	{ \
		CString temp = GET_ERROR_INFO(); \
		if (! temp.GetLength()) \
		{ \
			temp = GET_LAST_ERROR(); \
			if (! temp.GetLength() || ! temp.Left(lstrlen(_WIN_MSG)).CompareNoCase(_WIN_MSG)) \
				temp = _T("Error occurred."); \
		} \
		CString file(__FILE__); \
		file = file.Right(file.GetLength() - file.ReverseFind(_T('\\')) - 1); \
		LOG(_T("ERROR : %s [%s (%d) %s]\n"), temp, file, __LINE__, __func__); \
	} \
	else { \
		CString file(__FILE__); \
		file = file.Right(file.GetLength() - file.ReverseFind(_T('\\')) - 1); \
		LOG(_T("ERROR : %s [%s (%d) %s]\n"), msg, file, __LINE__, __func__); \
	} \
	return ret; \
}

inline void MESSAGE(std::ostream& stream, LPCTSTR lpFormatString, ...)
{
	va_list arg_list;
	va_start(arg_list, lpFormatString);
	CString msg;
	msg.FormatV(lpFormatString, arg_list);
	va_end(arg_list);

	USES_CONVERSION;
	stream << T2CA(msg);
}

#endif	//_CMN_ERROR_H_
