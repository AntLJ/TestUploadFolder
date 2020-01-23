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

#ifndef	__MSIGET_PARTS_INFO_H__
#define	__MSIGET_PARTS_INFO_H__

using	namespace	std;	// 2007.08.23 �C��

#include <stdio.h>
#include <vector>
#include <WKSHelper.h>	// 2003.08.21 QueryExterior���̑��̖��ɂ�肱����őΉ����邱�Ƃɂ���

/**
	2002.10.16	Get_Geo_from_Part��ǉ�
	2002.10.16	Get_Part_Polygon��simplify��p���ďC��
	2003.06.13	Get_Part_Polygon��simplifypreservefromto()��p���ďC��
	2003.10.25	Init()���ɁA�|���S���̏�Ԃ����Đ���ȏ�Ԃɍ\�z���������[�h��ǉ�
*/

#define	GPI_NORMAL_MODE		0x00	///< �m�[�}�����[�h
#define	GPI_RECONFIG_MODE	0x01	///< �|���S���č\�����[�h 2003.10.25

/**
	�W�I���g���̃p�[�g�����l������
*/
class	msi_get_parts_info
{
	long					e_lPart_Num;	///< �p�[�g��
	long					e_lSubPart_Max;	///< �T�u�p�[�g���̍ő吔
	IGeometryPtr			e_ipGeo;		///< �W�I���g��
	IGeometryCollectionPtr	e_ipGeoCol;		///< �W�I���g���R���N�V����
	esriGeometryType		e_eGeoType;		///< �W�I���g�����̂̃I�u�W�F�N�g�̃^�C�v
	IPolygon2Ptr			e_ipPolygon;	///< �|���S���C���^�t�F�[�X
	// ���Ǘp�ɒǉ� 2003.08.21 ------------------------------
	vector<RING>			e_vOuter;		///< �A�E�^�[�����O���
	vector<RING>			e_vChk_Rings;	///< �S�����O���i�`�F�b�N�p�j
	list<RING>				e_lRings;		///< 2003.08.21
	WKSPoint*				e_wpPoints;		///< 2003.08.21
	// -----------------------------------------------------
	// �G���[�o�͗p
	long					e_lObj_ID;		///< �I�u�W�F�N�gID
	string					e_sLayer_Name;	///< ���C������

	int						e_iMeshCode;	///< ���b�V���R�[�h
	FILE*					e_fpLog;		///< ���O�t�@�C���p

private:
	/**
		����Init����Ă��邩���`�F�b�N���A���炽�Ɏw�肳�ꂽ�I�u�W�F�N�g�Ł@\
		���������邩�����߂邽�߂Ɏg�p����
	*/
	void	Pre_Init_Chk	( void ) {
		// 2003.08.21 �ǉ�
		if( e_wpPoints != NULL ) {
			delete [] e_wpPoints;
			e_wpPoints = NULL;
		}
	
	}

	/**
		�A�E�^�[�����O�p�̗̈�̊m��
		@return	bool
	*/
	bool	Get_Outer_Buf	( void ) {
		// �_�~�[���
		return	( true );
	}

	/**
		�C���i�[�����O�p�̗̈�̊m��
		@return bool
	*/
	bool	Get_Inner_Buf	( void ) {
		// �_�~�[���
		return	( true );
	}

	/**
		�A�E�^�[�����O�p�̗̈�̍폜
		@return	�Ȃ�
	*/
	void	Del_Outer_Buf	( void ) {
		// �_�~�[���
	}


	/**
		�C���i�[�����O�p�̗̈�̍폜
		���C���i�[�́A�A�E�^�[�����O�̈����ɉ�������K�v������
		@return	�Ȃ�
	*/
	void	Del_Inner_Buf	( void ) {
		// �_�~�[���
	}

	
	/**
		�G���[�v�����g
		@return	����
	*/
	void			Error_Print	(
						char*	c_cpErr_Code,	///< �G���[�R�[�h
						char*	c_cpErr_Str		///< �o�̓G���[������
					);

	/** 2003.10.02
		�����O���Ȃ������钆���������O�i�T�u�p�[�g�j�̃`�F�b�N�p
		����{�I��Init�������Ŏg�p
		@return	�����������O��
	*/
	int				Inner_Ring_Chk(
						int		c_iIndx			///< �|���S���̃C���f�b�N�X
					);

	/** 2003.10.03
		�����O�ɑ΂��ē��������O�����̒������|���S�����ǂ������`�F�b�N����
		��Init��Ɏg�p
		@return	bool
	*/
	bool			Is_Inner_Ring(
						RING&	c_rOuter,	///< �O���|���S��
						RING&	c_rInner 	///< �����|���S��
					);
	/**�@2003.10.25
		�|���S���̏�Ԃ����čč\������i���p�[�g�������̂P���x������|���S���̏ꍇ�ɍs���j
		@return	TRUE	�č\���������������΂���
		@return	FALSE	�č\�����������������ꍇ
	*/
	bool			Reconfig_Polygon( void );

public:
	/**
		�R���X�g���N�^
	*/
	msi_get_parts_info	() {
		e_lPart_Num		= 0;
		e_lSubPart_Max	= 0;
		e_lObj_ID		= 0;		///< �I�u�W�F�N�gID
		e_wpPoints		= NULL;		///< 2003.08.21 �ǉ�

		e_iMeshCode		= 0;		///< ���b�V���R�[�h
		e_fpLog			= stderr;	///< ���O�t�@�C���p
	}
	/**
		�f�X�g���N�^
	*/
	~msi_get_parts_info	() {
		// ���ǂ�Pre_Init_Chk�Ɠ������Ƃ���肽���̂ł��̂܂܎g�p
		Pre_Init_Chk	( );
	
	}

	/**
		������(���̂Ƃ��ɏ������o��)
		��2002.10.9�@Init��������Ă΂ꂽ�Ƃ��ɂ��Ή�
		@return	���������� true
		@return ���������s false�@
	*/
	bool	Init	( 
				IGeometryPtr	c_ipGeo,		///< �W�I���g��
				long			c_lObj_ID,		///< �I�u�W�F�N�gID
				string			c_sLayer_Name,	///< ���C������
				int				c_iMeshCode,	///< ���b�V���R�[�h
				FILE*			c_fpLog=stderr,	///< ���O�t�@�C���p
				int				c_iMode=GPI_NORMAL_MODE	///< �č\�����[�h�̐ݒ�i�f�t�H���g�͍č\�����Ȃ��j
			);

	/**
		�p�[�g���̊l��
		@return	�p�[�g��
	*/
	long	Get_Part_Num	( void );

	/**
		�T�u�p�[�g���̊l��
		@return	�T�u�p�[�g��
	*/
	long	Get_SubPart_Num	(
				long	c_lPart_Num	///< �w��p�[�g�ԍ�
			);
	
	/**
		�w��p�[�g�A�T�u�p�[�g�̓_��̊l��
		@return IPointCollection�_��
	*/
	IPointCollectionPtr	Get_Points	(
				long	c_lPart_Num,	///< �p�[�g�ԍ�
				long	c_lSubPart_Num	///< �T�u�p�[�g�ԍ�
			);

	/**
		�V�F�C�v�̎w��p�[�g�A�T�u�p�[�g����A�V�F�C�v���쐬����
		@return	true�@����
		@return false ���s
	*/
	IGeometryPtr	Get_Geo_from_Part_Subpart	(
				long	c_lPart_Num,	///< �p�[�g�ԍ�
				long	c_lSubPart_Num	///< �T�u�p�[�g�ԍ�
			);

	/**
		�w��p�[�g�̃W�I���g�������o��
		
		@return �w��p�[�g�̃W�I���g��
	*/
	IGeometryPtr		Get_Geo_from_Part	(
				long	c_lPart_Num		///< �p�[�g�ԍ�
			);

	/**
		�w��p�[�g�̃|���S�������o���i�|���S���̏ꍇ�j
		���^����ꂽ�W�I���g�����|���S���ł���ꍇ�̂݁B�������C�����Ŏg�p�����ꍇ
		�@�ɂ́ANULL��Ԃ�
		@return �w��p�[�g�̃|���S��
	*/
	IPolygonPtr		Get_Part_Polygon	(
				long	c_lPart_Num		///< �p�[�g�ԍ�
			);
	
};
#endif
