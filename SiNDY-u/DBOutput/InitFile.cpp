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
#include "InitFile.h"

// ������
bool CInitFile::init( LPCTSTR lpcszFile, const ITablePtr& ipTable, const bool& bShape, const bool& bSkip )
{
	std::ifstream ifs( static_cast<CT2CW>(lpcszFile) );
	if( !ifs )
	{
		std::cerr << "#ERROR �ݒ�t�@�C���̓ǂݍ��݂Ɏ��s : " << CT2CA(lpcszFile) << std::endl;
		return false;
	}

	std::string strBuff;
	while( !ifs.eof() )
	{
		std::getline( ifs, strBuff );

		// �R�����g�܂��́A���s�����̎�����
		if( strBuff.empty() || strBuff.find("#") == 0 )
			continue;

		// �t�B�[���h�h�c�擾�ł�����A�t�B�[���hID�}�b�v�i�[
		CString strField( strBuff.c_str() );
		strField.MakeUpper();	// �啶���ɂ���

		LONG lFID = -1;

		// �ʒu���n�t�B�[���h���`�F�b�N
		if( isShapeField(strField) )
		{
			if( bShape )
			{
				++m_nShapeFieldCount;
				// ���b�V���R�[�h�Ȃ烁�b�V�����x���擾����
				if( FN_CITYMESH == strField )		m_nMeshLevel = 64;
				else if( FN_BASEMESH == strField )	m_nMeshLevel = 2;
				else if( FN_MIDDLEMESH == strField )	m_nMeshLevel = 1;

				m_listField.push_back( CFieldInfo(strField, -1, false, true) ); 
			}
			else if( !bSkip )
			{
				std::cerr << "#ERROR �}�`�Ȃ��̃e�[�u�����w�肵���ꍇ�A�ݒ�t�@�C���Ɉʒu���֘A�͎w��ł��܂��� : " << CT2CA(strField) << std::endl;
				return false;
			}
		}
		else if( strField.Right(7) == _T("_DOMAIN") )	// �h���C���w�肵�Ă��邩�`�F�b�N(_DOMAIN)
		{
			if( SUCCEEDED(ipTable->FindField(CComBSTR(strField.Left(strField.GetLength()-7)), &lFID)) && -1 != lFID )
			{
				m_listField.push_back( CFieldInfo(strField, lFID, true, false) );
			}
			else if( !bSkip )
			{
				std::cerr << "#ERROR �ݒ�t�@�C���Ɏw�肵�����ڂ��e�[�u���ɑ��݂��܂��� : " << CT2CA(strField) << std::endl;
				return false;
			}
		}
		else if( strField.Left((int)_tcslen(FN_DUMMY)) == FN_DUMMY )	// Ver0.8 ���_�~�[�t�B�[���h�ǉ�
		{
			CString strDummyVal;
			if( strField.Find(':') != -1 )
				strDummyVal = strField.Mid( strField.Find(':') + 1 );
			m_listField.push_back( CFieldInfo(strField, lFID, false, false, true, strDummyVal) );
		}
		else
		{
			if( SUCCEEDED(ipTable->FindField(CComBSTR(strField), &lFID)) && -1 != lFID )
			{
				m_listField.push_back( CFieldInfo(strField, lFID, false, false) );
			}
			else if( !bSkip )
			{
				std::cerr << "#ERROR �ݒ�t�@�C���Ɏw�肵�����ڂ��e�[�u���ɑ��݂��܂��� : " << CT2CA(strField) << std::endl;
				return false;
			}
		}
	}

	return true;
}
