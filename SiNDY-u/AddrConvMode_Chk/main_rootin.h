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
#include <libsmpprninfo.h>
#include "prog_init_cls.h"
//include "WKSHelper.h"
//#include "SiINClassTable.h"
#include "exec_time_handle.h"
#include <arctl/coinitializer.h>
#include "sindy/workspace/name.h"

using	namespace	std;

#ifndef	_MAIN_ROOTIN_H_
#define	_MAIN_ROOTIN_H_

#define		ON_ADCODE			0x01				///< �Z���R�[�h�R�[�h���X�g�w�胂�[�h 2002.9.24
#define		ON_MONO_ADCODE		0x02				///< �P�Z���R�[�h�w�胂�[�h
#define		ON_WHERE			0x04				///< Where��w�胂�[�h
#define		ON_LOGFILE			0x08				///< ���O�t�@�C���w��
#define		ON_INIT_FILE		0x10				///< �������t�@�C���ǂݍ��ݗp
#define		ON_MESH				0x20				///< �Z���R�[�h�R�[�h���X�g�w�胂�[�h 2002.9.24
#define		ON_MONO_MESH		0x40				///< �P�Z���R�[�h�w�胂�[�h
#define		ON_CODE_LIST_TXT	0x80				///< �R�[�h���X�g�e�L�X�g�t�@�C��
#define		ON_PGDB_MODE		0x100
#define		ON_CITY_ADMIN		0x200
#define		ON_GOU_POINT		0x400

struct	data_info
{
	int			e_iOID;				///< �I�u�W�F�N�gID
//	char		e_cpCityCode[6];	///< 5���R�[�h
//	char		e_cpAddrCode[7];	///< 6���R�[�h
	string		e_sCityCode;		///< 5���R�[�h
	string		e_sAddrCode;		///< 6���R�[�h
	string		e_sAddrCode2;		///< 6���R�[�h
//	int			e_iConvMode;		///< �ϊ����[�h
	/**
		�R���X�g���N�^
	*/
	data_info	( void ) {
		e_iOID		= 0;
	}
	/**
		�f�X�g���N�^
	*/
	~data_info	( void ) { }

	bool	operator<(const data_info c_dDat ) const {
		bool	ret	= false;
/*		if( strcmp( e_cpCityCode, c_dDat.e_cpCityCode ) != 0 ) {
			if( strcmp ( e_cpCityCode, c_dDat.e_cpCityCode ) < 0 ) {
				ret	= true;
			}
		}else
		if( 
*/		if( e_sCityCode != c_dDat.e_sCityCode ) {
			if( e_sCityCode < c_dDat.e_sCityCode ) {
				ret	= true;
			}
		}else
		if( e_sAddrCode != c_dDat.e_sAddrCode ) {
			if( e_sAddrCode < c_dDat.e_sAddrCode ) {
				ret	= true;
			}
		}

		return	( ret );
	}
};

typedef	multiset<data_info, less <data_info> >				ms_data_info;
typedef	multiset<data_info, less <data_info> >::iterator	ims_data_info;
typedef	set<data_info, less <data_info> >					s_data_info;
typedef	set<data_info, less <data_info> >::iterator			is_data_info;

typedef	set<string, less<string> >							s_string;
typedef	set<string, less<string> >::iterator				is_string;

/**
	���|�C���g�̏��
*/
struct	gou_info
{
	string	e_sChiban;	///< �n�ԕ�����i���ԍ��܂ށj
	string	e_sCode11_1;///< ������s���E��11���R�[�h(CITYCODE+ADDRCODE) [bug 8338]�A�Ή�
	string	e_sCode11_2;///< ������s���E��11���R�[�h(CITYCODE+ADDRCODE2)[bug 8338]�A�Ή�
	int		e_iGouType;	///< ���^�C�v 2008.12.03 [Bug 6543]�Ή�
	long	e_lGaiku;	///< �X�敄�� 2008.12.16 [Bug 6543]�Ή�
	string	e_sExtGaiku;///< �g���X�敄�� 2008.12.16 [Bug 6543]�Ή�
	int		e_iOID;		///< �I�u�W�F�N�gID
	/**
		�R���X�g���N�^
	*/
	gou_info	( void ) {
		e_iOID		= 0;
		e_lGaiku	= 0;
	}
	/**
		�f�X�g���N�^
	*/
	~gou_info	( void ) { }

	bool	operator<(const gou_info c_dDat ) const {
		bool	ret	= false;
		if( e_sChiban != c_dDat.e_sChiban ) {
			if( e_sChiban < c_dDat.e_sChiban ) {
				ret	= true;
			}
		}else	// [bug 8338]�A�Ή��ŏC��
		if( e_sCode11_1 != c_dDat.e_sCode11_1 ) {
			if( e_sCode11_1 < c_dDat.e_sCode11_1 ) {
				ret	= true;
			}
		}else
		if( e_sCode11_2 != c_dDat.e_sCode11_2 ) {
			if( e_sCode11_2 < c_dDat.e_sCode11_2 ) {
				ret	= true;
			}
		}

		return	( ret );
	}
};

typedef	multiset<gou_info, less<gou_info> >					ms_gou_info;
typedef	multiset<gou_info, less<gou_info> >::iterator		ims_gou_info;

struct	admin_info
{
	int				e_iOID;		///< �I�u�W�F�N�gID
	string			e_sCode1;	///< 11���R�[�h(CITYCODE+ADDRCODE)[bug 8338]�A�Ή�
	string			e_sCode2;	///< 11���R�[�h(CiTYCODE+ADDRCODE2)[bug 8338]�A�Ή�
	long			e_lGaiku;	///< �X�敄�� 2008.12.16 [Bug 6543]�Ή�
	string			e_sExtGaiku;///< �g���X�敄�� 2008.12.16 [Bug 6543]�Ή�
	IGeometryPtr	e_ipGeom;	///< �W�I���g��
	admin_info	() {
		e_iOID	= 0;
		e_lGaiku= 0;
	}
	~admin_info	() {}

	bool	operator<(const admin_info c_dDat ) const {
		bool	ret	= false;
		// [bug 8338]�A�Ή��ŏC��
		if( e_sCode1 != c_dDat.e_sCode1 ) {
			if( e_sCode1 < c_dDat.e_sCode1 ) {
				ret	= true;
			}
		}else
		if( e_sCode2 != c_dDat.e_sCode2 ) {
			if( e_sCode2 < c_dDat.e_sCode2 ) {
				ret	= true;
			}
		}
		return	( ret );
	}
};

typedef	vector<admin_info>				v_admin_info;
typedef	vector<admin_info>::iterator	iv_admin_info;

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
		char*					e_cpLog_File;			///< ���O�f�[�^�o�̓p�X
		char*					e_cpCode_List_Txt;		///< 11���R�[�h���X�g�t�@�C��
		char*					e_cpInit_File;			///< �������t�@�C��
		char*					e_sCheck_Layer;			///< �`�F�b�N���C����
		char*					e_sGou_Layer;			///< ���|�C���g���C����
		string					e_sWhereString;			///< �����Ɏg�p����Where��
		int						eMake_Type;				///< �f�[�^�쐬�^�C�v
		int						eExe_Type;				///< ���s�^�C�v
		int						eDigit;					///< ���b�V���R�[�h����
		char*					e_cpAddrCode;			///< �Z���R�[�h
		int						eExe_Flag;
		int						e_iMeshCode;			///< ���b�V���R�[�h
		double					e_Err_Dist;				///< �G���[����
		IFeatureClassPtr		e_ipF_Class;
		FILE*					e_fErr_Fp;				///<�@�G���[���O�t�@�C���p�t�@�C���|�C���^
		set<int, less<int> >	e_sExi_Mesh_List;		///<�@���݃��b�V���̃��X�g 2002.12.06
		set<int>				e_sOID_Set;				///<�@�I�u�W�F�N�gID�̃Z�b�g
		string					e_sVersion;				///< �c�[���o�[�W���� 2008.11.12
	private:
		prog_init				e_pTool_Init;			// DB�̏������p�ݒ�
		smp_sindy_err_info		e_sPrnInfo;				// �G���[���o�͗p
		mk_input_list			e_sInput_List;			// ���̓��X�g�p
		exec_time_handle		e_eExeTime;				// ���s���ԊǗ�
		msi_handle_ws			e_mCity_Admin_WS;		// �s���E�f�[�^
		msi_handle_ws			e_mGou_Point_WS;		// �s���E�f�[�^
	private:
		ms_data_info			e_mAdmin_Datas;			///< �s�����f�[�^�i�ϊ����[�h�m�F�p�j
		s_data_info				e_mAdmin_Datas_Uniq;	///< �s�����f�[�^�i11�����j�[�N�j
		s_string				e_sCode_8;				///< 8���R�[�h���X�g
		char*					e_cpPGDB_File;			///< PGDB�t�@�C��
		char*					WCARD1;
	public:

		main_rootin	() {
			e_cpMeshCode_File		= NULL;
			e_cpAddrCode_File		= NULL;
			e_cpLog_File			= NULL;
			e_cpCode_List_Txt		= NULL;
			e_cpInit_File			= NULL;
			e_sCheck_Layer			= NULL;
			e_sGou_Layer			= NULL;
			eMake_Type				= 0;
			eExe_Type				= 0;
			eDigit					= 0;
			eExe_Flag				= 0;
			e_iMeshCode				= 0;
			e_Err_Dist				= 100.0;	// 5.0km�ȏ�̓G���[
			e_fErr_Fp				= stderr;	// �f�t�H���g�͕W���G���[�o�͂�
			e_sExi_Mesh_List.clear();
			e_cpPGDB_File			= NULL;
			WCARD1					= "_";
		}
		~main_rootin	() {
			if( e_fErr_Fp != NULL ) {
				fclose ( e_fErr_Fp );
			}
		}
		/**
			SiNDYDB�ւ̐ڑ��I�[�v��
			@return	true	����
			@return	false	���s
		*/
		bool			SiNDY_DB_Open	(
							IWorkspacePtr&			c_ipWorkspace_Addr
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
							char*					c_cpInstance,		///< �C���X�^���X
							char*					c_cpServer,			///< �T�[�o��
							char*					c_cpVersion			///< �o�[�W����
						);

		/**
			������`�F�b�N
			@return	������
		*/
		string			Str_to_SJIS	(
							CComBSTR&	c_CStr		///< ������(Unicode)
						);

		/** ---------------------------------------------------------------------------
		//	�G���[���O�̃w�b�_���o�͂���
		//	@return	����
		-----------------------------------------------------------------------------*/
		void			Output_Log_Header	( void );

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
			Where()����擾����
		*/
		string			Get_Where_String(
							char*			c_cpLayerName		// ���C����
						);
		/**
			�`�F�b�N���s��
			�������ŃG���[�t�@�C���̏��������s��
			@return	����	true
			@return ���s	false
		*/
		bool			check_exe	( void );

		/**
			���C���I�[�v��
		*/
		bool			Open_Layers		(
							IWorkspacePtr	c_ipWorkspace_Addr	// �Z���p���[�N�X�y�[�X
						);


		/**
			�s���E�̃��[�v
		*/
		void			Admin_Check_Roops (
							IFeatureCursorPtr&	c_ipAdminCursor	///< �s���E�J�[�\��
						);

		/**
			�s���R�[�h���X�g�����Z�b�g�ɗ��Ƃ�
			@return	�f�[�^��
		*/
		int				Set_AdminCode_Data (
							IFeatureCursorPtr&	c_ipAdminCursor 	///< ���L�J�[�\��
						);

		/**
			11���R�[�h���X�g�̏o��
		*/
		void			Print_Code_List	( void );
		
		/**
			�ϊ����[�h�R�[�h�G���[�̃`�F�b�N
		*/
		void			Chk_ConvMode	( void );

		/**
			11������v����s���E�̒��ŁA�ϊ����[�h���Ⴄ���̂��`�F�b�N����
		*/
		void			Chk_SameCode11Admin ( void );

		/**
			8������v����s���E�̒��ŁA�ʃ|���S���ɓ������|�C���g�����邩�̃`�F�b�N
		*/
		void			Chk_SameCode8Admin ( void );

		/**
			8������v����s���E�̒��ŁA�ϊ����[�h���Ⴄ���̂��`�F�b�N����
		*/
		void			Chk_SameCode8Admin_Mode ( void );

		/**
			���|�C���g�̃��X�g���쐬����
		*/
		int				Set_Gou_Datas	(
							v_admin_info&		c_vAdmLists, 		///< �s���E
							ms_gou_info&		c_mGou_Infos		///< ���o�������f�[�^
						);						
		/** 
			�Z�b�g�����č��̏d�����`�F�b�N����
		*/
		void			Chk_Gou_Dupli	( 
							ms_gou_info&		c_mGou_Infos		///< ���|�C���g���
						);

		/**
			�s���E�̃��X�g���擾����i�W�I���g���܂ށj
		*/
		void			Set_Admin_List	(
							IFeatureCursorPtr&	c_ipAdminCursor,
							v_admin_info&		c_vAdmLists
						);
	
//------------------------------------------------------<�����܂�>

};

#endif	//_MAIN_ROOTIN_H_
