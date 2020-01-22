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
 * @brief �t�B�[���h�擾�֐�
 * @param strTableName   [in] ���C����(�G���[�o�͗p)
 * @param strFieldName   [in] �t�B�[���h��
 * @param ipFields       [in] �t�B�[���h�Q
 * @param lIndex         [out] �擾�����t�B�[���h�C���f�b�N�X
 * @return true ����
 * @return false ���s
 */
bool GetFieldIndex(const CString& strTableName, const CString& strFieldName, IFieldsPtr ipFields, long& lIndex);

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

/*
 * @brief ���[�N�X�y�[�X�擾�֐�
 * @param strTableName      [in] �e�[�u�����i�G���[�o�͗p�j
 * @param ipFeatureClass    [in] ��ԎQ�Ǝ擾��
 * @param ipSpRef           [out] �擾������ԎQ��
 * @return true ����
 * @return false ���s
 */
bool GetSpatialReference(const CString& strTableName, IFeatureClassPtr ipFeatureClass, ISpatialReferencePtr& ipSpRef);

/*
 * @brief ���[�N�X�y�[�X�擾�֐�
 * @param strTableName      [in] �e�[�u�����i�G���[�o�͗p�j
 * @param ipFeatureClass    [in] �t�B�[�`���擾��
 * @param lOID              [in] �擾�Ώۂ̃I�u�W�F�N�gID
 * @param ipFeature         [out] �擾�����t�B�[�`��
 * @return true ����
 * @return false ���s
 */
bool GetFeature(const CString& strTableName, IFeatureClassPtr ipFeatureClass, long lOID, IFeaturePtr& ipFeature);

/*
 * @brief ��ԎQ�ƕt�^�֐�
 * @param strTableName      [in] �e�[�u�����i�G���[�o�͗p�j
 * @param ipSpRef           [in] ��ԎQ��
 * @param ipGeom            [in,out] �t�^�Ώی`��
 * @return true ����
 * @return false ���s
 */
bool SetSpatialReference(const CString& strTableName, ISpatialReferencePtr ipSpRef, IGeometryPtr& ipGeom);

/*
 * @brief OID�擾�֐�
 * @param strTableName      [in] �e�[�u�����i�G���[�o�͗p�j
 * @param ipFeature         [in] �擾�Ώۃt�B�[�`��
 * @param lOID              [out] �擾����OID
 * @return true ����
 * @return false ���s
 */
bool GetOID(const CString& strTableName, IFeaturePtr ipFeature, long& lOID);
