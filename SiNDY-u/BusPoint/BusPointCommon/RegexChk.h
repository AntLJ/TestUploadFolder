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
#include "stdafx.h"
#include <boost/regex.hpp>

/**
 * @struct	dummy_predicate
 * @brief	regex_grep�Ŏg�p���邽�߂̃_�~�[
 */
struct dummy_predicate {
	bool operator ()(const boost::wsmatch& sm){
		return true;
	}
};

/**
 * @brief	���K�\����OK�̕�����ƂȂ��Ă��邩�m�F
 * @param	str   [in] ������	
 * @param	wreg   [in] �m�F�p���K�\��
 * @retval	true  OK
 * @retval	false NG
 */
inline bool IsOKRegEx( const CString& str, const boost::wregex& wreg )
{
	boost::wsmatch	wsm;

	std::wstring strTmp = str;

	if(	boost::regex_search( strTmp, wsm, wreg ) ){
		return true;
	}
	else{
		return false;
	}
}

/**
 * @brief	���K�\���Ŏw�肵�����̂̌��J�E���g
 * @param	str  [in] ������
 * @param	wreg  [in] �m�F�p���K�\��
 * @retval	long ���o��
 */
inline long GetRegExCnt( const CString& str, const boost::wregex& wreg )
{
	std::wstring strTmp = str;

	return boost::regex_grep( dummy_predicate(), strTmp, wreg );
}
