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

// CityList.h: CityList �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CITYLIST_H__6EDD6F68_3FAE_4F51_80DE_B01C8CC00BBF__INCLUDED_)
#define AFX_CITYLIST_H__6EDD6F68_3FAE_4F51_80DE_B01C8CC00BBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <set>
#include <stdio.h>
#include <windows.h>
#include <string.h>

using namespace std;

/// �s�撬���R�[�h�̃R���e�i
struct CityListCont
{

	char	eCityCode[6];		///< �s�撬���R�[�h
	char	eAddrCode[7];		///< �Z���R�[�h�i11���R�[�h��6���j

	/// �R���X�g���N�^
	CityListCont() {
		memset(eCityCode, '\0', 6);
		memset(eAddrCode, '\0', 7);
	}

	/// �R���X�g���N�^�i�����l�w��j
	CityListCont(char *str) {
		strncpy(eCityCode, str, 5);
		eCityCode[5] = '\0';
		if( strlen( str ) > 5 )
		{
		strncpy(eAddrCode, &str[5], 6);
		eAddrCode[6] = '\0';
		}
	}

	/// ���
	bool operator=(const char *str) {
		strncpy(eCityCode, str, 5);
		eCityCode[5] = '\0';
		if( strlen( str ) > 5 )
		{
		strncpy(eAddrCode, &str[5], 6);
		eAddrCode[6] = '\0';
		}
	}

	/// ��r
	bool operator<(const CityListCont &clc) const {
		if(strcmp(eCityCode, clc.eCityCode) != 0)
		{
			if(strcmp(eCityCode, clc.eCityCode) < 0) 
				return true;
		}
		else
		{
			if(strcmp(eAddrCode, clc.eAddrCode) != 0)
			{
				if(strcmp(eAddrCode, clc.eAddrCode) < 0)
					return true;
			}
		}
		return false;
	}

};

typedef set<CityListCont, less<CityListCont> >				CityListSet;
typedef set<CityListCont, less<CityListCont> >::iterator	CityListSet_i;

class CityList : public CityListSet
{

protected:

	FILE	*mCityListFP;		///< �s�撬���R�[�h���X�g�̃t�@�C���|�C���^

public:

	/// �R���X�g���N�^
	CityList();

	/// �f�X�g���N�^
	~CityList();

	/// �s�撬���R�[�h���X�g�t�@�C���̃I�[�v��
	/**
	* @param cFileName [in] �t�@�C����
	*
	* @retval true ����
	* @retval false ���s
	*/
	bool	Open(char *cFileName);

	/// ���X�g�ւ̎s�撬���R�[�h�̒ǉ�
	/**
	* @param cMeshCode [in] �s�撬���R�[�h�i������^�j
	*
	* @retval true ����
	* @retval false ���s�i���͒l���͈͊O�j
	*/
	bool	Add(char *cCityCode);

	/// ���X�g�ւ̎s�撬���R�[�h�̒ǉ�
	/**
	* @param cCityCode [in] �s�撬���R�[�h�i�����^�j
	*
	* @retval true ����
	* @retval false ���s�i���͒l���͈͊O�j
	*/
	bool	Add(LONG cCityCode);

};

#endif // !defined(AFX_CITYLIST_H__6EDD6F68_3FAE_4F51_80DE_B01C8CC00BBF__INCLUDED_)
