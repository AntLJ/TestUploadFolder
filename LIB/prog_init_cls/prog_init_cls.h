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

#pragma once

// 2002.06.05
// �c�[���̏������y�ѐݒ�Ɋւ��Ă̏������s���B
/**
	2002.10.18	�s���E�����k�ځA�s�s�n�}�ɕ����ꂽ�̂őΉ�
*/

/// �ڑ���`
#define		DB_MAP		"DB_MAP"			///< �n�}�nDB�ڑ�
#define		DB_ADDRESS	"DB_ADDRESS"		///< �Z���nDB�ڑ� 
#define		DB_BASE		"DB_BASE"			///< ���k��DB�ڑ�
#define		DB_ROAD		"DB_ROAD"			///< ���H�nDB�ڑ� [bug 7066]�ւ̑Ή� 2009.07.02
#define		DB_ANNO		"DB_ANNO"			///< ���L�nDB�ڑ� city��anno,poi�̕����Ή� 2014.05.07
#define		DB_ANY		"DB_ANY"			///< �ėp

/// ���C����`
#define		INI_UN		"USERNAME"			///< ���[�U��
#define		INI_PW		"PASSWORD"			///< �p�X���[�h
#define		INI_IT		"INSTANCE"			///< �C���X�^���X
#define		INI_SV		"DB_CONNECTION_PROPERTIES"			///< �T�[�o��
#define		INI_VER		"VERSION"			///< �o�[�W����

#define		INI_OPATH	"OUTPUT_PATH"		///< �f�[�^�o�̓p�X��

#define		INI_CMESH	"CITYMESH"			///< �s�s�n�}���b�V�����C��

#define		INI_CNOTE	"CITY_ANNOTATION"	///< �s�s�n�}���L�|�C���g���C��
#define		INI_CNLINE	"CITY_DISP_LINE"	///< �s�s�n�}���L���C�����C��
#define		INI_CHIBAN	"CITY_REP_POINT"	///< �n�Ԓ��L���C��

#define		INI_CLINE	"CITY_LINE"			///< �s�s�n�}�w�i�A�[�N���C��
#define		INI_CRWAY	"CITY_RAILWAY"		///< �s�s�n�}�S�����C��
#define		INI_CSITE	"CITY_SITE"			///< �s�s�n�}�X�惌�C��
#define		INI_CSTA	"CITY_STATION"		///< �s�s�n�}�w�Ƀ��C��
#define		INI_BLD		"BUILDING"			///< �s�s�n�}�������C��
#define		INI_BLDL	"BUILDING_LINE"		///< �s�s�n�}�����A�[�N���C��
#define		INI_STEP	"BUILDING_STEP"		///< �s�s�n�}kpl���C��
#define		INI_ROOF	"BUILDING_ROOF"		///< �s�s�n�}��������{�݌`�󃌃C��
#define		INI_TUNNEL	"CITY_TUNNEL"		///< �s�s�n�}�g���l���B���`�󃌃C��

#define		INI_GOU		"GOU_POINT"			///< ���|�C���g

#define		INI_ADM		"ADMIN"				///< �s���E���C�� ������g�p�H
#define		INI_BADM	"BASE_ADMIN"		///< ���k�ڍs���E
#define		INI_CADM	"CITY_ADMIN"		///< �s�s�n�}�s���E

#define		INI_BMESH	"BASEMESH"			///< ���k�ڃ��b�V�����C��

#define		INI_BNL1	"B_SC1DISP_LINE"	///< ���k�ڒ��L���C�����C���P
#define		INI_BNL2	"B_SC2DISP_LINE"	///< ���k�ڒ��L���C�����C���Q
#define		INI_BNL3	"B_SC3DISP_LINE"	///< ���k�ڒ��L���C�����C���R
#define		INI_BNL4	"B_SC4DISP_LINE"	///< ���k�ڒ��L���C�����C���S
#define		INI_BNOTE	"BASE_ANNOTATION"	///< ���k�ڒ��L

#define		INI_BCNT	"BASE_CONTOUR"		///< �i�ʃ��C��
#define		INI_BLINE	"BASE_LINE"			///< ���k�ڔw�i�A�[�N���C��
#define		INI_BRWAY	"BASE_RAILWAY"		///< ���k�ړS�����C��
#define		INI_BSITE	"BASE_SITE"			///< ���k�ڔw�i�A�{�݃��C��
#define		INI_BSTA	"BASE_STATION"		///< ���k�ډw�Ƀ��C��

#define		INI_MMESH	"MIDDLEMESH"		///< �~�h�����b�V�����C��

#define		INI_MNL1	"M_SC1DISP_LINE"	///< ���k�ڒ��L���C�����C���P
#define		INI_MNL2	"M_SC2DISP_LINE"	///< ���k�ڒ��L���C�����C���Q
#define		INI_MNL3	"M_SC3DISP_LINE"	///< ���k�ڒ��L���C�����C���R
#define		INI_MNL4	"M_SC4DISP_LINE"	///< ���k�ڒ��L���C�����C���S
#define		INI_MNOTE	"MIDDLE_ANNOTATION"	///< ���k�ڒ��L

#define		INI_MCNT	"MIDDLE_CONTOUR"	///< �i�ʃ��C��
#define		INI_MLINE	"MIDDLE_LINE"		///< ���k�ڔw�i�A�[�N���C��
#define		INI_MSITE	"MIDDLE_SITE"		///< ���k�ڔw�i�A�{�݃��C��

#define		INI_TMESH	"TOPMESH"

// �g�b�v�}�b�v���L�p
#define		INI_TNL1	"T_SC1DISP_LINE"	///< ���k�ڒ��L���C�����C���P
#define		INI_TNL2	"T_SC2DISP_LINE"	///< ���k�ڒ��L���C�����C���Q
#define		INI_TNL3	"T_SC3DISP_LINE"	///< ���k�ڒ��L���C�����C���R
#define		INI_TNL4	"T_SC4DISP_LINE"	///< ���k�ڒ��L���C�����C���S
#define		INI_TNOTE	"TOP_ANNOTATION"	///< ���k�ڒ��L
// �g�b�v�}�b�v�p 2003.01.05
#define		INI_TCNT	"TOP_CONTOUR"		///< �i�ʃ��C��
#define		INI_TLINE	"TOP_LINE"			///< �g�b�v�}�b�v�w�i�A�[�N���C��
#define		INI_TSITE	"TOP_SITE"			///< �g�b�v�}�b�v�w�i�A�{�݃��C��

#define		INI_RLINK	"ROAD_LINK"			///< ���H�����N���C��

#define		INI_PAREA	"PAREA"				///< PAREA��\�_���C��
#define		INI_BRP		"BASE_REP_POINT"	///< ���k��Addr�|�C���g

#define		INI_POI		"POI_POINT"			///< POI_POINT

// �����e�[�u���p
#define		INI_BACK_ATTR	"BACKGROUND_CLASS"	///< �w�i��ʗp�e�[�u��
#define		INI_ANNO_ATTR	"ANNOTATION_CLASS"	///< ���L��ʗp�e�[�u��

typedef	std::set<int>												s_int;		///< ���b�V�����X�g�p
typedef	std::set<int>::iterator										is_int;		///< ���b�V�����X�g�p�C�e���[�^
typedef	std::set<std::string, std::less<std::string> >				s_charp;	///< �ėp���X�g�p(�����񃊃X�g�j
typedef	std::set<std::string, std::less<std::string> >::iterator	is_charp;	///< �ėp���X�g�p(�����񃊃X�g�j

/**
	�悭�g�p����A���b�V����s���R�[�h���̃��X�g��ǂݍ��ޗp�ɍ쐬����
	�N���X
	2002.8.19 �s���R�[�h���̓ǂݍ��݂��o����悤�ɉ���
*/
class	mk_input_list
{
	private:
		int			mesh_digit;	///< ���b�V���̌���
		s_int		mesh_set;	///< ���b�V���̃��X�g
		is_int		mesh_indx;	///< ���b�V�����X�g�̃C���f�b�N�X
		s_charp		data_set;	///< �ǂݍ��񂾃f�[�^��
		is_charp	data_indx;	///< �f�[�^��̃C���f�b�N�X
	public:
		/**
			�R���X�g���N�^
		*/
		mk_input_list	()	{
			mesh_digit		= 0;
			mesh_set.clear();
			mesh_indx		= mesh_set.begin();
			data_set.clear();
			data_indx		= data_set.begin();
		}

		/**
			�f�X�g���N�^
		*/
		~mk_input_list	()	{
			// �ėp���X�g�́A�������̃A���P�[�g������Ă���̂ŁA������Ȃ��Ƃ���
		/*	for(data_indx = data_set.begin(); data_indx != data_set.end(); data_indx++)
			{
				if(*data_indx != NULL) {
					delete [] *data_indx;	// �������̉��
				}
			}*/
		}

		/**
			���b�V�����X�g�̓ǂݍ���
		*/
		int		read_mesh_list	(	char	*c_cFile	);

		/**
			���b�V�����X�g�̓ǂݍ���
			���w�肵�����b�V�����P�P�ǂݍ���ł���
		*/
		int		read_mesh_list	(	int		c_iMesh	);

		/**
			�T�C�Y��Ԃ�
			@return		����	0�@���s�@-1
		*/
		int		get_size	() {
					return	(mesh_set.size());
				}

		/**
			���b�V�����X�g�̍ŏ��̃��b�V����Ԃ�
		*/
		int		get_mesh_first	() {
					mesh_indx	= mesh_set.begin();
					int	mesh	= *mesh_indx;
					return	(mesh);
				}
		/**
			���b�V�����X�g�̎��̃��b�V����Ԃ�
		*/
		int		get_mesh_next	() {
					mesh_indx++;
					if(mesh_indx == mesh_set.end()) {
						return	(-1);
					}
					int	mesh	= *mesh_indx;
					return	(mesh);
				}
		/**
			���b�V�����X�g�̌�����Ԃ�
		*/
		int		get_mesh_digit	() {
					return	( mesh_digit );
				}

		//--------------------------------------------------
		/**
			�f�[�^���X�g�̓ǂݍ���
		*/
		int		read_data_list	(	char	*c_cFile	);

		/** 2007.06.28 �ǉ�
			�����񃌃R�[�h�̒ǉ�
		*/
		void	put_str_record	(	char	*c_cStr	);

		/**
			�f�[�^��̍ŏ��̃f�[�^��Ԃ�
		*/
		is_charp	get_indx_first	() {
						data_indx	= data_set.begin();
						return	( data_indx );
					}

		/**
			�f�[�^��̎��̃f�[�^��Ԃ�
		*/
		is_charp	get_indx_next	() {
						data_indx++;
						if( data_indx == data_set.end() ) {
							return	( data_indx );
						}
						return	( data_indx );
					}
		/**
			�f�[�^��̏I������
		*/
		bool		is_indx_end	() {
						if( data_indx == data_set.end() ) {
							return	( true );
						}
						return	( false );
					}
		//--------------------------------------------------

};

/**
	�v���O�����������p�̃N���X
*/

class	prog_init
{
	///=����DB�ڑ��ݒ�ɑΉ����邽�߂ɒǉ�
	std::string		e_sUserName_Map;		///< ���[�U��
	std::string		e_sPassword_Map;		///< �p�X���[�h
	std::string		e_sVersion_Map;			///< �o�[�W����
	std::string		e_sInstance_Map;		///< �C���X�^���X
	std::string		e_sServer_Map;			///< �T�[�o��
	std::string		e_sDataFile_Path_Map;	///< �f�[�^�t�@�C��(PGDB��)�̃p�X 2007.03.01

	std::string		e_sUserName_Address;	///< ���[�U��
	std::string		e_sPassword_Address;	///< �p�X���[�h
	std::string		e_sVersion_Address;		///< �o�[�W����
	std::string		e_sInstance_Address;	///< �C���X�^���X
	std::string		e_sServer_Address;		///< �T�[�o��
	std::string		e_sDataFile_Path_Address;///< �f�[�^�t�@�C��(PGDB��)�̃p�X 2007.03.01

	std::string		e_sUserName_Base;		///< ���[�U��
	std::string		e_sPassword_Base;		///< �p�X���[�h
	std::string		e_sVersion_Base;		///< �o�[�W����
	std::string		e_sInstance_Base;		///< �C���X�^���X
	std::string		e_sServer_Base;			///< �T�[�o��
	std::string		e_sDataFile_Path_Base;	///< �f�[�^�t�@�C��(PGDB��)�̃p�X 2007.03.01

	std::string		e_sUserName_Road;		///< ���[�U�� [bug 7066]�ւ̑Ή� 2009.07.02
	std::string		e_sPassword_Road;		///< �p�X���[�h [bug 7066]�ւ̑Ή� 2009.07.02
	std::string		e_sVersion_Road;		///< �o�[�W���� [bug 7066]�ւ̑Ή� 2009.07.02
	std::string		e_sInstance_Road;		///< �C���X�^���X [bug 7066]�ւ̑Ή� 2009.07.02
	std::string		e_sServer_Road;			///< �T�[�o�� [bug 7066]�ւ̑Ή� 2009.07.02
	std::string		e_sDataFile_Path_Road;	///< �f�[�^�t�@�C��(PGDB��)�̃p�X [bug 7066]�ւ̑Ή� 2009.07.02

	std::string		e_sUserName_Anno;		///< ���[�U��
	std::string		e_sPassword_Anno;		///< �p�X���[�h
	std::string		e_sVersion_Anno;		///< �o�[�W����
	std::string		e_sInstance_Anno;		///< �C���X�^���X
	std::string		e_sServer_Anno;			///< �T�[�o��
	std::string		e_sDataFile_Path_Anno;	///< �f�[�^�t�@�C��(PGDB��)�̃p�X

	std::string		e_sUserName_Any;		///< ���[�U��
	std::string		e_sPassword_Any;		///<�p�X���[�h
	std::string		e_sVersion_Any;			///< �o�[�W����
	std::string		e_sInstance_Any;		///< �C���X�^���X
	std::string		e_sServer_Any;			///< �T�[�o��
	std::string		e_sDataFile_Path_Any;	///< �f�[�^�t�@�C��(PGDB��)�̃p�X 2007.03.01

	///=[�����̐ݒ�͊����𐶂������߂̐ݒ�========================
	std::string		e_sUserName;		///< ���[�U��
	std::string		e_sPassword;		///<�p�X���[�h
	std::string		e_sVersion;			///< �o�[�W����
	std::string		e_sInstance;		///< �C���X�^���X
	std::string		e_sServer;			///< �T�[�o��
	std::string		e_sDataFile_Path;	///< �f�[�^�t�@�C��(PGDB��)�̃p�X 2007.03.01

//	std::string		e_sOutput_Path;		///< �f�[�^�o�̓p�X��

	std::string		e_sCity_Mesh;		///< �s�s�n�}���b�V�����C��

	std::string		e_sCity_Note;		///< �s�s�n�}���L�|�C���g���C��
	std::string		e_sCity_Note_Line;	///< �s�s�n�}���L���C�����C��
	std::string		e_sChiban;			///< �n�Ԓ��L���C��

	std::string		e_sCity_Line;		///< �s�s�n�}�w�i�A�[�N���C��
	std::string		e_sCity_RailWay;	///< �s�s�n�}�S�����C��
	std::string		e_sCity_Site;		///< �s�s�n�}�X�惌�C��
	std::string		e_sCity_Station;	///< �s�s�n�}�w�Ƀ��C��
	std::string		e_sCity_Building;	///< �s�s�n�}�������C��
	std::string		e_sCity_Build_Line;	///< �s�s�n�}�����A�[�N���C��
	std::string		e_sCity_Build_Step;		///< �s�s�n�}kpl���C��
	std::string     e_sCity_Building_Roof;  ///< �s�s��������{�݃|���S��
	std::string     e_sCity_Tunnel;     ///< �s�s�g���l���B���|���S��

	std::string		e_sGou_Point;		///< ���|�C���g

	std::string		e_sAdmin;			///< �s���E���C��
	std::string		e_sBase_Admin;		///< ���k�ڍs���E���C��
	std::string		e_sCity_Admin;		///< �s�s�n�}�s���E���C��

	std::string		e_sBase_Mesh;		///< ���k�ڃ��b�V�����C��

	std::string		e_sBase_Note_Line1;	///< ���k�ڒ��L���C�����C���P
	std::string		e_sBase_Note_Line2;	///< ���k�ڒ��L���C�����C���Q
	std::string		e_sBase_Note_Line3;	///< ���k�ڒ��L���C�����C���R
	std::string		e_sBase_Note_Line4;	///< ���k�ڒ��L���C�����C���S

	std::string		e_sBase_Note;		///< ���k�ڒ��L
	std::string		e_sBase_Contour;	///< �i�ʃ��C��
	std::string		e_sBase_Line;		///< ���k�ڔw�i�A�[�N���C��
	std::string		e_sBase_Railway;	///< ���k�ړS�����C��
	std::string		e_sBase_Site;		///< ���k�ڔw�i�A�{�݃��C��
	std::string		e_sBase_Station;	///< ���k�ډw�Ƀ��C��

	std::string		e_sMiddle_Mesh;		///< �~�h�����b�V�����C��

	std::string		e_sMiddle_Note_Line1;	///< �~�h�����L���C�����C���P
	std::string		e_sMiddle_Note_Line2;	///< �~�h�����L���C�����C���Q
	std::string		e_sMiddle_Note_Line3;	///< �~�h�����L���C�����C���R
	std::string		e_sMiddle_Note_Line4;	///< �~�h�����L���C�����C���S

	std::string		e_sMiddle_Note;		///< �~�h�����L
	std::string		e_sMiddle_Contour;	///< �i�ʃ��C��
	std::string		e_sMiddle_Line;		///< �~�h���w�i�A�[�N���C��
	std::string		e_sMiddle_Site;		///< �~�h���w�i�A�{�݃��C��

	// �g�b�v�}�b�v�p 2004.01.05

	std::string		e_sTop_Mesh;

	std::string		e_sTop_Contour;		///< �i�ʃ��C��
	std::string		e_sTop_Line;		///< �g�b�v�w�i�A�[�N���C��
	std::string		e_sTop_Site;		///< �g�b�v�w�i�A�{�݃��C��
	// �g�b�v�}�b�v���L�p 2004.02.13
	std::string		e_sTop_Note_Line1;	///< �g�b�v���L���C�����C���P
	std::string		e_sTop_Note_Line2;	///< �g�b�v���L���C�����C���Q
	std::string		e_sTop_Note_Line3;	///< �g�b�v���L���C�����C���R
	std::string		e_sTop_Note_Line4;	///< �g�b�v���L���C�����C���S
	std::string		e_sTop_Note;		///< �g�b�v���L

	std::string		e_sRoad_Link;		///< ���H�����N���C��
	std::string		e_sParea;			///< PAREA��\�_���C��
	std::string		e_sBase_Rep_Point;	///< ���k��Addr�|�C���g

	std::string		e_sPOI_Point;		///< POI_Point [bug 9264]�Ή�

	std::string		e_sBackground_Class;	///< �w�i��ʗp�e�[�u��
	std::string		e_sAnnotation_Class;	///< ���L��ʗp�e�[�u��
	///=[�����܂ł̐ݒ�͊����𐶂������߂̐ݒ�====================

	/** 2007.10.25
		FC���������ϐ��ɕϊ�
	*/
	char*	Add_FC	( char*	c_cpEnvStr )	///< ���ϐ�
	{
		static char	a_cpTmp[256];
		memset	( a_cpTmp, '\0', 256 );
		sprintf	( a_cpTmp, "FC_%s", c_cpEnvStr );
		return	( a_cpTmp );
	}

public:
	prog_init	()	{}	///< �R���X�g���N�^
	~prog_init	()	{}	///< �f�X�g���N�^
	

	//�@���ʐݒ�p�ɒǉ� ================================
	//----<�n�}�nDB�p>----------------------------------
	char*	get_UserName_Map() {		///< ���[�U��
				return	(&e_sUserName_Map[0]);
			}
	char*	get_Password_Map() {		///<�p�X���[�h
				return	(&e_sPassword_Map[0]);
			}
	char*	get_Instance_Map() {		///< �C���X�^���X
				return	(&e_sInstance_Map[0]);
			}
	char*	get_Server_Map	() {		///< �T�[�o��
				return	(&e_sServer_Map[0]);
			}
	char*	get_Version_Map	() {		///< �o�[�W����
				return	(&e_sVersion_Map[0]);
			}
	char*	get_DataFile_Path_Map	() {///< �f�[�^�t�@�C��(PGDB��)�̃p�X 2007.03.01
				return	(&e_sDataFile_Path_Map[0]);
			}
	//----<�Z���nDB�p>----------------------------------
	char*	get_UserName_Address() {		///< ���[�U��
				return	(&e_sUserName_Address[0]);
			}
	char*	get_Password_Address() {		///<�p�X���[�h
				return	(&e_sPassword_Address[0]);
			}
	char*	get_Instance_Address() {		///< �C���X�^���X
				return	(&e_sInstance_Address[0]);
			}
	char*	get_Server_Address	() {		///< �T�[�o��
				return	(&e_sServer_Address[0]);
			}
	char*	get_Version_Address	() {		///< �o�[�W����
				return	(&e_sVersion_Address[0]);
			}
	char*	get_DataFile_Path_Address () {	///< �f�[�^�t�@�C��(PGDB��)�̃p�X 2007.03.01
				return	(&e_sDataFile_Path_Address[0]);
			}
	//----<���k�ڌnDB�p>----------------------------------
	char*	get_UserName_Base() {		///< ���[�U��
				return	(&e_sUserName_Base[0]);
			}
	char*	get_Password_Base() {		///<�p�X���[�h
				return	(&e_sPassword_Base[0]);
			}
	char*	get_Instance_Base() {		///< �C���X�^���X
				return	(&e_sInstance_Base[0]);
			}
	char*	get_Server_Base	() {		///< �T�[�o��
				return	(&e_sServer_Base[0]);
			}
	char*	get_Version_Base	() {	///< �o�[�W����
				return	(&e_sVersion_Base[0]);
			}
	char*	get_DataFile_Path_Base () {	///< �f�[�^�t�@�C��(PGDB��)�̃p�X 2007.03.01
				return	(&e_sDataFile_Path_Base[0]);
			}
	//----<���H�nDB�p>----------------------------------
	// [Bug 7066]�ւ̑Ή� 2009.07.02
	char*	get_UserName_Road() {		///< ���[�U��
				return	(&e_sUserName_Road[0]);
			}
	char*	get_Password_Road() {		///<�p�X���[�h
				return	(&e_sPassword_Road[0]);
			}
	char*	get_Instance_Road() {		///< �C���X�^���X
				return	(&e_sInstance_Road[0]);
			}
	char*	get_Server_Road	() {		///< �T�[�o��
				return	(&e_sServer_Road[0]);
			}
	char*	get_Version_Road	() {	///< �o�[�W����
				return	(&e_sVersion_Road[0]);
			}
	char*	get_DataFile_Path_Road () {	///< �f�[�^�t�@�C��(PGDB��)�̃p�X
				return	(&e_sDataFile_Path_Road[0]);
			}
	//----<���L�nDB�p>----------------------------------
	// city��anno,poi�̕����Ή� 2014/05/07
	char*	get_UserName_Anno() {		///< ���[�U��
				return	(&e_sUserName_Anno[0]);
			}
	char*	get_Password_Anno() {		///<�p�X���[�h
				return	(&e_sPassword_Anno[0]);
			}
	char*	get_Instance_Anno() {		///< �C���X�^���X
				return	(&e_sInstance_Anno[0]);
			}
	char*	get_Server_Anno	() {		///< �T�[�o��
				return	(&e_sServer_Anno[0]);
			}
	char*	get_Version_Anno	() {	///< �o�[�W����
				return	(&e_sVersion_Anno[0]);
			}
	char*	get_DataFile_Path_Anno () {	///< �f�[�^�t�@�C��(PGDB��)�̃p�X
				return	(&e_sDataFile_Path_Anno[0]);
			}
	//==================================================
	//----<�����ݒ薔�͔ėp�ڑ�DB�p>----------------------------------
	char*	get_UserName	() {	///< ���[�U��
				return	(&e_sUserName[0]);
			}
	char*	get_Password	() {		///<�p�X���[�h
				return	(&e_sPassword[0]);
			}
	char*	get_Instance	() {		///< �C���X�^���X
				return	(&e_sInstance[0]);
			}
	char*	get_Server	() {			///< �T�[�o��
				return	(&e_sServer[0]);
			}
	char*	get_Version	() {			///< �o�[�W����
				return	(&e_sVersion[0]);
			}
	char*	get_DataFile_Path	() {	///< �f�[�^�t�@�C��(PGDB��)�̃p�X 2007.03.01
				return	(&e_sDataFile_Path[0]);
			}
	char*	get_City_Mesh	() {		///< �s�s�n�}���b�V�����C��
				return	(&e_sCity_Mesh[0]);
			}
	char*	get_City_Note	() {		///< �s�s�n�}���L�|�C���g���C��
				return	(&e_sCity_Note[0]);
			}
	char*	get_City_Note_Line	() {	///< �s�s�n�}���L���C�����C��
				return	(&e_sCity_Note_Line[0]);
			}
	char*	get_Chiban		() {
				return	(&e_sChiban[0]);
			}
	char*	get_City_Line	() {		///< �s�s�n�}�w�i�A�[�N���C��
				return	(&e_sCity_Line[0]);
			}
	char*	get_City_RailWay	() {	///< �s�s�n�}�S�����C��
				return	(&e_sCity_RailWay[0]);
			}
	char*	get_City_Site	() {		///< �s�s�n�}�X�惌�C��
				return	(&e_sCity_Site[0]);
			}
	char*	get_City_Station	() {	///< �s�s�n�}�w�Ƀ��C��
				return	(&e_sCity_Station[0]);
			}
	char*	get_City_Building() {		///< �s�s�n�}�������C��
				return	(&e_sCity_Building[0]);
			}
	char*	get_City_Building_Roof() {  ///< �s�s��������{�݃|���S��
	            return ( &e_sCity_Building_Roof[0]);
	        }
	char*	get_City_Tunnel() {         ///< �s�s�g���l���B���|���S��
	            return ( &e_sCity_Tunnel[0]);
	        }
	char*	get_Gou_Point() {			///< ���|�C���g���C�� 2003.7.18
				return	(&e_sGou_Point[0]);
			}
	char*	get_City_Build_Line() {		///< �s�s�n�}�����A�[�N���C��
				return	(&e_sCity_Build_Line[0]);
			}
	char*	get_City_Build_Step	() {	///< �s�s�n�}kpl���C��
				return	(&e_sCity_Build_Step[0]);
			}
	char*	get_Admin	() {			///< �s���E���C��
				return	(&e_sAdmin[0]);
			}
	char*	get_Base_Admin	() {		///< ���k�ڍs���E���C��
				return	(&e_sBase_Admin[0]);
			}
	char*	get_City_Admin	() {		///< �s�s�n�}�s���E���C��
				return	(&e_sCity_Admin[0]);
			}
	char*	get_Base_Mesh	() {		///< ���k�ڃ��b�V�����C��
				return	(&e_sBase_Mesh[0]);
			}
	char*	get_Base_Note_Line1	() {	///< ���k�ڒ��L���C�����C���P
				return	(&e_sBase_Note_Line1[0]);
			}
	char*	get_Base_Note_Line2	() {	///< ���k�ڒ��L���C�����C���Q
				return	(&e_sBase_Note_Line2[0]);
			}
	char*	get_Base_Note_Line3	() {	///< ���k�ڒ��L���C�����C���R
				return	(&e_sBase_Note_Line3[0]);
			}
	char*	get_Base_Note_Line4	() {	///< ���k�ڒ��L���C�����C���S
				return	(&e_sBase_Note_Line4[0]);
			}
	char*	get_Base_Note	() {		///< ���k�ڒ��L
				return	(&e_sBase_Note[0]);
			}
	char*	get_Base_Contour	() {	///< �i�ʃ��C��
				return	(&e_sBase_Contour[0]);
			}
	char*	get_Base_Line	() {		///< ���k�ڔw�i�A�[�N���C��
				return	(&e_sBase_Line[0]);
			}
	char*	get_Base_Railway	() {	///< ���k�ړS�����C��
				return	(&e_sBase_Railway[0]);
			}
	char*	get_Base_Site	() {		///< ���k�ڔw�i�A�{�݃��C��
				return	(&e_sBase_Site[0]);
			}
	char*	get_Base_Station	() {	///< ���k�ډw�Ƀ��C��
				return	(&e_sBase_Station[0]);
			}
	char*	get_Middle_Mesh	() {		///< �~�h�����b�V�����C��
				return	(&e_sMiddle_Mesh[0]);
			}
	char*	get_Middle_Note_Line1	() {	///< �~�h�����L���C�����C���P
				return	(&e_sMiddle_Note_Line1[0]);
			}
	char*	get_Middle_Note_Line2	() {	///< �~�h�����L���C�����C���Q
				return	(&e_sMiddle_Note_Line2[0]);
			}
	char*	get_Middle_Note_Line3	() {	///< �~�h�����L���C�����C���R
				return	(&e_sMiddle_Note_Line3[0]);
			}
	char*	get_Middle_Note_Line4	() {	///< �~�h�����L���C�����C���S
				return	(&e_sMiddle_Note_Line4[0]);
			}
	char*	get_Middle_Note	() {		///< �~�h�����L
				return	(&e_sMiddle_Note[0]);
			}
	char*	get_Middle_Contour	() {	///< �i�ʃ��C��
				return	(&e_sMiddle_Contour[0]);
			}
	char*	get_Middle_Line	() {		///< �~�h���w�i�A�[�N���C��
				return	(&e_sMiddle_Line[0]);
			}
	char*	get_Middle_Site	() {		///< �~�h���w�i�A�{�݃��C��
				return	(&e_sMiddle_Site[0]);
			}
	// �g�b�v�}�b�v�p 2003.01.05
	char*	get_Top_Mesh	() {		///< �~�h�����b�V�����C��
				return	(&e_sTop_Mesh[0]);
			}
	char*	get_Top_Contour	() {		///< �i�ʃ��C��
				return	(&e_sTop_Contour[0]);
			}
	char*	get_Top_Line	() {		///< �g�b�v�w�i�A�[�N���C��
				return	(&e_sTop_Line[0]);
			}
	char*	get_Top_Site	() {		///< �g�b�v�w�i�A�{�݃��C��
				return	(&e_sTop_Site[0]);
			}
	char*	get_Top_Note_Line1	() {	///< �g�b�v���L���C�����C���P
				return	(&e_sTop_Note_Line1[0]);
			}
	char*	get_Top_Note_Line2	() {	///< �g�b�v���L���C�����C���Q
				return	(&e_sTop_Note_Line2[0]);
			}
	char*	get_Top_Note_Line3	() {	///< �g�b�v���L���C�����C���R
				return	(&e_sTop_Note_Line3[0]);
			}
	char*	get_Top_Note_Line4	() {	///< �g�b�v���L���C�����C���S
				return	(&e_sTop_Note_Line4[0]);
			}
	char*	get_Top_Note	() {		///< �g�b�v���L
				return	(&e_sTop_Note[0]);
			}

	char*	get_Road_Link	() {		///< ���H�����N���C��
				return	(&e_sRoad_Link[0]);
			}
	char*	get_Parea		() {		///< PAREA��\�_���C��
				return	(&e_sParea[0]);
			}
	char*	get_Base_Rep_Point	() {	///< �w�i��ʗp�e�[�u��
				return	(&e_sBase_Rep_Point[0]);
			}
	char*	get_Background_Class	() {	///< ���k��Addr�|�C���g
				return	(&e_sBackground_Class[0]);
			}
	char*	get_Annotation_Class	() {	///< ���L��ʗp�e�[�u��
				return	(&e_sAnnotation_Class[0]);
			}
	char*	get_POI_Point () {	///< POI_POINT�e�[�u�� [bug 9264]
				return	(&e_sPOI_Point[0]);
			}

	/**
		�v���O�����������t�@�C���̓ǂݍ���
		@return �����@0�@���s�@-1
	*/
	int		read_init_file	(
				char	*c_cFile,	///< �ǂݍ��݃t�@�C����
				int		c_iMode=0	///< �ǂݍ��݃��[�h 0=SDE�p 1=SiNDY-C�p 2007.03.01
			);
	/**
		�v���O�������������ϐ��̓ǂݍ���
		@return �����@0�@���s�@-1
	*/
	int		read_init_env	(
				int		c_iMode=0	///< �ǂݍ��݃��[�h 0=SDE�p 1=SiNDY-C�p 2007.03.01
			);

	/**
		�ڑ����̎擾
		@return	����	true
		@return	���s	false
	*/
	bool	get_Connect_Info	(
				char*			c_cpConnect_Str,	///< �ڑ�������
				std::string*	c_UserName,			///< ���[�U��[out]
				std::string*	c_Password,			///< �p�X���[�h[out]
				std::string*	c_Version,			///< �o�[�W����[out]
				std::string*	c_Instance,			///< �C���X�^���X[out]
				std::string*	c_Server			///< �T�[�o��[out]
			);

	/**	2007.06.27 �ǉ� from CrossRoadsNameChk ����
		������������̓ǂݍ���
		@return	����	true
		@return	���s	false
	*/
	bool	read_init_str	(
				char	*c_cpConnect_Str
			);

};
