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

// RoadDispClassTable.cpp: CRoadDispClassTable �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RoadDispClassTable.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace roaddispclass {

// �e�[�u����S�ăL���b�V������
long CRoadDispClassTable::CreateTableCache( bool bDetatch/* = false*/ )
{
//	TRACEMESSAGE( _T("CRoadDispClassTable::CreateTableCache() : Started.\n") );

	CRoadDispClass cRoadDispClass;
	_ICursorPtr ipCursor = Search( NULL, bDetatch );

	// ���L�I�u�W�F�N�g�쐬
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	// �L���b�V���O�ɋ��L�I�u�W�F�N�g���V�K�ɍ쐬����Ă��܂����Ƃ�h�����߁A
	// �\����ʖ��̃C���f�b�N�X��O�����Ď擾
	long lDispClassIndex = spFields->FindField( schema::roaddispclass::kName );

	LPTSTR lpszDispClass = new TCHAR[64];
	while( SUCCEEDED( ipCursor->NextRow( cRoadDispClass.Release() ) ) )
	{
		if( cRoadDispClass == NULL ) break;

		// ���H�����N�̕\����ʖ��̂��L�[�ɂ��Ċi�[
		cRoadDispClass.GetStringValue( lDispClassIndex, 64, lpszDispClass );
		m_mapCache[ lpszDispClass ] = cRoadDispClass;
		m_mapCache[ lpszDispClass ].CreateCache( bDetatch, spFields, spNameString );
	}
	delete [] lpszDispClass;
	m_bCached = true;

	// �؂藣��
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CRoadDispClassTable::CreateTableCache() : Finished.\n") );

	return m_mapCache.size();
}

// �L���b�V�����N���A����
void CRoadDispClassTable::ClearTableCache()
{
	m_mapCache.clear();
	m_bCached = false;
}

} // roaddispclass

} // sindy
