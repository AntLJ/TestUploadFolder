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

#ifndef	_MAIN_ROOTIN_H_
#define	_MAIN_ROOTIN_H_

#include <sindy/schema.h>
#include "sindy/workspace/name.h"

#define		ON_MESH			0x02				///< ���b�V���R�[�h�w�胂�[�h 2002.9.24
#define		ON_INNER_CHK	0x04				///< ���k�ڍs���E����`�F�b�N���[�h�@2002.11.13
#define		ON_MONO_MESH	0x08				///< �P���b�V���w��	2002.04.01
#define		ON_LOGFILE		0x10				///< ���O�t�@�C���w��
#define		ON_INIT_FILE	0x20				///< �������t�@�C���ǂݍ��ݗp
// 2004.02.03
#define		ON_ROADNUM_ONLY	0x40				///< ���H�ԍ��̂݃`�F�b�N���[�h
#define		ON_FELLY_ONLY	0x80				///< �t�F���[�֌W�̂݃`�F�b�N���[�h
#define		ON_PGDB_MODE	0x100				///< PGDB�t�@�C�����[�h�iSiNDY-C�Ƃ͕ʁj
#define		ON_OUT_FUYOU_MESSAGE		0x200	///< �ו����s�v�̂��̂ɂ��Ă����O���o��
#define		ON_DEL_DISP_LINE_MESSAGE	0x400	///< �\�����C���ɂ��Ẵ��b�Z�[�W���o�͂��Ȃ�
#define		ON_NEAREST_CHK				0x800	///< ��ԋ߂��H���Ƃ̂݃`�F�b�N���s��

struct	disp_pt_info
{
	int			e_iStat;	// 1=�^�ʒu�A0=�\��������
	int			e_iOid;
	int			e_iScale;
	IPointPtr	e_ipPt;
	disp_pt_info(){
		e_iStat	= 0;
		e_iOid	= 0;
		e_iScale= 0;
	}
};

struct	data_info
{
	int					m_iOID;			///< �I�u�W�F�N�gID
//	int					m_iAttr;		///< ���
//	int					m_iUnder_Flag;	///< �n���t���O
	Feature_All_Attrs	m_faaAttr;		///< �I�u�W�F�N�g�̎��S�����l
	IGeometryPtr		m_ipGeo;		///< �W�I���g��
	/**
		�R���X�g���N�^
	*/
	data_info	( void ) {
		m_iOID	= 0;
	}
	/**
		�f�X�g���N�^
	*/
	~data_info	( void ) { }

	bool	operator<(const data_info c_dDat ) const {
		bool	ret	= false;
		if( m_iOID != c_dDat.m_iOID ) {
			if( m_iOID < c_dDat.m_iOID ) {
				ret	= true;
			}
		}
		return	( ret );
	}
/*	bool	operator!=(const data_info c_dDat ) const {
		bool	ret	= false;
		if( e_iMeshCode != c_dDat.e_iMeshCode ||
			e_iPrim_ID != c_dDat.e_iPrim_ID )
		{
			ret	= true;
		}
		return	( ret );
	}*/
};

typedef	multiset<data_info, less <data_info> >				ms_data_info;
typedef	multiset<data_info, less <data_info> >::iterator	ims_data_info;
typedef	set<data_info, less <data_info> >					s_data_info;
typedef	set<data_info, less <data_info> >::iterator			is_data_info;

class	main_rootin
{
public:
	///���O�ɏo�͂���G���[���b�Z�[�W�R�[�h�̒�`
	enum ERROR_MESSAGE_CODE {
		NONE_SUBDIVIDE,      //!< �ו����̕K�v����
		ABLE_SUBDIVIDE,      //!< �ו����\
		RAIL_INCONSISTENT,   //!< �H���ƕs����
		GROUND_INCONSISTENT, //!< �n��n��������
		NON_SUBJECT,         //!< �ΏۊO���
	};

	char*				e_cpAdminCode_File;		///< �s���R�[�h���X�g�t�@�C��
	char*				e_cpMeshCode_File;		///< ���b�V���R�[�h���X�g�t�@�C�� 2002.9.24
	char*				e_cpAddrTxt_Path;		///< �Z��DB�e�L�X�g�p�X
	char*				e_cpLog_File;			///< ���O�f�[�^�o�̓p�X
	char*				e_cpInit_File;			///< �������t�@�C��
	char*				e_cpErrList;			///< �G���[���X�g�t�@�C��
	int					eMake_Type;		///< �f�[�^�쐬�^�C�v 2002.06.26
	int					eExe_Type;		///< ���s�^�C�v
	int					m_iDigit;		///< ���b�V���R�[�h����
	int					eExe_Flag;
	double				e_Warn_Dist;	///< ���[�j���O����
	double				e_Err_Dist;		///< �G���[����
	double				e_dDev_Dist;	///< �f�o�C�X���W�|�C���g�ł̋����i�f�t�H���g16pt�j
	int					e_MeshCode;		///< �P���b�V��
//		IWorkspacePtr		e_ipWorkSpace;

	msi_handle_ws		m_mAny_Railway_WS;		// �s�s�n�}�A���k�ڌn�̓S���֘A�f�[�^
	msi_handle_ws		m_mAny_Note_WS;			// �s�s�n�}�A���k�ڌn�̒��L�֘A�f�[�^
	msi_handle_ws		m_mAny_Note_Line_WS[4]; // ���L���C���i4���C�����j
	msi_handle_ws		m_mAny_City_Line_WS;	// �s�s�n�}�w�i���C���i�t�F���[�q�H�֌W�̂��߁j

	string				m_sAny_Railway_Layer;	// �S�����C����
	string				m_sAny_Note_Layer;		// ���L���C����
	string				m_sAny_Note_Line_Layer[4];
//	char*				m_cpAny_City_Line_Layer;

	string				e_sVersion;				///< �c�[���o�[�W����

	IWorkspacePtr       m_ipWSpace_Map;         ///< �s�s�n�}�n���[�N�X�y�[�X
	IWorkspacePtr       m_ipWSpace_Base;        ///< ���k�ڌn���[�N�X�y�[�X
	
	mk_input_list		m_mChk_List;

	IFeatureClassPtr	e_ipF_Class;
	FILE*				e_fErr_Fp;		///<	�G���[���O�t�@�C���p�t�@�C���|�C���^
	prog_init			e_pTool_Init;	// DB�̏������p�ݒ�
	smp_sindy_err_info	e_sPrnInfo;		// �G���[���o�͗p

	char*				e_cpPGDB_File;	// PGDB�t�@�C��

	map<long, string, less<long> >	m_mAttrStrTbl;	// ��ʂƑ����̑Ή��e�[�u���i���L�ƓS�����݁j

//	char*				e_cpChk_Note_Layer;
private:
	/** ---------------------------------------------------------------------------
	//	��x�f�o�C�X���W�ɕϊ����A���̌�ܓx�o�x�ɖ߂�
	//	@return	IPointPtr
	-----------------------------------------------------------------------------*/
	IPointPtr	get_Device_Pt	(
					int				c_iMeshCode,
					IPointPtr		c_ipPoint,
					int				c_iScale 	///< �X�P�[��
				);

	/** ---------------------------------------------------------------------------
	//	SJIS�ւ̕ϊ�
	//	@return	�ϊ��㕶����
	-----------------------------------------------------------------------------*/
	string		Str_to_SJIS		(
					CComBSTR*		c_CStr		// ������(Unicode)
				);

	/** ---------------------------------------------------------------------------
	//	�w��̒������牽�x�ɓ����邩��Ԃ�
	//	@return	�x�P�ʂł̋���
	-----------------------------------------------------------------------------*/
	double		Get_Degree_Range(
					int				c_iMeshCode,	///< ���b�V���R�[�h 
					double			c_dMeter_Range 	///< ���[�g���P�ʂ͈̔�
				);

	/** ---------------------------------------------------------------------------
	//	�w��̒������牽�x�ɓ����邩��Ԃ�
	//	@return	�x�P�ʂł̋���
	-----------------------------------------------------------------------------*/
	double		Get_Degree_Range_Dev (
					int				c_iMeshCode,		///< ���b�V���R�[�h 
					int				c_iScale,			///< �X�P�[��
					int				c_iDev_Dist 		///< �f�o�C�X���W�_�l�ł̋���
				);


	/** ---------------------------------------------------------------------------
	//	�w��̒������牽�x�ɓ����邩��Ԃ�
	-----------------------------------------------------------------------------*/
	void		Get_Degree_Range_Dev (
					int				c_iMeshCode,		///< ���b�V���R�[�h[IN]
					int				c_iScale,			///< �X�P�[��[IN]
					int				c_iDev_Dist, 		///< �f�o�C�X���W�_�l�ł̋���[IN]
					double*			c_cpRetXDeg,		///< X�����̋���[OUT]
					double*			c_cpRetYDeg 		///< Y�����̋���[OUT]
				);

	/** ---------------------------------------------------------------------------
	//	�^�ʒu���܂߂��\���ʒu��S�Ď��o��
	-----------------------------------------------------------------------------*/
	bool		Get_Disp_Pts	(
					int						c_iMeshCode,		// ���b�V���R�[�h
				//	int						c_iAttr_Code,		// �����R�[�h
					Feature_All_Attrs&		c_fFAtt,			// ���L�^�ʒu�̏��
					msi_handle_ws*			c_mAny_Note_Line_WS,// �Y�����C���̌����p
					vector<disp_pt_info>&	c_vPoints
				);
										
	/** ---------------------------------------------------------------------------
	//	���C������L�����S�ʒu�ɂ�����_�����o��
	-----------------------------------------------------------------------------*/
	IPointPtr	Get_Center_Pt	(
					IGeometryPtr	c_ipGeo		// �W�I���g��
				);

	/** ---------------------------------------------------------------------------
	//	���C������L�����S�ʒu�ɂ�����_�����o��
	-----------------------------------------------------------------------------*/
	IPointPtr	Get_Center_Pt2	(
					IGeometryPtr	c_ipGeo,		///< �W�I���g��
					int				c_iMeshCode,	///< ���b�V���R�[�h
					int				c_iScale		///< �X�P�[��
				//	int				c_iFontSize 	///< �t�H���g�T�C�Y
				);

	/** ---------------------------------------------------------------------------
		�|�C���g�ƃA�[�N�̋��������߂�
		@return	double	����
	-----------------------------------------------------------------------------*/
	double		Get_Dist	(
					IPointPtr&		c_ipPt,			///< �|�C���g[IN]
					const IGeometryPtr&	c_ipLine		///< ���C��[IN]
				);

public:
	//	enum make_type {
	//		back			= 1,
	//		pol				= 2,
	//		note			= 3
	//	};
		/// �R���X�g���N�^
		main_rootin	()
		{
			namespace anno_code = sindy::schema::annotation::annotation_code;

			e_cpAdminCode_File		= NULL;
			e_cpMeshCode_File		= NULL;
			e_cpAddrTxt_Path		= NULL;
			e_cpLog_File			= NULL;
			e_cpInit_File			= NULL;
			e_cpErrList				= NULL;
		//	m_cpAny_City_Line_Layer	= NULL;

			e_cpPGDB_File			= NULL;

			eMake_Type				= 0;
			eExe_Type				= 0;
			m_iDigit				= 0;
			eExe_Flag				= 0;
			e_dDev_Dist				= 16.0;
			e_Warn_Dist				= 1.0;	// 1.0km�ȏ�Ae_Err_Dist�����̓��[�j���O
			e_Err_Dist				= 3.0;	// 3.0km�ȏ�̓G���[
			e_MeshCode				= 0;
			e_fErr_Fp				= NULL;
			// ��ʖ��̂̕\
			m_mAttrStrTbl[anno_code::kShinkansenLineName]  = "�V�����H����";
			m_mAttrStrTbl[anno_code::kJRLineName]          = "�i�q�H����";
			m_mAttrStrTbl[anno_code::kQuasiPublicLineName] = "��R�Z�N�^�[�H����";
			m_mAttrStrTbl[anno_code::kPrivateLineName]     = "���S�H����";
			m_mAttrStrTbl[anno_code::kSubwayLineName]      = "�n���S�H����";
			m_mAttrStrTbl[anno_code::kMonorailLineName]    = "���m���[���H����";
			m_mAttrStrTbl[anno_code::kShinkotsuLineName]   = "�V��ʘH����";

			m_mAttrStrTbl[anno_code::kShinkansenSubLineName]  = "�V�����H�����i�n���j";
			m_mAttrStrTbl[anno_code::kJRSubLineName]          = "�i�q�H�����i�n���j";
			m_mAttrStrTbl[anno_code::kQuasiPublicSubLineName] = "��R�Z�N�^�[�H�����i�n���j";
			m_mAttrStrTbl[anno_code::kPrivateSubLineName]     = "���S�H�����i�n���j";
			m_mAttrStrTbl[anno_code::kSubwaySubLineName]      = "�n���S�H�����i�n���j";
			m_mAttrStrTbl[anno_code::kMonorailSubLineName]    = "���m���[���H�����i�n���j";
			m_mAttrStrTbl[anno_code::kShinkotsuSubLineName]   = "�V��ʘH�����i�n���j";

			m_mAttrStrTbl[1] = "�V����";
			m_mAttrStrTbl[2] = "�i�q";
			m_mAttrStrTbl[3] = "��R�Z�N�^�[";
			m_mAttrStrTbl[4] = "���S";
			m_mAttrStrTbl[6] = "�n���S";
			m_mAttrStrTbl[7] = "���m���[��";
			m_mAttrStrTbl[8] = "�V���";
			// �s�s�n�}�p(�����l*10+�n���t���O�Ō�������)
			//[GS]1.�n���S�ȊO��1.���ʓS�� 5.�V��ʃV�X�e�� 6.���m���[�� 7.�P�[�u���J�[�ɕ������ꂽ����
			//50,51,60,61,70,71�̗v�f��ǉ� 2014/3/19 ���Z�p����l�Z�pG ����
			m_mAttrStrTbl[10] = "���ʓS���i�n��j";
			m_mAttrStrTbl[50] = "�V��ʃV�X�e���i�n��j";
			m_mAttrStrTbl[60] = "���m���[���i�n��j";
			m_mAttrStrTbl[70] = "�P�[�u���J�[�i�n��j";
			m_mAttrStrTbl[11] = "���ʓS���i�n���j";
			m_mAttrStrTbl[51] = "�V��ʃV�X�e���i�n���j";
			m_mAttrStrTbl[61] = "���m���[���i�n���j";
			m_mAttrStrTbl[71] = "�P�[�u���J�[�i�n���j";
			m_mAttrStrTbl[20] = "�n���S�i�n��j";
			m_mAttrStrTbl[21] = "�n���S�i�n���j";

		//	e_cpChk_Note_Layer		= NULL;
		}
		~main_rootin	()
		{
			if( e_fErr_Fp != NULL ) {
				fclose ( e_fErr_Fp );
			}
		}

		/** ---------------------------------------------------------------------------
		//	�����`�F�b�N
		//	@return	����	true
		//	@return	����	false
		-----------------------------------------------------------------------------*/
		bool			arg_check	(
							int		argc,			///< ������
							char	**argv			///< ������
						);

		/** ---------------------------------------------------------------------------
		//	�c�[���̎g�p�@�̕\��
		-----------------------------------------------------------------------------*/
		void			print_usage	(
							char	**argv			///< ������
						);

		/** ---------------------------------------------------------------------------
		//	�o�[�W�������̕\��
		//	@return		����
		-----------------------------------------------------------------------------*/
		void			print_version	( void );


		/** ---------------------------------------------------------------------------
		//	�s���R�[�h���X�g�t�@�C������Ԃ�
		//	@return		�t�@�C�����ւ̃|�C���^
		-----------------------------------------------------------------------------*/
		char*			Ret_Adm_Code_File	( void ) {
							return	( e_cpAdminCode_File );
						}

		/** ---------------------------------------------------------------------------
		//	���b�V�����X�g�t�@�C������Ԃ�
		//	@return		�t�@�C�����ւ̃|�C���^
		-----------------------------------------------------------------------------*/
		char*			Ret_Mesh_File	( void ) {
							return	( e_cpMeshCode_File );
						}

		/** ---------------------------------------------------------------------------
		//	���O�f�[�^�o�̓t�@�C������Ԃ�
		//	@retur		�p�X���ւ̃|�C���^
		-----------------------------------------------------------------------------*/
		char*			Ret_Log_File	( void ) {
							return	( e_cpLog_File );
						}

		/** ---------------------------------------------------------------------------
		//	�������t�@�C������Ԃ�
		//	@return		�t�@�C�����ւ̃|�C���^
		-----------------------------------------------------------------------------*/
		char*			Ret_Init_File	( void ) {
							return	( e_cpInit_File );
						}

		/** ---------------------------------------------------------------------------
		//	�G���[���X�g�t�@�C��
		//	@return		�t�@�C�����ւ̃|�C���^
		-----------------------------------------------------------------------------*/
		char*			Ret_Err_List	( void ) {
							return	( e_cpErrList );
						}

		/** ---------------------------------------------------------------------------
		//	���b�V����`��DB����l�����郁�\�b�h�̏����� \
		//	��get_mesh_rect �����O�ɕK���Ă�
		-----------------------------------------------------------------------------*/
		bool			get_mesh_init	(
							string			c_sMeshLayer,		///< ���b�V�����C����
							IWorkspacePtr	c_ipWorkSpace		///< �w�肷�郏�[�N�X�y�[�X
						);

		/** ---------------------------------------------------------------------------
		//	���b�V����`��GeoDB����擾���� \
		//	��GeoDB�ւ̐ڑ����m������Ă��邱�Ƃ��O��
		-----------------------------------------------------------------------------*/
		IEnvelopePtr	get_mesh_rect	(
							int				c_iMeshCode			///< ���b�V���R�[�h
						);

		/** ---------------------------------------------------------------------------
		//	�w��|�C���g�𒆐S�ɁA�w�肵��X,Y�����̕�����������`���쐬
		-----------------------------------------------------------------------------*/
		IEnvelopePtr	make_rect	(
							IPointPtr&		c_ipPt,				///< ���S�|�C���g
							double			c_dDegX,			///< X����
							double			c_dDegY				///< ��`
						);

		/** ---------------------------------------------------------------------------
		//	���[�j���O������Ԃ�
		//	@return	�G���[���苗��
		-----------------------------------------------------------------------------*/
		double			Get_Warn_Dist	( void ) {
							return	( e_Warn_Dist );
						}

		/** ---------------------------------------------------------------------------
		//	�G���[������Ԃ�
		//	@return	�G���[���苗��
		-----------------------------------------------------------------------------*/
		double			Get_Err_Dist	( void ) {
							return	( e_Err_Dist );
						}

		/** ---------------------------------------------------------------------------
		//	�f�o�C�X���W�|�C���g��Ԃ�
		//	@return	�f�o�C�X���W�l�|�C���g
		-----------------------------------------------------------------------------*/
		double			Get_Dev_Dist	( void ) {
							return	( e_dDev_Dist );
						}

		/** ---------------------------------------------------------------------------
		//	�G���[���O�t�@�C�����I�[�v������
		//	@return	����	true
		//	@return	���s	false
		-----------------------------------------------------------------------------*/
		bool			Open_Err_Log	( void );

		/** ---------------------------------------------------------------------------
		//	�G���[���O�̃w�b�_���o�͂���
		//	@return	����
		-----------------------------------------------------------------------------*/
		void			Output_Log_Header	(
							long					c_lDigit	///< ���b�V������[IN]
						);

		/** ---------------------------------------------------------------------------
		//	DB�ւ̐ڑ��I�[�v��
		//	@return	true	����
		//	@return	false	���s
		-----------------------------------------------------------------------------*/
		bool			DB_Open	(							
							IWorkspacePtr&			c_ipWorkspace,							
							IWorkspacePtr&			c_ipWorkspace_Base
						);

		/** ---------------------------------------------------------------------------
		//	�`�F�b�N�̏���������
		-----------------------------------------------------------------------------*/
		bool			check_init	( void );

		/** ---------------------------------------------------------------------------
		//	�ǂݍ��񂾃f�[�^���f�[�^�Z�b�g�ɗ��Ƃ�
		//	@return	�f�[�^��
		-----------------------------------------------------------------------------*/
		int				Get_Data_info (
							IFeatureCursorPtr&		c_ipCursor,		///< �`�F�b�N���J�[�\��[IN]
							ms_data_info&			c_ipGetData		///< �`�F�b�N���f�[�^�̃Z�b�g[OUT]
						);

		/** ---------------------------------------------------------------------------
		//	���L�ƓS���̑��������ă`�F�b�N
		//	@return	�f�[�^��
		-----------------------------------------------------------------------------*/
		void			Anno_Railway_Chk (
							data_info&				c_dAnnoData,	///< ���L�f�[�^[IN]
							disp_pt_info&			c_dCurDat,		///< �\���ʒu�f�[�^[IN]
							ms_data_info&			c_mRailway_Data	///< �����œ���ꂽ�S���f�[�^[IN]
						);

		/** ---------------------------------------------------------------------------
		//	�`�F�b�N���s��
		//	�������ŃG���[�t�@�C���̏��������s��
		//	@return	����	true
		//	@return ���s	false
		-----------------------------------------------------------------------------*/
		bool			check_exe		( void );
};

#endif	//_MAIN_ROOTIN_H_
