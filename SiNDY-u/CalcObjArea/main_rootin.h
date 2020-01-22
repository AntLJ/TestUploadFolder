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

#include "sindy/workspace/name.h"

#ifndef	_MAIN_ROOTIN_H_
#define	_MAIN_ROOTIN_H_

#define		ON_MESH			0x01		///< ���b�V���R�[�h�w�胂�[�h 2002.9.24
#define		ON_BASEMODE		0x02		///< ���k�ڃ��[�h 2002.10.25
#define		ON_MONO_MESH	0x04
#define		ON_WHERE_STR	0x08
#define		ON_PGDB_MODE	0x10
#define		ON_LOGFILE		0x20		///< ���O�t�@�C��
#define		ON_OUTFILE		0x40		///< �o�̓t�@�C��
#define		ON_ADDRDB		0x80		///< �Z��DB
#define		ON_FIELDS		0x100		///< ���擾�t�B�[���h
#define		ON_DB_CNCT		0x200		///< DB������
#define		ON_CHKLAYER		0x400		///< �`�F�b�N���C��
#define		ON_MESHLAYER	0x800		///< ���b�V�����C��
#define		ON_KU_MODE		0x1000		///< ���ʋ�͂܂Ƃ߂Ďs�ɂ���
#define		ON_GUN_MODE		0x2000		///< xx�S������

struct	data_info
{
	string		e_sAddrCode;	// �Z���R�[�h
	string		e_sChimei;		// �n��
	vector<CComVariant>	e_vcAttrs;	// �擾�����l

//	int			e_iMeshCode;	// ���b�V���R�[�h
//	unsigned	e_uDataType;	// �f�[�^�^�C�v 0x30, 0x50
//	unsigned	e_uLayerID;		// ���C��ID
//	unsigned	e_uAttrCode;	// ��ʃR�[�h

	double		e_dSum;			// ���v�l

	/**
		�R���X�g���N�^
	*/
	data_info	( void ) {
//		e_iMeshCode	= 0;
//		e_uDataType	= 0;
//		e_uLayerID	= 0;
//		e_uAttrCode	= 0;
		e_dSum	= 0.0;
	}
	/**
		�f�X�g���N�^
	*/
	~data_info	( void ) { }

	bool	operator<(const data_info c_dDat ) const {
		bool	ret	= false;
		if( e_vcAttrs != c_dDat.e_vcAttrs ) {
			if( e_vcAttrs < c_dDat.e_vcAttrs ) {
				ret	= true;
			}
		}else
		if( e_sAddrCode != c_dDat.e_sAddrCode ) {
			if( e_sAddrCode < c_dDat.e_sAddrCode ) {
				ret	= true;
			}
		}else
		if( e_sChimei != c_dDat.e_sChimei ) {
			if( e_sChimei < c_dDat.e_sChimei ) {
				ret	= true;
			}
		}
		return	( ret );
	}
};

typedef	multiset<data_info, less <data_info> >				ms_data_info;
typedef	multiset<data_info, less <data_info> >::iterator	ims_data_info;

struct	Norm_Pt
{
	int	e_iX;
	int	e_iY;
	Norm_Pt() {
		e_iX	= 0;
		e_iY	= 0;
	}
};

/**
	2005.09.28	Get_Obj_Len_Area�����ɍ쐬
*/

class	main_rootin
{
	public:
		string					e_sMeshCode_File;		///< ���b�V���R�[�h���X�g�t�@�C�� 2002.9.24
		string					e_sLog_File;			///< ���O�f�[�^�o�̓p�X
		string					e_sConnect_Info;		///< �ڑ���񕶎���
		string					e_sCheck_Layer;			///< �`�F�b�N���C����
		string					e_sMesh_Layer;			///< ���b�V�����C����
		string					e_sOut_File;			///< �o�̓t�@�C��
		int						eMake_Type;				///< �f�[�^�쐬�^�C�v 2002.06.26
		int						eExe_Type;				///< ���s�^�C�v
		int						eDigit;					///< ���b�V���R�[�h����
		int						eExe_Flag;
		int						e_iMeshCode;			///< ���b�V���R�[�h
		IFeatureClassPtr		e_ipF_Class;
		FILE*					e_fpErr_Fp;				///< �G���[���O�t�@�C���p�t�@�C���|�C���^
		FILE*					e_fpOut_Fp;				///< �o�̓t�@�C���p�|�C���^
		set<int, less<int> >	e_sExi_Mesh_List;		///< ���݃��b�V���̃��X�g 2002.12.06
	private:
		prog_init				e_pTool_Init;		// DB�̏������p�ݒ�
		string					e_sWhere_Str;		///< Where��p
		common_use				e_cCU;				///< 2007.12.10
		string					e_sVersion;			///< �c�[���̃o�[�W����
		string					e_sConnectAddrDB;	///< �Z��DB�̐ڑ��ݒ�
		string					e_sFields;			///< �擾�t�B�[���h�w��
		vector<string>			e_vsGetField;		///< �����擾����t�B�[���h

		addr_db_handle			e_aAddr_DB_Hndl;	///< �Z��DB�̃n���h�����O

	public:

		main_rootin	()
		{
			eMake_Type				= 0;
			eExe_Type				= 0;
			eDigit					= 0;
			eExe_Flag				= 0;
			e_iMeshCode				= 0;
			e_fpErr_Fp				= stderr;	// �f�t�H���g�͕W���G���[�o�͂�
			e_fpOut_Fp				= stdout;	// �f�t�H���g�͕W���o�͂�
			e_sExi_Mesh_List.clear();
			e_sWhere_Str			= "";
		}
		~main_rootin	()
		{
			if( e_fpErr_Fp != NULL && e_fpErr_Fp != stderr && e_fpErr_Fp != stdout ) {
				fclose ( e_fpErr_Fp );
			}
			if( e_fpOut_Fp != NULL && e_fpOut_Fp != stderr && e_fpOut_Fp != stdout ) {
				fclose ( e_fpOut_Fp );
			}
		}

		//------------------------------------------------------
		// �G���[���b�Z�[�W
		//------------------------------------------------------
		void			PrnMsg	( FILE* c_cpFp,	const char* c_cpFormat, ... ) {
							
							va_list	a_vArgList = NULL;

							va_start(a_vArgList, c_cpFormat );	// ������

							vfprintf_s ( c_cpFp, c_cpFormat, a_vArgList );
							fflush	( c_cpFp );
						}

		/**
			DB�ւ̐ڑ��I�[�v��
			@return	true	����
			@return	false	���s
		*/
		bool			DB_Open	(
							IWorkspacePtr&			c_ipWorkspace
						);

		/**
			�Z��DB�֌W�̃I�[�v��
			@return ����	true
			@return	���s	false
		*/
		bool			Connect_to_Adm_DB	( void );	///< �ڑ�������[IN]


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
			�G���[���O�̃w�b�_���o�͂���
			@return	����
		*/
		void			Output_Log_Header	( void );

		/**
			���b�V����`��DB����l�����郁�\�b�h�̏����� \
			��get_mesh_rect �����O�ɕK���Ă�
		*/
		bool			get_mesh_init	(
							mk_input_list&	c_mMList,			// ���̓��X�g�N���X
						//	string			c_sMeshLayer,		///< ���b�V�����C����
							IWorkspacePtr	c_ipWorkSpace		///< �w�肷�郏�[�N�X�y�[�X
						);
		/**
			���b�V����`��GeoDB����擾���� \
			��GeoDB�ւ̐ڑ����m������Ă��邱�Ƃ��O��
		*/
		IPolygonPtr		get_mesh_rect	(
							int				c_iMeshCode			///< ���b�V���R�[�h
						);
	
		/**
			�G���[���O�t�@�C�����I�[�v������
		*/
		bool			Open_Err_Log	( void );

		/**
			���C���������r�p�̑�����������쐬����
		*/
		//string	Get_Attr_Str	(	string	c_sLayer,	///< ���C����

		/**
			�t�B�[�`�����瑮�������o��
		*/
		void			GetFAttrs	(
							Feature_All_Attrs&	c_fAttrs,	///< �����l[IN]
							data_info&			c_dInfo		///< ���o�������[OUT]
						);

		/**
			�`�F�b�N���s��
			�������ŃG���[�t�@�C���̏��������s��
			@return	����	true
			@return ���s	false
		*/
		bool			check_exe	(
							IWorkspacePtr	c_ipWSpace		///< ���[�N�X�y�[�X
						);

		/**
			��`�̃��C����
			@return	IPolylinePtr
		*/
		IPolylinePtr	Get_Rect_Line	(
							IEnvelopePtr	c_ipEnv			///< ���b�V����`
						);
		/**
			��`�̃|���S����
			@return	IPolygonPtr
		*/
		IPolygonPtr		Get_Rect_Polygon	(
							IEnvelopePtr	c_ipEnv			///< ���b�V����`
						);

		/**
			�N���b�v�}�`��Ԃ�
		*/
		IGeometryPtr	Get_Clip_Obj	(
							IGeometryPtr	c_ipGeo,		///< �W�I���g��
							IGeometryPtr	c_ipClip		///< ����ŃN���b�v
						);

		/**
			�I�u�W�F�N�g�̖ʐϋy�уA�[�N����Ԃ�
		*/
		double			Get_Obj_LA	(
							IGeometryPtr	c_ipGeo,		///< �W�I���g��[IN]
							int				c_iMeshCode,	///< ���b�V���R�[�h[IN]
							int*			c_iType			///< �f�[�^�^�C�v 1=line, 2=polygon
						);

		/**
			�ʐϒl�����߂�
		*/
		double			CalcArea	(
							IGeometryPtr	c_ipSrc_Geo,	///< �N���b�v�O�̃W�I���g��
							IGeometryPtr	c_ipCliped_Geo	///< �N���b�v��̃W�I���g��
						);

		/**
			�����̌v�Z
			@return	���K�����W�l�ł̋���	
		*/
		double			Get_Length	(
							int				c_iPtCount,	// �|�C���g��
							Norm_Pt*		c_ipPts		// IN�_��
						);

		/**
			�|���S���̖ʐ�
		*/
		double			Get_Real_Area	(
							int				c_count,	// ��ԓ_��
							Norm_Pt*		c_ipPts,	// IN�_��
							int				c_meshcode	// ���b�V���R�[�h
						);
		/**
			�|���S���̖ʐ�
		*/
		double			calc_area	(
							int				c_count,	// ��ԓ_��
							Norm_Pt*		c_ipPts,	// IN�_��
							double			c_xratio,	// x�����䗦?
							double			c_yratio	// y�����䗦?
						);

		/**
			���̒ǉ�
			@return	��
		*/
		void			Put_Info	(
							ms_data_info&	c_mInfos,
							data_info		c_dInfo
						);

		/**
			���̏o��(�W�v)
			@return	����
		*/
		void			Print_Info	(
							ms_data_info&	c_mInfos,		///< �o�͏��
							FILE*			c_fpOutFp		///< �o�̓t�@�C���|�C���^
						);

		/**
			���̏o��(�P��)
			@return	����
		*/
		void			Print_Info	(
							int				c_iMeshCode,	///< ���b�V���R�[�h[IN]
							data_info&		c_mInfos,		///< �o�͏��[IN]
							FILE*			c_fpOutFp		///< �o�̓t�@�C���|�C���^[IN]
						);


		/**
			�_��ɂ���
		*/	
		bool			Get_Norm_Ps	(
							IGeometryPtr		c_ipGeo,	///< �W�I���g��[IN]
							int					c_iMeshCode,///< ���b�V���R�[�h[IN]
							int					c_iMode,	///< 0=line,1=Polygon
							vector<Norm_Pt>&	c_vPts		///< �_��[OUT]
						);


};

//------------------------------------------------------<�����܂�>



#endif	//_MAIN_ROOTIN_H_
