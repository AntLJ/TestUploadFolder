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
#include <libsmpprninfo.h>
#include "msi_handle_ws.h"
#include <feature_info.h>
#include "prog_init_cls.h"
#include "exec_time_handle.h"
#include "exp_poly_class.h"
#include "coord_converter.h"
#include <atlstr.h>
#include "..\..\Building_Diff\AKAttrHandler.h" // [Bug 10159] �Ή�

using	namespace	std;

#ifndef	_MAIN_ROOTIN_H_
#define	_MAIN_ROOTIN_H_

#define		ON_LOGFILE		0x02	///< ���O�t�@�C���w��
#define		ON_INIT_FILE	0x04	///< �������t�@�C���ǂݍ��ݗp
#define		ON_MESH			0x08	///< �Z���R�[�h�R�[�h���X�g�w�胂�[�h 2002.9.24
#define		ON_MONO_MESH	0x10	///< �P�Z���R�[�h�w�胂�[�h
#define		ON_ATTR_FILE	0x40	///< �����e�[�u���t�@�C���p

/**
* @brief ���b�Z�[�W�̏o��
*/
void PrnMsg	( FILE* c_cpFp, FILE* c_cpFp2, const char* c_cpFormat, ... );

struct	data_info
{
	int				e_iOID;				///< �I�u�W�F�N�gID
	int				e_iMeshCode;		///< ���b�V���R�[�h
	int				e_iPrim_ID;			///< �v���~�B�e�B�uID
	int				e_iStat;			///< �Ή��̏�ԁi0=�Ή����Ȃ�,1=�Ή������������j
	int				e_iAttr;			///< �w�i���
//	char			e_cRP_F;			///< �n��L���t���O�ia roofed passage�j
	IGeometryPtr	e_ipGeo;			///< �W�I���g��
	/**
	* @brief �R���X�g���N�^
	*/
	data_info	( void ) {
		e_iOID			= 0;
		e_iMeshCode		= 0;
		e_iPrim_ID		= 0;
		e_iStat			= 0;
		e_iAttr			= 0;
//		e_cRP_F			= 0;
	}
	/**
	* @brief �f�X�g���N�^
	*/
	~data_info	( void ) { }

	/**
	* @brief ��r���Z�q
	* @return src<tgt�̏ꍇtrue ����ȊOfalse
	*/
	bool	operator<(const data_info c_dDat ) const {
		bool	ret	= false;
		if( e_iMeshCode != c_dDat.e_iMeshCode ) {
			if( e_iMeshCode < c_dDat.e_iMeshCode ) {
				ret	= true;
			}
		}else
		if( e_iPrim_ID != c_dDat.e_iPrim_ID ) {
			if( e_iPrim_ID < c_dDat.e_iPrim_ID ) {
				ret	= true;
			}
		}
		return	( ret );
	}
	/**
	* @brief �񓙒l���Z�q
	* @return src!=tgt�̏ꍇtrue ����ȊOfalse
	*/
	bool	operator!=(const data_info c_dDat ) const {
		bool	ret	= false;
		if( e_iMeshCode != c_dDat.e_iMeshCode ||
			e_iPrim_ID != c_dDat.e_iPrim_ID )
		{
			ret	= true;
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

class	main_rootin
{
	public:
		string					e_cpMeshCode_File;		///< ���b�V���R�[�h���X�g�t�@�C��
		string					e_cpLog_File;			///< ���O�f�[�^�o�̓p�X
		string					e_cpInit_File;			///< �������t�@�C��
		string					e_cpCheck_Src_Layer;	///< �`�F�b�N�����C����
		string					e_cpAttr_Tbl_File;		///< �����Ή��e�[�u���t�@�C��
		int						eDigit;					///< ���b�V���R�[�h����
		int						eExe_Flag;              ///< ���s�t���O
		int						e_iMeshCode;			///< ���b�V���R�[�h
		IFeatureClassPtr		e_ipF_Class;
		FILE*					e_fErr_Fp;				///<�@�G���[���O�t�@�C���p�t�@�C���|�C���^
		set<int, less<int> >	e_sExi_Mesh_List;		///<�@���݃��b�V���̃��X�g 2002.12.06
		set<int>				e_sOID_Set;				///<�@�I�u�W�F�N�gID�̃Z�b�g
	private:
		prog_init				e_pTool_Init;			///< DB�̏������p�ݒ�
		exec_time_handle		e_eExeTime;				///< ���s���ԊǗ�
		msi_handle_ws			e_mChk_Src_WS;			///< �`�F�b�N��
		IGeometryPtr			e_ipMeshRect;			///< ���b�V����`
	private:
		AKAttrHandler           mAKAttrHandler;         ///< AK�Ή������戵���N���X [Bug 10159]�Ή�
	private:
		FILE*					e_fpAttr_Fp;			///< �����Ή��e�[�u���t�@�C��
		CString                 m_sConnectStr;          ///< DB�ڑ�������
	public:
		/**
		* @brief �R���X�g���N�^
		*/
		main_rootin	() {
			eDigit            = 0;
			eExe_Flag         = 0;
			e_iMeshCode       = 0;
			e_fErr_Fp         = stderr;	// �f�t�H���g�͕W���G���[�o�͂�
			e_sExi_Mesh_List.clear();
			e_fpAttr_Fp       = NULL;
		}
		/**
		* @brief �f�X�g���N�^
		*/
		~main_rootin	() {
			if( e_fErr_Fp != NULL ) {
				fclose ( e_fErr_Fp );
			}
			if( e_fpAttr_Fp != NULL ) {
				fclose ( e_fpAttr_Fp );
			}
		}
		/**
		* @brief SiNDYDB�ւ̐ڑ��I�[�v��
		* @return	true	����
		* @return	false	���s
		*/
		bool			SiNDY_DB_Open	(
							IWorkspacePtr&			c_ipWorkspace_Base
						);

		/**
		* @brief �����`�F�b�N
		* @return	����	true
		* @return	����	false
		*/
		bool			arg_check	(
							int		argc,			///< [in] ������
							char	**argv			///< [in] ������
						);

		/**
		* @brief �c�[���̎g�p�@�̕\��
		*/
		void			print_usage	(
							char	**argv			///< [in] ������
						);

		/**
		* @brief �o�[�W�������̕\��
		* @return		����
		*/
		void			print_version	( void );

		/**
		* @brief ���b�V����`��DB����l�����郁�\�b�h�̏����� \
		* @note  ��get_mesh_rect �����O�ɕK���Ă�
		*/
		bool			get_mesh_init	(
							mk_input_list&	c_mMList,			///< [in] ���̓��X�g�N���X
							IWorkspacePtr&	c_ipWorkSpace		///< [in] �w�肷�郏�[�N�X�y�[�X
						);
		/**
		* @brief ���b�V����`��GeoDB����擾���� \
		* @note  ��GeoDB�ւ̐ڑ����m������Ă��邱�Ƃ��O��
		*/
		IEnvelopePtr	get_mesh_rect	(
							int				c_iMeshCode			///< [in] ���b�V���R�[�h
						);
		/**
		* @brief �G���[���O�t�@�C�����I�[�v������
		* @return ����=true, ���s=false
		*/
		bool			Open_Err_Log	( void );

		/**
		* @brief �����e�[�u���t�@�C�����I�[�v������
		* @return ����=true, ���s=false
		*/
		bool			Open_Attr_Tbl	( void );

		/**
		* @brief �`�F�b�N���s��
		* @note  �������ŃG���[�t�@�C���̏��������s��
		* @return ���� true
		* @return ���s false
		*/
		bool			check_exe	( void );

		/**
		* @brief �`�F�b�N�p�s���E���X�g�A���b�V���R�[�h���X�g�̓ǂݍ���
		*/
		bool			Read_Chk_List	(
							IWorkspacePtr&	c_ipWSpace,			///< [in] ���[�N�X�y�[�X
							mk_input_list&	c_aChk_List_Cls		///< [in] ���X�g�Ǘ��N���X�̎Q��
						);

		/**
			���C���I�[�v��
		* @return ����=true, ���s=false
		*/
		bool			Open_Layers		(
							IWorkspacePtr&	ipWorkspace_Src		///< [in] �`�F�b�N�����[�N�X�y�[�X
						);

		/**
		* @brief �������[�v
		*/
		void			Exec_Roops (
							IFeatureCursorPtr&	c_ipChk_Src_Cursor	///< [in] �`�F�b�N���ƂȂ�f�[�^�̃J�[�\��
						);

		/**
		* @brief �Ή��e�[�u���̏o��
		*/
		void			Print_Table		(
							ms_data_info&		c_ipChk_Src_Data ///< [in] �`�F�b�N���f�[�^�̃Z�b�g
						);

		/**
		* @brief �`�F�b�N�f�[�^���Z�b�g�ɗ��Ƃ�
		* @return	�f�[�^��
		*/
		int				Set_Data_Info (
							IFeatureCursorPtr&	c_ipChk_Cursor, ///< [in] �`�F�b�N�J�[�\��
							ms_data_info&		c_ipChk_Data	///< [out] �`�F�b�N�f�[�^�̃Z�b�g
						);

//------------------------------------------------------<�����܂�>

};

#endif	//_MAIN_ROOTIN_H_
