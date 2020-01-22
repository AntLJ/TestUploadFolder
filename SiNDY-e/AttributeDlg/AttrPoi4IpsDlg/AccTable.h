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
#include <map>
/**
 * @brief ACC�R�[�h��`�N���X
 */
class AccTable
{
public:
	AccTable(void){};
	~AccTable(void){};

	static StringMasterTbl initCodes()
	{
		std::map< long , std::pair<CString, CString> > codeMap;
		codeMap[1] = std::pair<CString, CString>(_T("1S"), _T("�s�s�n�}���x���@�ƌ`"));
		codeMap[2] = std::pair<CString, CString>(_T("3A"), _T("�s�s�n�}���x���@�~�n"));
		codeMap[3] = std::pair<CString, CString>(_T("2A"), _T("�s�s�n�}���x��"));
		codeMap[4] = std::pair<CString, CString>(_T("1A"), _T("���k���x���@�ƌ`�E�~�n�E���L"));
		codeMap[5] = std::pair<CString, CString>(_T("1B"), _T("���k���x���@�m�F��"));
		codeMap[6] = std::pair<CString, CString>(_T("1C"), _T("���k���x���@�m�F�s�\"));
		codeMap[7] = std::pair<CString, CString>(_T("UK"), _T("���Њ�w�x"));
		
		return codeMap;
	}

	static StringMasterTbl codes;
};
