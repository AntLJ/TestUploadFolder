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
#include "Smp_db_obj_attr.h"

/**
	�\���̂�SdeRow����f�[�^���Z�b�g����
	@return	bool
*/
bool	Smp_db_obj_attr::
Attr_Set_AOBJ	(	IFeaturePtr		c_Feature )		///< �t�F�b�`���Ă����t�B�[�`��[IN]
{					
	LONG			a_Col_Size	= 0; 
	IFieldsPtr		a_Col_Defs;					// �J�����̒�`
	c_Feature->get_Fields		( &a_Col_Defs );
	a_Col_Defs->get_FieldCount	( &a_Col_Size );
	
	LONG		i = 0;
	CComBSTR	a_Col_Name;
	for( i = 0 ; i < a_Col_Size ; i++ )
	{
		IFieldPtr	a_Col_Def;
		
		CComVariant	a_Value;
		a_Col_Defs->get_Field	( i, &a_Col_Def );
		a_Col_Name.Empty();

		esriFieldType	a_Col_Type;				// �t�B�[���h�^�C�v
		a_Col_Def->get_Type	( &a_Col_Type );	// �J�����^�C�v
		a_Col_Def->get_Name	( &a_Col_Name );	// �J��������
		c_Feature->get_Value( i, &a_Value );	// �����l�̎擾

		if(a_Col_Name == O_ID) {	// �I�u�W�F�N�gID
			e_Obj_ID		= a_Value.intVal;
		}else
		if( (a_Col_Name == DSC1_F) || (a_Col_Name == SC1DT_C) ){	// �\���X�P�[���P�i�x�[�X�A�~�h���A�g�b�v�j�w�i�E���L
			e_Sc1_Disp_Type_C= a_Value.intVal;
		}else
		if( (a_Col_Name == DSC2_F) || (a_Col_Name == SC2DT_C) ){	// �\���X�P�[���Q
			e_Sc2_Disp_Type_C= a_Value.intVal;
		}else
		if( (a_Col_Name == DSC3_F) || (a_Col_Name == SC3DT_C) || (a_Col_Name == DST_C) ){	// �\���X�P�[���R�i�s�s�n�}���L���Ή��j
			e_Sc3_Disp_Type_C= a_Value.intVal;
		}else
		if( (a_Col_Name == DSC4_F) || (a_Col_Name == SC4DT_C) ){	// �\���X�P�[���S
			e_Sc4_Disp_Type_C= a_Value.intVal;
		}else
		if( a_Col_Name == SC1BGC_C ){	// �\����ʃR�[�h�E�X�P�[���P�i�x�[�X�A�~�h���A�g�b�v�j�w�i
			e_Sc1_Disp_Code_C	= a_Value.intVal;
			e_Cur_Attr_Cls		= BG_C;
		}else
		if( a_Col_Name == SC2BGC_C ){	// �\����ʃR�[�h�E�\���X�P�[���Q
			e_Sc2_Disp_Code_C	= a_Value.intVal;
			e_Cur_Attr_Cls		= BG_C;
		}else
		if( a_Col_Name == SC3BGC_C ){	// �\����ʃR�[�h�E�\���X�P�[���R
			e_Sc3_Disp_Code_C	= a_Value.intVal;
			e_Cur_Attr_Cls		= BG_C;
		}else
		if( a_Col_Name == SC4BGC_C ){	// �\����ʃR�[�h�E�\���X�P�[���S
			e_Sc4_Disp_Code_C	= a_Value.intVal;
			e_Cur_Attr_Cls		= BG_C;
		}else
		if(a_Col_Name == UNDER_F){				// �n��E�n���t���O�i�S���֌W�j
			e_Under_Ground_F= a_Value.intVal;
		}else
		if(a_Col_Name == CNT_C){				// �i�ʎ��
			e_Class_C		= a_Value.intVal;	///< �i��
			e_Cur_Attr_Cls	= CNT_C;
		}else
		if(a_Col_Name == CLR_CODE) {			///< �X��F�ԍ�
			e_Color_Code	= a_Value.intVal;
		}else
		if(a_Col_Name == BG_C){					///< �w�i��ʁi�s�s�A�x�[�X�A�~�h���A�g�b�v�j
			e_Class_C		= a_Value.intVal;	///< �s�s�n�}�A���k�ڎ��
			e_Cur_Attr_Cls	= BG_C;
		}else
		if(a_Col_Name == RAIL_C){				// �S���H�����
			e_Class_C		= a_Value.intVal;	///< �S���H��
			e_Cur_Attr_Cls	= RAIL_C;
		}else
		if(a_Col_Name == STAT_C){				// �S���w�Ɏ��
			e_Class_C		= a_Value.intVal;	///< �w��
			e_Cur_Attr_Cls	= STAT_C;
		}else
		if(a_Col_Name == BLD_C){				// �������
			e_Class_C		= a_Value.intVal;	///< �����|���S��
			e_Cur_Attr_Cls	= BLD_C;
		}else
		if(a_Col_Name == ROAD_C){				// ���H�\�����
			e_Class_C		= a_Value.intVal;	///< ���H
			e_Cur_Attr_Cls	= ROAD_C;
		}else
		if(a_Col_Name == C_CODE){				// �s�撬���R�[�h
			// �s���E�̂�
			string			a_City_Code_Str;
			a_City_Code_Str	= Str_to_SJIS ((CComBSTR *)(&a_Value.bstrVal));
			e_City_Code		= a_City_Code_Str;
		}else
		if(a_Col_Name == A_CODE){				// �����ڃR�[�h
			string			a_Addr_Code_Str;
			a_Addr_Code_Str	= Str_to_SJIS ((CComBSTR *)(&a_Value.bstrVal));
			e_Addr_Code		= a_Addr_Code_Str;
		}else
		if(a_Col_Name == N_STR_1){				// ���̕�����P
			e_Str_1			= Str_to_SJIS ((CComBSTR *)(&a_Value.bstrVal));
		}else
		if(a_Col_Name == N_STR_2){				// ���̕�����Q
			e_Str_2			= Str_to_SJIS ((CComBSTR *)(&a_Value.bstrVal));
		}else
		if(a_Col_Name == N_STR_3){				// ���̕�����R
			e_Str_3			= Str_to_SJIS ((CComBSTR *)(&a_Value.bstrVal));
		}else
		if(a_Col_Name == S_NUM_1){				// ���̕������P
			e_Str_Num_1		= a_Value.intVal;
		}else
		if(a_Col_Name == S_NUM_2){				// ���̕������Q
			e_Str_Num_2		= a_Value.intVal;
		}else
		if(a_Col_Name == S_NUM_3){				// ���̕������R
			e_Str_Num_3		= a_Value.intVal;
		}else
		if(a_Col_Name == AN_CLS_C){				// ���L���
			e_Anno_Class_C	= a_Value.intVal;
			e_Cur_Attr_Cls	= AN_CLS_C;
		}else
		if(a_Col_Name == TMP_AN_CLS){			// �e���|�������
			e_Tmp_Anno_Class= Str_to_SJIS ((CComBSTR *)(&a_Value.bstrVal));
		}else
		if(a_Col_Name == M_TYPE){				//  �}�`ID�ɂ�����
//			e_Mark_Type		= a_Value.intVal;
		}else
		if(a_Col_Name == CHINUM) {				// �n�Ԕԍ�
			e_Chiban_No		= a_Value.intVal;
		}else
		if(a_Col_Name == A_F) {					// �n�Ԓ��L�t���O
			e_Anno_F		= a_Value.intVal;
		}else
		if(a_Col_Name == F_SIZE) {				// �t�H���g�T�C�Y
			e_FontSize		= a_Value.intVal;
		}else
		if(a_Col_Name == A_ID) {				// ���C���ɑΉ�����ID
			e_Note_ID		= a_Value.intVal;
		}else
		if(a_Col_Name == TARGET_S) {			// �\�������񂪂ǂꂩ
			e_Target_Str_Num= a_Value.intVal;
		}else
		if(a_Col_Name == MESH_C) {				// ���b�V���R�[�h��`�p
			e_MeshCode		= a_Value.intVal;
		}else
		if(a_Col_Name == SPEC_F) {
			e_Spec_F		= a_Value.intVal;	// �i�����
		}else
		if(a_Col_Name == SHP){					// �V�F�C�v�f�[�^
		//	if( a_Col_Type == SE_SHAPE_TYPE ){
			//	IGeometryPtr	a_Tmp_Shp;
			//	c_Feature->get_ShapeCopy	( &a_Tmp_Shp );
			//	e_Geometry_Ptr	= a_Tmp_Shp;
			//	c_Feature->get_Shape		( &c_Shape );			// �V�F�C�v���擾
		//	}
		}
	} // End for
	return	( true );
}

/**
	Unicode�̕����񂩂�SJIS�ɂ���
*/
string		Smp_db_obj_attr::
Str_to_SJIS		(	CComBSTR*	c_CStr	)	// ������(Unicode)
{	
	string		ret_str;
#ifdef	_ARC_9_1_
	USES_CONVERSION;
	if (*c_CStr != 0) {
		ret_str = OLE2T(*c_CStr);
	}
#else	
	USES_CONVERSION;
#ifndef _SINDY_A_
	long		length	= c_CStr->Length() * 2;

	char*		tmp_moji	= new char[length+1];

	memset		(tmp_moji, '\0', length+1);
	_bstr_t		a_bTmp ( *c_CStr, false );
//	wchar_t*	tmp_wide	= OLE2W(*c_CStr);
	wchar_t*	tmp_wide	= (wchar_t *)a_bTmp;

	if(tmp_wide == NULL) {
		ret_str	= "";
	}else {
		if(wcstombs( tmp_moji, tmp_wide, length+1 ) == -1){
			ret_str	= "";
		}else {
			ret_str	= tmp_moji;
		}
	}
	delete [] tmp_moji;
#else
	if (*c_CStr != 0) {
		ret_str = OLE2T(*c_CStr);
	}
#endif
#endif	// end of _ARC_9_1_
	return	(ret_str);
}

/**
	�W�I���g��������_��ɕϊ�
	@return	bool
*/
bool		Smp_db_obj_attr::
Inport_Geo	(	IGeometryPtr	c_ipGeo	)	///< �W�I���g��
{

//	HRESULT				hr;

	esriGeometryType	a_eGeoType;

	if( c_ipGeo == NULL ) {
		fprintf	( stderr, "�W�I���g����NULL�ł�\n");
		return	( false );
	}

	if( FAILED(	c_ipGeo->get_GeometryType( &a_eGeoType )) ) {
		fprintf	( stderr, "[Inport_Geo] �W�I���g���^�C�v���Ƃ�܂���\n" );
		return	( false );
	}

	// �W�I���g���̃^�C�v���擾����
	if( a_eGeoType == esriGeometryPolygon ) {

		e_Pts_Type	= PTS_POLY;
	
	}else
	if( a_eGeoType == esriGeometryPolyline ) {
		
		e_Pts_Type	= PTS_LINE;

	}else
	if( a_eGeoType == esriGeometryPoint ) {
	
		e_Pts_Type	= PTS_POINT;

	}else {
		fprintf	( stderr, "���Ή��̃W�I���g���^�C�v�ł�<msi_get_parts_info>\n" );
		return	( false );
	}


	// �W�I���g���R���N�V�����ɂ��ăp�[�g�A�T�u�p�[�g���ɂ΂�΂�̓_��ɂ���
	IGeometryCollectionPtr	a_ipGeoColl ( c_ipGeo );
	long					a_lGeo_Count	= 0;	// �W�I���g���̌�
	
	a_ipGeoColl->get_GeometryCount ( &a_lGeo_Count );

	int	i = 0;
	for ( i = 0; i < a_lGeo_Count; i++ )
	{
		IGeometryPtr	a_ipCurGeo;
		
		// �W�I���g�������o��
		a_ipGeoColl->get_Geometry ( i, &a_ipCurGeo );

		IPointCollectionPtr	a_ipTmpPts;	// ��ԓ_��

		if( a_eGeoType == esriGeometryPolygon ) {	// �|���S��
			a_ipTmpPts	= (IRingPtr)a_ipCurGeo;
		}else
		if( a_eGeoType == esriGeometryPolyline ) {	// �|�����C��
			a_ipTmpPts	= (IPathPtr)a_ipCurGeo;
		}else {										// �|�C���g
			a_ipTmpPts	= (IPointPtr)a_ipCurGeo;
		}

		long	a_lPtCount	= 0;	// ��ԓ_��
		
		a_ipTmpPts->get_PointCount ( &a_lPtCount );

		// �����_��̗̈���m�ۂ��Ă���
		v_Dpos		a_TmpPts;		// �P�W�I���g���̓_��
		int	i = 0;
		for ( i = 0; i < a_lPtCount; i++ )
		{
			IPointPtr	a_ipPt;
			Dpos		a_dPos;
			a_ipTmpPts->get_Point ( i, &a_ipPt );

			a_ipPt->get_X ( &a_dPos.xpos );
			a_ipPt->get_Y ( &a_dPos.ypos );
			a_TmpPts.push_back ( a_dPos );		// �p�[�g���Ƃ̓_��ɒǉ�
		}
		e_vDPtsArray.push_back ( a_TmpPts );	// �_����x�N�^�ɒǉ� 
	}
	return ( true );
}

/**
	�����_����W�I���g���ɕϊ�
	@return	IGeometryPtr	����
	@return	NULL	���s
*/
IGeometryPtr	Smp_db_obj_attr::
Export_Geo	( void )
{

	IGeometryPtr			a_ipRetGeo	= NULL;
	IGeometryCollectionPtr	a_ipTmpColl;
	IPolygonPtr				a_ipPoly ( CLSID_Polygon );
	IPolylinePtr			a_ipLine ( CLSID_Polyline );
	IPointPtr				a_ipPoint( CLSID_Point );

	if( e_Pts_Type == PTS_POLY ) {
		a_ipTmpColl	= a_ipPoly;
	}else
	if( e_Pts_Type == PTS_LINE ) {
		a_ipTmpColl	= a_ipLine;
	}else {
		a_ipTmpColl	= a_ipPoint;
	}

	iv_v_Dpos	a_ivIndx;	// �_��p�[�g�̃C���f�b�N�X
	for( a_ivIndx = e_vDPtsArray.begin(); a_ivIndx != e_vDPtsArray.end(); a_ivIndx++ )
	{
		v_Dpos			a_vCurPts	= *a_ivIndx;
		IGeometryPtr	a_ipTmpGeo;
		IRingPtr		a_ipRing ( CLSID_Ring );
		IPathPtr		a_ipPath ( CLSID_Path );
		IPointCollectionPtr	a_ipPts;
	
		//
		if( e_Pts_Type == PTS_POLY ) {	// �|���S���̏ꍇ
			a_ipTmpGeo	= a_ipPoly;
			a_ipPts		= a_ipPoly;
		}else
		if( e_Pts_Type == PTS_LINE ) {	// �|�����C���̏ꍇ
			a_ipTmpGeo	= a_ipLine;
			a_ipPts		= a_ipLine;
		}else {							// �|�C���g�̏ꍇ
			a_ipTmpGeo	= a_ipPoint;
			a_ipPts		= a_ipPoint;
		}

		// �_���ǉ����Ă���
		iv_Dpos	a_ivIndx2;
		for ( a_ivIndx2 = a_vCurPts.begin(); a_ivIndx2 != a_vCurPts.end(); a_ivIndx2++ )
		{
			Dpos	a_TmpPt	= *a_ivIndx2;

			IPointPtr	a_ipPt ( CLSID_Point );
			a_ipPt->put_X ( a_TmpPt.xpos );
			a_ipPt->put_Y ( a_TmpPt.ypos );
			a_ipPts->AddPoint ( a_ipPt );
		}

		// �R���N�V�����ɒǉ�����
		a_ipTmpColl->AddGeometry ( a_ipTmpGeo );
	}

	// �R���N�V�����ɒǉ������f�[�^�ɂ��ăV���v���t�@�C������
	if( e_Pts_Type == PTS_POLY ) {

		if( FAILED ( a_ipPoly->SimplifyPreserveFromTo () ) ) {
			fprintf	( stderr, "Simplify�Ɏ��s\n" );
			return	( NULL );
		}
		a_ipRetGeo	= a_ipPoly;

	}else
	if( e_Pts_Type == PTS_LINE ) {
		
		if( FAILED ( a_ipLine->SimplifyNetwork () ) ) {
			fprintf	( stderr, "Simplify�Ɏ��s\n" );
			return	( NULL );
		}
		a_ipRetGeo	= a_ipLine;

	}else {
		a_ipRetGeo	= a_ipPoint;
	}
	return	( a_ipRetGeo );
}
