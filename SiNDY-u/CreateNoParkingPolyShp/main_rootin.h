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
#include <vector>
#include <set>
#include <stdio.h>
#include <string>
#include <crd_cnv.h>
#include <smp_prn_info.h>
#include "msi_handle_ws.h"
#include <feature_info.h>
#include "prog_init_cls.h"
#include "util_class.h"

//include "WKSHelper.h"
//#include "SiINClassTable.h"
//#include "addr_db_handle.h"
//#include "poi_text_handle.h"
#include "exec_time_handle.h"
//#include "stringcomp.h"
//#include "addr_str_work.h"
#include "MSiGet_Parts_Info.h"

//using	namespace	stlport;

#ifndef	_MAIN_ROOTIN_H_
#define	_MAIN_ROOTIN_H_

#define		ON_LOGFILE		0x01				///< ���O�t�@�C���w��
#define		ON_MESH			0x02				///< ���b�V�����X�g�w�胂�[�h
#define		ON_MONO_MESH	0x04				///< �P���b�V���R�[�h�w�胂�[�h
#define		ON_PGDB_MODE	0x08				///< PGDB���[�h
#define		ON_OUTPATH		0x10				///< �f�[�^�o�̓p�X
// 2007.08.21
#define		ON_INIT_NP_AREA	0x20				///< ���Ԉᔽ����G���A���������
#define		ON_FC_NP_AREA	0x80				///< ���Ԉᔽ����G���A�t�B�[�`���N���X
#define		ON_INIT_MESH	0x100				///< ���b�V�����������
#define		ON_FC_MESH		0x200				///< ���b�V���t�B�[�`���N���X

#define		CITY_NOTE		0x01
#define		BASE_NOTE		0x02
#define		MID_NOTE		0x04
#define		STR_FIELD_LENG	128

struct	data_info
{
	int			e_iOID;			///< �I�u�W�F�N�gID
	int			e_iAttrCode;	///< �������ރR�[�h (914@(�ŏd�_)=1 915@(�d�_)=2)
	int			e_iGroupCode;	///< ���ʔԍ�
//	int			e_iAnno_Cls;	///< ���L����
	IGeometryPtr	e_ipGeo;	///< ���L�W�I���g��

	/**
		�R���X�g���N�^
	*/
	data_info	( void ) {
		e_iOID		= 0;
	//	e_dX		= 0.0;
	//	e_dY		= 0.0;
	}
	/**
		�f�X�g���N�^
	*/
	~data_info	( void ) { }

	bool	operator<(const data_info c_dDat ) const {
		bool	ret	= false;
		if( e_iAttrCode != c_dDat.e_iAttrCode ) {
			if( e_iAttrCode > c_dDat.e_iAttrCode ) {
				ret	= true;
			}
		}else
		if( e_iGroupCode != c_dDat.e_iGroupCode ) {
			if( e_iGroupCode < c_dDat.e_iGroupCode ) {
				ret	= true;
			}
		}else
		if( e_iOID != c_dDat.e_iOID ) {
			if( e_iOID < c_dDat.e_iOID ) {
				ret	= true;				
			}
		}
//		if( e_iOID != c_dDat.e_iOID ) {
//			if( e_iOID < c_dDat.e_iOID ) {
//				ret	= true;
//			}
//		}
/*		if( e_dX != c_dDat.e_dX ) {
			if( e_dX < c_dDat.e_dX ) {
				ret	= true;
			}
		}else
		if( e_dY != c_dDat.e_dY ) {
			if( e_dY < c_dDat.e_dY ) {
				ret	= true;
			}
		}*/
	/*	if( e_sNoteStr1 != c_dDat.e_sNoteStr1 ) {
			if( e_sNoteStr1 < c_dDat.e_sNoteStr1 ) {
				ret	= true;
			}
		}
	*/
		return	( ret );
	}
};

typedef	std::multiset<data_info, std::less <data_info> >			ms_data_info;
typedef	std::multiset<data_info, std::less <data_info> >::iterator	ims_data_info;

struct	Norm_Pt
{
	int	e_iX;
	int	e_iY;
	Norm_Pt() {
		e_iX	= 0;
		e_iY	= 0;
	}
};

class	main_rootin
{
	public:
		char*					e_cpMeshCode_File;		///< ���b�V���R�[�h���X�g�t�@�C�� 2002.9.24
		char*					e_cpAddrCode_File;		///< �`�F�b�N�p�Z���R�[�h�t�@�C��
		char*					e_cpGentyou_File;		///< ���������_���X�g�t�@�C��
		char*					e_cpLog_File;			///< ���O�f�[�^�o�̓p�X
		char*					e_cpInit_Str_NP_AREA;	///< ������������i���Ԉᔽ����G���A�j
		char*					e_cpInit_Str_MESH;		///< ������������i���b�V���j
		char*					e_cpNP_AREA_FC;			///< ���Ԉᔽ����G���A���C�� 2007.08.21
		char*					e_cpMesh_FC;			///< ���b�V�����C��		
		char*					e_cpOutPath;			///< �f�[�^�o�̓p�X��
		char*					e_cpPGDB_File;			///< PGDB�t�@�C��
		std::string				e_sWhereString;			///< �����Ɏg�p����Where��
		int						eMake_Type;				///< �f�[�^�쐬�^�C�v
		int						eExe_Type;				///< ���s�^�C�v
		int						eDigit;					///< ���b�V���R�[�h����
		char*					e_cpAddrCode;			///< �Z���R�[�h
		int						eExe_Flag;
		int						e_iMeshCode;			///< ���b�V���R�[�h
		int						e_iCurMeshCode;			///< ���ݎ��s���̃��b�V��
//		IWorkspacePtr			e_ipWorkSpace;
		IFeatureClassPtr		e_ipF_Class;
		FILE*					e_fErr_Fp;				///<�@�G���[���O�t�@�C���p�t�@�C���|�C���^
		std::set<int, std::less<int> >	e_sExi_Mesh_List;		///<�@���݃��b�V���̃��X�g 2002.12.06
		std::set<int>					e_sOID_Set;				///<�@�I�u�W�F�N�gID�̃Z�b�g
	private:

//		SiINClassTable			e_sAttrTable;
		smp_sindy_err_info		e_sPrnInfo;				// �G���[���o�͗p
		mk_input_list			e_sInput_List;			// ���̓��X�g�p
		exec_time_handle		e_eExeTime;				// ���s���ԊǗ�
		msi_handle_ws			e_mNP_AREA_WS;			// ���Ԉᔽ����G���A�f�[�^

		crd_cnv					e_cCrd;
		util_class				e_cUC;
	private:
		ms_data_info			e_mNote_Data;			///< ���Ԉᔽ����G���A�f�[�^
		ms_data_info			e_mAll_Data;
		int						e_iFontChkFlag;			///< �t�H���g�`�F�b�N���邩�ǂ����i�~�h���̂݁j

		IWorkspacePtr			e_ipWorkspace_NP_AREA;
		IWorkspacePtr			e_ipWorkspace_Mesh;

		string					m_sVersion;				//< 2009.09.30

	public:

		main_rootin	() {
			e_cpMeshCode_File		= NULL;
			e_cpAddrCode_File		= NULL;
			e_cpLog_File			= NULL;
			e_cpGentyou_File		= NULL;
			e_cpInit_Str_NP_AREA	= NULL;
			e_cpInit_Str_MESH		= NULL;
			e_cpNP_AREA_FC			= NULL;
			e_cpMesh_FC				= NULL;
			e_cpOutPath				= NULL;
			e_cpPGDB_File			= NULL;
			eMake_Type				= 0;
			eExe_Type				= 0;
			eDigit					= 0;
			eExe_Flag				= 0;
			e_iMeshCode				= 0;
			e_iCurMeshCode			= 0;
			e_fErr_Fp				= stderr;	// �f�t�H���g�͕W���G���[�o�͂�
			e_sExi_Mesh_List.clear();
			e_iFontChkFlag			= 0;
		}
		~main_rootin	() {
			if( e_fErr_Fp != NULL ) {
				fclose ( e_fErr_Fp );
			}
		}

		/**
			DB�ւ̐ڑ��I�[�v��
			@return	���[�N�X�y�[�X�̃|�C���^
		*/
		IWorkspacePtr	DB_Open	(
							char*	c_cpTargetDB	///< �Ώ�DB��
						);

		/**
			������`�F�b�N
			@return	������
		*/
		std::string		Str_to_SJIS	(
							CComBSTR*	c_CStr		///< ������(Unicode)
						);

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
			���b�V�����C���I�[�v��
		*/
		bool			Open_Mesh_Layers	( void );

		/**
			���b�V����`��GeoDB����擾���� \
			��GeoDB�ւ̐ڑ����m������Ă��邱�Ƃ��O��
		*/
		IEnvelopePtr	get_mesh_rect	(
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
		/**
			�ϊ����s��
			�������ŃG���[�t�@�C���̏��������s��
			@return	����	true
			@return ���s	false
		*/
		bool			conv_exe	( void );

		/**
			�`�F�b�N�p�s���E���X�g�A���b�V���R�[�h���X�g�̓ǂݍ���
		*/
		bool			Read_Mesh_List	( void );

		/** ------------------------------------------------------------------------------
			���C���I�[�v��
			@return	����	true
			@return ���s	false
		--------------------------------------------------------------------------------*/
		bool			Open_Layers		( void );

		/** ------------------------------------------------------------------------------
			���b�V���P�ʃt�@�C�����̍쐬
		--------------------------------------------------------------------------------*/
		std::string		make_mesh_fname (
							int		c_iMeshCode,	///< ���b�V���R�[�h
							int		e_iSubPathNum,	///< �T�u�p�X�̊K�w
							char*	c_cpSrcShpPath,	///< �\�[�X�V�F�C�v�p�X
							char*	c_cpAddStr,		///< ���b�V���ւ̒ǉ�������
							string&	c_sMkSubPath	///< �������ꂽ�T�u�f�B���N�g��
						);

		/** ------------------------------------------------------------------------------
			�t�@�C���̃t���p�X������A�V�F�C�v�t�@�C�����𒊏o����֐�
			Function to extract the shape file name from the fullpath of the file
			@return	�Ȃ�
		--------------------------------------------------------------------------------*/
		bool			GetFeatureClass	(
							const std::string&	sourceFile,		///< �\�[�X�t�@�C����
							std::string&		className		///< �N���X��
						);

		/** ------------------------------------------------------------------------------
			�t���p�X������A�t�@�C�����������āA�f�B���N�g������Ԃ��֐�
			// Function to remove the file name from the fullpath and return the directory
			@return	�Ȃ�
		--------------------------------------------------------------------------------*/
		bool			GetFilePath	(
							const std::string&	sourceFile,		///< �\�[�X�t�@�C����
							std::string&		filePath		///< �p�X��
						);

		/** ------------------------------------------------------------------------------
			�f�[�^��ǂݍ��݁AShape�t�@�C���ɕϊ�
			@return	����	true
			@return ���s	false
		--------------------------------------------------------------------------------*/
		bool			Read_NP_AREA_to_Shape	( void );

		/** ------------------------------------------------------------------------------
			�`��N���b�v
		--------------------------------------------------------------------------------*/
		IGeometryPtr	Mk_Clip_Data (
							IGeometryPtr&		c_ipClipPoly,	///< �N���b�v�p�|���S��
							IGeometryPtr&		c_ipTgtGeo, 	///< �N���b�v�����}�`
							int					c_iClipType,	///< �N���b�v���� 0=Intersect 1=Difference
							int*				c_ipEmpty_F		///< ��t���O
						);
		
		/** ------------------------------------------------------------------------------
			���Ԉᔽ����G���A�f�[�^���Z�b�g�ɗ��Ƃ�
			@return	����	true
			@return ���s	false
		--------------------------------------------------------------------------------*/
		bool			Set_NP_AREA_Data (
							IGeometryPtr&		c_ipMeshRect,		///< ���b�V����`
							IFeatureCursorPtr&	c_ipNoteCursor, 	///< ���L�J�[�\��
							ms_data_info&		c_mNP_Area_Data,	///< ���Ԉᔽ�|���S���f�[�^
							int					c_iData_Type		///< �f�[�^�^�C�v
						);

		/** ------------------------------------------------------------------------------
			���Ԉᔽ����G���A�f�[�^���V�F�[�v�t�@�C���ɕϊ�
			@return	bool
		--------------------------------------------------------------------------------*/
		bool			NP_Area_to_Shape (
							ISpatialReferencePtr&	c_ipSpRef,			///< ��ԎQ�Ə��
							ms_data_info&			c_mNP_Area_Data,	///< ���Ԉᔽ����G���A�f�[�^
							char*					c_cpOutFName		///< �o�̓V�F�C�v�t�@�C����
						);

		//------------------------------------------------------
		// 2009.09.30
		// �G���[���b�Z�[�W
		//------------------------------------------------------
		void			PrnMsg	( FILE* c_cpFp,	const char* c_cpFormat, ... ) {
							
							va_list	a_vArgList = NULL;

							va_start(a_vArgList, c_cpFormat );	// ������

							vfprintf_s ( c_cpFp, c_cpFormat, a_vArgList );
							fflush	( c_cpFp );
						}

	private:

};
//------------------------------------------------------<�����܂�>



#endif	//_MAIN_ROOTIN_H_
