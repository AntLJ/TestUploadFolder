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
#include <vector>
#include <list>
#include <set>
//#include "poly_class.h"

/**
	�X�V����
		2003.02.03	���H���C���̃t�F���[�����������Ă����̂Œǉ�
*/

#ifndef	_SMP_DB_OBJ_ATTR_H_
#define	_SMP_DB_OBJ_ATTR_H_

using	namespace	std;

/**
	DB��ŃI�u�W�F�N�g�����������̏��́A�V�F�C�v�Ƃ��Ď��o����
	���_�ő����Ɛ؂藣����Ă��܂��̂ŁA������Ȃ�Ƃ��������č쐬
	�J�������̂ɂ��Ă��A�����Œ�`���Ă������Ƃɂ���
*/

#define	O_ID		"OBJECTID"
#define	OPE			"OPERATOR"
#define	PUR_C		"PURPOSE_C"
#define	MDA			"MODIFYDATE"
#define	UDA			"UPDATETYPE_C"
#define	CLA_F		"USERCLAIM_F"
#define	MESH		"TMP_MESHCODE"
#define	PRIM		"TMP_PRIMID"

// �X�P�[���ʕ\���Ή��p
#define	DSC1_F		"DISPSCALE1_F"	///< �w�i�p�X�P�[���P�t���O
#define	DSC2_F		"DISPSCALE2_F"	///< �w�i�p�X�P�[���Q�t���O
#define	DSC3_F		"DISPSCALE3_F"	///< �w�i�p�X�P�[���R�t���O
#define	DSC4_F		"DISPSCALE4_F"	///< �w�i�p�X�P�[���S�t���O
#define	SC1BGC_C	"SC1BGCLASS_C"	///< �w�i��ʃR�[�h�i�X�P�[���P�j
#define	SC2BGC_C	"SC2BGCLASS_C"	///< �w�i��ʃR�[�h�i�X�P�[���Q�j
#define	SC3BGC_C	"SC3BGCLASS_C"	///< �w�i��ʃR�[�h�i�X�P�[���R�j
#define	SC4BGC_C	"SC4BGCLASS_C"	///< �w�i��ʃR�[�h�i�X�P�[���S�j
#define	SC1DT_C		"SC1DISPTYPE_C"	///< ���L�p�X�P�[���P�\���R�[�h
#define	SC2DT_C		"SC2DISPTYPE_C"	///< ���L�p�X�P�[���Q�\���R�[�h
#define	SC3DT_C		"SC3DISPTYPE_C"	///< ���L�p�X�P�[���R�\���R�[�h
#define	SC4DT_C		"SC4DISPTYPE_C"	///< ���L�p�X�P�[���S�\���R�[�h
#define	DST_C		"DISPTYPE_C"	///< �s�s�n�}���L�\���R�[�h

#define	UNDER_F		"UNDERGROUND_F"
#define	CNT_C		"CNTCLASS_C"
#define	BG_C		"BGCLASS_C"
#define	RAIL_C		"RAILWAYCLASS_C"
#define	STAT_C		"STATIONCLASS_C"
#define	BLD_C		"BLDCLASS_C"
#define	ROAD_C		"ROADCLASS_C"
#define	C_CODE		"CITYCODE"
#define	A_CODE		"ADDRCODE"
#define	T_CODE_F	"TMPCODE_F"
#define	GAI			"GAIKUFUGO"
#define	EXGAI		"EXTGAIKUFUGO"
#define	ADDR_C		"ADDRCLASS_C"
#define	MAN_F		"MANAGE_F"
#define	CORR_F		"CORRIDOR_F"

#define	FLR			"FLOORS"
#define	S_RF		"STEP_RF"
#define	SPEC_F		"SPEC_F"

#define	SHP			"SHAPE"
#define	SHP_A		"SHAPE.AREA"
#define	SHP_L		"SHAPE.LEN"
// ���L�̂�
#define	N_STR_1		"NAME_STRING1"
#define	N_STR_2		"NAME_STRING2"
#define	N_STR_3		"NAME_STRING3"
#define	S_NUM_1		"STRING_NUM1"
#define	S_NUM_2		"STRING_NUM2"
#define	S_NUM_3		"STRING_NUM3"
#define	AN_CLS_C	"ANNOCLASS_C"
#define	TMP_AN_CLS	"TMP_ANNO_CLASS"
#define	M_TYPE		"MARKTYPE"
#define	F_SIZE		"FONTSIZE"
#define	A_ID		"ANNO_ID"
// �n�Ԓ��L�̂�
#define	CHINUM		"CHIBAN_NO"		///< �n�Ԕԍ�
#define	A_F			"ANNO_F"		///< �n�Ԓ��L�t���O

#define	TARGET_S	"TARGET_STR_C"
#define	MESH_C		"MESHCODE"
//�s���E�̂�
#define	CLR_CODE	"COLORCODE"

//typedef	vector<Dpos>				v_Dpos;
//typedef	vector<Dpos>::iterator		iv_Dpos;
//typedef	vector<v_Dpos>				v_v_Dpos;
//typedef	vector<v_Dpos>::iterator	iv_v_Dpos;

struct	Smp_db_obj_attr
{
	string			e_sLayer_Name;		///< ���C����
	unsigned long	e_Obj_ID;			///< �I�u�W�F�N�gID
	string			e_Operator;			///< ��Ǝ�
	int				e_Purpose;			///< ?
	string			e_Modify_Date;		///< ��Ɠ���
	int				e_Update_Type;		///< ?
	int				e_User_Claim_F;		///< ?
	int				e_Tmp_Meshcode;		///< �e���|�������b�V���R�[�h
	int				e_Tmp_Prim_ID;		///< �e���|�����v���~�B�e�B�uID
	int				e_Sc1_Disp_Type_C;	///< �X�P�[���P�\���^�C�v�R�[�h(�w�i�E���L���p�j
	int				e_Sc2_Disp_Type_C;	///< �X�P�[���Q�\���^�C�v�R�[�h�i�V�j
	int				e_Sc3_Disp_Type_C;	///< �X�P�[���R�\���^�C�v�R�[�h�i�V�j
	int				e_Sc4_Disp_Type_C;	///< �X�P�[���S�\���^�C�v�R�[�h�i�V�j

	int				e_Sc1_Disp_Code_C;	///< �X�P�[���P�\����ʃR�[�h(���k�ځE�~�h���w�i�p�j
	int				e_Sc2_Disp_Code_C;	///< �X�P�[���Q�\����ʃR�[�h�i�V�j
	int				e_Sc3_Disp_Code_C;	///< �X�P�[���R�\����ʃR�[�h�i�V�j
	int				e_Sc4_Disp_Code_C;	///< �X�P�[���S�\����ʃR�[�h�i�V�j

//	int				e_Disp_Type_C;		///< �s�s�n�}���L�\���^�C�v�R�[�h
	int				e_Under_Ground_F;

	string			e_Cur_Attr_Cls;		///< ���݂̑����N���X��

//	int				e_Cnt_Class_C;		///< �i��
//	int				e_Bg_Class_C;		///< �s�s�n�}�A���k�ڎ��
//	int				e_Railway_Class_C;	///< �S���H��
//	int				e_Station_Class_C;	///< �w��
//	int				e_Bld_Class_C;		///< �����|���S��
	int				e_Class_C;			///< ���݂̑���

	// �s���E�̂�
	string			e_City_Code;		///< �s�撬���R�[�h
	string			e_Addr_Code;		///< �����ڃR�[�h
	int				e_Tmp_Code_F;		///< ���R�[�h
	int				e_Gaiku_Fugo;		///< �X�敄���ԍ�
	string			e_Ext_Gaiku_Fugo;	///< �g���X�敄��
	int				e_Addr_Class_C;
	int				e_Manage_F;			///< �Ǘ��t���O
	int				e_Color_Code;		///< �X��F�ԍ�

	// �����̂�
	int				e_Corridor_F;		///< �n��L���t���O
	int				e_Step_RF;
	int				e_Floors;
	int				e_Spec_F;			///< �i���t���O�H 2002.7.22

	double			e_Shape_Len;
	double			e_Shape_Area;

	// ���L�̂�
	string			e_Str_1;			///< ���̕�����P
	string			e_Str_2;			///< ���̕�����Q
	string			e_Str_3;			///< ���̕�����R
	int				e_Str_Num_1;		///< ���̕������P
	int				e_Str_Num_2;		///< ���̕������Q
	int				e_Str_Num_3;		///< ���̕������R
	int				e_Anno_Class_C;		///< ���L���
	string			e_Tmp_Anno_Class;	///< �e���|�������
//	int				e_Mark_Type;		///< �}�`ID�ɂ����� 2002.7.12�@�R�����g�A�E�g

	// �n�ԃ��C���̂�
	int				e_Chiban_No;		///< �n�Ԕԍ� 2002.7.29
	int				e_Anno_F;			///< �n�Ԓ��L�t���O 2002.7.29

	int				e_FontSize;			///< �t�H���g�T�C�Y
	int				e_Note_ID;			///< ���L�h�c
	int				e_Target_Str_Num;	///< �\�������񂪂P�A�Q�̂ǂ��炩

	int				e_MeshCode;			///< �s�s�n�}�A�x�[�X�A�~�h���A�g�b�v�}�b�v�̃��b�V����

	IGeometry**		e_Geometry_Ptr;		///< �V�F�C�v�ւ̃|�C���^
	
	// �}�`�����̂��߂ɓ_��ɂ΂炵�Ă��܂�
	enum	data_type {
		PTS_LINE	= 0,
		PTS_POLY,
		PTS_POINT
	};
	int				e_Pts_Type;			///< �_��̃^�C�v�i�|���S���A�|�����C���j
//	int				e_iPt_Count;		///< �_��J�E���g
//	int				e_iPart_Count;		///< �_��̃p�[�g��
//	v_v_Dpos		e_vDPtsArray;		///< �W�I���g�����玝���Ă����_��

	/**
		�f�t�H���g�R���X�g���N�^
	*/
	Smp_db_obj_attr	() {
		e_sLayer_Name		= "";
		e_Obj_ID			= 0;
		e_Purpose			= 0;
		e_Update_Type		= 0;
		e_User_Claim_F		= 0;
		e_Tmp_Meshcode		= 0;
		e_Tmp_Prim_ID		= 0;

		e_Sc1_Disp_Type_C	= 0;
		e_Sc2_Disp_Type_C	= 0;
		e_Sc3_Disp_Type_C	= 0;
		e_Sc4_Disp_Type_C	= 0;

		e_Sc1_Disp_Code_C	= 0;
		e_Sc2_Disp_Code_C	= 0;
		e_Sc3_Disp_Code_C	= 0;
		e_Sc4_Disp_Code_C	= 0;

		e_Under_Ground_F	= 0;

//		e_Cnt_Class_C	= 0;		///< �i��
//		e_Bg_Class_C	= 0;		///< �s�s�n�}�A���k�ڎ��
//		e_Railway_Class_C	= 0;	///< �S���H��
//		e_Station_Class_C	= 0;	///< �w��
//		e_Bld_Class_C		= 0;	///< �����|���S��
		e_Class_C		= 0;

		// �s���E�̂�
		e_City_Code		= "";
		e_Addr_Code		= "";
		e_Tmp_Code_F	= 0;		///< ���R�[�h
		e_Gaiku_Fugo	= 0;

		e_Addr_Class_C	= 0;
		e_Manage_F		= 0;		///< �Ǘ��t���O
		e_Color_Code	= 0;		///< �X��F�ԍ�

		// �����̂�
		e_Corridor_F	= 0;
		e_Step_RF		= 0;
		e_Floors		= 0;
		e_Spec_F		= 0;

		// ���L�̂�
		e_Str_Num_1		= 0;		///< ���̕������P
		e_Str_Num_2		= 0;		///< ���̕������Q
		e_Str_Num_3		= 0;		///< ���̕������R
		e_Anno_Class_C	= 0;		///< ���L���
//		e_Mark_Type		= 0;		///< �}�`ID�ɂ�����
		e_FontSize		= 0;		///< �t�H���g�T�C�Y
		e_Note_ID		= 0;
		e_Target_Str_Num= 0;

		// �n�Ԓ��L�̂�
		e_Chiban_No		= 0;
		e_Anno_F		= 0;

		e_Shape_Len		= 0.0;
		e_Shape_Area	= 0.0;

	//	e_Geometry_Ptr	= NULL;

		// �V�K�ɒǉ�
		e_Pts_Type		= 0;			///< �_��̃^�C�v�i�|���S���A�|�����C���j
	//	e_vDPtsArray.clear();			///< �W�I���g�����玝���Ă����_��
	}

	/**
		�I�u�W�F�N�g�̃N���[���A�b�v
	*/
/*	void	Obj_Clean	( void )
	{
		iv_v_Dpos	a_ivIndx;
		for( a_ivIndx = e_vDPtsArray.begin(); a_ivIndx != e_vDPtsArray.end(); a_ivIndx++ )
		{
			a_ivIndx->clear();
		}
	}
*/
	/**
		�W�I���g��������_��ɕϊ�
		@return	bool
	*/
	bool			Inport_Geo	(
						IGeometryPtr	c_ipGeo		///< �W�I���g��
					);

	/**
		�����_����W�I���g���ɕϊ�
		@return	IGeometryPtr	����
		@return	NULL	���s
	*/
	IGeometryPtr	Export_Geo	( void );


	// ���L���C���̔�r�p
	struct less_Note_ID : public std::binary_function<Smp_db_obj_attr, Smp_db_obj_attr, bool>
	{
		// ���L���C���́Ae_Note_ID�Ń\�[�g����ƃT�[�`���y
		int operator () (const Smp_db_obj_attr c_Dat_1, const Smp_db_obj_attr c_Dat_2) const
		{
			int	ret	= 0;
			if( c_Dat_1.e_Note_ID != c_Dat_2.e_Note_ID ) {
				if( c_Dat_1.e_Note_ID < c_Dat_2.e_Note_ID ) {
					ret	= 1;
				}
			}
			return (ret);
		}
	};

	int	operator==(const Smp_db_obj_attr c_Dat) {
			int	ret	= 0;
			if( e_Obj_ID			== c_Dat.e_Obj_ID ) {		///< �I�u�W�F�N�gID
				ret	= 1;
			}
			return	( ret );
	/*			e_Operator			== c_Dat.e_Operator &&		///< ��Ǝ�
				e_Purpose			== c_Dat.e_Purpose &&		///< ?
				e_Modify_Date		== c_Dat.e_Modify_Date &&	///< ��Ɠ���
				e_Update_Type		== c_Dat.e_Update_Type &&	///< ?
				e_User_Claim_F		== c_Dat.e_User_Claim_F;	///< ?
				e_Tmp_Meshcode		== c_Dat.e_Tmp_Meshcode;	///< �e���|�������b�V���R�[�h
				e_Tmp_Prim_ID		== c_Dat.e_Tmp_Prim_ID;		///< �e���|�����v���~�B�e�B�uID
				e_Sc1_Disp_Type_C	== c_Dat.	///< �X�P�[���P�\���^�C�v�R�[�h(�w�i�E���L���p�j
				e_Sc2_Disp_Type_C	== c_Dat.	///< �X�P�[���Q�\���^�C�v�R�[�h�i�V�j
				e_Sc3_Disp_Type_C	== c_Dat.	///< �X�P�[���R�\���^�C�v�R�[�h�i�V�j
				e_Sc4_Disp_Type_C	== c_Dat.	///< �X�P�[���S�\���^�C�v�R�[�h�i�V�j
				e_Under_Ground_F	== c_Dat.
				e_Cur_Attr_Cls		== c_Dat.		///< ���݂̑����N���X��
				e_Class_C			== c_Dat.			///< ���݂̑���
				e_City_Code			== c_Dat.		///< �s�撬���R�[�h
				e_Addr_Code			== c_Dat.		///< �����ڃR�[�h
				e_Tmp_Code_F		== c_Dat.		///< ���R�[�h
				e_Gaiku_Fugo		== c_Dat.		///< �X�敄���ԍ�
				e_Ext_Gaiku_Fugo	== c_Dat.	///< �g���X�敄��
				e_Addr_Class_C		== c_Dat.
				e_Manage_F			== c_Dat.			///< �Ǘ��t���O
				e_Color_Code		== c_Dat.		///< �X��F�ԍ�
				e_Corridor_F		== c_Dat.		///< �n��L���t���O
				e_Step_RF			== c_Dat.
				e_Floors			== c_Dat.
				e_Shape_Len			== c_Dat.
				e_Shape_Area		== c_Dat.
				e_Str_1				== c_Dat.			///< ���̕�����P
				e_Str_2				== c_Dat.			///< ���̕�����Q
				e_Str_Num_1			== c_Dat.		///< ���̕������P
				e_Str_Num_2			== c_Dat.		///< ���̕������Q
				e_Str_Num_3			== c_Dat.		///< ���̕������R
				e_Anno_Class_C		== c_Dat.		///< ���L���
				e_Tmp_Anno_Class	== c_Dat.	///< �e���|�������
				e_Mark_Type			== c_Dat.		///< �}�`ID�ɂ�����
				e_FontSize			== c_Dat.			///< �t�H���g�T�C�Y
				e_Note_ID			== c_Dat.			///< ���L�h�c
				e_Target_Str_Num	== c_Dat.	///< �\�������񂪂P�A�Q�̂ǂ��炩
				e_MeshCode	== c_Dat.			///< �s�s�n�}�A�x�[�X�A�~�h���A�g�b�v�}�b�v�̃��b�V����
				e_Geometry_Ptr	== c_Dat.		///< �V�F�C�v�ւ̃|�C���^*/
		}

/*
	int	operator=(const Smp_db_obj_attr c_Dat) {
			e_Obj_ID			= c_Dat.e_Obj_ID;			///< �I�u�W�F�N�gID
			e_Operator			= c_Dat.e_Operator;			///< ��Ǝ�
			e_Purpose			= c_Dat.e_Purpose;			///< ?
			e_Modify_Date		= c_Dat.e_Modify_Date;		///< ��Ɠ���
			e_Update_Type		= c_Dat.e_Update_Type;		///< ?
			e_User_Claim_F		= c_Dat.e_Under_Ground_F;	///< ?
			e_Tmp_Meshcode		= c_Dat.e_Tmp_Code_F;		///< �e���|�������b�V���R�[�h
			e_Tmp_Prim_ID		= c_Dat.e_Tmp_Prim_ID;		///< �e���|�����v���~�B�e�B�uID
			e_Disp_Scale_1_F	= c_Dat.e_Disp_Scale_1_F;	
			e_Disp_Scale_2_F	= c_Dat.e_Disp_Scale_2_F;
			e_Disp_Scale_3_F	= c_Dat.e_Disp_Scale_3_F;
			e_Disp_Scale_4_F	= c_Dat.e_Disp_Scale_4_F;
			e_Under_Ground_F	= c_Dat.e_Under_Ground_F;

			e_Cur_Attr_Cls		= c_Dat.e_Cur_Attr_Cls;		///< ���݂̑����N���X��

			e_Class_C			= c_Dat.e_Class_C;			///< ���݂̑���

			// �s���E�̂�
			e_City_Code			= c_Dat.e_City_Code;		///< �s�撬���R�[�h
			e_Addr_Code			= c_Dat.e_Addr_Code;		///< �����ڃR�[�h
			e_Tmp_Code_F		= c_Dat.e_Tmp_Code_F;		///< ���R�[�h
			e_Gaiku_Fugo		= c_Dat.e_Gaiku_Fugo;		///< �X�敄���ԍ�
			e_Ext_Gaiku_Fugo	= c_Dat.e_Ext_Gaiku_Fugo;	///< �g���X�敄��
			e_Addr_Class_C		= c_Dat.e_Addr_Class_C;
			e_Manage_F			= c_Dat.e_Manage_F;			///< �Ǘ��t���O

			// �����̂�
			e_Corridor_F		= c_Dat.e_Corridor_F;		///< �n��L���t���O
			e_Step_RF			= c_Dat.e_Step_RF;
			e_Floors			= c_Dat.e_Floors;

			e_Shape_Len			= c_Dat.e_Shape_Len;
			e_Shape_Area		= c_Dat.e_Shape_Area;

			e_Shape_Ptr			= c_Dat.e_Shape_Ptr;		///< �V�F�C�v�f�[�^�ւ̃|�C���^
		}*/

	/** 2006.04.28 �������̂��߂ɒǉ�
		��r�֐�
	*/
	bool	operator<(const Smp_db_obj_attr c_Dat) const {
		bool	a_bRet	= false;
		if( e_Obj_ID != c_Dat.e_Obj_ID ) {
			if( e_Obj_ID < c_Dat.e_Obj_ID ) {
				a_bRet	= true;
			}
		}
		return	( a_bRet );
	}

	/**
		�\���̂�SdeRow����f�[�^���Z�b�g����(ArcObjects�Ή��Łj
		@return	bool
	*/
	bool	Attr_Set_AOBJ	(
					IFeaturePtr		c_Feature		///< �t�F�b�`���Ă����t�B�[�`��[IN]
				//	IGeometryPtr	c_Shape		///< �V�F�C�v�f�[�^
			);

	/**
		���C�����̂��w�肷��
		��	������g��Ȃ��ꍇ�́A���C�����t�B�[���h��
	*/
	void	Set_Layer_Name	(	string	c_sLayer_Name	) {	///< ���C������
				e_sLayer_Name	= c_sLayer_Name;
			}

	/**
		Unicode�̕����񂩂�SJIS�ɂ���
	*/
//	string	Str_to_SJIS		(	CComBSTR*	c_CStr	);	// ������(Unicode)

};

//typedef		vector<Smp_db_obj_attr>				v_Smp_db_obj_attr;
//typedef		vector<Smp_db_obj_attr>::iterator	iv_Smp_db_obj_attr;
typedef		list<Smp_db_obj_attr>				v_Smp_db_obj_attr;
typedef		list<Smp_db_obj_attr>::iterator		iv_Smp_db_obj_attr;
typedef		list<int>							l_int;
typedef		list<int>::iterator					il_int;

typedef		multiset<Smp_db_obj_attr, Smp_db_obj_attr::less_Note_ID>			s_Smp_db_obj_attr_NLine;
typedef		multiset<Smp_db_obj_attr, Smp_db_obj_attr::less_Note_ID>::iterator	is_Smp_db_obj_attr_NLine;


/**
	�s���E�|���S���́A�s���R�[�h���ɂ܂Ƃ߂ď����ł���悤�ɂ���
*/
struct	admin_geo
{
	string			e_lCode_5;	///< �T���s���R�[�h
	string			e_lCode_6;	///< �U���s���R�[�h
	long			e_lOID;		///< ��\�I�u�W�F�N�gID
	long			e_lColor;	///< �F
//	IGeometry**		e_ipGeo;	///< �W�I���g��
	admin_geo	() {
		e_lCode_5		= "";
		e_lCode_6		= "";
		e_lOID			= 0;
		e_lColor		= 0;
//		e_ipGeo			= NULL;
	}
	~admin_geo	() {}

	/**
		��r���Z�q��`
	*/
	int		operator<(const admin_geo&	cDat) const
			{
				int	ret	= 0;
				if( e_lCode_5 != cDat.e_lCode_5 ) {	// �|���S���Ԃ̋���
					if( e_lCode_5 < cDat.e_lCode_5 )
						ret	= 1;
				}else
				if( e_lCode_6 != cDat.e_lCode_6 ) {	// �����𒲂ׂ��x�N�^�z��|���S���̃C���f�b�N�X
					if( e_lCode_6 < cDat.e_lCode_6 )
						ret	= 1;
				}else
				// 2006.07.20 �X��F���Ή��Œǉ�
				if( e_lColor != cDat.e_lColor ) {
					if( e_lColor < cDat.e_lColor )
						ret	= 1;
				}
				return	( ret );
			}
};

typedef		vector<admin_geo>							v_admin_geo;
typedef		vector<admin_geo>::iterator					iv_admin_geo;
typedef		set<admin_geo, less<admin_geo> >			s_admin_geo;
typedef		set<admin_geo, less<admin_geo> >::iterator	is_admin_geo;


#endif
