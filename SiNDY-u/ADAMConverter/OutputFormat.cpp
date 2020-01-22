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

#include "stdafx.h"
#include "OutputFormat.h"

bool COutputFormat::init(const Arguments& args)
{
	bool bReturn = true;

	// �����R�[�h�ݒ�
	if(!setEncode(args.m_output_encode.c_str())){
		bReturn = false;
	}

	// �f���~�^�ݒ�
	if(!setDelimiter(args.m_output_delimiter.c_str())){
		bReturn = false;
	}

	// ���s�R�[�h�ݒ�
	if(!setNewline(args.m_output_newline.c_str())){
		bReturn = false;
	}

	// ���n�n�ݒ�
	if(!setGeoReference(args.m_output_georef.c_str())){
		bReturn = false;
	}
	
	// �`��^�C�v�ݒ�
	if(!setShapeType(args.m_output_shapetype.c_str())){
		bReturn = false;
	}

	// �|���S���̌����ݒ�
	if(!args.m_output_polygontype.empty()){
		if(!setPolygonType(args.m_output_polygontype.c_str())){
			bReturn = false;
		}
	}

	// �w�b�_�o�͗L��
	m_Header = args.m_output_header;

	return bReturn;
}


bool COutputFormat::setEncode(const CString& encode)
{
	if(0 == encode.CompareNoCase(OP_CH_ENCODE_SJIS)){
		m_Encode = ENCODE::SHIFT_JIS;
	} else if(0 == encode.CompareNoCase(OP_CH_ENCODE_UTF8)){
		m_Encode = ENCODE::UTF_8;
	} else {
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgOpEncode, encode);
		return false;
	}

	return true;
}
bool COutputFormat::setDelimiter(const CString& delimiter)
{
	if(0 == delimiter.CompareNoCase(OP_CH_DELIMITER_COMMA)){
		m_Delimiter = OP_SY_DELIMITER_COMMA;
	} else if(0 == delimiter.CompareNoCase(OP_CH_DELIMITER_TAB)){
		m_Delimiter = OP_SY_DELIMITER_TAB;
	} else {
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgOpDelimiter, delimiter);
		return false;
	}

	return true;
}
bool COutputFormat::setNewline(const CString& newline)
{
	if(0 == newline.CompareNoCase(OP_CH_NEWLINE_CR)){
		m_Newline = OP_SY_NEWLINE_CR;
	} else if(0 == newline.CompareNoCase(OP_CH_NEWLINE_LF)){
		m_Newline = OP_SY_NEWLINE_LF;
	} else if(0 == newline.CompareNoCase(OP_CH_NEWLINE_CRLF)){
		m_Newline = OP_SY_NEWLINE_CRLF;
	} else {
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgOpNewline, newline);
		return false;
	}

	return true;
}

bool COutputFormat::setGeoReference(const CString& georef)
{
	m_GeoRef = _ttol(georef);

	switch(m_GeoRef){
	case GEOREF::TOKYO97:
		m_GeoRefName = _T("���{���n�n(TOKYO)�A�ܓx�o�x");
		break;
	case GEOREF::JGD2000:
		m_GeoRefName = _T("���E���n�n(JGD2000)�A�ܓx�o�x");
		break;
	case GEOREF::JGD2011:
		m_GeoRefName = _T("���E���n�n(JGD2011)�A�ܓx�o�x");
		break;
	default:
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgOpGeoref, georef);
		return false;
		break;
	}

	return true;
}

bool COutputFormat::setShapeType(const CString& shaptype)
{
	if(0 == shaptype.CompareNoCase(OP_CH_SHAPETYPE_NORMAL)){
		m_ShapeType = SHAPETYPE::NORMAL;
	} else if(0 == shaptype.CompareNoCase(OP_CH_SHAPETYPE_WKT)){
		m_ShapeType = SHAPETYPE::WKT;
	} else {
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgOpShapeType, shaptype);
		return false;
	}

	return true;
}

bool COutputFormat::setPolygonType(const CString& polygontype)
{
	// TODO:�|���S���̌������l������ꍇ�͒ǋL��

	return true;
}
