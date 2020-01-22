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

#include <stdio.h>
#include <StdAfx.h>
#include <vector>
#include <set>
#include <stdio.h>
#include <string>
#include <crd_cnv.h>
#include "feature_info.h"
#include "prog_init_cls.h"
#include <smp_prn_info.h>
#include "msi_handle_ws.h"

using	namespace	std;	// 2007.09.03 �C��

#ifndef	_MAIN_ROOTIN_H_
#define	_MAIN_ROOTIN_H_

#define		ON_MESH			0x01				///< ���b�V���R�[�h�w�胂�[�h 2002.9.24
#define		ON_MONO_MESH	0x02				///< �P���b�V���w��	2003.04.07
#define		ON_LOG_FILE		0x04				///< ���O�t�@�C���o��
#define		ON_PGDB_MODE	0x08				///< PGDB���[�h 2007.05.21
#define		ON_INIT_FILE	0x10				///< �������ݒ�t�@�C�� 2007.10.25

#define	IS_SCCLS	1	///< 	�X�P�[�����
#define	IS_SCFLG	2	///<	�X�P�[���t���O

struct	Geo_Rect
{
	double	xmin;
	double	ymin;
	double	xmax;
	double	ymax;

	Geo_Rect () {
		xmin	= 0.0;
		ymin	= 0.0;
		xmax	= 0.0;
		ymax	= 0.0;
	}
	~Geo_Rect() {
	}
	bool	operator<(const Geo_Rect c_cDat ) const {
		bool	ret	= 0;
		if( xmin != c_cDat.xmin ) {
			if( xmin < c_cDat.xmin ) {
				ret	= 1;
			}
		}else
		if( ymin != c_cDat.ymin ) {
			if( ymin < c_cDat.ymin ) {
				ret	= 1;
			}
		}else
		if( xmax != c_cDat.xmax ) {
			if( xmax < c_cDat.xmax ) {
				ret	= 1;
			}
		}else
		if( ymax != c_cDat.ymax ) {
			if( ymax < c_cDat.ymax ) {
				ret	= 1;
			}
		}
		return	( ret );
	}
	bool	operator!=(const Geo_Rect c_cDat ) const {
		bool	ret	= 1;
		if( xmin == c_cDat.xmin &&
			ymin == c_cDat.ymin &&
			xmax == c_cDat.xmax &&
			ymax == c_cDat.ymax )
		{
			ret	= 0;
		}
		return	( ret );
	}
	void	operator=( Geo_Rect c_cDat ) {
		xmin	= c_cDat.xmin;
		ymin	= c_cDat.xmin;
		xmax	= c_cDat.xmax;
		ymax	= c_cDat.ymax;
	}
	//--------------------------------------------------------------------
	//	��`���m�̌����`�F�b�N
	//	�O�ڒ����`�ŁA�����Ƀ`�F�b�N����
	//	�Ԃ�l
	//		��`1�Ƌ�`2������ = 1
	//		���� = 1 �O = 0
	//--------------------------------------------------------------------
	bool	Rect_Cross_Chk	(	Geo_Rect c_cDat	)	//	��`2
	{
		// �O�������Ń`�F�b�N����
		if((xmax < c_cDat.xmin ||
			xmin > c_cDat.xmax ||
			ymax < c_cDat.ymin ||
			ymin > c_cDat.ymax))
		{
			return(false);
		}
		return(true);
	}
};

/**
	IGeometryPtr���̂�vector�Ƃ��̎g�p�����܂������Ȃ��̂ŁA�v�f��IGeometryPtr��
	���\���̂��쐬����	2002/05/23
*/
struct	geometry_inc
{
	int				e_iOid;			// �I�u�W�F�N�gID
	IGeometryPtr	e_ipGeometry;	// �W�I���g��
	Geo_Rect		e_igRect;		// ��`

	geometry_inc	(){
		e_iOid	= 0;
	};
	geometry_inc	(IGeometryPtr	c_ipGeometry)
	{
		e_iOid			= 0;
		e_ipGeometry	= c_ipGeometry;
	}
	~geometry_inc	(){};

	bool	operator<(const geometry_inc c_cDat ) const {
		bool	ret	= 0;
		if( e_igRect != c_cDat.e_igRect ) {
			if( e_igRect < c_cDat.e_igRect ) {
				ret	= 1;
			}
		}
		return	( ret );
	}

};

typedef		vector<geometry_inc>				v_geometry_inc;
typedef		vector<geometry_inc>::iterator		iv_geometry_inc;

typedef		multiset<geometry_inc,less<geometry_inc> >	ms_geometry_inc;
typedef		multiset<geometry_inc,less<geometry_inc> >::iterator	ims_geometry_inc;

/**
	�`�F�b�N�ς݂��m�F����悤
*/
struct	checked_id
{
	int		e_iOid_1;
	int		e_iOid_2;
	checked_id	() {
		e_iOid_1	= 0;
		e_iOid_2	= 0;
	}
	~checked_id() {}

	bool	operator<(const checked_id c_cDat ) const {
		bool	ret	= 0;
		if( e_iOid_1 != c_cDat.e_iOid_1 ) {
			if( e_iOid_1 < c_cDat.e_iOid_1 ) {
				ret	= 1;
			}
		}else
		if( e_iOid_2 != c_cDat.e_iOid_2 ) {
			if( e_iOid_2 < c_cDat.e_iOid_2 ) {
				ret	= 1;
			}
		}
		return	( ret );
	}
};

class	main_rootin
{
	public:
		char*					e_cpMeshCode_File;		///< ���b�V���R�[�h���X�g�t�@�C�� 2002.9.24
		char*					e_cpLog_File;			///< ���O�f�[�^�o�̓p�X
		char*					e_cpInit_File;			///< �������t�@�C��
		char*					e_sCheck_Layer;			///< �`�F�b�N���C����
		char*					e_cpPGDB_File;			///< PGDB�t�@�C���� 2007.05.21
		int						eMake_Type;				///< �f�[�^�쐬�^�C�v 2002.06.26
		int						eExe_Type;				///< ���s�^�C�v
		int						eDigit;					///< ���b�V���R�[�h����
		int						eExe_Flag;
		int						e_iMeshCode;			///< �w�胁�b�V���R�[�h 2003.4.7
		double					e_iRange_Dist;			///< �ݒ�͈́im�P�ʁj
//		IWorkspacePtr			e_ipWorkSpace;
		IFeatureClassPtr		e_ipF_Class;
		FILE*					e_fErr_Fp;				///<�@�G���[���O�t�@�C���p�t�@�C���|�C���^
		set<int, less<int> >	e_sExi_Mesh_List;		///<�@���݃��b�V���̃��X�g 2002.12.06
		smp_sindy_err_info		e_sErr_Log;				///<  �G���[���O�p 2007.05.21
		prog_init				e_pTool_Init;			// DB�̏������p�ݒ�

	public:

		main_rootin	()
		{
			e_cpMeshCode_File		= NULL;
			e_cpLog_File			= NULL;
			e_cpInit_File			= NULL;
			e_sCheck_Layer			= NULL;
			e_cpPGDB_File			= NULL;
			eMake_Type				= 0;
			eExe_Type				= 0;
			eDigit					= 0;
			eExe_Flag				= 0;
			e_iMeshCode				= 0;
			e_iRange_Dist			= 0.0;
			e_fErr_Fp				= stderr;	// �f�t�H���g�͕W���G���[�o�͂�
			e_sExi_Mesh_List.clear();
		}
		~main_rootin	()
		{
			if( e_fErr_Fp != NULL ) {
				fclose ( e_fErr_Fp );
			}
		}

		/**
			�����`�F�b�N
			@return	����	true
			@return	����	false
		*/
		bool			arg_check	(
							int		argc,			///< ������
							char	**argv			///< ������
						);

		/**
			�c�[���̎g�p�@�̕\��
		*/
		void			print_usage	(
							char	**argv			///< ������
						);

		/**
			�o�[�W�������̕\��
			@return		����
		*/
		void			print_version	( void );


		/**
			���b�V�����X�g�t�@�C������Ԃ�
			@return		�t�@�C�����ւ̃|�C���^
		*/
		char*			Ret_Mesh_File	( void ) {
							return	( e_cpMeshCode_File );
						}

		/**
			���O�f�[�^�o�̓t�@�C������Ԃ�
			@retur		�p�X���ւ̃|�C���^
		*/
		char*			Ret_Log_File	( void ) {
							return	( e_cpLog_File );
						}

		/**
			�������t�@�C������Ԃ�
			@return		�t�@�C�����ւ̃|�C���^
		*/
		char*			Ret_Init_File	( void ) {
							return	( e_cpInit_File );
						}

		/**
			�`�F�b�N���郌�C���̖���
			@return		�t�@�C�����ւ̃|�C���^
		*/
		char*			Ret_Check_Layer	( void ) {
							return	( e_sCheck_Layer );
						}

		/**
			���b�V����`��DB����l�����郁�\�b�h�̏����� \
			��get_mesh_rect �����O�ɕK���Ă�
		*/
		bool			get_mesh_init	(
							string			c_sMeshLayer,		///< ���b�V�����C����
							IWorkspacePtr	c_ipWorkSpace		///< �w�肷�郏�[�N�X�y�[�X
						);
		/**
			���b�V����`��GeoDB����擾���� \
			��GeoDB�ւ̐ڑ����m������Ă��邱�Ƃ��O��
		*/
		IEnvelopePtr	get_mesh_rect	(
							int				c_iMeshCode			///< ���b�V���R�[�h
						);

		/**
			���b�V�����X�g��GeoDB�ォ��擾����
			@return	bool
		*/
		bool			get_mesh_list	(
							long	c_lDigit					///< ���b�V���R�[�h�̌���
						);

		/**
			�w�胁�b�V�������݂��邩�̃`�F�b�N
			@return	bool
		*/
		bool			chk_mesh_exist	(
							int				c_iMeshCode			///< ���b�V���R�[�h
						);
	
		/**
			�G���[���O�t�@�C�����I�[�v������
		*/
		bool			Open_Err_Log	( void );

		/**
			�G���[���O�𑗂�t�@�C���|�C���^��Ԃ�
		*/
		FILE*			Log_Fp			( void ) {
							return	( e_fErr_Fp );
						}

		// 2006.05.16
		/**
			�X�P�[����ʂ̔���
			@return	0		�X�P�[����ʂłȂ�
			@return	IS_SCCLS(1)	�X�P�[�����
			@return	IS_SCFLG(2)	�X�P�[���t���O
		*/
		int				Scale_Attr_Chk	(
							string&		c_sFieldName,	///< �t�B�[���h��
							int*		c_ipScale		///< �Y���X�P�[��
						);

		/**
			�X�P�[����ʂ̐������̔���
		*/
		int				Chk_Scale_Class_Adjust (
							int*		c_ipScaleAttr
						);

		/**
			�X�P�[���t���O�̐������̔���
		*/
		int				Chk_Scale_Flag_Adjust (
							int*		c_ipScaleAttr
						);

		/**
			DB�ւ̐ڑ��I�[�v��
			@return	true	����
			@return	false	���s
		*/
		bool			DB_Open	(
							IWorkspaceFactoryPtr&	c_ipWorkspaceFactory,
							IPropertySetPtr&		c_ipPropertySet,
							IWorkspacePtr&			c_ipWorkspace
						);

		/**
			�`�F�b�N���s��
			�������ŃG���[�t�@�C���̏��������s��
			@return	����	true
			@return ���s	false
		*/
		bool			check_exe	( void );

		/**
			���C�����̂���K�v�ȃ��[�N�X�y�[�X�𔻒肷��
		*/
		IWorkspacePtr	Get_Ref_Workspace	(
							IWorkspacePtr	c_ipWSpace_Map,		///< ���[�N�X�y�[�X
							IWorkspacePtr	c_ipWSpace_Base,	///< ���[�N�X�y�[�X
							IWorkspacePtr	c_ipWSpace_Addr,	///< ���[�N�X�y�[�X
							char*			c_cpLayerName
						);

};

#endif	//_MAIN_ROOTIN_H_
