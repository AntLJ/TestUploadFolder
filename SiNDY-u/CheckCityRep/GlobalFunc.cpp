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
#include "GlobalFunc.h"

#include <sindy/workspace.h>

namespace gf
{	
	// ���[�N�X�y�[�X�擾
	IWorkspacePtr GetWorkspace( LPCTSTR lpcszDBProp )
	{
		IUnknownPtr ipUnknown;
		for( int i = 0; i < 5; ++i )
		{
			INamePtr ipName( sindy::create_workspace_name(lpcszDBProp) );
			if( NULL == ipName )
				return NULL;

			ipName->Open( &ipUnknown );
			if( ipUnknown )
				break;
		}
		return ipUnknown;
	}

	// ���ϐ�������A���[�N�X�y�[�X�ƃt�B�[�`���N���X���I�[�v��
	bool GetFCFromEnv( const _TCHAR* lpcszEnv, IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFC )
	{
		CString strTmp = GetEnvValue(lpcszEnv);
		if( !strTmp.IsEmpty() )
		{
			((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass( _bstr_t(strTmp), &ipFC );
			if( ipFC )
				return true;
			else
			{
				cerr << "#Error �t�B�[�`���N���X�̃I�[�v���Ɏ��s(" << lpcszEnv << ") : " << strTmp << endl;
				return false;
			}
		}
		else
		{
			cerr << "#Error ���ϐ� " << lpcszEnv << " �����ݒ�" << endl;
			return false;
		}
	}

	// ��ԁE��������
	IFeatureCursorPtr SelectByShape( IGeometryPtr& ipGeo, IFeatureClassPtr& ipFeatureClass, long* pCount /* = NULL */,
									 esriSpatialRelEnum spatialRel /* = esriSpatialRelIntersects */,
									 LPCTSTR lpszWhereClause /* = NULL */, LPCTSTR lpszSubFields /* = NULL */ )
	{
		// �N�G���t�B���^�쐬
		ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
		CComBSTR bstrFieldName;	
		ipFeatureClass->get_ShapeFieldName( &bstrFieldName );
		ipFilter->put_GeometryField( bstrFieldName );
		ipFilter->putref_Geometry( ipGeo );
		ipFilter->put_SpatialRel( spatialRel );
		
		// Where��w�肳��Ă�����A�Z�b�g����
		if( lpszWhereClause )
		{
			ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );	// Where��Z�b�g
			ipFilter->put_SearchOrder( esriSearchOrderSpatial );	// �����D�揇�ʂ��w�肷��
		}

		// SubFields�w�肳��Ă�����A�Z�b�g����
		if( lpszSubFields )
			ipFilter->put_SubFields( CComBSTR(lpszSubFields) );

		// �������ʐ��擾([Bug 6175]�C�ӂɕύX)
		if( pCount )
		{
			if( FAILED(ipFeatureClass->FeatureCount(ipFilter, pCount)) )
				*pCount = -1;

			// �������R�[�h���킩���Ă���Ƃ��́A������Search���Ȃ�
			if( *pCount < 1 )
				return NULL;
		}

		// ����
		IFeatureCursorPtr ipFeatureCursor;
		if( FAILED(ipFeatureClass->Search(ipFilter, VARIANT_FALSE, &ipFeatureCursor)) && pCount )	// [Bug 6419]
			*pCount = -1;
		return ipFeatureCursor;
	}
}
