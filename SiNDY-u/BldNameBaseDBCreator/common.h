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


// �c�[����
const LPCTSTR TOOLNAME = _T("BldNameBaseDBCreator");

// ����
const LPCSTR INI		= "ini";
const LPCSTR PROCLOG	= "proc_log";
const LPCSTR ERRLOG		= "err_log";
const LPCSTR UPDATE		= "u";
const LPCSTR SURVEY		= "s";

// �ݒ�t�@�C������
const LPCSTR SRC_DB				= "SRC_DB";
const LPCSTR MAP_DB				= "MAP_DB";
const LPCSTR ADDR_DB			= "ADDR_DB";
const LPCSTR TWN_DB				= "TWN_DB";
const LPCSTR POI_DB				= "POI_DB";
const LPCSTR BASE_DB			= "BASE_DB";
const LPCSTR MESH_DB			= "MESH_DB";
const LPCSTR AM_DB				= "AM_DB";
const LPCSTR SURVEY_LIST		= "SURVEY_LIST";
const LPCSTR TGT_ADDR_LIST		= "TGT_ADDR_LIST";
const LPCSTR SRC_PRI_LIST_PRIOR	= "SRC_PRI_LIST_PRIOR";
const LPCSTR SRC_PRI_LIST_MATCH	= "SRC_PRI_LIST_MATCH";
const LPCSTR POI_PRI_LIST		= "POI_PRI_LIST";
const LPCSTR SUP_POI_PRI_LIST	= "SUP_POI_PRI_LIST";
const LPCSTR BLD_EXC_LIST		= "BLD_EXC_LIST";
const LPCSTR REPLACE_LIST		= "REPLACE_LIST";
const LPCSTR COND_LIST			= "COND_LIST";
const LPCSTR TWN_TBL_NAME		= "TWN_TBL_NAME";
const LPCSTR SOURCENAME			= "SOURCENAME";
const LPCSTR SOURCEDATE			= "SOURCEDATE";

// �C���|�[�g�����P��
const long FETCHNUM = 1000;

// �����R�R�[�h�u�x�~���v
const LPCSTR CLOSE_SUSPENDED	= "b";

/**
 * @brief �������̃|�C���g���R�[�h�o�^���
 */
struct BldNamePtInfo
{
	CString e_name;			//!< ����
	long e_bldOID;			//!< �����|���S��ID
	long e_srcType;			//!< �o�T�R�[�h
	CString e_srcName;		//!< �o�T���i�t�����Ƃ̕R�t���Ɏg�p�j
	CString e_srcID;		//!< �o�T����ID�i�t�����Ƃ̕R�t���Ɏg�p�j
	long e_srcOID;			//!< �Q�Ƒf��OID
	CString e_surveyDate;	//!< �������iYYYY/MM/DD HH:MI:SS �`���H�j �c �K���ȓ��t�^�ɂ����ق�����������
	CString e_anyPlaceID;	//!< AnyPlace ID
	long e_priority;		//!< �M���x�R�[�h
	double e_matchPer;		//!< �}�b�`���O���iDB�i�[���ɐ����ɕϊ��j
	CAdapt<IGeometryPtr> e_ipGeometry;	//!< �`��

	BldNamePtInfo() : e_bldOID(-1), e_srcType(-1), e_srcOID(-1), e_priority(-1), e_matchPer(-1) {}

	/**
	 * @brief ���Z�b�g
	 */
	void Reset() {
		e_name.Empty();
		e_bldOID		= -1;
		e_srcType		= -1;
		e_srcOID		= -1;
		e_surveyDate.Empty();
		e_anyPlaceID.Empty();
		e_priority		= -1;
		e_matchPer		= -1;
		e_ipGeometry	= nullptr;
	}

	/**
	 * @brief  �o�T����ID����R���e���c�R�[�h���擾
	 * @note   (��)�u2335-1137-543�v �� �u233501137�v
	 * @return �R���e���c�R�[�h�i���LPOI�R���łȂ��ꍇ��擾�ł��Ȃ��ꍇ�́u-1�v�j
	 */
	long GetContentsCode() const;

	/**
	 * @brief  �o�T����ID����R���e���c���V�[�P���X���擾
	 * @note   (��)�u2335-1137-543�v �� �u543�v
	 * @return �R���e���c���V�[�P���X�i���LPOI�R���łȂ��ꍇ��擾�ł��Ȃ��ꍇ�́u-1�v�j
	 */
	long GetContentsSeq() const;

private:
	/**
	 * @brief  �o�T����ID���u-�v�Ńg�[�N���ɕ���
	 * @note   (��)�u2335-1137-543�v �� �u2355�v�u1137�v�u543�v
	 * @param  token		[out] �g�[�N��
	 * @retval true ����
	 * @retval false ���s
	 */
	bool TokenizeSrcID( std::vector< long > &tokens ) const;
};

/**
 * @brief �������̕t�����
 */
struct BldNameAddInfo
{
	CString	e_name;			//!< ����
	CString	e_nameSep;		//!< ���́i�����j
	CString	e_yomiSep;		//!< ���~�i�����j
	long	e_bldgClass1;	//!< ������ʃR�[�h1
	long	e_bldgClass2;	//!< ������ʃR�[�h2
	long	e_bldgClass3;	//!< ������ʃR�[�h3
	long	e_floors;		//!< �K��
	long	e_suitable;		//!< �r�����̑Ó��t���O
	long	e_fixclass;		//!< ��ʊm��R�[�h

	BldNameAddInfo() : e_bldgClass1(-1), e_bldgClass2(-1), e_bldgClass3(-1), e_floors(-1), e_suitable(0), e_fixclass(0) {}
	~BldNameAddInfo() {}
};

/**
 * @brief �u�������񃊃X�g���R�[�h
 */
struct StrPair
{
	CString e_orgStr;
	CString e_replaceStr;

	StrPair(CString orgStr, CString replaceStr) : e_orgStr(orgStr), e_replaceStr(replaceStr) {}
	StrPair() {}
};

/**
 * @brief ID���L�[�Ƃ����t�B�[�`���̃}�b�v
 */
typedef std::map<long, CAdapt<IFeaturePtr>> IDFeatureMap;

/**
 * @brief ���ʊ֐�
 */
namespace common_proc {

	/**
	 * @brief  �t�B�[���h�l�擾�֐�
	 * @param  ipRow		[in]  IRowPtr
	 * @param  layerName	[in]  �e�[�u����(�G���[�o�͗p)
	 * @param  fieldName	[in]  �t�B�[���h��
	 * @param  indeces		[in]  �t�B�[���h�C���f�b�N�X���
	 * @param  val			[out] �擾�����l
	 * @retval true ����
	 * @retval false ���s
	 */
	bool GetValue(_IRowPtr ipRow, LPCTSTR talbleName, LPCTSTR fieldName, const std::map<CString, long>& indeces, CComVariant* val);

	/**
	 * @brief  �t�B�[���h�l�Z�b�g�֐�
	 * @param  ipBuffer		[in]  IBufferPtr
	 * @param  tableName	[in]  �e�[�u����(�G���[�o�͗p)
	 * @param  fieldName	[in]  �t�B�[���h��
	 * @param  indeces		[in]  �t�B�[���h�C���f�b�N�X���
	 * @param  val			[in]  �Z�b�g����l
	 * @retval true ����
	 * @retval false ���s
	 */
	bool SetValue(IRowBufferPtr ipBuffer, LPCTSTR tableName, LPCTSTR fieldName, const std::map<CString, long>& indeces, const CComVariant& val);

	/**
	 * @brief  �D��t�B�[�`���擾�֐�
	 * @note�@ ���̕�������������t�B�[�`���̂����D��x�̍����t�B�[�`�����擾����֐�
	 * @param  tgtFeatures		[in]  �t�B�[�`���Q
	 * @param  tableName		[in]  �e�[�u����
	 * @param  fieldName		[in]  �t�B�[���h��
	 * @param  indeces			[in]  �t�B�[���h�C���f�b�N�X���
	 * @param  headID			[out] �I�΂ꂽ�t�B�[�`����OBJECTID
	 * @param  ipHeadFeature	[out] �I�΂ꂽ�t�B�[�`��
	 * @retval true ����
	 * @retval false ���s
	 */
	bool SelectHeadNameFeature(const IDFeatureMap& tgtFeatures, LPCTSTR tableName, LPCTSTR fieldName, const std::map<CString, long>& indeces, long* headID, IFeaturePtr& ipHeadFeature);

	/**
	 * @brief  Where��ɂ�錟���֐��i�t�B�[�`���N���X�j
	 * @param  ipFeatureClass	[in]  �����Ώۃt�B�[�`���N���X
	 * @param  ipFeatureCursor	[out] �t�B�[�`���J�[�\���i�������ʁj
	 * @param  whereClause		[in]  Where��
	 * @param  bRecycling		[in]  ���T�C�N�����O���邩�ǂ���
	 * @retval true ����
	 * @retval false ���s
	 */
	bool SearchByWhereClause(IFeatureClassPtr ipFeatureClass, IFeatureCursorPtr& ipFeatureCursor, const CString& whereClause = _T(""), bool bRecycling = false);

	/**
	 * @brief  Where��ɂ�錟���֐��i�e�[�u���j
	 * @param  ipTable			[in]  �����Ώۃe�[�u��
	 * @param  ipCursor			[out] �t�B�[�`���J�[�\���i�������ʁj
	 * @param  whereClause		[in]  Where��
	 * @param  bRecycling		[in]  ���T�C�N�����O���邩�ǂ���
	 * @retval true ����
	 * @retval false ���s
	 */
	bool SearchByWhereClause(ITablePtr ipTable, _ICursorPtr& ipCursor, const CString& whereClause = _T(""), bool bRecycling = false);

	/**
	 * @brief  Geometry�ɂ���Ԍ����֐�
	 * @param  ipFeatureClass	[in]  �����Ώۃt�B�[�`���N���X
	 * @param  ipFeatureCursor	[out] �t�B�[�`���J�[�\���i�������ʁj
	 * @param  ipGeometry		[in]  ������Geometry
	 * @param  spRel			[in]  �����Ώۂ̌�����Geometry�Ƃ̊֌W
	 * @param  bRecycling		[in]  ���T�C�N�����O���邩�ǂ���
	 * @retval true ����
	 * @retval false ���s
	 */
	bool SearchByGeometry(IFeatureClassPtr ipFeatureClass, IFeatureCursorPtr& ipFeatureCursor, const IGeometryPtr& ipGeometry, esriSpatialRelEnum spRel, bool bRecycling = false);

	/**
	 * @brief  �R�[�h�l�h���C���̎擾�i�����l�j
	 * @param  ipTable		[in]  �Ώۃe�[�u��
	 * @param  layerName	[in]  �e�[�u����(�G���[�o�͗p)
	 * @param  fieldName	[in]  �t�B�[���h��
	 * @param  indeces		[in]  �t�B�[���h�C���f�b�N�X���
	 * @param  codedValues  [out] �R�[�h�l�̃��X�g
	 * @retval true ����
	 * @retval false ���s
	 */
	bool GetCodedValues(const ITablePtr& ipTable, LPCTSTR talbleName, LPCTSTR fieldName, const std::map<CString, long>& indeces, std::set<long>& codedValues);
};
