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

#define WALK_ID 100000000

/**
 * @brief OBJECTID��DIVID�Ń��j�[�N�ɂ����邽�߂̍\����
 */
struct UNIQID{
	UNIQID(long lOID, long lDivID){
		this->eID = lOID;
		this->eDivID = lDivID;
	}
	UNIQID(){
		this->eID = 0;
		this->eDivID = 0;
	}
	long eID;
	long eDivID;
	bool operator<(const UNIQID uniqID) const {
		return (this->eID != uniqID.eID) ? (this->eID < uniqID.eID) : (this->eDivID < uniqID.eDivID);
	}
	bool operator==(const UNIQID uniqID) const {
		return (this->eID == uniqID.eID && this->eDivID == uniqID.eDivID);
	}
	bool operator!=(const UNIQID uniqID) const {
		return (this->eID != uniqID.eID || this->eDivID != uniqID.eDivID);
	}
};

/**
 * @brief �o�ܓx�{�m�[�hID�m�ۗp�̍\����
 */
struct LONLAT{
	double eLon;
	double eLat;
	UNIQID eNodeID;

	bool operator<(const LONLAT LonLat) const {
		return (this->eLon != LonLat.eLon) ? (this->eLon < LonLat.eLon) : (this->eLat < LonLat.eLat);
	}
	bool operator==(const LONLAT LonLat) const {
		return (this->eLon == LonLat.eLon && this->eLat == LonLat.eLat);
	}
	bool operator!=(const LONLAT LonLat) const {
		return (this->eLon != LonLat.eLon || this->eLat != LonLat.eLat);
	}
};

// �`�F�b�N���[�h
namespace check_mode {
	enum ECode {
		kNone        =  0, //!< ���ݒ�
		kSegmentAttr =  1, //!< �����N����������
		kWalkNode    =  2, //!< ���s�҃m�[�h����
		kHeightNode  =  3, //!< �����m�[�h����

		kIntegrity   = 99, //!< RWL,RWN��NW�����`�F�b�N���[�h�i����J�j
	};
}

/*
 * @brief �`��̈ܓx�o�x���擾�֐�
 * @param strTableName [in] �擾�Ώی`��̃e�[�u����
 * @param ipGeom       [in] �擾�Ώۂ̌`��
 * @param vecLonLat    [out] �擾�����ܓx�o�x���
 * @return true ����
 * @return false ���s
 */
bool GetLonLatData(const CString& strTableName, IGeometryPtr ipGeom, std::vector<LONLAT>& vecLonLat);

/*
 * @brief ���[�N�X�y�[�X�擾�֐�
 * @param strConnectInfo [in] ���[�N�X�y�[�X�ւ̃p�X
 * @param ipWorkspace    [out] �擾�������[�N�X�y�[�X
 * @return true ����
 * @return false ���s
 */
bool GetWorkspace(const CString& strConnectInfo, IWorkspacePtr& ipWrokspace); 

/*
 * @breif �t�B�[�`���N���X�擾�֐�
 * @param strTableName   [in] ���C����
 * @param ipWorkspace    [in] ���[�N�X�y�[�X
 * @param ipFeatureClass [out] �擾�����t�B�[�`���N���X
 * @return true ����
 * @return false ���s
 */
bool GetFeatureClass(const CString& strTableName, IWorkspacePtr ipWorkspace, IFeatureClassPtr& ipFeatureClass);

/*
 * @breif �e�[�u���擾�֐�
 * @param strTableName   [in] ���C����
 * @param ipWorkspace    [in] ���[�N�X�y�[�X
 * @param ipTableClass   [out] �擾�����e�[�u��
 * @return true ����
 * @return false ���s
 */
bool GetTable(const CString& strTableName, IWorkspacePtr ipWorkspace, ITablePtr& ipTable);

/*
 * @brief �t�B�[���h���擾�֐�
 * @param strTableName   [in] ���C����(�G���[�o�͗p)
 * @param ipFields       [in] �t�B�[���h�Q
 * @param lCount         [out] �擾�����t�B�[���h��
 * @return true ����
 * @return false ���s
 */
bool GetFieldCount(const CString& strTableName, IFieldsPtr ipFields, long& lCount);

/*
 * @brief �t�B�[���h�Q�擾�֐�
 * @param strTableName   [in] ���C����(�G���[�o�͗p)
 * @param ipFeatureClsas [in] �t�B�[�`���N���X
 * @param ipFields       [out] �擾�����t�B�[���h�Q
 * @return true ����
 * @return false ���s
 */
bool GetFields(const CString& strTableName, IFeatureClassPtr ipFeatureClass, IFieldsPtr& ipFields);

/*
 * @brief �t�B�[���h�Q�擾�֐�
 * @param strTableName   [in] ���C����(�G���[�o�͗p)
 * @param ipTable        [in] �t�B�[�`���N���X
 * @param ipFields       [out] �擾�����t�B�[���h�Q
 * @return true ����
 * @return false ���s
 */
bool GetFields(const CString& strTableName, ITablePtr ipTable, IFieldsPtr& ipFields);

/*
 * @brief �t�B�[���h�擾�֐�
 * @param strTableName   [in] ���C����(�G���[�o�͗p)
 * @param ipFields       [in] �t�B�[���h�Q
 * @param lIndex         [in] �t�B�[���h�C���f�b�N�X
 * @param ipField        [out] �擾�����t�B�[���h
 * @return true ����
 * @return false ���s
 */
bool GetField(const CString& strTableName, IFieldsPtr ipFields, long lIndex, IFieldPtr& ipField);

/*
 * @brief ��ԃt�B���^�擾�֐�
 * @param ipSpGeom         [in] ��Ԍ����`��
 * @param ipSpFilter       [out] �擾������ԃt�B���^
 * @return true ����
 * @return false ���s
 */
bool GetSpatialFilter(IGeometryPtr ipSpGeom, ISpatialFilterPtr& ipSpFilter);

/*
 * @brief �t�B�[�`���J�[�\���擾�֐�
 * @param strTableName     [in] ���C����(�G���[�o�͗p)
 * @param ipFeatureClass   [in] �t�B�[�`���N���X
 * @param ipSpFilter       [in] ��ԃt�B���^
 * @param ipFeatureCursor  [out] �擾�����t�B�[�`���J�[�\��
 * @return true ����
 * @return false ���s
 */
bool GetFeatureCursor(const CString& strTableName, IFeatureClassPtr ipFeatureClass, ISpatialFilterPtr ipSpFilter, IFeatureCursorPtr& ipFeatureCursor);

/*
 * @brief �J�[�\���擾�֐�
 * @param strTableName     [in] ���C����(�G���[�o�͗p)
 * @param ipTable          [in] �e�[�u��
 * @param ipQFilter        [in] �����t�B���^
 * @param ipCursor         [out] �擾�����J�[�\��
 * @return true ����
 * @return false ���s
 */
bool GetCursor(const CString& strTableName, ITablePtr ipTable, IQueryFilterPtr ipQFilter, _ICursorPtr& ipCursor);

/*
 * @brief �t�B�[�`�����擾�֐�
 * @param strTableName     [in] ���C����(�G���[�o�͗p)
 * @param ipFeatureClass   [in] �t�B�[�`���N���X
 * @param ipSpFilter       [in] ��ԃt�B���^
 * @param lCount           [out] �擾�����t�B�[�`����
 * @return true ����
 * @return false ���s
 */
bool GetFeatureCount(const CString& strTableName, IFeatureClassPtr ipFeatureClass, ISpatialFilterPtr ipSpFilter, long& lCount);

/*
 * @brief ���R�[�h���擾�֐�
 * @param strTableName     [in] ���C����(�G���[�o�͗p)
 * @param ipFeatureClass   [in] �t�B�[�`���N���X
 * @param ipQFilter        [in] �����t�B���^
 * @param lCount           [out] �擾�������R�[�h��
 * @return true ����
 * @return false ���s
 */
bool GetRowCount(const CString& strTableName, ITablePtr ipTable, IQueryFilterPtr ipQFilter, long& lCount);

/*
 * @brief �t�B�[���h�l�擾�֐�
 * @param strTableName [in] ���C����(�G���[�o�͗p)
 * @param strFieldName [in] �t�B�[���h��
 * @param ipFeature    [in] �t�B�[�`��
 * @param lIndex       [in] �t�B�[���h�C���f�b�N�X
 * @param vaValue      [out] �擾�����l
 * @return true ����
 * @return false ���s
 */
bool GetValue(const CString& strTableName, const CString& strFieldName, IFeaturePtr ipFeature, long lIndex, CComVariant& val);

/*
 * @brief �t�B�[���h�l�擾�֐�
 * @param strTableName [in] ���C����(�G���[�o�͗p)
 * @param strFieldName [in] �t�B�[���h��
 * @param ipRow        [in] ���R�[�h
 * @param lIndex       [in] �t�B�[���h�C���f�b�N�X
 * @param vaValue      [out] �擾�����l
 * @return true ����
 * @return false ���s
 */
bool GetValue(const CString& strTableName, const CString& strFieldName, _IRowPtr ipRow, long lIndex, CComVariant& val);

/*
 * @brief �`��擾�֐�
 * @param strTableName [in] ���C����(�G���[�o�͗p)
 * @param ipFeature    [in] �t�B�[�`��
 * @param ipGeometry   [out] �擾�����`��
 * @return true ����
 * @return false ���s
 */
bool GetGeometry(const CString& strTableName, const IFeaturePtr ipFeature, IGeometryPtr &ipGeometry);

/*
 * @brief �|�C���g���擾�֐�
 * @param strTableName [in] ���C����(�G���[�o�͗p)
 * @param ipPointCol   [in] �|�C���g�R���N�V����
 * @param lCount       [out] �|�C���g��
 * @return true ����
 * @return false ���s
 */
bool GetPointCount(const CString& strTableName, IPointCollectionPtr ipPointCol, long& lCount);

/*
 * @brief WKS�|�C���g�Q�擾�֐�
 * @param strTableName [in] ���C����(�G���[�o�͗p)
 * @param ipPointCol   [in] �|�C���g�R���N�V����
 * @param lSize        [in] �|�C���g��
 * @param vecWKSPoint  [out] �擾����WKS�|�C���g�Q
 * @return true ����
 * @return false ���s
 */
bool GetQueryWKSPoints(const CString& strTableName, IPointCollectionPtr ipPointCol, long lSize, std::vector<WKSPoint>& vecWKSPoint);

/*
 * @brief ���b�V���`��擾�֐�
 * @param strMeshCode   [in] ���b�V���R�[�h
 * @param ipMeshClass   [in] ���b�V���N���X
 * @param ipMeshGeom    [out] �擾�������b�V���`��
 * @return true ����
 * @return false ���s
 */
bool GetMeshGeom(const CString& strMeshCode, IFeatureClassPtr ipMeshClass, IGeometryPtr& ipMeshGeom);
