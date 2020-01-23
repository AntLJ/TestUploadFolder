#pragma once

// �R�}���h���C������
const LPCSTR INI = "ini";
const LPCSTR PROCLOG = "proc_log";
const LPCSTR ERRLOG = "err_log";


// �ݒ�t�@�C������
const LPCSTR INPUTDIR = "INPUT_DIR";
const LPCSTR HEIGHTDB = "HEIGHT_DB";
const LPCSTR HEIGHTSCHEMA = "HEIGHT_SCHEMA";
const LPCSTR MESHDB = "MESH_DB";
const LPCSTR MESHSCHEMA = "MESH_SCHEMA";
const LPCSTR OUTPUTDIR = "OUTPUT_DIR";
const LPCSTR MESHLIST = "MESHLIST";
const LPCSTR BUFFER = "BUFFER";

const long HUNDRED_MILLION = 100000000;

// �t�B�[���h���
struct FieldInfo
{
	CString m_fieldName;	// �t�B�[���h��
	esriFieldType m_fieldType;	// �t�B�[���h�^�C�v
	long m_length; // �t�B�[���h��

	FieldInfo(LPCTSTR fieldName, esriFieldType fieldType, long length)
	{
		m_fieldName = fieldName;
		m_fieldType = fieldType;
		m_length = length;
	}
};


namespace common_proc {

	/*
	 *	@brief	���[�N�X�y�[�X�擾�֐�
	 *	@param	connectInfo [in] ���[�N�X�y�[�X�ւ̃p�X
	 *	@param	ipWorkspace [out] ���[�N�X�y�[�X
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool GetWorkspace(LPCTSTR connectInfo, IWorkspacePtr& ipWorkspace); 


	/*
	 *	@brief	�t�B�[�`���N���X�擾�֐�
	 *	@param  ipWorkspace [in] ���[�N�X�y�[�X
	 *	@param  tableName [in] �e�[�u����
	 *	@param  schemaName [in] �Ώۃe�[�u���i�[�X�L�[�}
	 *  @param  ipFeatureClass [out] �t�B�[�`���N���X
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool GetFeatureClass(const IWorkspacePtr& ipWorkspace, LPCTSTR tableName, const CString& schemaName, IFeatureClassPtr& ipFeatureClass);


	/*
	 *	@brief  �t�B�[�`���J�[�\���擾�֐�
	 *	@param  ipFeatureClass [in] �t�B�[�`���N���X
	 *	@param  LPCTSTR tableName [in] �e�[�u����
	 *	@param  ipGeometry [in] ��Ԍ����p�`��
	 *	@param	spRel [in] ��Ԍ����p��Ԋ֌W
	 *	@param  whereClasue [in] ��������
	 *	@param  ipFeatureCursor [out] �t�B�[�`���J�[�\��
	 *	@retval true ����
	 *	@retval fasle ���s
	 */
	bool GetFeatureCursor(const IFeatureClassPtr& ipFeatureClass, LPCTSTR tableName, const IGeometryPtr& ipGeometry, esriSpatialRelEnum spRel, const CString& whereClause, IFeatureCursorPtr& ipFeatureCursor);

	/*
	 *	@brief �t�B�[���h�l�擾�֐�
	 *	@param	ipRow	[in] IRowPtr
	 *	@param	layerName [in] ���C����(�G���[�o�͗p)
	 *	@param	fieldName [in] �t�B�[���h��(�G���[�o�͗p)
	 *	@param	index [in] �t�B�[���hindex
	 *	@param	val	[out]�@�擾�����l
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool GetValue(_IRowPtr ipRow, LPCTSTR layerName, LPCTSTR fieldName, long index, CComVariant& val);

	/*
	 *	@brief	�t�B�[���h�l�ݒ�֐�
	 *	@param	ipRowBuffer	[in] IRowBufferPtr
	 *	@param	layerName [in] ���C����(�G���[�o�͗p)
	 *	@param	fieldName [in] �t�B�[���h��(�G���[�o�͗p)
	 *	@param	index [in] �t�B�[���hindex
	 *	@param	val [in] �ݒ肷��l
	 */
	bool PutValue(IRowBufferPtr ipRowBuffer, LPCTSTR layerName, LPCTSTR fieldName, long index, CComVariant& val);


};