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

#include <libsmpprninfo.h>
#include <smp_prn_info.h>
#include <feature_info.h>
#include "msi_handle_ws.h"
#include "prog_init_cls.h"
#include "util_class.h"

#include "exec_time_handle.h"
#include "MSiGet_Parts_Info.h"


#ifndef	_MAIN_ROOTIN_H_
#define	_MAIN_ROOTIN_H_

#define		ON_LOGFILE		0x01				///< ���O�t�@�C���w��
#define		ON_MESH			0x02				///< ���b�V�����X�g�w�胂�[�h
#define		ON_MONO_MESH	0x04				///< �P���b�V���R�[�h�w�胂�[�h
#define		ON_PGDB_MODE	0x08				///< PGDB���[�h
#define		ON_OUTPATH		0x10				///< �f�[�^�o�̓p�X
// 2007.08.21
#define		ON_INIT_HZ_AREA	0x20				///< �ЊQ�G���A���������
#define		ON_FC_HZ_AREA	0x80				///< �ЊQ�G���A�t�B�[�`���N���X
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
		string					m_sMeshCode_File;		///< ���b�V���R�[�h���X�g�t�@�C�� 2002.9.24
		string					m_sAddrCode_File;		///< �`�F�b�N�p�Z���R�[�h�t�@�C��
		string					m_sLog_File;			///< ���O�f�[�^�o�̓p�X
		string					m_sInit_Str_HZ_AREA;	///< ������������i�ЊQ�G���A�j
		string					m_sInit_Str_MESH;		///< ������������i���b�V���j
		string					m_sHZ_AREA_FC;			///< �ЊQ�G���A���C�� 2007.08.21
		string					m_sMesh_FC;				///< ���b�V�����C��		
		string					m_sOutPath;				///< �f�[�^�o�̓p�X��
		string					m_sPGDB_File;			///< PGDB�t�@�C��
		int						m_iDigit;				///< ���b�V���R�[�h����
		int						m_iExe_Flag;
		int						m_iMeshCode;			///< ���b�V���R�[�h
		int						m_iCurMeshCode;			///< ���ݎ��s���̃��b�V��
		IFeatureClassPtr		m_ipF_Class;
		FILE*					m_fErr_Fp;				///<�@�G���[���O�t�@�C���p�t�@�C���|�C���^
	private:
		prog_init				m_pTool_Init_NP;		// DB�̏������p�ݒ�
		prog_init				m_pTool_Init_Mesh;		// ���b�V���p

		smp_sindy_err_info		m_sPrnInfo;				// �G���[���o�͗p
		mk_input_list			m_sInput_List;			// ���̓��X�g�p
		exec_time_handle		m_eExeTime;				// ���s���ԊǗ�
		msi_handle_ws			m_mHZ_AREA_WS;			// �ЊQ�G���A�f�[�^

		crd_cnv					m_cCrd;
		util_class				m_cUC;
	private:
		ms_data_info			m_mAll_Data;

		IWorkspacePtr			m_ipWorkspace_HZ_AREA;
		IWorkspacePtr			m_ipWorkspace_Mesh;

		string					m_sVersion;				//< 2009.09.30

	public:

		main_rootin	() {
			m_iDigit				= 0;
			m_iMeshCode				= 0;
			m_iCurMeshCode			= 0;
			m_fErr_Fp				= stderr;	// �f�t�H���g�͕W���G���[�o�͂�
		}
		~main_rootin	() {
			if( m_fErr_Fp != NULL ) {
				fclose ( m_fErr_Fp );
			}
		}
		/**
			SiNDYDB�ւ̐ڑ��I�[�v��
			@return	true	����
			@return	false	���s
		*/
		bool			SiNDY_DB_Open	(
							IWorkspacePtr&			c_ipWorkspace_Note
						);
		/**
			DB�ւ̐ڑ��I�[�v��
			@return	true	����
			@return	false	���s
		*/
		bool			DB_Open	(
							IWorkspacePtr&			c_ipWorkspace,
							char*					c_cpUserName,		///< ���[�U��
							char*					c_cpPassword,		///< �p�X���[�h
							char*					c_cpVersion,		///< �o�[�W����
							char*					c_cpInstance,		///< �C���X�^���X
							char*					c_cpServer			///< �T�[�o��
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
			���b�V����`��DB����l�����郁�\�b�h�̏����� \
			��get_mesh_rect �����O�ɕK���Ă�
		*/
		bool			get_mesh_init	(
							mk_input_list&	c_mMList,			// ���̓��X�g�N���X
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
			�G���[���O�t�@�C�����I�[�v������
		*/
		bool			Open_Err_Log	( void );

		/**
			�ϊ����s��
			�������ŃG���[�t�@�C���̏��������s��
			@return	����	true
			@return ���s	false
		*/
		bool			conv_exe	( void );

		/**
			��`�̃|���S����
			@return	IPolygonPtr
		*/
		IPolygonPtr		Get_Rect_Polygon	(
							IEnvelopePtr	c_ipEnv			///< ���b�V����`
						);

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
		bool			Read_HZ_AREA_to_Shape	( void );

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
			�ЊQ�G���A�f�[�^���Z�b�g�ɗ��Ƃ�
			@return	�f�[�^��
		--------------------------------------------------------------------------------*/
		int				Set_HZ_AREA_Data (
							IGeometryPtr&		c_ipMeshRect,		///< ���b�V����`
							IFeatureCursorPtr&	c_ipNoteCursor, 	///< ���L�J�[�\��
							ms_data_info&		c_mHZ_Area_Data,	///< �ЊQ�|���S���f�[�^
							int					c_iData_Type		///< �f�[�^�^�C�v
						);

		/** ------------------------------------------------------------------------------
			�ЊQ�G���A�f�[�^���V�F�[�v�t�@�C���ɕϊ�
			@return	bool
		--------------------------------------------------------------------------------*/
		bool			HZ_Area_to_Shape (
							ISpatialReferencePtr&	c_ipSpRef,			///< ��ԎQ�Ə��
							ms_data_info&			c_mHZ_Area_Data,	///< �ЊQ�G���A�f�[�^
							char*					c_cpOutFName		///< �o�̓V�F�C�v�t�@�C����
						);

		/** ------------------------------------------------------------------------------
			���s�����o�͂��� 2006.05.17
		--------------------------------------------------------------------------------*/
		void			Print_Check_Info	( void );

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
};
//------------------------------------------------------<�����܂�>



#endif	//_MAIN_ROOTIN_H_
