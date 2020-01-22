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
#include "msi_handle_ws.h"


/**
	�t�@�C����
*/
bool	msi_handle_ws::
Init	(	IWorkspacePtr	c_ipWSpace,		///< �I�[�v���������[�N�X�y�[�X
			char*			c_cpLayer	)	///< ���C������
{
	HRESULT					hr;

	long	a_lName_Len	= 0;

	a_lName_Len	= strlen ( c_cpLayer );

	e_cpLayer	= new char[a_lName_Len+1];
	if( e_cpLayer == NULL ) {
		return ( false );
	}
	strcpy ( e_cpLayer, c_cpLayer );

//	IFeatureCursorPtr		e_ipFeCsr;		///< ��r���t�B�[�`���J�[�\��

	////
	IFeatureWorkspacePtr	a_FeWspace	( c_ipWSpace );	// �t�B�[�`�����[�N�X�y�[�X

	// �t�B�[�`���i���C���H�j�̃I�[�v��
	hr	= a_FeWspace->OpenFeatureClass ( CComBSTR ( c_cpLayer ), &e_ipFeCls );
	if( FAILED( hr ) ) {
		fprintf	( stderr, "���C��<%s>�̃I�[�v���Ɏ��s\n", c_cpLayer );
		return	( false );
	}else {
		fprintf	( stdout,	"<%s> ���C�����I�[�v��\n", c_cpLayer );
	}

	return ( true );
}

/**
	�w��t�B�[�`���ɑ΂���esriSpatialRelEnum�Ŏw�肵���֌W�����f�[�^��
	�Ƃ��Ă���
*/
IFeatureCursorPtr	msi_handle_ws::
Search_Feature	(	IGeometryPtr		c_ipGeo,			///< �w��W�I���g��
					esriSpatialRelEnum	c_ipSpatialRel,		///< �֌W�w��
					string				c_sWhere,			///< Where�������
					int					c_iRetType		)	///< ����Ԃ���
{
	HRESULT				hr;

	ISpatialFilterPtr	a_ipFilter(CLSID_SpatialFilter);
	CComBSTR			a_ccSpCol;

	hr	= e_ipFeCls->get_ShapeFieldName ( &a_ccSpCol );
	if (FAILED(hr)) {
		fprintf	( stderr, "�V�F�C�v�t�B�[���h�����擾�ł��Ȃ�[Shp_Dat_Search::Search]\n");
		return	( NULL );
	}

	// Arc10.3.1�Ń������s���ɂȂ蓮���~����Ή��Ƃ��āA�T�[�`�p�W�I���g���̃N���[�����쐬
	IClonePtr c_ipGeo_Clone;
	hr = IClonePtr(c_ipGeo)->Clone(&c_ipGeo_Clone);
	if (FAILED(hr)) {
		fprintf(stderr, "�T�[�`�p�̃W�I���g���̃N���[���쐬�Ɏ��s[Shp_Dat_Search::Search]\n");
		return	(NULL);
	}

	hr = a_ipFilter->putref_Geometry(IGeometryPtr(c_ipGeo_Clone));
	if (FAILED(hr))	{
		fprintf	( stderr, "�T�[�`�p�̃W�I���g�����ݒ�ł��Ȃ�[Shp_Dat_Search::Search]\n");
		return	( NULL );
	}

	hr	= a_ipFilter->put_GeometryField	( a_ccSpCol );
	if (FAILED(hr)) {
		fprintf	( stderr, "�W�I���g���t�B�[���h���ݒ�ł��Ȃ�[Shp_Dat_Search::Search]\n");
		return	( NULL );
	}

	hr	= a_ipFilter->put_SpatialRel	( c_ipSpatialRel );	// �w��V�F�C�v�Ɨאڂ������
	if (FAILED(hr)) {
		fprintf	( stderr, "SpatialRel()���ݒ�ł��Ȃ�[Shp_Dat_Search::Search]\n");
		return	( NULL );
	}
	hr	= a_ipFilter->put_SearchOrder	( esriSearchOrderSpatial );
	if (FAILED(hr))	{
		fprintf	( stderr, "�T�[�`�I�[�_�[�̐ݒ肪�ł��Ȃ�[Shp_Dat_Search::Search]\n");
		return	( NULL );
	}

	// Where�傪�P�������ƂȂ񂩂�΂����Ȃ̂�
	if( c_sWhere.size() > 1 ) {

		char	a_tmpstr[1024];	//�@�����͂Ȃ�Ƃ��������C������
		sprintf	( a_tmpstr, "%s", &c_sWhere[0]);	// ���b�V���R�[�h�𕶎����
		_bstr_t	a_Where_Str ( a_tmpstr );

		hr	= a_ipFilter->put_WhereClause	( a_Where_Str );
		if ( FAILED ( hr ) )	{
			Print_Err	( "�����pWHERE��̐ݒ肪�ł��Ȃ�", "Search_Where" );
			return		( NULL );
		}
	}

	// �f�[�^�J�E���g�����
	if( c_iRetType & RET_COUNT ) {
		//---------------------------------------------------------
		long		a_lData_Count	= 0;
		hr	= e_ipFeCls->FeatureCount ( (IQueryFilterPtr)a_ipFilter, &a_lData_Count );
		if ( FAILED ( hr ) )	{
			fprintf	( stderr, "�����f�[�^�����擾�ł��Ȃ�<msi_handle_ws::Search_Where>\n");
			return	( NULL );
		}
		e_lData_Count	= a_lData_Count;
		//---------------------------------------------------------
	}
	IFeatureCursorPtr	a_ipFeCsr;		///< ��r���t�B�[�`���J�[�\��
	if( c_iRetType & RET_FCURSOR ) {// ��r���f�[�^���擾����i�N�G�����̂͂��܂�Ӗ����������A�t�@�C�����f�[�^��S�����j
		hr = e_ipFeCls->Search((IQueryFilterPtr)a_ipFilter, VARIANT_FALSE, &a_ipFeCsr);
		if (FAILED(hr)) {
			fprintf	( stderr, "�f�[�^�̃T�[�`���ł��Ȃ�[Shp_Dat_Search::Search]\n");
			return	( NULL );
		}
	}
	return	( a_ipFeCsr );
}

/**
	WHERE����w�肵�ăf�[�^������Ă���
	@return	IFeatureCursorPtr
*/
IFeatureCursorPtr	msi_handle_ws::
Search_Where	(	std::string		c_sWhere,		///< �w��WHERE��
					int				c_iRetType,		///< ����Ԃ���
					int				c_iOrder )		///< �\�[�g���邩�̂��Ă��i�P=�\�[�g�A���̑�=�\�[�g���Ȃ��j
{
	HRESULT			hr;
	
	BSTR			a_bWhere_Str;
	char*			a_tmpstr	= &c_sWhere[0];
	a_bWhere_Str	= T2BSTR( _T(a_tmpstr) );		// string��BSTR�ɕϊ�

	IQueryFilterPtr		a_ipQ_Filter	( CLSID_QueryFilter );		// ��Ԍ����t�B���^
	
	hr	= a_ipQ_Filter->put_WhereClause	( a_bWhere_Str );
	if ( FAILED ( hr ) )	{
		fprintf	( stderr, "�����pWHERE��̐ݒ肪�ł��Ȃ�<msi_handle_ws::Search_Where>\n");
		return	( NULL );
	}

	// �f�[�^�J�E���g�����
		// �f�[�^�J�E���g�����
	if( c_iRetType & RET_COUNT ) {
		//---------------------------------------------------------
		long		a_lData_Count	= 0;
		hr	= e_ipFeCls->FeatureCount ( a_ipQ_Filter, &a_lData_Count );
		if ( FAILED ( hr ) )	{
			fprintf	( stderr, "�����f�[�^�����擾�ł��Ȃ�<msi_handle_ws::Search_Where>\n");
			return	( NULL );
		}
		e_lData_Count	= a_lData_Count;
		//---------------------------------------------------------
	}

	// �T�[�`���Ă���
	IFeatureCursorPtr	a_ipFeCursor;
	if( c_iRetType & RET_FCURSOR ) {// ��r���f�[�^���擾����i�N�G�����̂͂��܂�Ӗ����������A�t�@�C�����f�[�^��S�����j
		if( c_iOrder != 1 ) {	// �\�[�g���Ȃ�

			if( FAILED(e_ipFeCls->Search	( a_ipQ_Filter, VARIANT_FALSE, &a_ipFeCursor )) ) {
				fprintf	( stderr, "�f�[�^��QUERY�����Ɏ��s\n" );
				return	( NULL );
			}

		}else {					// �I�u�W�F�N�gID�Ń\�[�g����

			ITableSortPtr						a_ipTableSort ( CLSID_TableSort ); 
			a_ipTableSort->put_Fields			( T2BSTR( _T("OBJECTID") ) );
			a_ipTableSort->putref_Table			( ITablePtr(e_ipFeCls) );
			a_ipTableSort->putref_QueryFilter	( a_ipQ_Filter );
			a_ipTableSort->Sort					( NULL );
			/// ����
			_ICursorPtr	a_ipCursor;
			a_ipTableSort->get_Rows				( &a_ipCursor );
			a_ipFeCursor	= IFeatureCursorPtr	( a_ipCursor );
		}
	}
	return	( a_ipFeCursor );
}

/**
	�G���[���o�͂���
	@return	����
*/
void	msi_handle_ws::
Print_Err	(	char*	c_cpAddMsg,		///< ��{�G���[���ɒǉ����������b�Z�[�W
				char*	c_cpFuncName )	///< �֐���
{
	fprintf	( stderr,	"<%s>���C�� �F %s : [msi_handle_ws::%s]\n",
						e_cpLayer,
						c_cpAddMsg,
						c_cpFuncName );
}

