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

// �R�}���h���C���I�v�V�����p
#define OP_CH_ENCODE_SJIS	_T("SHIFT_JIS")
#define OP_CH_ENCODE_UTF8	_T("UTF-8")

#define OP_CH_DELIMITER_COMMA	_T("COMMA")
#define OP_CH_DELIMITER_TAB	_T("TAB")
#define OP_SY_DELIMITER_COMMA	_T(",")
#define OP_SY_DELIMITER_TAB	_T("\t")

#define OP_CH_NEWLINE_CR	_T("CR")
#define OP_CH_NEWLINE_LF	_T("LF")
#define OP_CH_NEWLINE_CRLF	_T("CRLF")
#define OP_SY_NEWLINE_CR	_T("\r")
#define OP_SY_NEWLINE_LF	_T("\n")
#define OP_SY_NEWLINE_CRLF	_T("\r\n")

#define OP_CH_SHAPETYPE_NORMAL	_T("NORMAL")
#define OP_CH_SHAPETYPE_WKT	_T("WKT")

#define OP_CH_POLYGONTYPE_NORMAL	_T("NORMAL")
#define OP_CH_POLYGONTYPE_CLOCKWISE	_T("CLOCKWISE")
#define OP_CH_POLYGONTYPE_ANTICLOCKWISE	_T("ANTICLOCKWISE")

// �ϊ��e�[�u���A�ϊ��t�B�[���h����
#define	CTF_COMMENT_CHAR	_T("#")
#define	CTF_SPLIT_CHAR	_T("\t")
#define	CTF_COMMA_CHAR	_T(",")
#define	CTF_SPACE_CHAR	_T(" ")
#define	CTF_PELIOD_CHAR	_T(".")
#define	CTF_GROUP_CHAR_S	_T("[")
#define	CTF_GROUP_CHAR_E	_T("]")

// �ϊ��e�[�u���p
#define	CT_TABCOUNT	2
#define	CT_SEARCHTYPE	_T("SEARCHTYPE")
#define	CT_SRCTABLE	_T("SRCTABLE")
#define	CT_DSTFILE	_T("DSTFILE")
#define	CT_SUBTABLES	_T("SUBTABLES")
#define	CT_JOINKEYS	_T("JOINKEYS")
#define	CT_OIDFIELD	_T("OIDFIELD")
#define	CT_PREFIX	_T("PREFIX")
#define	CT_PREFIX_DISTINCT	_T("DISTINCT")
#define	CT_POSTFIX	_T("POSTFIX")
#define	CT_POSTFIX_ORDERBY	_T("ORDER BY ")

// �����^�C�v������
#define	CT_SEARCHTYPE_NORMAL	_T("NORMAL")
#define	CT_SEARCHTYPE_QUERYDEF_LIMITED	_T("QUERYDEF_LIMITED")
#define	CT_SEARCHTYPE_QUERYDEF_FREE	_T("QUERYDEF_FREE")


// �ϊ��t�B�[���h�Ή��\�p
#define	CF_TABCOUNT	5
#define	CF_OPTIONTABCOUNT	3
#define	CF_SHAPE_FIELD	_T("SHAPE")

#define	CF_FIELDINFO_INTEGER	_T("INTEGER")
#define	CF_FIELDINFO_DOUBLE	_T("DOUBLE")
#define	CF_FIELDINFO_STRING	_T("STRING")
#define	CF_FIELDINFO_SHAPE	_T("SHAPE")

#define	CF_CANLNULL_NG	_T("0")
#define	CF_CANLNULL_OK	_T("1")

#define	CF_OPTION_ROUND	_T("ROUND")
#define	CF_OPTION_ROUND_NULL	_T("null")
#define	CF_OPTION_VALIDVALUE	_T("VALIDVALUE")
#define	CF_OPTION_VALIDRANGE	_T("VALIDRANGE")
#define	CF_OPTION_ERRVALUE	_T("ERRVALUE")
#define	CF_OPTION_ERRRANGE	_T("ERRRANGE")
#define	CF_OPTION_NULLVALUE	_T("NULLVALUE")
#define	CF_OPTION_FIXEDVALUE	_T("FIXEDVALUE")


// �`�󌅐��ݒ�
#define	COORD_NUM_X	3			//!< �����l
#define	COORD_DEC_X	_T("%.8f")	//!< �����_�ȉ�
#define	COORD_NUM_Y	3			//!< �����l
#define	COORD_DEC_Y	_T("%.8f")	//!< �����_�ȉ�
#define	COORD_NUM_Z	8			//!< �����l
#define	COORD_DEC_Z	_T("%.3f")	//!< �����_�ȉ�
#define	COORD_NUM_M	8			//!< �����l
#define	COORD_DEC_M	_T("%.3f")	//!< �����_�ȉ�

#define	NUMBER_MINUS	_T("-")

// �͈͋L��
#define	SY_GREATER_EQ	_T(">=")
#define	SY_LESS_EQ	_T("<=")
#define	SY_GREATER	_T(">")
#define	SY_LESS	_T("<")
#define	SY_EQ	_T("=")

// �͈͍ő�ŏ��l�iINT�̍ő�ŏ���ݒ�j
#define RANGE_MAX	_T("2147483647")
#define RANGE_MIN	_T("-2147483648")

// �����`�F�b�N�p
#define	NUMBER	_T("0123456789")

// UNIQ�p�o�b�t�@�T�C�Y
#define	UNIQ_BUFF_SIZE	100000

// ����������擾
inline CString getNowTime()
{
	CString strTime;
	
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	strTime.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	return strTime;
}

/**
 * @brief UTF16->UTF8�ϊ�
 * @param strText [in] UTF16������
 * @return UTF8������
 */
inline CStringA UTF16ToUTF8(const CStringW& strText)
{
	// ���C�h������ƃo�C�g������𑊌ݕϊ�����N���X������
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;

	// UTF-16��UTF-8�ϊ�
	std::string sUTF8 = conv.to_bytes(strText);

	// �o��
	return sUTF8.c_str();
}

/**
 * @brief ���l�`�F�b�N
 * @param strValue [in] �`�F�b�N�Ώە�����
 * @return true ���l�̂�
 * @return false ���l�ȊO
 */
inline bool checkNumber(const CString& strValue)
{
	CString strTmp = strValue;

	// �s���I�h�L�薳��
	int iPeriod = strValue.Find(CTF_PELIOD_CHAR);

	if(0 <= iPeriod){
		// ��������
		CString strLeft = strValue.Left(iPeriod);
		CString strRight = strValue.Mid(iPeriod + 1);
		if(0 != strLeft.CompareNoCase(CString(std::to_string(_ttol(strLeft)).c_str()))){
			return false;
		}
		if(strRight != strRight.SpanIncluding(NUMBER)){
			return false;
		}
	} else {
		// �����l�̂�
		if(0 != strValue.CompareNoCase(CString(std::to_string(_ttol(strValue)).c_str()))){
			return false;
		}
	}
	return true;
}

inline int findStr(const CString& strBase, const CString& strFind)
{
	CString tmpBase = strBase;
	CString tmpFind = strFind;
	tmpBase.MakeLower();
	tmpFind.MakeLower();

	return tmpBase.Find(tmpFind);
}
inline int reverseFindStr(const CString& strBase, const CString& strFind)
{
	CStringW tmpBase = strBase;
	CStringW tmpFind = strFind;
	tmpBase.MakeLower();
	tmpFind.MakeLower();

	return tmpBase.ReverseFind(tmpFind.GetString()[0]);
}