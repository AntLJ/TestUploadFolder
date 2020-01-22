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
#include <smp_prn_info.h>
#include "msi_handle_ws.h"
#include <feature_info.h>
#include "prog_init_cls.h"
#include "exec_time_handle.h"
#include "exp_poly_class.h"
#include "coord_converter.h"
#include "AKAttrHandler.h"

using	namespace	std;

#ifndef	_MAIN_ROOTIN_H_
#define	_MAIN_ROOTIN_H_

#define		ON_WHERE		0x01	///< Where��w�胂�[�h
#define		ON_LOGFILE		0x02	///< ���O�t�@�C���w��
#define		ON_INIT_FILE	0x04	///< �������t�@�C���ǂݍ��ݗp
#define		ON_MESH			0x08	///< �Z���R�[�h�R�[�h���X�g�w�胂�[�h 2002.9.24
#define		ON_MONO_MESH	0x10	///< �P�Z���R�[�h�w�胂�[�h
#define		ON_REVICE_LIST	0x20	///< �����C�����X�g
#define		ON_MATCH_FILE	0x40	///< �o�͑Ή��e�[�u���t�@�C���p
#define		ON_LOWACC_MODE	0x80	///< �����Ή��ƒᐸ�x�̐؂�ւ��p 2006.06.12

#define		STAT_EQUAL		0x01	///< ������
#define		STAT_MODIFY		0x02	///< �`��ύX
#define		STAT_RINOBJ		0x04	///< �א�
#define		STAT_DUPLI		0x08	///< �`��d��
#define		STAT_MIDEQUAL	0x10	///< ���b�V���ƌ����p�}�`ID����v����

// 2005.12.14 �폜�E�ǉ��I�u�W�F�N�g�֌W�̑Ή��Œǉ�
#define		STAT_LOWA_EQUAL		0x20	///< �`���v�i�ᐸ�x�j
#define		STAT_LOWA_MODIFY	0x40	///< �`��ύX�i�ᐸ�x�j
#define		STAT_MESHOUTER		0x1000	///< ���b�V���O

/**
* @brief ���b�Z�[�W�̏o��
*/
void PrnMsg	( FILE* c_cpFp, FILE* c_cpFp2, const char* c_cpFormat, ... );

struct	data_info
{
	int				e_iOID;				///< �I�u�W�F�N�gID
	int				e_iMeshCode;		///< ���b�V���R�[�h
	int				e_iStat;			///< �Ή��̏�ԁi0=�Ή����Ȃ�,1=�Ή������������j
	int				e_iAttr;			///< �w�i���
	IGeometryPtr	e_ipGeo;			///< �W�I���g��
	/** ------------------------------------------------------------------------------
	* @brief �R���X�g���N�^
	------------------------------------------------------------------------------- */
	data_info	( void ) {
		e_iOID			= 0;
		e_iMeshCode		= 0;
		e_iStat			= 0;
		e_iAttr			= 0;
	}
	/** ------------------------------------------------------------------------------
	* @brief �f�X�g���N�^
	------------------------------------------------------------------------------- */
	~data_info	( void ) { }

	/**
	* @brief ��r���Z�q
	* @return src<tgt �̏ꍇ��true��ԋp
	*/
	bool	operator<(const data_info c_dDat ) const {
		bool	ret	= false;
		if( e_iOID != c_dDat.e_iOID ) {
			if( e_iOID < c_dDat.e_iOID ) {
				ret	= true;
			}
		}
		return	( ret );
	}
	/**
	* @brief �񓙒l���Z�q
	* @return src!=tgt �̏ꍇ��true��ԋp
	*/
	bool	operator!=(const data_info c_dDat ) const {
		bool	ret	= false;
		if( e_iOID != c_dDat.e_iOID ) {
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

/** ------------------------------------------------------------------------------
* @brief  ��v�����f�[�^���Ǘ�����N���X
------------------------------------------------------------------------------- */
struct	match_record
{
	data_info	e_dData1; ///< �f�[�^1���
	data_info	e_dData2; ///< �f�[�^1�ƈ�v�Ƃ݂Ȃ��f�[�^�̏��
	
	/**
	* @brief �R���X�g���N�^
	*/
	match_record() {};

	/**
	* @brief �f�X�g���N�^
	*/
	~match_record() {};

	/**
	* @brief ��r���Z�q
	* @return src<tgt �̏ꍇ��true��ԋp
	*/
	bool	operator<(const match_record c_dDat ) const {
		bool	ret	= false;
		if( e_dData1 != c_dDat.e_dData1 ) {
			if( e_dData1 < c_dDat.e_dData1 ) {
				ret	= true;
			}
		}else if( e_dData2 != c_dDat.e_dData2 ) {
			if( e_dData2 < c_dDat.e_dData2 ) {
				ret	= true;
			}
		}
		return	( ret );
	}
};

typedef	multiset<match_record, less <match_record> >			ms_match_record;
typedef	multiset<match_record, less <match_record> >::iterator	ims_match_record;

/**
* @brief �����l�̃|�C���g�Ǘ��p�N���X
*/
struct	Real_Pt
{
	double	e_iX; ///< X���W�l
	double	e_iY; ///< Y���W�l

	/**
	* @brief �R���X�g���N�^
	*/	
	Real_Pt() {
		e_iX			= 0.0;
		e_iY			= 0.0;
	}
	/**
	* @brief �f�X�g���N�^
	*/	
	~Real_Pt() {}

	/**
	* @brief ��r���Z�q
	* @return src<tgt �̏ꍇ��true��ԋp
	*/
	bool	operator<(const Real_Pt& c_dDat ) const {
		bool	ret	= false;
		if( e_iX != c_dDat.e_iX ) {
			if( e_iX < c_dDat.e_iX ) {
				ret	= true;
			}
		}else if( e_iY != c_dDat.e_iY ) {
			if( e_iY < c_dDat.e_iY ) {
				ret	= true;
			}
		}
		return	( ret );
	}
	/**
	* @brief ������Z�q
	*/
	void	operator=(const WKSPoint& c_dDat ) {
		e_iX	= c_dDat.X;
		e_iY	= c_dDat.Y;
	}
};

/** ------------------------------------------------------------------------------
* @brief Real_Pt�͊����@�\�����̂܂܂ɂ��Ă��������̂ŁA�p�����ăN���X���쐬
------------------------------------------------------------------------------- */
struct	Real_Pt2 : public Real_Pt
{

	int		e_iInteriorFlag;	///< �C���e���A���̃t���O

	/**
	* @brief �R���X�g���N�^
	*/
	Real_Pt2() {
		e_iInteriorFlag	= 0;	
	}
	/**
	* @brief �f�X�g���N�^
	*/	
	~Real_Pt2() {}

	/**
	* @brief ��r���Z�q
	* @return src<tgt �̏ꍇ��true��ԋp
	*/
	bool	operator<(const Real_Pt2& c_dDat ) const {
		bool	ret	= false;
		if( e_iX != c_dDat.e_iX ) {
			if( e_iX < c_dDat.e_iX ) {
				ret	= true;
			}
		}else if( e_iY != c_dDat.e_iY ) {
			if( e_iY < c_dDat.e_iY ) {
				ret	= true;
			}
		}else if( e_iInteriorFlag != c_dDat.e_iInteriorFlag ) {
			if( e_iInteriorFlag < c_dDat.e_iInteriorFlag ) {
				ret	= true;
			}
		}
		return	( ret );
	}
	/**
	* @brief ������Z�q
	*/
	void	operator=(const WKSPoint& c_dDat ) {
		e_iX	= c_dDat.X;
		e_iY	= c_dDat.Y;
	}

};

//typedef	map<_Rb_tree_base_iterator::_Base_ptr, ims_data_info, less<_Rb_tree_base_iterator::_Base_ptr> >				m_ims_data_info;
//typedef	map<_Rb_tree_base_iterator::_Base_ptr, ims_data_info, less<_Rb_tree_base_iterator::_Base_ptr> >::iterator	im_ims_data_info;
typedef	map<long, ims_data_info, less<long> >			m_ims_data_info;
typedef	map<long, ims_data_info, less<long> >::iterator	im_ims_data_info;

/**
* @brief �c�[���̃��C����
*/
class	main_rootin
{
	public:
		char*					e_cpMeshCode_File;		///< ���b�V���R�[�h���X�g�t�@�C��
		char*					e_cpLog_File;			///< ���O�f�[�^�o�̓p�X
		char*					e_cpInit_File;			///< �������t�@�C��
		char*					e_cpInit_File_Tgt;		///< �������t�@�C��
		char*					e_cpCheck_Src_Layer;	///< �`�F�b�N�����C����
		char*					e_cpCheck_Tgt_Layer;	///< �`�F�b�N�惌�C����
		char*					e_cpMatch_Tbl_File;		///< �o�͑Ή��e�[�u���t�@�C��
		int						eDigit;					///< ���b�V���R�[�h����
		int						eExe_Flag;              ///< ���s�t���O
		int						e_iMeshCode;			///< ���b�V���R�[�h
		IFeatureClassPtr		e_ipF_Class;            ///< ���b�V���t�B�[�`���N���X
		FILE*					e_fErr_Fp;				///<�@�G���[���O�t�@�C���p�t�@�C���|�C���^
	private:
		prog_init				e_pTool_Init;			///< DB�̏������p�ݒ�
		prog_init				e_pTool_Init_Tgt;		///< DB(Tgt)�̏������p�ݒ�
		smp_sindy_err_info		e_sPrnInfo;				///< �G���[���o�͗p
		exec_time_handle		e_eExeTime;				///< ���s���ԊǗ�
		msi_handle_ws			e_mChk_Src_WS;			///< �`�F�b�N��
		msi_handle_ws			e_mChk_Tgt_WS;			///< �`�F�b�N��f�[�^
		IGeometryPtr			e_ipMeshRect;			///< ���b�V����`
		IGeometryPtr			e_ipMeshRect_Line;		///< ���b�V����`�̋��E��
	private:
		ms_data_info			e_msdChk_Src_Datas;		///< �`�F�b�N���f�[�^
		ms_data_info			e_msdChk_Tgt_Datas;		///< �`�F�b�N�Ώۃf�[�^
	private:
		ms_match_record			e_msMatch_Tbl;			///< �ƌ`��v���R�[�h
		AKAttrHandler           mAKAttrHandler;         ///< AK�Ή������戵���N���X
	private:
		double		m_CompMaxX; ///< �}�[�W����p�ő�X
		double		m_CompMaxY; ///< �}�[�W����p�ő�Y
		double		m_CompMinX; ///< �}�[�W����p�ŏ�X
		double		m_CompMinY; ///< �}�[�W����p�ŏ�Y
	private:
		FILE*					e_fpMatch_Fp;			///< �Ή��e�[�u���t�@�C��
		string					m_sVersion;				///< �o�[�W������2009.10.23 �ǉ�
	public:

		/**
		* @brief �R���X�g���N�^
		*/
		main_rootin	() {
			e_cpMeshCode_File		= NULL;
			e_cpLog_File			= NULL;
			e_cpInit_File			= NULL;
			e_cpInit_File_Tgt		= NULL;
			e_cpCheck_Src_Layer		= NULL;
			e_cpCheck_Tgt_Layer		= NULL;
			e_cpMatch_Tbl_File		= NULL;
			eDigit					= 0;
			eExe_Flag				= 0;
			e_iMeshCode				= 0;
			e_fErr_Fp				= stderr;	// �f�t�H���g�͕W���G���[�o�͂�
			e_msMatch_Tbl.clear();

			m_CompMaxX = 0.0;
			m_CompMaxY = 0.0;
			m_CompMinX = 0.0;
			m_CompMinY = 0.0;

			e_fpMatch_Fp			= NULL;
		}
		/**
		* @�f�X�g���N�^
		*/
		~main_rootin	() {
			if( e_fErr_Fp != NULL ) {
				fclose ( e_fErr_Fp );
			}
			if( e_fpMatch_Fp != NULL ) {
				fclose ( e_fpMatch_Fp );
			}
		}

		/** ------------------------------------------------------------------------------
		* @brief SiNDYDB�ւ̐ڑ��I�[�v��
		* @return	true	����
		* @return	false	���s
		------------------------------------------------------------------------------- */
		bool			SiNDY_DB_Open	(
							IWorkspacePtr&			c_ipWorkspace_Base,
							IWorkspacePtr&			c_ipWorkspace_Addr
						);

		/** ------------------------------------------------------------------------------
		* @brief �����`�F�b�N
		* @return	����	true
		* @return	����	false
		------------------------------------------------------------------------------- */
		bool			arg_check	(
							int		argc,			///< [in] ������
							char	**argv			///< [in] ������
						);

		/** ------------------------------------------------------------------------------
		* @brief �c�[���̎g�p�@�̕\��
		------------------------------------------------------------------------------- */
		void			print_usage	(
							char	**argv			///< [in] ������
						);

		/** ------------------------------------------------------------------------------
		* @brief �o�[�W�������̕\��
		------------------------------------------------------------------------------- */
		void			print_version	( void );

		/** ------------------------------------------------------------------------------
		* @brief ���b�V����`��DB����l�����郁�\�b�h�̏����� \
		* @note ��get_mesh_rect �����O�ɕK���Ă�
		* @return ����=true ���s=false
		------------------------------------------------------------------------------- */
		bool			get_mesh_init	(
							mk_input_list&	c_mMList,			///< [in] ���̓��X�g�N���X
							IWorkspacePtr&	c_ipWorkSpace		///< [in] �w�肷�郏�[�N�X�y�[�X
						);
		/** ------------------------------------------------------------------------------
		* @brief	���b�V����`��GeoDB����擾���� \
		* @note ��GeoDB�ւ̐ڑ����m������Ă��邱�Ƃ��O��
		* @return ���b�V����`�̃G���x���[�v
		------------------------------------------------------------------------------- */
		IEnvelopePtr	get_mesh_rect	(
							int				c_iMeshCode			///< [in] ���b�V���R�[�h
						);
		/** ------------------------------------------------------------------------------
		* @brief �G���[���O�t�@�C�����I�[�v������
		* @return ����=true ���s=false
		------------------------------------------------------------------------------- */
		bool			Open_Err_Log	( void );

		/** ------------------------------------------------------------------------------
		* @brief �o�͑Ή��e�[�u���t�@�C�����I�[�v������
		* @return ����=true ���s=false
		------------------------------------------------------------------------------- */
		bool			Open_Match_Tbl	( void );

		/** ------------------------------------------------------------------------------
		* @brief �`�F�b�N���s��
		* @note �������ŃG���[�t�@�C���̏��������s��
		* @return ����	true
		* @return ���s	false
		------------------------------------------------------------------------------- */
		bool			check_exe	( void );

		/** ------------------------------------------------------------------------------
		* @brief ��`�̃|���S����
		* @return	IPolygonPtr
		------------------------------------------------------------------------------- */
		IPolygonPtr		Get_Rect_Polygon	(
							IEnvelopePtr&	c_ipEnv			///< [in] ���b�V����`
						);

		/** ------------------------------------------------------------------------------
		* @brief �`�F�b�N�p�s���E���X�g�A���b�V���R�[�h���X�g�̓ǂݍ���
		* @return ����=true ���s=false
		------------------------------------------------------------------------------- */
		bool			Read_Chk_List	(
							IWorkspacePtr&	c_ipWSpace,			///< [in] ���[�N�X�y�[�X
							mk_input_list&	c_aChk_List_Cls		///< [in] ���X�g�Ǘ��N���X�̎Q��
						);

		/** ------------------------------------------------------------------------------
		* @brief ���C���I�[�v��
		* @return ����=true ���s=false
		------------------------------------------------------------------------------- */
		bool			Open_Layers		(
							IWorkspacePtr&	ipWorkspace_Src,	///< [in] �`�F�b�N�����[�N�X�y�[�X
							IWorkspacePtr&	ipWorkspace_Tgt 	///< [in] �`�F�b�N�惏�[�N�X�y�[�X
						);

		/** ------------------------------------------------------------------------------
		* @brief �����`�F�b�N�̃��[�v
		------------------------------------------------------------------------------- */
		void			Diff_Check_Roops (
							IFeatureCursorPtr&	c_ipChk_Src_Cursor,	///< [in] �`�F�b�N���ƂȂ�f�[�^�̃J�[�\��
							IFeatureCursorPtr&	c_ipChk_Tgt_Cursor 	///< [in] �`�F�b�N��ƂȂ�f�[�^�̃J�[�\��
						);

		/**
		* @brief �Ή��e�[�u���̏o��
		*/
		void			Print_Table		(
							ms_data_info&		c_ipChk_Src_Data,	///< [in] �`�F�b�N���f�[�^�̃Z�b�g
							ms_data_info&		c_ipChk_Tgt_Data	///< [in] �`�F�b�N���f�[�^�̃Z�b�g
						);

		/** ------------------------------------------------------------------------------
		* @brief ���L�`�F�b�N�S�ʂ��܂Ƃ߂čs��
		------------------------------------------------------------------------------- */
		void			Note_Check	(
							char*				c_cpAddrCode,		///< [in] �Z���R�[�h
							IFeatureCursorPtr&	c_ipNoteCursor,		///< [in] ���L�t�B�[�`��
							int&				c_ipCityName_Count,	///< [in] �s�撬�����J�E���g
							int&				c_ipCityOffice_Count///< [in] �s�撬���̖�����
						);

		/** ------------------------------------------------------------------------------
		* @brief �`�F�b�N�f�[�^���Z�b�g�ɗ��Ƃ�
		* @return �f�[�^��
		------------------------------------------------------------------------------- */
		int				Set_Data_Infos (
							IFeatureCursorPtr&	c_ipChk_Cursor, ///< [in] �`�F�b�N�J�[�\��
							ms_data_info&		c_ipChk_Data	///< [out] �`�F�b�N�f�[�^�̃Z�b�g
						);

		/** ------------------------------------------------------------------------------
		* @brief �\�[�X�ƃ^�[�Q�b�g�̃`�F�b�N���s��
		------------------------------------------------------------------------------- */
		void			Chk_Src_Tgt_Data ( 
							ms_data_info&		c_ipChk_Src_Data,	///< [in] �`�F�b�N���f�[�^�̃Z�b�g
							ms_data_info&		c_ipChk_Tgt_Data, 	///< [in] �`�F�b�N���f�[�^�̃Z�b�g
							msi_handle_ws&		c_mChk_Src_WS,		///< [in] �^�[�Q�b�g�̃��[�N�X�y�[�X�n���h��
							msi_handle_ws&		c_mChk_Tgt_WS,		///< [in] �^�[�Q�b�g�̃��[�N�X�y�[�X�n���h��
							char*				c_cpCheck_Src_Layer	///< [in] �`�F�b�N���C����
						);

		/** ------------------------------------------------------------------------------
		* @brief �폜�E�ǉ��Ɣ��肳�ꂽ���̂ɑ΂��Đ��x���኱���Ƃ����`�ōă`�F�b�N���s��
		------------------------------------------------------------------------------- */
		void			Chk_Retry ( 
							ms_data_info&		c_ipChk_Src_Data,	///< [in] �`�F�b�N���f�[�^�̃Z�b�g
							ms_data_info&		c_ipChk_Tgt_Data, 	///< [in] �`�F�b�N���f�[�^�̃Z�b�g
							msi_handle_ws&		c_mChk_Src_WS,		///< [in] �^�[�Q�b�g�̃��[�N�X�y�[�X�n���h��
							msi_handle_ws&		c_mChk_Tgt_WS,		///< [in] �^�[�Q�b�g�̃��[�N�X�y�[�X�n���h��
							char*				c_cpCheck_Src_Layer	///< [in] �`�F�b�N���C����
						);


		/** ------------------------------------------------------------------------------
		* @brief ���v�����f�[�^���}�b�`�e�[�u���ɓo�^����
		------------------------------------------------------------------------------- */
		void			Entry_to_Match_Tbl(
							data_info&	c_dData1, ///< [in] �f�[�^1
							data_info&	c_dData2  ///< [in] �f�[�^1�ƈ�v�Ƃ݂Ȃ��ꂽ�f�[�^
						);

		/** ------------------------------------------------------------------------------
		* @brief ���b�V�����E�ɑ��݂���f�[�^���擾���A�}�[�W�����}�`���쐬����
		* @note	���}�[�W��̃W�I���g���ƃ}�[�W�������ōł��Ⴂ���b�V���ԍ���PrimID���\�Ƃ��ĕԂ�\
		*       �����b�V�������̃f�[�^�ɂ��Ă������Ń��b�V�����E�Ƃ̔�������A���b�V�����E�łȂ����\
		*         false�̒l��Ԃ����̂Ƃ���
		* @return true	���b�V�����E�ŗאڂ���f�[�^���������ꍇ
		* @return false	�אڃf�[�^�������ꍇ
		------------------------------------------------------------------------------- */
		bool			Get_Neibour_Obj ( 
							data_info&			c_ipChk_Src_Data,	///< [in] �`�F�b�N���f�[�^�̃Z�b�g
							bool				c_iMFlag,			///< [in] ��x�}�[�W�����f�[�^���ǂ����̃t���O
							msi_handle_ws&		c_mChk_Src_WS,		///< [in] �����̃��[�N�X�y�[�X�n���h��
							char*				c_cpCheck_Src_Layer,///< [in] �`�F�b�N���C����
							data_info&			c_ipMerged_Data		///< [out] �אڌ`����}�[�W��������
						);

		/** ------------------------------------------------------------------------------
		* @brief �ᐸ�x�I�u�W�F�N�g�̍쐬�i�|���S���j
		* @return �W�I���g��
		------------------------------------------------------------------------------- */
		IGeometryPtr	Get_LowAccuracy_Obj (
							IGeometryPtr&		c_ipSrcObj,		///< [in] �I�u�W�F�N�g
							int					c_iValue		///< [in] �����_�ȉ������̂P�܂Ō��邩
						);

		/**
		* @brief �g�債����`���쐬
		* @return �g���`�̃G���x���[�v
		*/
		IEnvelopePtr	Get_Expand_Rect	(
							IEnvelopePtr&		c_iRect,	///< [in] ���͋�`
							int					c_iMode,	///< [in] �쐬���[�h�i0=����`�ƍ������_���ꏏ�ɍ쐬�A1=����`�𒆐S�ɍ쐬)
							double				c_dX,		///< [in] X�����̔{��
							double				c_dY		///< [in] Y�����̔{��
						);

		/** ------------------------------------------------------------------------------
		* @brief �W�I���g������_��̏����擾����
		------------------------------------------------------------------------------- */
		void			Get_PtsWithOrientation	(
							IGeometryPtr&			c_ipGeo,	///< [in] �W�I���g��
							set<Real_Pt2>&			c_srPts		///< [out] �_��
						);

		/** ------------------------------------------------------------------------------
		* @brief �`��̃}�b�`���O�����擾����
		* @return	STAT_EQUAL	��v
		* @return	STAT_MODIFY	�`��ύX
		* @return	STAT_LOWA_EQUAL	�`���v�i�ᐸ�x�j
		* @return	STAT_LOWA_MODIFY	�`��ύX�i�ᐸ�x�j
		* @return	0	�s��v
		--------------------------------------------------------------------------------*/
		int				Get_Match_Info (
							data_info&		c_dSrcDat,			///< [in] �`�F�b�N���f�[�^
							data_info&		c_dTgtDat,			///< [in] �`�F�b�N��f�[�^
							ms_data_info&	c_ipChk_Tgt_Data	///< [in] �`�F�b�N���f�[�^�̃Z�b�g
						);

		/** ------------------------------------------------------------------------------
		* @brief �`���v�̃`�F�b�N���s���A��Ԃ�Ԃ�
		* @return	STAT_EQUAL	��v
		* @return	STAT_MODIFY	�`��ύX
		* @return	STAT_LOWA_EQUAL	�`���v�i�ᐸ�x�j
		* @return	STAT_LOWA_MODIFY	�`��ύX�i�ᐸ�x�j
		* @return	0	�s��v
		------------------------------------------------------------------------------- */
		int				Shape_Chk_and_Ret_Stat ( 
							data_info&		c_dSrcDat,		///< [in] �`�F�b�N���f�[�^
							data_info&		c_dTgtDat		///< [in] �`�F�b�N��f�[�^
						);

		/** ------------------------------------------------------------------------------
		* @brief �`���v�̃`�F�b�N
		* @return	true	��v
		* @return	false	�s��v
		------------------------------------------------------------------------------- */
		bool			Shape_Chk	(
							IGeometryPtr&	c_dSrcGeo,		///< [in] �`�F�b�N���f�[�^
							IGeometryPtr&	c_dTgtGeo		///< [in] �`�F�b�N��f�[�^
						);

		/** ------------------------------------------------------------------------------
		* @brief �`���v�̃`�F�b�N�i�ᐸ�x�Łj 2006.09.25
		* @note  ���I�u�W�F�N�g�̏d�����̖ʐς��Z�o���A�d���ʐς��`�F�b�N���f�[�^��1/10�ȉ��Ȃ�\
		*          �s��v�Ƃ��Ĕ��肷��悤�ɂ����B\
		*          �����������́A���̃|�C���g���O���������̂��̂��̏����݂Ĕ��肷��悤�ɂ����B
		* @return	STAT_LOWA_EQUAL		�`���v�i�ᐸ�x�j
		* @return	STAT_LOWA_MODIFY	�`��ύX�i�ᐸ�x�j
		* @return	0	�s��v
		------------------------------------------------------------------------------- */
		int				Shape_Chk_LowAccuracy (
							IGeometryPtr&	c_ipSrcGeo,		///< [in] �`�F�b�N���f�[�^
							IGeometryPtr&	c_ipTgtGeo		///< [in] �`�F�b�N��f�[�^
						);

#ifdef _DEBUG
		void			Print_Geo	(
							data_info&	c_dSrc,
							data_info&	c_dTgt
						);
#endif
		/**
		* @brief �}�[�W�̕K�v���`�F�b�N
		* @return �K�v=true �s�v=false
		*/
		bool			fnIsNeedMergePolygon2(
		                    IGeometryPtr& ipSrcGeom, ///< [in] ���W�I���g��
		                    IGeometryPtr& ipTgtGeom  ///< [in] �^�[�Q�b�g�̃W�I���g��
		                );


//------------------------------------------------------<�����܂�>

};

#endif	//_MAIN_ROOTIN_H_
