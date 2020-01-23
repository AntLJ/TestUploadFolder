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
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <list>
#include <set>
#include "feature_info.h"

#include "util_class.h"
#include "chk_attr_value_tbl.h"
#include "note_relchk.h"
#include "prog_init_cls.h"
#include "SiINClassTable.h"
#include <coord_converter.h>
#include <arctl/coinitializer.h>

using	namespace	std;

#ifndef	_MAIN_ROOTIN_H_
#define	_MAIN_ROOTIN_H_

#define		FLENMAX		32

#define		ON_OUTFILE		0x01
#define		ON_LOGFILE		0x02
#define		ON_MESHLIST		0x04
#define		ON_MONO_MESH	0x08
#define		ON_SUBDIR		0x10
#define		ON_ADDSTR		0x20
#define		ON_NOTEDISP		0x40	// ���L�̊֌W�`�F�b�N�p
#define		ON_MARKDISP		0x80	// ���L�̊֌W�`�F�b�N�p
#define		ON_NOTECOUNT	0x1000	// ���L�J�E���g������
#define		ON_MESHCOUNT	0x2000	// ���b�V���ʃJ�E���g
#define		ON_BGCLASS_TBL		0x4000
#define		ON_ANNOCLASS_TBL	0x8000
#define		ON_INPUT_PATH	0x10000
#define		ON_ALLATTR		0x20000	// 2010.08.19 �ǉ�
#define		ON_DATATYPEOFF	0x40000

#define	IS_SCCLS	1	///< 	�X�P�[�����
#define	IS_SCFLG	2	///<	�X�P�[���t���O

/**
	shape�̃I�u�W�F�N�g�̏��
*/
struct	data_info
{
	int			e_iMeshCode;	// ���b�V���R�[�h
//	unsigned	e_uDataType;	// �f�[�^�^�C�v 0x30, 0x50
//	unsigned	e_uLayerID;		// ���C��ID
	string		e_sFIELD;		// �t�B�[���h��
	int			e_iSindyAttr;	// SiNDY��̑���
	int			e_iUnderF;		// �n���t���O
	unsigned	e_uAttrCode;	// ��ʃR�[�h
	int			e_iSCount[4];	// �X�P�[���J�E���g
	int			e_iSCount_M[4];	// �X�P�[���J�E���g�i�L���j
	int			e_iTrueCount;	// �^�ʒu�J�E���g 2010.08.17
	double		e_dLength;		// ����

	/**
		�R���X�g���N�^
	*/
	data_info	( void ) {
		e_iMeshCode	= 0;
//		e_uDataType	= 0;
//		e_uLayerID	= 0;
		e_sFIELD	= "";
		e_iSindyAttr= 0;
		e_iUnderF	= 0;
		e_uAttrCode	= 0;
		for(int i=0;i<4;i++)
		{
			e_iSCount[i]	= 0;
			e_iSCount_M[i]	= 0;
		}
		e_iTrueCount= 0;
		e_dLength	= 0.0;
	}
	/**
		�f�X�g���N�^
	*/
	~data_info	( void ) { }

	bool	operator<(const data_info c_dDat ) const {
		bool	ret	= false;
		if( e_iMeshCode != c_dDat.e_iMeshCode ) {
			if( e_iMeshCode < c_dDat.e_iMeshCode ) {
				ret	= true;
			}
		}else
//		if( e_uDataType != c_dDat.e_uDataType ) {
//			if( e_uDataType < c_dDat.e_uDataType ) {
//				ret	= true;
//			}
//		}else
//		if( e_uLayerID != c_dDat.e_uLayerID ) {
//			if( e_uLayerID < c_dDat.e_uLayerID ) {
//				ret	= true;
//			}
//		}else
		if( e_iSindyAttr != c_dDat.e_iSindyAttr ) {
			if( e_iSindyAttr < c_dDat.e_iSindyAttr ) {
				ret	= true;
			}
		}else
		if( e_iUnderF != c_dDat.e_iUnderF ) {
			if( e_iUnderF < c_dDat.e_iUnderF ) {
				ret	= true;
			}
		}else
		if( e_uAttrCode != c_dDat.e_uAttrCode ) {
			if( e_uAttrCode < c_dDat.e_uAttrCode ) {
				ret	= true;
			}
		}

		return	( ret );
	}
};

typedef	multiset<data_info, less <data_info> >				ms_data_info;
typedef	multiset<data_info, less <data_info> >::iterator	ims_data_info;

class	main_rootin
{
	public:
		string		m_sSrcShpFile;			///< �Q�ƌ��V�F�C�v�t�@�C��
		string		m_sSrcShpPath;			///< �Q�ƌ��V�F�C�v�t�@�C���܂ł̃p�X
		string		m_sSrcFClass;			///< �t�B�[�`���N���X���i�V�F�C�v�t�@�C�����j
		string		m_sOutInfoFile;			///< �o�͏��t�@�C��
		string		m_sOutLogFile;			///< �o�̓��O�t�@�C��
		int			m_iExe_Flag;
		FILE*		m_fpOutInfoFp;			///< �o�͏��p
		FILE*		m_fpOutLogFp;			///< �o�̓��O�t�@�C��
	//	string		m_sInit_File;			///< �������t�@�C�� 2007.01.09

	//	prog_init	m_pTool_Init;			// DB�̏������p�ݒ� 2007.01.09
		SiINClassTable	m_sAttrTable;		// �����e�[�u�� 2007.01.09
		int			m_iInput_Lv;			// �f�[�^�̃��x�� 2007.01.10
		ms_data_info	m_mData_Info;		// �w�i�E���L�E�s���E 2007.01.10

		int			m_iSubPathNum;			//�@�T�u�K�w�̐�
		string		m_sMeshList;
		string		m_sAddStr;
		set<int>	m_sMesh;
		long		m_lMeshCode;			///< �P���b�V���w�莞�̃��b�V���R�[�h

		string		m_DB_Init;              //!< DB�ڑ�

		int			m_iDigit;

		// ���L�`�F�b�N�p
		string		m_sNoteDispFile;		///< ���L�\�����t�@�C��
		string		m_sMarkDispFile;		///< �L���\�����t�@�C��
		string		m_sDispRuleFile;		///< �\�����[�����L�q�����t�@�C���iDB����G�N�X�|�[�g���č쐬�B���Ƃ�DB���ɂ��Ή����������j
		string		m_sAttrTableName;		///< �����e�[�u����

		string		m_sVersion;

		util_class	m_uUC;
		crd_cnv		m_cCrd;

	//	chk_attr_value_tbl		m_cAttValTbl;		///< �����l�e�[�u���N���X
		string					m_sAttValTblFile;	///< �����l�e�[�u���t�@�C��
		esriGeometryType		m_eGeometryType;	///< �W�I���g���^�C�v

		enum {
			BGP	= 0,
			BGL,
			BLD,
			RAL,
			USP,
			STA,
			CNT,
			ADM,
			DCL
		};

	public:
		main_rootin	()
		{
			m_iExe_Flag		= 0;
			m_fpOutInfoFp	= stdout;
			m_fpOutLogFp	= stderr;
			m_iInput_Lv		= 0;
			m_iSubPathNum	= 0;
			m_lMeshCode		= 0;
			m_iDigit		= 0;
			m_eGeometryType	= esriGeometryPoint;
		}
		~main_rootin ()
		{
		/*	if( e_cpSrcShpPath != NULL ) {
				delete [] e_cpSrcShpPath;
				e_cpSrcShpPath	= NULL;
			}
			if( e_cpSrcFClass != NULL ) {
				delete [] e_cpSrcFClass;
				e_cpSrcFClass	= NULL;
			}*/
			if (m_fpOutInfoFp != NULL) {
				fflush(m_fpOutInfoFp);
				if (m_fpOutInfoFp != stdout)
					fclose(m_fpOutInfoFp);
			}
			m_fpOutInfoFp = stdout;

			if (m_fpOutLogFp != NULL) {
				fflush(m_fpOutLogFp);
				if (m_fpOutLogFp != stderr)
					fclose(m_fpOutLogFp);
			}
			m_fpOutLogFp = stderr;
		}

		/**
			�����`�F�b�N
			@return	����	true
			@return	����	false
		*/
		bool	arg_check	(
			int		argc,			///< ������
			char	**argv			///< ������
		);

		/**
			�c�[���̎g�p�@�̕\��
		*/
		void	print_usage	(
			char	**argv			///< ������
		);

		/**
			�o�[�W�������̕\��
		*/
		void	print_version ( void );

		/**
			�t�@�C���̃t���p�X������A�V�F�C�v�t�@�C�����𒊏o����֐�
			Function to extract the shape file name from the fullpath of the file
			@return	�Ȃ�
		*/
		bool	GetFeatureClass	(
			const char*	sourceFile,		///< �\�[�X�t�@�C�����i�p�X���܂ށj
			string&		className		///< �t�B�[�`���N���X��
		);

		/**
			�t���p�X������A�t�@�C�����������āA�f�B���N�g������Ԃ��֐�
			// Function to remove the file name from the fullpath and return the directory
			@return	�Ȃ�
		*/
		bool	GetFilePath	(
			const char*		sourceFile,		///< �\�[�X�t�@�C����
			string&			filePath		///< �p�X��
		);

		/**
			�����s��
		*/
		bool 	exec_command(
			IWorkspacePtr	c_ipWSpace		///< ���[�N�X�y�[�Xvoid
		);

		/**
			���L�ȊO�̃f�[�^�̃J�E���g���s��
		*/
		bool	count_info (
			int		c_iMeshCode,
			char*	c_cpSrcShpPath,
			char*	c_cpSrcFClass
		);

		/**
			���L�̑����J�E���g���s��
		*/
		bool	count_note_info (
			int						c_iMeshCode,
			v_Feature_All_Attrs&	c_vpFSrc_Data,
			v_Feature_All_Attrs&	c_vpFNDF_Data,
			v_Feature_All_Attrs&	c_vpFMDF_Data
		);

		/**
			����ǂݍ���
		*/
		bool	read_info (
			int						c_iMeshCode,
			char*					c_cpSrcShpPath,
			char*					c_cpSrcFClass,
			v_Feature_All_Attrs*	c_vpFAttr_List
		);

		/**
			�f�[�^���`�F�b�N�p�̍\���ɂ���
			@return	bool
		*/
		bool	mk_data_for_chk	(
			v_Feature_All_Attrs&	c_vpFSrc_Data,
			mm_note_layer*			c_mpNote_Data
		);
		/**
			�f�[�^���`�F�b�N�p�̍\���ɂ���
			@return	bool
		*/
		bool	mk_data_for_chk (
			v_Feature_All_Attrs&	c_vpFSrc_Data,
			mm_note_disp_info*		c_mpNote_Disp_Data
		);
		/**
			�f�[�^���`�F�b�N�p�̍\���ɂ���
			@return	bool
		*/
		bool	mk_data_for_chk (
			v_Feature_All_Attrs&	c_vpFSrc_Data,
			mm_mark_disp_info*		c_mpMark_Disp_Data
		);

		/**
			�X�P�[����ʂ̔���
			@return	0	�X�P�[����ʂłȂ�
			@return	1	�X�P�[�����
			@return	2	�X�P�[���t���O
		*/
		int		Scale_Attr_Chk	(
			string&		a_sFieldName,	///< �t�B�[���h��
			int*		c_ipScale		///< �Y���X�P�[��
		);

		/**
			�\�����[���̑Ó������m�F
		*/
		bool	Chk_Disp_Rule	(
			int		c_iDispState,	///< �\�����
			int		c_iRuleType 	///< �\�����[���̒l
		);

		/**
			���L�\���̃X�P�[���Ԑ����`�F�b�N
		*/
		bool	Chk_Scale_Disp_Rule (
			int				c_iDigit,			///< �Y�����x����\�����b�V������
			int*			c_ipNote_State,		///< ���L�̕\�����
			note_disp_rule&	c_nFoundRule		///< ���L�̃��[��
		);

		/**
			���b�V���P�ʃt�@�C�����̍쐬
		*/
		string	make_mesh_fname (
			int		c_iMeshCode,	///< ���b�V���R�[�h
			char*	c_cpSrcShpPath,	///< �\�[�X�V�F�C�v�p�X
			char*	c_cpAddStr		///< ���b�V���ւ̒ǉ�������
		);

		/** 2007.01.09
			DB�ւ̐ڑ��I�[�v��
			@return	true	����
			@return	false	���s
		*/
		bool	DB_Open	(
			IWorkspaceFactoryPtr&	c_ipWorkspaceFactory,
			IPropertySetPtr&		c_ipPropertySet,
			IWorkspacePtr&			c_ipWorkspace
		);

		/** 2007.01.10
			���̒ǉ�
			@return	��
		*/
		void	Put_Info	(
			data_info		c_dInfo		///< �������݃f�[�^[IN]
		);

		/** 2007.01.10
			���̏o��
			@return	����
		*/
		void	Print_Info	(
			ms_data_info*	c_mpmspCur_DSets,	///< ���ݒ��ڒ��̃f�[�^
			int				c_iData_Level,		///< �f�[�^���x��
			FILE*			c_fpOutFp			///< �o�̓t�@�C���|�C���^
		);

		/** 2007.01.10
			SiNDY��̑����t�B�[���h�����擾
		*/
		bool	GetDBAttrFld(
			string*			c_sSiNDYFld,	///< SiNDY��̃t�B�[���h��[OUT]
			int*			c_ipDataType,	///< �f�[�^�^�C�v[OUT]
			int*			c_ipShpType		///< �V�F�C�v�f�[�^�̃^�C�v[OUT]
		);
};

#endif
