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
 * @file FeatureContainer.cpp
 * @brief <b>CFeatureContainer�N���X�����t�@�C��
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @vertion $Id$
 */
#include "stdafx.h"
#include "FeatureContainer.h"
#include "GlobalFunctions.h"
#include "ErrorObject.h"
#include "GeometryRule.h"
#include "Rule.h"
#include "GlobalRules.h"
#include "util.h"
#include "RowClassFactory.h"
#include "RuleModel.h"

namespace
{
#ifdef SINDY_FOR_ENGLISH
	LPCTSTR NOT_EXIST_RELATIONS  = _T("[%s.%d] : [%s.%d] which should be related does not exist");
#else
	LPCTSTR NOT_EXIST_RELATIONS  = _T("[%s.%d] : �֘A����͂���[%s.%d]�����݂��܂���");
#endif // SINDY_FOR_ENGLISH
} // namespace


#if _MSC_VER >= 1700
	#include "TDC/useful_headers/text_util.h"
#else

#include<TDC/useful_headers/str_util.h>
namespace uh{
	namespace text_util{
		std::list<long> ToLongList( const CString& str )
		{
			std::list<long> lists;
			std::vector<CString> strlists = uh::str_util::split( str, _T(",") );
			for( std::vector<CString>::const_iterator it = strlists.begin(); it != strlists.end(); ++it )
			{
				lists.push_back( _ttol( *it ) );
			}
			return lists;
		}
	} // text_util
}// uh
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//

namespace sindy {
using namespace errorcode;

// �`��Ō�������
sindyErrCode CFeatureContainer::Select( IGeometry* ipGeom, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* pErrs/* = NULL*/, bool bInit /*= true*/  )
{
	return Select( ipGeom, typeid(CFeature&), bRecycling, bCreateCache, pErrs, bInit );
}
sindyErrCode CFeatureContainer::Select( IGeometry* ipGeom, const type_info& info, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* pErrs/* = NULL*/, bool bInit /*= true*/  )
{
	RETURN_IF( ! p, sindyErr_ClassMemberMissing )

	// �W�I���g����NULL�ł������ł��邪�A�����ł͂�����
	// �G���[�Ƃ��邱�Ƃŗ\�����Ȃ��S���������������
	RETURN_IF( ! ipGeom, sindyErr_ArgIsNull )

	// ����
	return Select( AheInitSpatialFilter( NULL, ipGeom ), info, bRecycling, bCreateCache, pErrs, bInit );
}
sindyErrCode CFeatureContainer::Select( IQueryFilter* ipFilter, const type_info& info, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* pErrs/* = NULL*/, bool bInit /*= true*/  )
{
	RETURN_IF(! ipFilter,sindyErr_ArgIsNull)

	// ����
	return Add( Search( ipFilter, bRecycling ), info, GetTableType(), GetFieldMap(), GetNameString(), bRecycling, bCreateCache, pErrs, bInit );
}

// ����
_ICursorPtr _Select( const CTable& table, const CString& strIDs, const CString& field, bool bRecycling = false )
{
	_ICursorPtr cur;
	
	if( field.IsEmpty() )
	{
		// �t�B�[���h�w�肪�Ȃ��ꍇ��ID���ڎw��Ō���
		cur = table.GetRows( uh::text_util::ToLongList((LPCTSTR)strIDs), bRecycling );
	}
	else
	{
		// ����ꍇ�͕�����쐬
		cur = table.Search( AheInitQueryFilter( NULL, NULL, _T("%s in (%s)"), field, strIDs  ), bRecycling );
	}
	return cur;
}

sindyErrCode CFeatureContainer::Select( LPCTSTR lpcszTableName, const std::list<long> listIDs, const type_info& info, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* pErrs/* = NULL*/, bool bInit/* = true*/, LPCTSTR lpcszWhereFieldName/* = NULL*/, LPCTSTR lpcszParentTableName/* = NULL*/ )
{
	if( listIDs.empty() )
		return sindyErr_NoErr;

	if( ! lpcszTableName )
		return sindyErr_ArgIsNull;

	if( 0 == lstrlen(lpcszTableName) )
		return sindyErr_ArgLimitOver;

	sindyErrCode emErr = sindyErr_NoErr; // �߂�l

	// ������
	if( bInit )
		clear();

	CTable cClass( OpenSameOwnerTable( lpcszTableName ) );
	sindyTableType::ECode emType = cClass.GetTableType();
	LOGASSERTEERR_IF( cClass != NULL, sindyErr_TableNotFound )
	{
		// ���L�I�u�W�F�N�g�̎擾
		CSPTableNameString spNameString( cClass.GetNameString() );
		CSPFieldMap spFieldMap( cClass.GetFieldMap() );

		
		std::list<CString> strIDs;
		IDs2Str( listIDs, strIDs );
		for( std::list<CString>::const_iterator itIDs = strIDs.begin(); itIDs != strIDs.end(); ++itIDs )
		{
			_ICursorPtr cur = _Select( cClass, *itIDs, lpcszWhereFieldName, bRecycling );
			LOGASSERTEERR_IF( cur, sindyErr_SearchFailed )
			{
				Add( cur, info, emType, spFieldMap, spNameString, bRecycling, bCreateCache, pErrs, bInit, lpcszWhereFieldName, lpcszParentTableName );
			}
		}

		ATLTRACE(_T("�e�[�u��[%s] �����F%d��\n"), lpcszTableName, size(emType) );
	}
	return emErr;
}
sindyErrCode CFeatureContainer::SelectByObject( _IRow* ipRow, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, bool bInit /*= true*/  )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	_ASSERTE(false); // TODO: ���̊֐��͂��������I�I

	// ������
	if( bInit )
		clear();

	CSPRow cFeat( new CFeature( ipRow ) ); // ipRow��this�̃e�[�u���^�C�v���قȂ�ꍇ���l��

	if( cFeat == NULL )
		return emErr;

	// �����t�B�[�`���ł�������o�^�͂��Ȃ�
	if( ! bInit )
	{
		for( const_iterator it = begin(); it != end(); ++it )
		{
			for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			{
				if( (*it2)->IsEqualObject((_IRow*)*cFeat) )
					return emErr;
			}
		}
	}

	push_back( cFeat ); // �����o�Ɋi�[

	// ���T�C�N������ꍇ�̓L���b�V�������
	if( bRecycling || bCreateCache )
		cFeat->CreateCache( bRecycling ? true : false );

	return emErr;
}

// ���f���Œ�`����Ă���e�[�u����R�t����
void _Relate( std::map<long,CSPRow>& rows, const type_info& info, bool bRecycling = false, bool bCreateCache = false )
{
	if( rows.empty() )
		return;
	
	// �e�̃e�[�u�����m��
	CTable parentT( rows.begin()->second->GetTable() );

	// it <0>:�e�̃e�[�u���� <1>:�폜�Ώۂ̃e�[�u���� <2>:<1>�̃e�[�u���ŃL�[�ƂȂ�t�B�[���h��
	for( CRuleRelationTable::const_value_iterator it = g_cRuleRelationTable.begin(parentT.GetTableName());
		it != g_cRuleRelationTable.end(parentT.GetTableName());
		++it )
	{
		CTable childT( parentT.OpenSameOwnerTable( it->get<1>() ) );
		CSPTableNameString spNameString( childT.GetNameString() );
		CSPFieldMap spFieldMap( childT.GetFieldMap() );

		// 1������������͔̂�����Ȃ̂ŁA�܂Ƃ߂ăN�G���𓊂���
		std::list<long> ids;
		for( std::map<long,CSPRow>::const_iterator itRow = rows.begin();
			itRow != rows.end();
			++itRow )
		{
			ids.push_back( itRow->second->GetOID() );
		}

		if( ids.empty() )
			continue;

		// �����̎q���ێ��p
		std::map<long,CSPRow> childRows;

		std::list<CString> strIDs;
		CTable::IDs2Str( ids, strIDs );
		for( std::list<CString>::const_iterator itID = strIDs.begin();
			itID != strIDs.end();
			++itID
			)
		{
			_ICursorPtr cur = _Select( childT, *itID, it->get<2>(), bRecycling );

			long idx = spFieldMap->FindField( it->get<2>() );
			_IRowPtr row;
			int count = 0;
			while( S_OK == cur->NextRow( &row ) && row )
			{
				// ���ɂ����疳��
				CComVariant vaId = 0;
				row->get_Value( idx, &vaId );
				CRow& parent = *rows[vaId.lVal];
				if( parent.find( childT.GetTableType(), sindy::schema::kObjectID, CComVariant(AheGetOID(row)) ) !=
					parent.end( childT.GetTableType() ) )
					continue;

				CSPRow child( RowClassFactory( info, row, childT.GetTableType(), spFieldMap, spNameString ) );
				if( bRecycling || bCreateCache )
					child->CreateCache( bRecycling, spFieldMap, spNameString );

				// �����̎q�ǂ��Ƃ��ēo�^
				parent.push_back( child, false );
				childRows[child->GetOID()] = child;
				++count;
			}

			ATLTRACE(_T("ADD : �e�[�u��[%s] �J�[�\���������F%d��\n"), parentT.GetTableName(), count );
		}

		// ����Ɋ֘A�Â����̂�����ΕR�t����
		_Relate( childRows, info, bRecycling, bCreateCache );
	}

}
sindyErrCode CFeatureContainer::Add( _ICursor* ipCursor, const type_info& info, sindyTableType::ECode emType, const CSPFieldMap& cFields, const CSPTableNameString& cNames, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* pErrs /*= NULL*/, bool bInit/* = true*/, LPCTSTR lpcszOuterKeyFieldName/* = NULL*/, LPCTSTR lpcszParentTableName/* = NULL*/ )
{
	if( ! ipCursor )
		return sindyErr_ArgIsNull;

	// ������
	if( bInit )
		clear();

	sindyTableType::ECode emParentTableType = ( lpcszParentTableName ) ? AheTableName2TableType(lpcszParentTableName) : sindyTableType::other;
	long lOuterKeyFieldIndex = ( lpcszOuterKeyFieldName ) ? cFields->FindField( lpcszOuterKeyFieldName ) : -1;

	// bInit��false�̏ꍇ��find���Ń��R�[�h�P�ʂŖ��񃋁[�v
	// �܂킷�̂͌�������������̂Ŏ��O�ɑΉ��\���쐬���Ă����ibug 7301�j
	std::map<long,CSPRow> mapRowIndex, mapParentRowIndex;
	std::set<long> setChildRowID;
	if( ! bInit )
	{
		for( const_rows_iterator it = begin(emType), itEnd = end(emType); it != itEnd; ++it )
		{
			mapRowIndex[(*it)->GetOID()] = *it;
		}

		if( sindyTableType::other != emParentTableType )
		{
			for( const_rows_iterator it = begin(emParentTableType), itEnd = end(emParentTableType); it != itEnd; ++it )
			{
				mapParentRowIndex[(*it)->GetOID()] = *it;

				// ����I�u�W�F�N�g�̏ꍇ�e�I�u�W�F�N�g�P�ʂł����������肪�ł��Ȃ�
				// ���̂��߁A�Ⴆ�Γ��H�����N�𕪊��������ă����N�������̐e�����N��
				// �ύX�ɂȂ�����ɒǉ���������ƁA���Ɍ����ς݂ɂ��ւ�炸��������
				// �N���������ǉ�����鎖�ɂȂ�B
				// �Ȃ̂ŁA���O�ɑΉ��\���쐬���Ă���
				for( const_rows_iterator it2 = (*it)->begin(emType); it2 != (*it)->end(emType); ++it2 )
					setChildRowID.insert((*it2)->GetOID());
			}
		}
	}

	long count = 0;
	_IRowPtr ipRow;
	while( S_OK == ipCursor->NextRow( &ipRow ) && ipRow )
	{
		++count;

		long lOID = -1;
		ipRow->get_OID( &lOID );

		// �����t�B�[�`���ł�������o�^�͂��Ȃ�
		if( ! bInit )
		{
			if( mapRowIndex.find(lOID) != mapRowIndex.end() ||    // �x�[�X�I�u�W�F�N�g�̏ꍇ
				setChildRowID.find(lOID) != setChildRowID.end() ) // �x�[�X�I�u�W�F�N�g���ێ��������I�u�W�F�N�g�̏ꍇ
				continue;
		}

		CSPRow cRow( RowClassFactory( info, ipRow, emType, cFields, cNames ) );
		if( ! bInit )
			mapRowIndex[lOID] = cRow;

		// �L���b�V���쐬
		if( bRecycling || bCreateCache )
			cRow->CreateCache( bRecycling ? true : false, cFields, cNames );

		// �e�e�[�u�����A�O���L�[�t�B�[���h�����w�肳��Ă���Ȃ�֘A�t���Ċi�[
		if( emParentTableType != sindyTableType::other && lOuterKeyFieldIndex > 0 )
		{
			//rows_iterator it = find( emParentTableType, cRow->GetLongValue( lOuterKeyFieldIndex, -1 ) );
			//LOGASSERTEM_IF( it != end( emParentTableType ), sindyErr_RelFeatureNotFound, NOT_EXIST_RELATIONS, cRow->GetTableName(), cRow->GetOID(), GetTableName(), cRow->GetLongValue( lOuterKeyFieldIndex, -1 ) )
			long keyId = cRow->GetLongValue( lOuterKeyFieldIndex, -1 );
			LOGASSERTEM_IF( mapParentRowIndex.find( keyId ) != mapParentRowIndex.end(), sindyErr_RelFeatureNotFound, NOT_EXIST_RELATIONS, cRow->GetTableName(), cRow->GetOID(), GetTableName(), cRow->GetLongValue( lOuterKeyFieldIndex, -1 ) )
				mapParentRowIndex[keyId]->push_back( cRow, false ); // ����I�u�W�F�N�g�̏d���͊��Ɏ�菜���Ă���̂ł����̓��j�[�N���ǂ����C�ɂ���K�v�͂Ȃ�

		}
		else
		{
			// �����łȂ��ꍇ�͒P���ɃR���e�i�Ɋi�[
			// bInit��true�̎��́A�����t�B�[�`�������邩�ǂ����̃`�F�b�N�͂���Ȃ��A
			// bInit��false�̎��́A���ɏ�Ń`�F�b�N���Ă���̂�bSecureUnique��false�ɂ��Ă���
			push_back( cRow, false );
		}
	}
	ATLTRACE(_T("ADD : �e�[�u��[%s] �J�[�\���������F%d��\n"), GetTableName(), count );

	// ����Ɋ֘A����O���e�[�u��������Ό�������
	std::map<long,CSPRow> mapOuter;
	for( std::map<long,CSPRow>::const_iterator itParent = mapParentRowIndex.begin();
		itParent != mapParentRowIndex.end();
		++itParent )
	{
		for( const_rows_iterator itChild = itParent->second->begin(emType);
			itChild != itParent->second->end(emType);
			++itChild )
		{
			mapOuter[(*itChild)->GetOID()] = *itChild;
		}
	}
	_Relate( mapOuter, info, bRecycling, bCreateCache );

	return sindyErr_NoErr;
}

} // sindy
