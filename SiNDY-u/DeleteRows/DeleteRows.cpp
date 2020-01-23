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
#include "DeleteRows.h"
#include "WinLib/VersionInfo.h"
#include "sindy/workspace.h"
#include <ArcHelperEx/GlobalFunctions.h>

using namespace std;

/**
	* @brief	������
	* @param	cArgc	[in]	�����̐�
	* @param	cArgv	[in]	����
	* @return	bool
	*/
bool DeleteRows::Init()
{
	m_ofs.open( strLog );
	if( !m_ofs ){
		cerr << "#ERROR ���O�t�@�C���̃I�[�v���Ɏ��s" << endl;
		return false;
	}
	
	ifstream	fin;
	fin.open( strFile );
	if( !fin )
	{
		cerr << "#Error �t�@�C�����I�[�v���ł��܂���" << endl;
		return false;
	}

	int		i = 0;
	while(1)
	{
		char	acBuf[1024];
		i++;

		fin.getline( acBuf, 1024 );

		// �I�[�Ȃ�΃��[�v�𔲂���
		if( fin.eof() )
			break;

		if( !fin )
		{
			cerr << "�y " << i << " �s�ځz�ǂݍ��ݎ��s" << endl;
			fin.close();
			return false;
		}
		string	asBuf	= acBuf;
		if(asBuf.find_first_not_of("0123456789") != string::npos)
		{
			cerr << "�y " << i << " �s�ځz�s����ID( " << asBuf << " )" << endl;
			fin.close();
			return false;
		}
		long	aOID = atol( asBuf.c_str() );
		m_lOIDList.push_back( aOID );
	}
	fin.close();

	fnPrintHeader();
	return true;
}

/**
	* @brief	���s
	*/
bool DeleteRows::Execute()
{
	if( !Init() ){
		cerr << "#ERROR �������Ɏ��s" << endl;
	}

	// ����FGDB���[�N�X�y�[�X�I�[�v��
	m_ipWorkspace = sindy::create_workspace( strInput );

	if( !m_ipWorkspace ){
		cerr << "#ERROR DB�ڑ����s" << endl;
		return false;
	}
	m_ofs << "#DB:" << CT2CA(strInput) << endl;

	((IFeatureWorkspacePtr)m_ipWorkspace)->OpenTable( _bstr_t( strFcname ), &m_ipTable );
	if( !m_ipTable ){
		cerr << "#ERROR �t�B�[�`���N���X�I�[�v�����s" << endl;
		return false;
	}

	m_ofs << "#FC:" << CT2CA(strFcname) << endl;

	if( !fnEditStart() ){
		return false;
	}

	fnDelete();

	if( !fnEditStop() ){
		return false;
	}

	m_ofs << "#����I��" << endl;

	return true;
}

/**
	* @brief	���O�Ƀw�b�_�[���o��
	*/
void DeleteRows::fnPrintHeader()
{
	// �Œ蕔���̏o��
	m_ofs << "# FREESTYLELOG" << endl;

	CVersion cVer;
	m_ofs << "# " << CT2CA(cVer.GetOriginalFilename()) << " FILEVERSION " << CT2CA(cVer.GetFileVersion()) << " PRODUCTVERSION " << CT2CA(cVer.GetProductVersion()) << endl;

	m_ofs << "LAYER\tOBJECTID\tSTATUS" << endl;
}

/**
	* @brief	�ҏW�J�n
	* @return	bool
	*/
bool DeleteRows::fnEditStart()
{
	// �ҏW�J�n�iUndo Redo ������Ȃ����߁Afalse�j
	if( S_OK != m_ipWorkspace->StartEditing( VARIANT_FALSE ) || S_OK != m_ipWorkspace->StartEditOperation() ){
		m_ofs << "#ERROR �ҏW�J�n�Ɏ��s" << endl;
		return false;
	}
	return true;
}

/**
	* @brief	�ҏW�I��
	* @return	bool
	*/
bool DeleteRows::fnEditStop()
{
	// �ҏW�m��
	if( S_OK != m_ipWorkspace->StopEditOperation() ){
		m_ofs << "#ERROR �ҏW�ۑ��Ɏ��s" << endl;
		m_ipWorkspace->AbortEditOperation();
		return false;
	}
	
	// �ҏW�I���i�ۑ�����K�v�����邽�߁Atrue�j
	if( S_OK != m_ipWorkspace->StopEditing( VARIANT_TRUE ) ){
		m_ofs << "#ERROR �ҏW�I���Ɏ��s" << endl;
		return false;
	}
	return true;
}

/**
	* @brief	�폜����
	* @return	bool
	*/
void DeleteRows::fnDelete()
{
	for( auto& oID : m_lOIDList ){

		CString strWhere;
		strWhere.Format( _T("OBJECTID = %d"), oID );

		IQueryFilterPtr ipQuery( AheInitQueryFilter(NULL, NULL, strWhere) );

		_ICursorPtr ipCursor;
		m_ipTable->Search( ipQuery, VARIANT_FALSE, &ipCursor );

		if( !ipCursor ){
			m_ofs << CT2CA(strFcname) << "\t" << oID << "\t�������s" << endl;
			continue;
		}

		_IRowPtr ipRow;
		if( S_OK == ipCursor->NextRow( &ipRow ) && ipRow ){

			if( SUCCEEDED( ipRow->Delete() )){
				m_ofs << CT2CA(strFcname) << "\t" << oID << "\t�폜OK" << endl;
			}
			else{
				m_ofs << CT2CA(strFcname) << "\t" << oID << "\t�폜���s" << endl;
			}
		}
		else{
			m_ofs << CT2CA(strFcname) << "\t" << oID << "\tDB��ɑ��݂��Ȃ�" << endl;
		}
	}
}

