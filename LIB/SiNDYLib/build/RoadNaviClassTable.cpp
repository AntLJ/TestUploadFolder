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

/**
 * @file RoadNaviClassTable.cpp
 * @brief <b>CRoadNaviClassTable�N���X�����t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "RoadNaviClassTable.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace roadnaviclass {

// �e�[�u����S�ăL���b�V������
long CRoadNaviClassTable::CreateTableCache( bool bDetatch/* = false*/ )
{
//	TRACEMESSAGE( _T("CRoadNaviClassTable::CreateTableCache() : Started.\n") );

	CRoadNaviClass cRoadNaviClass;
	_ICursorPtr ipCursor = Search( NULL, bDetatch );

	// ���L�I�u�W�F�N�g�쐬
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	// �L���b�V���O�ɋ��L�I�u�W�F�N�g���V�K�ɍ쐬����Ă��܂����Ƃ�h�����߁A
	// �o�H��ʖ��̃C���f�b�N�X��O�����Ď擾
	long lNaviClassIndex = spFields->FindField( schema::roadnaviclass::kName );

	LPTSTR lpszNaviClass = new TCHAR[64];
	while( SUCCEEDED( ipCursor->NextRow( cRoadNaviClass.Release() ) ) )
	{
		if( cRoadNaviClass == NULL ) break;

		// ���H�����N�̌o�H��ʖ��̂��L�[�ɂ��Ċi�[
		cRoadNaviClass.GetStringValue( lNaviClassIndex, 64, lpszNaviClass );
		m_mapCache[ lpszNaviClass ] = cRoadNaviClass;
		m_mapCache[ lpszNaviClass ].CreateCache( bDetatch, spFields, spNameString );
	}
	delete [] lpszNaviClass;
	m_bCached = true;

	// �؂藣��
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CRoadNaviClassTable::CreateTableCache() : Finished.\n") );

	return m_mapCache.size();
}

// �L���b�V�����N���A����
void CRoadNaviClassTable::ClearTableCache()
{
	m_mapCache.clear();
	m_bCached = false;
}

} // roadnaviclass

} // sindy
