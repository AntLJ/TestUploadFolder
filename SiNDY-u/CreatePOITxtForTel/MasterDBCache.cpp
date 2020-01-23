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
#include "MasterDBCache.h"
#include "sindy/schema/sindyk.h"

using namespace std;

CMasterDBCache::CMasterDBCache(void)
{
}

CMasterDBCache::~CMasterDBCache(void)
{
}

// �L���b�V���쐬
bool CMasterDBCache::Create( IWorkspacePtr ipWorkspace )
{
	ITablePtr ipMasterTbl;

	// CONTENTS_MASTER �e�[�u���擾
	((IFeatureWorkspacePtr)ipWorkspace)->OpenTable( _bstr_t(m_strContentsMaster), &ipMasterTbl );
	if( ! ipMasterTbl )
	{
		// �e�[�u��������肭�\���ł��Ȃ��̂ŁALPWSTR�ɃL���X�g
		wcerr << L"#Error �e�[�u���̎擾�Ɏ��s : " << (LPWSTR)(LPCWSTR)m_strContentsMaster << endl;
		return false;
	}

	// �t�B�[���h�C���f�b�N�X�擾
	long lMasterGrpCodeFID = -1, lMasterChainCodeFID = -1, lMasterContentsCodeFID = -1;

	ipMasterTbl->FindField( _bstr_t(sindy::schema::sindyk::contents_master::kGroupCode), &lMasterGrpCodeFID );
	ipMasterTbl->FindField( _bstr_t(sindy::schema::sindyk::contents_master::kChainCode), &lMasterChainCodeFID );
	ipMasterTbl->FindField( _bstr_t(sindy::schema::sindyk::contents_master::kContentsCode), &lMasterContentsCodeFID );
	if( lMasterGrpCodeFID== -1 || lMasterChainCodeFID == -1 || lMasterContentsCodeFID == -1)
	{
		wcerr << L"#Error �t�B�[���h�C���f�b�N�X�̎擾�Ɏ��s : " << endl;
		return false;
	}

	CString aStr;
	aStr.Format( _T("%s in (0, 1)"), sindy::schema::sindyk::contents_master::kContentsType );

	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	ipQuery->put_WhereClause( CComBSTR(aStr) );

	long lAllCount = 0;
	ipMasterTbl->RowCount( ipQuery, &lAllCount );

	_ICursorPtr ipMasterCursor;
	ipMasterTbl->Search( ipQuery, VARIANT_FALSE, &ipMasterCursor );
	long lCount = 1;
	_IRowPtr ipMasterRow;
	while( ipMasterCursor && ipMasterCursor->NextRow(&ipMasterRow) == S_OK )
	{
		long lContentsCode = 0;
		POIMASTERDBINFO info;

		CComVariant vaValue;

		// �R���e���c�R�[�h�擾
		ipMasterRow->get_Value( lMasterContentsCodeFID, &vaValue );
		lContentsCode = vaValue.lVal;

		// �f�[�^���ރR�[�h�擾
		ipMasterRow->get_Value( lMasterGrpCodeFID, &vaValue );
		info.first = vaValue.lVal;

		// �X�܃R�[�h�擾
		ipMasterRow->get_Value( lMasterChainCodeFID, &vaValue );
		info.second = vaValue.lVal;

		// �L�[�F�R���e���c�R�[�h �l�FGRPCODE��CHAINCODE�̃y�A�^
		m_mapInfo.insert( pair<long, POIMASTERDBINFO>(lContentsCode, info) );

		// POI�|�C���g���擾����ۂ�Where��Ƃ��āA�R���e���c�R�[�h���g�p���邽�߁A���߂Ă����iBug9215�Ή��j
		m_setContentsCode.insert( lContentsCode );

		wcerr << lCount++ << L" / " << lAllCount << L" ���擾����\r";
	}
	wcerr << endl;

	return true;
}
