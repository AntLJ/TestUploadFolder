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
 * @file AnnotationClassTable.cpp
 * @brief<b>CAnnotationClassTable�N���X�����t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "AnnotationClassTable.h"
#include "GlobalFunctions.h"
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace annotation_class {
using namespace errorcode;

double g_dGetRowByAnnoCodeTime = 0.0;	//!< GetRowByAnnoCode�ɂ����鑍����
// ���L��ʂŌ������s��
const CAnnotationClass* CAnnotationClassTable::GetRowByAnnoCode( sindyTableType::ECode emTableType, schema::annotation::annotation_code::ECode emCode ) const
{
	double dStart = GetTickCount();

	const CAnnotationClass* pAnnoClass = NULL;	// �Ԃ�l

	// �L���b�V���ɑ��݂��邩�ǂ����`�F�b�N
	if( m_bCached )
	{
		std::list<CAnnotationClass>::const_iterator it = m_listCache.end();
		for( it = m_listCache.begin(); it != m_listCache.end(); ++it )
		{
			if( it->GetClassCode() == emCode )
			{
				bool bFind = false;
				switch( emTableType )
				{
					case sindyTableType::city_annotation:	bFind = it->GetCityFlag();   break;
					case sindyTableType::d1_annotation:		bFind = true;   break;
					case sindyTableType::base_annotation:	bFind = it->GetBaseFlag();   break;
					case sindyTableType::middle_annotation: bFind = it->GetMiddleFlag(); break;
					case sindyTableType::top_annotation:	bFind = it->GetTopFlag();    break;
					default: 
						ERRORLOG(sindyErr_UnknownTableType);
						SASSERT( false );
						break;
				}
				if( bFind )
				{
					pAnnoClass = &(*it);
					break;
				}
			}
		}	
	}
	// �L���b�V���ɖ�����Ό���
	else {
		_ASSERT( false );	// ������
//		Search(
	}

	g_dGetRowByAnnoCodeTime += (double)GetTickCount()-dStart; // �����Ԃ��v�Z

	return pAnnoClass;
}

// �e�[�u����S�ăL���b�V������
long CAnnotationClassTable::CreateTableCache( bool bDetatch/* = false*/ )
{
//	TRACEMESSAGE( _T("CAnnotationClassTable::CreateTableCache() : Started.\n") );

	CAnnotationClass cAnnoClass;
	_ICursorPtr ipCursor = Search( NULL, bDetatch );

	// ���L�I�u�W�F�N�g�쐬
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	while( SUCCEEDED( ipCursor->NextRow( cAnnoClass.Release() ) ) )
	{
		if( cAnnoClass == NULL ) break;

		m_listCache.push_front( cAnnoClass );
		m_listCache.begin()->CreateCache( bDetatch, spFields, spNameString );
	}

	m_bCached = true;

	// �؂藣��
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CAnnotationClassTable::CreateTableCache() : Finished.\n") );

	return m_listCache.size();
}

// �L���b�V�����N���A����
void CAnnotationClassTable::ClearTableCache()
{
	m_listCache.clear();
	m_bCached = false;
}

} // annotation_class

} // sindy
