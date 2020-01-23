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

#include <sindy/workspace.h>
typedef std::set<int> MESHLIST;
typedef CString ORG_COLNAME;
typedef CString DEST_COLNAME;
typedef std::pair<ORG_COLNAME, DEST_COLNAME> PAIR_COL;
typedef std::pair<DEST_COLNAME, CAdapt<IFieldPtr>> PAIR_FIELD;
typedef std::map<long, long> TRANS_VAL;				// key�l��Val�l�ɒu��������
typedef std::map<CComVariant,CComVariant> TRANS_VALUE;				// key�l��Val�l�ɒu��������

const static CString FIELD_MARK_NEW          = _T("@");
const static CString FIELD_MARK_ROUND        = _T(":");
const static CString FIELD_MARK_DESIGNATION  = _T("=");
const static CString FIELD_MARK_FIELDINFO    = _T("/");
const static CString FIELD_MARK_DEFAULTVALUE = _T("!");
const static CString FIELD_MARK_ATTR         = _T("[");

namespace field_attr_mark {
	enum ECode {
		kNull         = 0,
		kNew          = 1,
		kRound        = 2,
		kDesignation  = 3,
		kFiledInfo    = 4,
		kDefaultValue = 5,
		kAttr         = 6,
	};
}

//const static CString FIELDTYPE_SHORT  = _T("SHORT");
const static CString FIELDTYPE_LONG   = _T("LONG");
//const static CString FIELDTYPE_FLOAT  = _T("FLOAT");
const static CString FIELDTYPE_DOUBLE = _T("DOUBLE");
const static CString FIELDTYPE_STRING   = _T("STRING");
//const static CString FIELDTYPE_DATE   = _T("DATE");

namespace field_type {
	enum ECode {
		kNull      = 0,
		kShort     = 2,
		kLong      = 3,
		kFloat     = 4,
		kDouble    = 5,
		kText      = 6,
		kDate      = 7,
	};
}

const static CString FIELDINFO_TYPE      = _T("TYPE");
const static CString FIELDINFO_NAME      = _T("NAME");
const static CString FIELDINFO_SCALE     = _T("SCALE");
const static CString FIELDINFO_PRECISION = _T("PRECISION");
const static CString FIELDINFO_LENGTH    = _T("LENGTH");
const static CString FIELDINFO_NULLABLE  = _T("NULLABLE");
const static CString FIELDINFO_EDITABLE  = _T("EDITABLE");

const static CComVariant SHAPE_NUMERIC_NULL = CComVariant(-1);
const static CComVariant SHAPE_STRING_NULL = CComVariant(_T(" "));


// �V�F�[�v�t�@�C���̑����ƌ`��
struct SHAPE_INFO
{
	CString m_strSortKey;		// m_mapValue���ɂ���L�[�Ń\�[�g����
	std::map<DEST_COLNAME, CString> m_mapValue;
	CAdapt<IGeometryPtr> m_ipGeom;

	// �L�[�̒l�ŏ��������ɕ��ׂ�
	bool operator < (const SHAPE_INFO& cInfo) const
	{
		std::map<DEST_COLNAME, CString>::const_iterator it1 = m_mapValue.find( m_strSortKey );
		std::map<DEST_COLNAME, CString>::const_iterator it2 = cInfo.m_mapValue.find( m_strSortKey );
		
		long lComp1 = _ttol( it1->second );
		long lComp2 = _ttol( it2->second );
		// �\�[�g�L�[�́A���l�ł��邱�Ƃ�O��Ƃ���
		if( lComp1 < lComp2 )
			return true;

		return false;
	}
};

// �ϊ����
struct CONVERT_INFO
{
	bool m_bBatch;				// �ꊇ�o�͂��ǂ����itrue:�ꊇ, false:���b�V�������j
	long m_lTotalSizeDBF;		// Total dbf�t�@�C���T�C�Y�v�Z�p
	long m_lTotalSizeShp;		// Total shp�t�@�C���T�C�Y�v�Z�p
	long m_lDivideNum;			// �t�@�C��������
	bool m_bIsNew;				// New�t�@�C��(��Create)���H

	CONVERT_INFO(){
		m_bBatch = true;
		m_lDivideNum = 0;
		m_lTotalSizeDBF = 0;
		m_lTotalSizeShp = 0;
		m_bIsNew = true;
	}
};

//
//bool LoadMeshlist(CString aStrMeshlistPath, MESHLIST& r_MeshlistSet);
bool CreateMeshList(IFeatureClassPtr ipMesh, MESHLIST& setMeshlist);

//FILE* GetFile(bool bIsUTF8, CString strFile);

//CString GetTokenize(CString str, CString strToken, long& lPos);

// Shape�쐬Util
//bool SetField(CDBFHandle& cDBFHdl, long& lIndex, LPCTSTR lpszFieldName, DBFFieldType eFieldType, int length, int decimals);
//template <class X> bool SetFieldValue(CDBFHandle& incDBFHdl, int fid, int index, const X &xVal, LPCTSTR lpszFieldName)
//{
//	if( !incDBFHdl.write_Attribute(fid, index, xVal) )
//	{
//		CString strErrMsg;
//		strErrMsg.Format(_T("ERROR\t9001\t�����t�^�Ɏ��s���܂����F%s"), lpszFieldName);
//		WriteLog( ETYPE_ERR, CT2CA( strErrMsg ).m_psz );
//		return false;
//	}
//	return true;
//};
//bool SetUTF8Value(CDBFHandle& cDBFHdl, int fid, int index, CString strVal, LPCTSTR lpszFieldName);

// MinMax�擾
//bool GetMinMax(const IGeometryPtr ipGeom, double& dx_min, double& dy_min, double& dx_max, double& dy_max);

/**
// �t�B�[�`���N���X����AID�ƌ`��̃}�b�v���擾����
// @param IFeatureClassPtr		ipFC		�Ώۃt�B�[�`���N���X(in)
// @param double				dBuffer		�`��̃o�b�t�@(in)
// @param std::map<long, CAdapt<IPolygonPtr>>&	mapPolygon		ID�ƌ`��̃}�b�v(out)
//
**/
//bool GetMapGeom(IFeatureClassPtr ipFC, double dBuffer, std::map<long, CAdapt<IPolygonPtr>>& mapPolygon);

// ��ԎQ�Ƃ��쐬����
//ISpatialReferencePtr CreateSpatialReference();

//bool IsTouchesEdge(const IPolylinePtr& ipBase, const IPolylinePtr& ipComp);

/**
 * @brief �t�B�[���h�C���f�b�N�X�̃}�b�v�擾�֐�
 * @note �t�B�[���h�C���f�b�N�X�ƃt�B�[���h���̂̃}�b�v���擾
 * @param ipTablePtr [in] �t�B�[���h�C���f�b�N�X�擾�Ώۃe�[�u��
 * @param mapFieldIndex [out] �t�B�[���h�C���f�b�N�X
 * @retval true  ��������
 * @retval false �������s
 */
bool GetFieldIndex(ITablePtr ipTable, std::map<long,CString>& mapFieldIndex);

/**
 * @brief �t�B�[���h�C���f�b�N�X�擾�֐�
 * @note �t�B�[���h���̂ƃt�B�[���h�C���f�b�N�X�}�b�v���擾
 * @param ipTablePtr [in] �t�B�[���h�C���f�b�N�X�擾�Ώۃe�[�u��
 * @param mapFieldIndex [out] �t�B�[���h�C���f�b�N�X
 * @retval true  ��������
 * @retval false �������s
 */
bool GetFieldNameIndex(ITablePtr ipTable, std::map<CString,long>& mapFieldNameIndex);


/**
 * @brief �t�B�[���h�^�C�v�擾�֐�
 * @param ipTablePtr [in] �t�B�[���h�^�C�v�擾�Ώۃe�[�u��
 * @param mapFieldIndex [out] �t�B�[���h�^�C�v
 * @retval true  ��������
 * @retval false �������s
 */
bool GetFieldTypes(ITablePtr ipTable, std::map<CString,esriFieldType>& mapFieldType);

/**
 * @brief ������}�b�`���O
 * @param strFilter [in] �}�b�`���O������
 * @param strCheck [in] �����Ώە�����
 * @return true �}�b�`���OOK
 * @return true �}�b�`���ONG
 */
bool IsMatch(const CString& strFilter, const CString& strCheck);

/**
 * @brief ���l���ǂ����i�����_��OK�j
 * @param str [in] �����Ώە�����
 * @return true ���l�ł���
 * @return true ���l�ł͂Ȃ�
 */
bool IsNumeric(const CString& str);

/**
 * @brief �����l���ǂ����i�����_��NG�j
 * @param str [in] �����Ώە�����
 * @return true ���l�ł���
 * @return true ���l�ł͂Ȃ�
 */
bool IsInteger(const CString& str);
