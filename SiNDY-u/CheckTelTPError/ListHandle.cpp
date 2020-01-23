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

// CLAccess.cpp: CLAccess �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ListHandle.h"
#include <boost/tokenizer.hpp>

// �f���~�^�J�E���g
inline int countDeli( const char* lpszBuff )
{
	int nCount = 0;
	if( lpszBuff )
	{
		int nLen = strlen( lpszBuff );
		for( int i = 0; i < nLen; ++i )
		{
			if( lpszBuff[i] == '\t' )
				++nCount;
		}
	}
	
	return nCount;
}

// �f�[�^���[�h
bool CListHandle::loadData( const _TCHAR* lpcszFile )
{
	std::ifstream ifs( static_cast<CT2CW>(lpcszFile) );
	if( !ifs )
		return false;

	char szSep[] = "\t\n";	// �Z�p���[�^
	boost::char_separator<char> sep( szSep, NULL, boost::keep_empty_tokens );
	for( int i = 1; !ifs.eof(); ++i )
	{
		std::string strBuff;
		std::getline( ifs, strBuff );
		if( strBuff.empty() || strBuff.find('#') == 0 )
			continue;

		// ���X�g�t�H�[�}�b�g��[�V�[�P���X�R�[�h��ObjectID]�̂�(�^�u2��)��[���L�{���b�V��XY��AddXY�t](�^�u6��)��2�p�^�[��
		int nCount = countDeli( strBuff.c_str() );
		if( 2 == nCount || nCount >= 5 )
		{
			boost::tokenizer< boost::char_separator<char> > tok( strBuff, sep );
			boost::tokenizer< boost::char_separator<char> >::iterator itr = tok.begin();

			CLRec	cRec;
			cRec.m_strClaimNo	= itr->c_str();	++itr;		// �N���[��No
			cRec.m_strObjectID	= itr->c_str(); ++itr;		// ObjectID
			cRec.m_strSequence	= itr->c_str();	++itr;		// �V�[�P���X

			if( nCount >= 5 )
			{
				cRec.m_cMXY.m_nMesh	= atoi(itr->c_str());	++itr;	// ���b�V���R�[�h
				cRec.m_cMXY.m_nX	= atoi(itr->c_str());	++itr;	// ���b�V��X
				cRec.m_cMXY.m_nY	= atoi(itr->c_str());	++itr;	// ���b�V��Y
				
				if( nCount >= 6 )
				{
					cRec.m_lAddXY	= atol(itr->c_str());	++itr;	// ���W�t�^��� 
				}
			}
			m_listRecord.push_back( cRec );
		}
	}
	return true;
}
