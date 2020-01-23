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

/* ************************************************
	���e :
	���� :  
	�ǋL : 
 
	���� :  Fukuta.K
************************************************* */

/* ----- �C���N���[�h�t�@�C�� ----- */

#include <iostream>
#include <strstream>

#include "RNSString.h"

using namespace std;

/* ===== public ���\�b�h ===== */

// ----- ������ ----- // 

/// �R���X�g���N�^�iconst char*�^���������j
RNSString::RNSString(const char *iStr)
	: string(iStr)
{
}

/// �R���X�g���N�^�istring�^���������j
RNSString::RNSString(const std::string &iStr)
	: string(iStr)
{
}

/// �R���X�g���N�^�i�����w��j
RNSString::RNSString(int iValue)
{
	*this += iValue;
}

/// �R���X�g���N�^�i�����Ȃ������w��j
RNSString::RNSString(unsigned int iValue)
{
	*this += iValue;
}

/// ������ǉ�
RNSString& RNSString::append(const RNSString &iStr)
{
	appendPreEffect();
	string::append(iStr);
	appendEpiEffect();
	return *this;
}

/// �w�肵��������������ǉ�����
RNSString& RNSString::append(size_type n, char &iChar)
{
	appendPreEffect(); 
	string::append(n, iChar); 
	appendEpiEffect(); 
	return *this; 
}

// ----- �ҏW ----- // 

/// �R�����g�����istring�^�j
void RNSString::eraseComment(const std::string& iStr)
{
	int aCommentPos = find(iStr);

	if(aCommentPos != std::string::npos) {
		erase(aCommentPos);
	}
}

/// �R�����g�����iconst char*�^�j
void RNSString::eraseComment(const char* iStr)
{
	eraseComment(string(iStr));
}

/// �g���~���O
RNSString RNSString::trimedString(const char* iTrimChars)
{
	int aHeaderNumber = find_first_not_of(iTrimChars);
	int aFooterNumber = find_last_not_of(iTrimChars);

	return RNSString(substr(aHeaderNumber, aFooterNumber));
}

// ----- �擾 ----- // 

/// �����񕪒f�istring�^�j
int RNSString::divide(std::vector< RNSRef<RNSString> >* oDivideStrList, const std::string& iDelim, const char* iTrimChars) const
{
	int aBeforePos = 0;
	int aSeparatePos = 0;

	aSeparatePos = find(iDelim, aBeforePos);
	for( ; aSeparatePos != std::string::npos; aSeparatePos = find(iDelim, aBeforePos)) {
		if(iTrimChars) {
			RNSString	aString(substr((aBeforePos, aSeparatePos - aBeforePos)));
			oDivideStrList->push_back(new RNSString(aString.trimedString(iTrimChars)));
		}
		else {
			oDivideStrList->push_back(new RNSString(substr(aBeforePos, aSeparatePos - aBeforePos)));
		}
		aBeforePos = aSeparatePos + 1;
	}

	if(! aBeforePos)
		return 0;

	oDivideStrList->push_back(new RNSString(substr(aBeforePos)));

	return oDivideStrList->size();
}

// ----- �I�y���[�^ ----- // 

/// �������������
RNSString& RNSString::operator = (const std::string &iStr)
{
	*static_cast<string*>(this) = iStr; 
	return *this;
}

/// ������萔��������
RNSString& RNSString::operator = (const char *iStr)
{
	*static_cast<string*>(this) = iStr;
	return *this;
}

/// ���l�𕶎���Ƃ��Ēǉ�
RNSString& RNSString::operator += (int iValue)
{
	char		buffer[32];
	strstream	strm(buffer, 32, ios::out);

	strm << iValue << ends;
	append(buffer);

	return *this;
}

/// �����Ȃ����l�𕶎���Ƃ��Ēǉ�
RNSString& RNSString::operator += (unsigned int iValue)
{
	char		buffer[32];
	strstream	strm( buffer, 32, ios::out );

	strm << iValue << ends;
	append( buffer );

	return *this;
}

/// �������ǉ�����
RNSString& RNSString::operator += (const char* iStr)
{
	append(iStr); 
	return *this; 
}
 
/// �������ǉ�����
RNSString& RNSString::operator += (const RNSString &iStr)
{
	append(iStr); 
	return *this; 
}
 
/// ������ǉ�����
RNSString& RNSString::operator += (char iChar )
{
	append(1, iChar); 
	return *this; 
} 

/// �������ǉ�����
RNSString& RNSString::operator << (const RNSString &iStr)
{
	*this += iStr;
	return *this;
}

/// ������ǉ�����
RNSString& RNSString::operator << (char iChar)
{
	*this += iChar; 
	return *this;
}

/// �����𕶎��Ƃ��Ēǉ�����
RNSString& RNSString::operator << (int iValue)
{
	*this += iValue;
	return *this;
}

// ----- ���̑��̊֐� ----- // 

/**
 * @brief	���C�h��������A�Ή�����}���`�o�C�g������ɕϊ�����
 *
 * @param	cInput	[in]	�ϊ��Ώۃ��C�h������
 * @return	�Ή�����}���`�o�C�g������
 */
const std::string narrow(const std::wstring& cInput)
{
#if _MSC_VER > 1200
	return std::string(CW2A(cInput.c_str()));
#else
	char*	pBuffer = new char[cInput.size() * MB_CUR_MAX + 1];
	wcstombs(pBuffer, cInput.c_str(), cInput.size() * MB_CUR_MAX);
	std::string aResult = pBuffer;
	delete [] pBuffer;
	return aResult;
#endif
}

/**
 * @brief	�}���`�o�C�g��������A�Ή����郏�C�h������ɕϊ�����
 *
 * @param	cInput	[in]	�ϊ��Ώۃ}���`�o�C�g������
 * @return	�Ή����郏�C�h������
 */
const std::wstring widen(const std::string& cInput)
{
	wchar_t*	pBuffer = new wchar_t[cInput.size() + 1];
	mbstowcs(pBuffer, cInput.c_str(), cInput.size());
	std::wstring aResult = pBuffer;
	delete [] pBuffer;
	return aResult;
}
