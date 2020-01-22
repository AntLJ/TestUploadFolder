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

#include "StdAfx.h"
#include "TmpCodeCreator.h"

// ���R�[�h�擾
std::string CTmpCodeCreator::GetTmpCode( const std::string& strAddrCode, const std::string& strPntName )
{
	TMPCODEMAP::iterator itrTmp = m_TmpCodeMap.find( strAddrCode );
	if( itrTmp != m_TmpCodeMap.end() )
	{
		STRINGMAP::iterator itrCode = itrTmp->second.find( strPntName );
		if( itrCode != itrTmp->second.end() )
			return itrCode->second;
	}

	// ������������܂��R�[�h���蓖�Ă���Ă��Ȃ�
	return insertNewTmpCode( strAddrCode, strPntName );
}

//////////////////////////////////////
// Private�֐�
//////////////////////////////////////

// �V�R�[�h�\��
std::string CTmpCodeCreator::insertNewTmpCode( const std::string& strAddrCode, const std::string& strPntName )
{
	TMPCODEMAP::iterator itrTmp = m_TmpCodeMap.find( strAddrCode );
	if( itrTmp == m_TmpCodeMap.end() )
	{
		STRINGMAP stmap;
		m_TmpCodeMap.insert( std::pair<std::string, STRINGMAP>(strAddrCode, stmap) );

		itrTmp = m_TmpCodeMap.find( strAddrCode );	// ���o�^�������̂��ēx�擾
		if( itrTmp == m_TmpCodeMap.end() )
			return "";
	}

	// ��ԑ傫���R�[�h���擾
	char szMaxPrefix = 'A';
	int iMaxCodeNum = -1;
	for( STRINGMAP::iterator itrCode = itrTmp->second.begin(); itrCode != itrTmp->second.end(); ++itrCode )
	{
		char szPrefix = itrCode->second[0];
		int iCodeNum = atoi( itrCode->second.substr(1).c_str() );
		if( szMaxPrefix < szPrefix || (szMaxPrefix == szPrefix && iMaxCodeNum < iCodeNum) )
		{
			szMaxPrefix = szPrefix;
			iMaxCodeNum = iCodeNum;
		}
	}

	char szNewCode[6] = "";
	sprintf_s( szNewCode, sizeof(szNewCode), "%c%04d", 
			   (iMaxCodeNum == 9999)? ++szMaxPrefix : szMaxPrefix,
			   (iMaxCodeNum == 9999)? 0 : ++iMaxCodeNum );

	itrTmp->second.insert( std::pair<std::string, std::string>(strPntName, szNewCode) );

	return std::string(szNewCode);
}
