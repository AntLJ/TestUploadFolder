#pragma once

// option�p�ϐ�
const LPCSTR ROADDB = "road_conn_db";
const LPCSTR ROADOWNER = "road_owner";
const LPCSTR ROADWHERE = "road_where";
const LPCSTR WARINIGSIGNDB = "warning_sign_conn_db";
const LPCSTR WARINGSIGNOWNER = "warning_sign_owner";
const LPCSTR BUFFER = "buffer";
const LPCSTR OUTPUTLOG ="log";
const LPCSTR RUNLOG = "runlog";
const LPCSTR ERRLOG = "errlog";


namespace common_proc {

	/*
	 *	@brief	���[�N�X�y�[�X�擾�֐�
	 *	@param	connectInfo [in] ���[�N�X�y�[�X�ւ̃p�X
	 *	@param	ipWorkspace [out] ���[�N�X�y�[�X
	 *	@retval	true ����
	*	@retval	false ���s
	 */
	bool getWorkspace(LPCTSTR connectInfo, IWorkspacePtr& ipWrokspace);

	/*
	 *	@brief	�t�B�[�`���N���X�擾�֐�
	 *	@param  ipWorkspace [in] ���[�N�X�y�[�X
	 *	@param  tableName [in] �e�[�u����
	 *	@param  schemaName [in] �Ώۃe�[�u���i�[�X�L�[�}
	 *  @param  ipFeatureClass [out] �t�B�[�`���N���X
	 *	@retval	true ����
	 *	@retval	false ���s
	 */
	bool getFeatureClass(const IWorkspacePtr& ipWorkspace, LPCTSTR tableName, const CString& schemaName, IFeatureClassPtr& ipFeatureClass);

	/*
	 *	@brief  �t�B�[�`���J�[�\���擾�֐�
	 *	@param  ipFeatureClass [in] �t�B�[�`���N���X
	 *	@param  LPCTSTR tableName [in] �e�[�u����
	 *	@param  ipGeometry [in] ��Ԍ����p�`��
	 *	@param	spRel [in] ��Ԍ����p��Ԋ֌W
	 *	@param  whereClasue [in] ��������
	 *	@param  ipFeatureCursor [out] �t�B�[�`���J�[�\��
	 *	@retval	true ����
	 *	@retval	false ���s
	 */
	bool getFeatureCursor(const IFeatureClassPtr& ipFeatureClass, LPCTSTR tableName, const IGeometryPtr& ipGeometry, esriSpatialRelEnum spRel, const CString& whereClause, IFeatureCursorPtr& ipFeatureCursor);

	/*
	 * @brief �t�B�[���h�Q�擾�֐�
	 * @param tableName      [in] ���C����(�G���[�o�͗p)
	 * @param ipFeatureClsas [in] �t�B�[�`���N���X
	 * @param ipFields       [out] �擾�����t�B�[���h�Q
	 *	@retval	true ����
	 *	@retval	false ���s
	 */
	bool getFields(LPCTSTR tableName, IFeatureClassPtr& ipFeatureClass, IFieldsPtr& ipFields);

	/*
	 * @brief �t�B�[���h�擾�֐�
	 * @param tableName      [in] ���C����(�G���[�o�͗p)
	 * @param ipFields       [in] �t�B�[���h�Q
	 * @param lIndex         [in] �t�B�[���h�C���f�b�N�X
	 * @param ipField        [out] �擾�����t�B�[���h
	 *	@retval	true ����
	 *	@retval	false ���s
	 */
	bool getField(LPCTSTR tableName, IFieldsPtr& ipFields, long lIndex, IFieldPtr& ipField);
}

