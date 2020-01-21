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
#include "GouPoints.h"

#include <ArcHelperEx/GlobalFunctions.h>

namespace addr
{
using namespace sindy;
using namespace errorcode;

// �s���E�ȉ��ɂ��鍆�|�C���g���擾(�Z���R�[�h�w��Ver)
errorcode::sindyErrCode CGouPoints::SelectByAddrCode( const _TCHAR* lpcszAddrCode )
{
	clear();	// �����O�ɒ��g����x�N���A

	// �s�s�n�}�s���E����Y���Z���R�[�h�����I�u�W�F�N�g���擾����
	CString strWhere = GetQueryStr( lpcszAddrCode );
	if( !strWhere.IsEmpty() )
	{
		IQueryFilterPtr ipQuery(CLSID_QueryFilter);
		ipQuery->put_WhereClause( _bstr_t(strWhere) );
		return Select(ipQuery);
	}
	else
		return sindyErr_NoErr;
}

// �s���E�ȉ��ɂ��鍆�|�C���g���擾(Where��w��Ver)
errorcode::sindyErrCode CGouPoints::SelectByWhereClause( const _TCHAR* lpcszWhere )
{
	clear();	// �����O�ɒ��g����x�N���A

	if( lpcszWhere )
	{
		IQueryFilterPtr ipQuery(CLSID_QueryFilter);
		ipQuery->put_WhereClause( _bstr_t(lpcszWhere) );
		return Select(ipQuery);
	}
	else
		return sindyErr_NoErr;
}

///////////////////////////////////////////////
//	private
///////////////////////////////////////////////

// �N�G���쐬
CString CGouPoints::GetQueryStr( const _TCHAR* lpcszAddrCode )
{
	long lCodeLen = _tcslen( lpcszAddrCode );
	if( lCodeLen >= 5 )
	{
		CString strWhere;
		strWhere.Format( _T("%s = '%s'"), city_admin::kCityCode, CString(lpcszAddrCode).Left(5) );

		switch( lCodeLen )
		{
		case 5:
			break;
		case 8:
			strWhere.AppendFormat( _T(" and %s like '%s%%'"), city_admin::kAddrCode, CString(lpcszAddrCode).Mid(5, 3) );
			break;
		case 11:
			strWhere.AppendFormat( _T(" and %s = '%s'"), city_admin::kAddrCode, CString(lpcszAddrCode).Mid(5, 6) );
			break;
		case 16:
			strWhere.AppendFormat( _T(" and %s = '%s' and %s = %d"), 
								   city_admin::kAddrCode, CString(lpcszAddrCode).Mid(5, 6), city_admin::kGaikuFugo, _tstol(CString(lpcszAddrCode).Mid(11, 5)) );
			break;
		default:
			return _T("");
		}

		return strWhere;
	}
	else
		return _T("");
}

// �`��Ō�������
errorcode::sindyErrCode CGouPoints::Select( IQueryFilter* ipFilter, bool bRecycling, bool bCreateCache, CErrorObjects* pErrs, bool bInit )
{
	_ICursorPtr ipCursor( Search(ipFilter, bRecycling) );
	if( ! ipCursor )
		return sindyErr_ArgIsNull;

	_IRowPtr ipRow;
	while( S_OK == ipCursor->NextRow( &ipRow ) )
	{
		if( ! ipRow )
			break;

		long lOID = -1;
		ipRow->get_OID( &lOID );

		// �����t�B�[�`���ł�������o�^�͂��Ȃ�
		if( ! bInit )
		{
			const_rows_iterator it = find( sindyTableType::city_admin, lOID );
			if( it != end(sindyTableType::city_admin) )
				continue;
		}

		CSPRow cRow( new CCityAdmin(ipRow, sindyTableType::city_admin, false, GetFieldMap(), GetNameString() ) );

		// �L���b�V���쐬
		if( bRecycling || bCreateCache )
			cRow->CreateCache( bRecycling );

		// �R���e�i�Ɋi�[
		push_back( cRow );
	}

	// ���|�C���g�������I�Ɏw�肳��Ă��Ȃ���΁A�s���E�Ɠ����I�[�i�E���[�N�X�y�[�X�̍��|�C���g���擾
	if( m_cGouFC == NULL )
		m_cGouFC.SetObject( OpenSameOwnerTable(gou_point::kTableName) );

	// �s���E���̍��|�C���g���擾����
	for( const_rows_iterator itAdmin = begin(GetTableType()), itAdminEnd = end(GetTableType()); itAdmin != itAdminEnd; ++itAdmin )
	{
		// ���|�C���g���s���E�̕R�t���p�ɂ�����C���X�^���X�p��
		CSPCityAdmin cAdmin( new CCityAdmin(**itAdmin, sindyTableType::city_admin, false, GetFieldMap(), GetNameString()) );
		IFeatureCursorPtr ipFeatureCursor( m_cGouFC.Search(AheInitSpatialFilter(NULL, (*itAdmin)->GetShapeCopy()), false) );
		
		if( ipFeatureCursor )
		{
			IFeaturePtr ipFeature;
			while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
			{
				// �ƌ`�ƍ��|�C���g�������Ń����N����H�H�v����

				// �s���E�ƍ��|�C���g��R�t��
				CSPGouPoint pGou( new CGouPoint(ipFeature, sindyTableType::gou_point, false, m_cGouFC.GetFieldMap(), m_cGouFC.GetNameString()) );
				pGou->SetCityAdmin( cAdmin );	// ���|�C���g���猩���s���E�ւ̕R�t���������Ŏ��{(������Ԃ̂��̂ɂ���)
				(*itAdmin)->push_back( pGou );
			}
		}
	}

	return sindyErr_NoErr;
}

}	// addr
