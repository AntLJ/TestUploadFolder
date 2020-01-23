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
					std::string			c_sWhere,			///< Where�������
					int					c_iRetType		)	///< ����Ԃ���
{
	HRESULT				hr;

	ISpatialFilterPtr	a_ipFilter(CLSID_SpatialFilter);
	CComBSTR			a_ccSpCol;

	hr	= e_ipFeCls->get_ShapeFieldName ( &a_ccSpCol );
	if (FAILED(hr)) {
		Print_Err	( "�V�F�C�v�t�B�[���h�����擾�ł��Ȃ�", "Search_Feature" );
		return	( NULL );
	}

	hr	= a_ipFilter->putref_Geometry	( c_ipGeo );
	if (FAILED(hr))	{
		Print_Err	( "�T�[�`�p�̃W�I���g�����ݒ�ł��Ȃ�", "Search_Feature" );
		return	( NULL );
	}

	hr	= a_ipFilter->put_GeometryField	( a_ccSpCol );
	if (FAILED(hr)) {
		Print_Err	( "�W�I���g���t�B�[���h���ݒ�ł��Ȃ�", "Search_Feature" );
		return	( NULL );
	}

	hr	= a_ipFilter->put_SpatialRel	( c_ipSpatialRel );	// �w��V�F�C�v�Ɨאڂ������
	if (FAILED(hr)) {
		Print_Err	( "SpatialRel()���ݒ�ł��Ȃ�", "Search_Feature" );
		return	( NULL );
	}
	hr	= a_ipFilter->put_SearchOrder	( esriSearchOrderSpatial );
	if (FAILED(hr))	{
		Print_Err	( "�T�[�`�I�[�_�[�̐ݒ肪�ł��Ȃ�", "Search_Feature" );
		return	( NULL );
	}

	// Where�傪�P�������ƂȂ񂩂�΂����Ȃ̂�
	if( c_sWhere.size() > 1 ) {

		char	a_tmpstr[1024];	//�@�����͂Ȃ�Ƃ��������C������
		sprintf	( a_tmpstr, "%s", &c_sWhere[0]);	// ���b�V���R�[�h�𕶎����
		_bstr_t	a_Where_Str ( a_tmpstr );

		hr	= a_ipFilter->put_WhereClause	( a_Where_Str );
		if ( FAILED ( hr ) )	{
			Print_Err	( "�����pWHERE��̐ݒ肪�ł��Ȃ�", "Search_Feature" );
			return		( NULL );
		}
	}

	// �f�[�^�J�E���g�����
	if( c_iRetType & RET_COUNT ) {
		//---------------------------------------------------------
		long		a_lData_Count	= 0;
		hr	= e_ipFeCls->FeatureCount ( (IQueryFilterPtr)a_ipFilter, &a_lData_Count );
		if ( FAILED ( hr ) )	{
			Print_Err	( "�����f�[�^�����擾�ł��Ȃ�", "Search_Feature" );
			return	( NULL );
		}
		e_lData_Count	= a_lData_Count;
		//---------------------------------------------------------
	}
	IFeatureCursorPtr	a_ipFeCsr;		///< ��r���t�B�[�`���J�[�\��
	if( c_iRetType & RET_FCURSOR ) {// ��r���f�[�^���擾����i�N�G�����̂͂��܂�Ӗ����������A�t�@�C�����f�[�^��S�����j
		hr = e_ipFeCls->Search((IQueryFilterPtr)a_ipFilter, VARIANT_FALSE, &a_ipFeCsr);
		if (FAILED(hr)) {
			Print_Err	( "�f�[�^�̃T�[�`���ł��Ȃ�", "Search_Feature" );
			return	( NULL );
		}
	}
	return	( a_ipFeCsr );
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

