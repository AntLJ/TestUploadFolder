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
#include <string>
#include <coord_converter.h>
#include "exec_time_handle.h"
#include <atlstr.h>
#include <SiNDYLib/Workspace.h>
#include "GeometryFinder.h"
#include "MakeShapeFile.h"
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>


#ifndef	_MAIN_ROUTINE_H_
#define	_MAIN_ROUTINE_H_

#define		ON_LOGFILE  0x01 //!< ���O�t�@�C���w��
#define		ON_DELETE2  0x02 //!< 2���C���ڂ̊֌W�I�u�W�F�N�g�̍폜���������{
#define     ON_DELMODE  0x04 //!< �폜���[�h�w��i�f�t�H���g�̓`�F�b�N���[�h�j

namespace {
	typedef boost::char_separator<char> char_separator;
	typedef boost::tokenizer<char_separator> tokenizer;
}

/**
* @brief �G���[���b�Z�[�W
* @param c_cpFp     [in] �o�̓t�@�C���|�C���^
* @param c_cpFp2    [in] �o�̓t�@�C���|�C���^�Q(�t�@�C����stdout�ɓ����E�E�̂悤�ȏꍇ��z��j
* @param c_cpFormat [in] �o�͕ϊ�����
*/
void PrnMsg ( FILE* c_cpFp, FILE* c_cpFp2, const char* c_cpFormat, ... );

// typedef std::map<long, CAdapt<IFeaturePtr>, std::less<long> > mIFeatureContainer;
typedef std::map<long, CAdapt<IGeometryPtr>, std::less<long> > mIFeatureContainer;

/**
* @brief �I�u�W�F�N�g�̓��Y���b�V���ŏ������邩�̔���Ŏg�p
*/
struct XYPt
{

	double mX; //!< �o�x
	double mY; //!< �ܓx

	/**
	* @brief �R���X�g���N�^
	*/
	XYPt() {
		mX = 0;
		mY = 0;
	}
	/**
	* @brief �f�X�g���N�^
	*/
	~XYPt() {}

	/**
	* @brief ��r���Z�q
	* @return src<tgt true, ����ȊO=false
	*/
	bool operator<(const XYPt& cTgt) const {
		bool bRet = false;
		if( mX != cTgt.mX ) {
			if( mX < cTgt.mX ) { bRet = true; }
		}else {
			if( mY != cTgt.mY ) {
				if( mY < cTgt.mY ) { bRet = true; }
			}
		}
		return bRet;
	}
};

/**
 * @brief �I�u�W�F�N�g�̗D��x�Ǘ��p�N���X
 * @note �D��x�́APOI�E���L�f�[�^�@���s���Y��񁄖h�ΑΏە����POI�E���L�f�[�^�A��POI�E���L�f�[�^�B �̏�
 * @note �f�[�^�^�C�v�A���L��ʁA�R���e���c�R�[�h���L�[�ɁA�D��x��Ԃ�
 */
struct PriorityManager
{
	enum {
		TYPE_POI = 1,       //!< POI
		TYPE_REALESTATE = 2,//!< �s���Y���
		TYPE_FIREPROOF = 3, //!< �h�ΑΏە����
	};

	/**
	* @brief �D��x���N���X
	*/
	struct PriorityInfo {
		long        mlDataType;      //!< �f�[�^�̃^�C�v
	//	long        mlAnnoAttr;      //!< ���L��ʃR�[�h
		long        mlContentsCode; //!< �R���e���c�R�[�h
		long        mlPriority;      //!< �S�̂Ƃ��Ă̗D��x
		std::string msName;          //!< �����̖���

		/**
		* @brief �R���X�g���N�^
		*/
		PriorityInfo(){
			mlDataType = -1;
		//	mlAnnoAttr = -1;
			mlContentsCode = -1;
			mlPriority = -1;
		}
		/**
		* @brief �f�X�g���N�^
		*/
		~PriorityInfo() {
		}

		/**
		* @brief ��r���Z�q
		* @note  �f�[�^�^�C�v�A���L��ʁA�R���e���c�R�[�h�Ń\�[�g
		* @return src<tgt = true ���L�ȊO=false0
		*/
		bool operator<(const PriorityInfo& cTgt) const {
			if( mlDataType != cTgt.mlDataType ) {
				if( mlDataType < cTgt.mlDataType ) // �f�[�^�^�C�v
					return true;
			}else if( mlContentsCode != cTgt.mlContentsCode ) {
				if( mlContentsCode < cTgt.mlContentsCode ) // �R���e���c�R�[�h
					return true;
			}
			return false;
		}

	};

	// PriorityManager�̃����o
	std::set<PriorityInfo> msPriorityInfo; //!< �D��x���

	/**
	* @brief �R���X�g���N�^
	*/
	PriorityManager(){}
	
	/**
	* @brief �f�X�g���N�^
	*/
	~PriorityManager(){}

	/**
	* @brief �e�L�X�g�t�@�C���̍s����͂��āA�D��x���R�[�h�ɕϊ�����
	* @return ����=true,���s=false
	*/
	bool analyze_line( const char*   c_cpTxt,  ///< [in] �e�L�X�g�̈�s�̃f�[�^
	                   PriorityInfo& c_pinfo ) ///< [out] �D��x���
	{
		char_separator csSep("\t", "", boost::keep_empty_tokens); // �����ݒ�
		string         sBuf;
		sBuf         = c_cpTxt;
		tokenizer      a_tTokens(sBuf, csSep); // ����

		tokenizer::iterator iIter;
		long lLoop = 0;
		for (iIter = a_tTokens.begin(); iIter != a_tTokens.end(); ++iIter++, ++lLoop)
		{
			switch (lLoop)
			{
			case	0:
				if( (*iIter) == "POI" ) {
					c_pinfo.mlDataType = TYPE_POI;
				}else if( (*iIter) == "�s���Y" ) {
					c_pinfo.mlDataType = TYPE_REALESTATE;
				}else if( (*iIter) == "�h�ΑΏە�" ) {
					c_pinfo.mlDataType = TYPE_FIREPROOF;
				}else {
					return false;
				}
				break;	// �^�C�v
			case	1:	c_pinfo.mlContentsCode	= atol((*iIter).c_str()); break; // �R���e���c�R�[�h
			case	2:	c_pinfo.mlPriority      = atol((*iIter).c_str());	break;	// �o��FC��
			case	3:	c_pinfo.msName          = (*iIter);	break; // ��ʖ�
			}
		}
		//-----------------------------------------------
		return true;
	}

	/**
	* @brief �D��x���X�g�ǂݍ���
	* @return ����=true, ���s=false
	*/
	bool Init( std::string csPriorityList ) //!< [in] �D��x���X�g
	{
		const int bmax	= 2048;
		FILE*     a_pFp;		
		char      a_cBuf[bmax];
		int       a_StrLen = 0;	

		a_pFp = fopen ( csPriorityList.c_str(), "r" );
		if(a_pFp == NULL) {
			return false;
		}

		msPriorityInfo.clear(); // �O�̂��߃N���A

		while( fgets ( a_cBuf, bmax, a_pFp ) != NULL)
		{
			a_StrLen	= strlen ( a_cBuf );
			// �Ō��"\n"����菜��
			if(a_cBuf[a_StrLen - 1] == '\n') {
				a_cBuf[a_StrLen - 1] = '\0';
			}
			// �R�����g�����΂�
			if(a_cBuf[0] == '#' || a_cBuf[0] == '\0') {
				continue;
			}
			PriorityInfo pCurInfo;
			if( !analyze_line( a_cBuf, pCurInfo ) ){
				return false;
			}
			// ���X�g�ɒǉ�
			msPriorityInfo.insert ( pCurInfo );
		}
		fclose ( a_pFp );
		return true;
	}

	/**
	* @brief 
	* @note �s���Y���A�h�ΑΏە����ɂ��ẮA�u�R���e���c�R�[�h�v��'-1'���w�肷����̂Ƃ���
	* @return �D��x(�Ώۂ�������Ȃ��ꍇ��-1��ԋp����)
	*/
	long GetPriority( long& clDataType,      //!< [in] �f�[�^�^�C�v
			          long& clContentsCode,  //!< [in] �R���e���c�R�[�h
					  std::string& csName )  //!< [out] ��ʖ���
	{
		PriorityInfo pKey;
		pKey.mlDataType     = clDataType;
		pKey.mlContentsCode = clContentsCode;

		std::set<PriorityInfo>::iterator iIter;
		iIter = msPriorityInfo.find ( pKey );
		if( iIter != msPriorityInfo.end() ) {
			csName = (*iIter).msName; // ����
			return (*iIter).mlPriority;
		}else {
			csName = ""; // ����
			return -1;
		}
	}
};

typedef std::multiset<PriorityManager> msPriorityManager;

/**
* @brief �|�C���g�̏��p�N���X
*/
struct PointInfo {
	long         mlOID;      //!< �|�C���g�̃I�u�W�F�N�gID
	long         mlDataType; //!< �f�[�^�^�C�v(PriorityManager�Œ�`�̂���)
	long         mlContentsCode; //!< �R���e���c�R�[�h(POI�ȊO��-1)
	long         mlPriority; //!< �Ή��\���瓾��ꂽ�D��x
	CString      mcsName;    //!< ����
	IGeometryPtr mipGeom;    //!< �ʒu���
	/**
	* @brief �R���X�g���N�^
	*/
	PointInfo(){
		mlPriority = -1;
		mlOID = 0;
		mlDataType = 0;
		mlContentsCode = -1;
	}
	/**
	* @brief �f�X�g���N�^
	*/
	~PointInfo(){}
	/**
	* @brief ��r���Z�q
	*/
	bool operator<(const PointInfo& cTgt) const {
		if( mlPriority != cTgt.mlPriority ) {
			if( mlPriority < cTgt.mlPriority )
				return true;
		}else if( mcsName != cTgt.mcsName ) { 
			if( mcsName < cTgt.mcsName )
				return true;
		}
		return false;
	}
};

typedef std::multiset<PointInfo> mPointInfo;

/**
* @brief �|�C���g�̏��p�N���X(XY�D��\�[�g�j
*/
struct PointInfo_XY : PointInfo {
	double	mX; // X���W�l
	double	mY; // Y���W�l
	/**
	* @brief �R���X�g���N�^
	*/
	PointInfo_XY(){
		mX = 0;
		mY = 0;
	}
	/**
	* @brief �f�X�g���N�^
	*/
	~PointInfo_XY(){}
	/**
	* @brief ��r���Z�q
	*/
	bool operator<(const PointInfo_XY& cTgt) const {
		if( mX != cTgt.mX ) {
			if( mX < cTgt.mX )
				return true;
		}else if( mY != cTgt.mY ) { 
			if( mY < cTgt.mY )
				return true;
		}
		return false;
	}
	/**
	* @brief ������Z�q
	*/
/*	void operator=(const PointInfo& cTgt) {
		mlOID          = cTgt.mlOID; // �|�C���g�̃I�u�W�F�N�gID
		mlDataType     = cTgt.mlDataType; // �f�[�^�^�C�v
		mlContentsCode = cTgt.mlContentsCode; // �R���e���c�R�[�h(POI�ȊO��-1)
		mlPriority     = cTgt.mlPriority; // �Ή��\���瓾��ꂽ�D��x
		mcsName        = cTgt.mcsName;    // ����
		mipGeom        = cTgt.mipGeom;    // �ʒu���
	}*/
};

typedef std::multiset<PointInfo_XY> mPointInfo_XY;

/**
 * @brief ���C�������N���X
 */
class	main_routine
{
	/**
	 * @brief �֌W��`�p
	 */
	enum {
		contains = 1,
		overlaps,
		within,
		touches,
		equals,
		crosses
	};

	public:
		//------------------------------------------------------------------------
		std::string   m_sDB_Building;   //!< �����̐ڑ�DB
		std::string   m_sFC_Building;   //!< �����̃t�B�[�`���N���X��
		std::string   m_sMeshList;      //!< ���b�V�����X�g
		std::string   m_sPriorityList;  //!< POI�E���L�f�[�^�D��x�ݒ胊�X�g
		std::string   m_sShapeFName;    //!< �o��Shape�t�@�C����
		std::string	  m_sLog_File;      //!< �G���[���O�f�[�^�o�̓p�X
		std::string   m_sExeLog_File;   //!< ���s���O�t�@�C��

		std::string   m_sDB_Mesh;       //!< ���b�V���̐ڑ�DB
		std::string   m_sFC_Mesh;       //!< ���b�V���̃t�B�[�`���N���X��
		std::string   m_sDB_POI;        //!< POI�E���L�̐ڑ�DB
		std::string   m_sFC_POI;        //!< POI�E���L�̃t�B�[�`���N���X��
		std::string   m_sDB_RealEstate; //!< �s���Y���̐ڑ�DB
		std::string   m_sFC_RealEstate; //!< �s���Y���̃t�B�[�`���N���X��
		std::string   m_sDB_FireProof;  //!< �h�ΑΏە����̐ڑ�DB
		std::string   m_sFC_FireProof;  //!< �h�ΑΏە����̃t�B�[�`���N���X��

		std::string   m_sDB_Connect1;   //!< DB�ڑ����P
		std::string   m_sFeatureClass1; //!< �t�B�[�`���N���X�P����
		std::string   m_sDB_Connect2;   //!< DB�ڑ����Q
		std::string   m_sFeatureClass2; //!< �t�B�[�`���N���X�Q����
		std::string   m_sObjectID_File; //!< �����I�u�W�F�N�gID���X�g
		std::string	  m_sRel_Str;       //!< �֌W�w�蕶����
		long          m_lRel_Type;      //!< �w�肵���I�u�W�F�N�g���m�̊֌W�̌`��

		std::set<long>    m_sMESH_List; //!< ���b�V���R�[�h���X�g

		IFeatureClassPtr  m_ipFC_Building; //!< �t�B�[�`���N���X(����)
		IFeatureClassPtr  m_ipFC_Mesh;     //!< �t�B�[�`���N���X(���b�V��)
		IFeatureClassPtr  m_ipFC_POI;      //!< �t�B�[�`���N���X(POI)
		IFeatureClassPtr  m_ipFC_RealEstate;//!< �t�B�[�`���N���X(�s���Y���)
		IFeatureClassPtr  m_ipFC_FireProof;//!< �t�B�[�`���N���X(�h�ΑΏە����)

		IWorkspacePtr     m_ipWSpace_Building; //!< ���[�N�X�y�[�X(����)
		IWorkspacePtr     m_ipWSpace_Mesh;     //!< ���[�N�X�y�[�X(���b�V��)
		IWorkspacePtr     m_ipWSpace_POI;      //!< ���[�N�X�y�[�X(POI)
		IWorkspacePtr     m_ipWSpace_RealEstate;//!< ���[�N�X�y�[�X(�s���Y���)
		IWorkspacePtr     m_ipWSpace_FireProof;//!< ���[�N�X�y�[�X(�h�ΑΏە����)

		IWorkspaceEditPtr m_ipWorkspaceEdit_1; //!< �G�f�B�b�g�p
		IWorkspaceEditPtr m_ipWorkspaceEdit_2; //!< �G�f�B�b�g�p

		ISpatialReferencePtr m_ipMeshSpRef;    //!< ��ԎQ��

		MakeShapeFile     m_mShpFile; //!< �V�F�C�v�t�@�C���쐬�p�N���X

		PriorityManager   m_pPriorityMgr; //!< �D��x�Ǘ��N���X

		//------------------------------------------------------------------------
		long					m_lExe_Flag;
		FILE*					m_fErr_Fp;	//!< �G���[���O�t�@�C���p�t�@�C���|�C���^
		FILE*                   m_fExe_Fp;  //!< ���s���O�t�@�C���p
	private:
		exec_time_handle		m_eExeTime;	//!< ���s���ԊǗ�

		crd_cnv					m_cCrd;
	private:
		CString       m_sVersion;  //!< �c�[���o�[�W����

	public:
		//!< �R���X�g���N�^
		main_routine () {
			m_lExe_Flag		= 0;
			m_sRel_Str      = "contains";
			m_lRel_Type     = contains;
			m_fErr_Fp		= stderr; // �f�t�H���g�͕W���G���[�o�͂�
			m_fExe_Fp       = stdout;
		}
		//!< �f�X�g���N�^
		~main_routine () {
			if( m_fErr_Fp != NULL && m_fErr_Fp != stderr && m_fErr_Fp != stdout ) {
				fclose ( m_fErr_Fp );
			}
			if( m_fExe_Fp != NULL && m_fExe_Fp != stderr && m_fExe_Fp != stderr ) {
				fclose ( m_fExe_Fp );
			}
		}

		/**
		 * @brief DB�ւ̐ڑ��I�[�v��
		 * @return  true   ����
		 * @return  false  ���s
		 */
		bool	DB_Open	( void );

		/**
		 * @brief �G���[���O�t�@�C�����I�[�v������
		 * @return true ����
		 * @return  false  ���s
		 */
		bool    Open_Log (
		            std::string& csFName, //!< [in] �t�@�C����
		            FILE**       cFp      //!< [out] �t�@�C���|�C���^
		        );

		/**
		 * @brief �c�[���̎g�p�@�̕\��
		 */
		void    print_usage (
		            char** argv //!< [in] ������
		        );

		/**
		 * @brief ���s���\��
		 */
		void    PrintExecInfo( void );

		/**
		 * @brief �����`�F�b�N
		 * @return	����	true
		 * @return	����	false
		 */
		bool    arg_check (
		            int    argc, //!< [in] ������
		            char** argv  //!< [in] ������
		        );

		/**
		 * @brief ���X�g�̓ǂݍ���
		 * @return	����	true
		 * @return	����	false
		 */
		bool    Read_List (
		            const std::string& c_sFileName, //!< [in] �t�@�C����
		            std::set<long>&    c_sList      //!< [in] ���X�g�iID���Ɏg�p�j
		        );

		/**
		 * @brief ���b�V����`��GeoDB����擾����
		 * @return IGeometryPtr(���s����NULL)
		 */
		IGeometryPtr    GetMeshRect (
		                    int   c_iMeshCode //!< ���b�V���R�[�h
		                );

		/**
		 * @brief �w��t�B�[�`���ɑ΂���esriSpatialRelEnum�Ŏw�肵���֌W�����f�[�^���Ƃ��Ă���
		 * @return IFeatureCursorPtr
		 */
		IFeatureCursorPtr   Search_Feature (
		                        const IFeatureClassPtr& c_ipFeCls,     //!< [in] �t�B�[�`���N���X
		                        const IGeometryPtr&     c_ipGeo,       //!< [in] �����p�W�I���g��
		                        esriSpatialRelEnum      c_ipSpatialRel,//!< [in] �֌W�w��
		                        std::string             c_sWhere,      //!< [in] Where��w��
		                        long&                   c_iCount       //!< [out] ������
							);

		/**
		 * @brief ��\�_���擾
		 * @return 
		 * @return	����	true
		 * @return	����	false
		 */
		bool   GetPoint (
		           const IGeometryPtr& c_ipGeom, //!< [in] �W�I���g��
		           double&             c_dX,     //!< [out] �o�x
		           double&             c_dY      //!< [out] �o�x
		       );

		/**
		 * @brief ���s��
		 * @note �������ŃG���[�t�@�C���̏��������s��
		 * @return	����	true
		 * @return	����	false
		 */
		bool exec ( void );
		
		/**
		* @brief �f�[�^�̃R���e�i���쐬����
		* @note  
		* @return	�f�[�^���i���s�̏ꍇ��-1�j
		*/
		int    MakeDataContainer (
		           long                     cMeshCode,     //!< [in] �����b�V���R�[�h
		           const IFeatureCursorPtr& cipFCursor,    //!< [in] �t�B�[�`���J�[�\��
		           mIFeatureContainer&      cmWork_Data,   //!< [in] ��Ɨp�f�[�^
				   bool                     bLeftDownPtMode//!< [in] ��ԍ����̃|�C���g��
		       );

		/**
		* @brief �|�C���g���̃R���e�i���쐬����
		* @note  �f�[�^�^�C�v��-1�̏ꍇ�́A�������̃|�C���g���́u�v�t�B�[���h�̃^�C�v�Ŕ��ʂ���
		* @return	�f�[�^���i���s�̏ꍇ��-1�j
		*/
		int    MakePointContainer (
		           const IFeatureCursorPtr& cipFCursor,    //!< [in] �t�B�[�`���J�[�\��
				   CString&                 cNameFidldName,//!< [in] ���̃t�B�[���h���w��
		           mPointInfo_XY&           cmWork_Data,   //!< [out] ��Ɨp�f�[�^
		           long                     clDataType=-1  //!< [in] �f�[�^�^�C�v(PriorityManager�Œ�`�̂���)
		       );

		/**
		* @brief �����`�󂩂�O�ڋ�`���쐬����
		* @note  
		* @return	IGeometryPtr
		*/
		IGeometryPtr    MakeBuildingsEnvelope (
		                    const mIFeatureContainer& cmWork_Data //!< [in] ��Ɨp�f�[�^
		                );

		/**
		* @brief XY�\�[�g�����|�C���g�񂩂�A�|���S���Ɋ܂܂��|�C���g�́A�D��x�\�[�g�̃R���e�i���쐬
		* @return	����=true,���s=false
		*/
		bool   GetInnerPoint (
		           IGeometryPtr&   cipPolygon,   //!< [in] �|���S���W�I���g��
		           mPointInfo_XY&  cmPoint_Data, //!< [in] �|�C���g���R���e�i(XY�\�[�g)
		           mPointInfo&     cmOutput      //!< [out] �|�C���g���R���e�i(�D��x�\�[�g)
			   );

		/**
		* @brief �f�[�^�o��
		* @note  
		* @return	����=true,���s=false
		*/
		bool   PrintOut (
		           mIFeatureContainer&      cmBuilding_Data, //!< [in] �����f�[�^�R���e�i
		           mPointInfo_XY&           cmPoint_Data     //!< [out] �|�C���g���R���e�i
		       );

};
//------------------------------------------------------<�����܂�>

#endif	//_MAIN_ROOTIN_H_
