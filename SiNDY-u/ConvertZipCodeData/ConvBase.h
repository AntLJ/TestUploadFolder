#pragma once

/**
 * @brief �f�[�^�ϊ��p�̊��N���X
 */
class CConvBase
{
public:
	CConvBase() : m_bFeatureClass(false){}
	virtual ~CConvBase(){
	}

public:
	/** 
	 * @brief �������p
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool init();

	/** 
	 * @brief �f�[�^�ϊ��p
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool convert();

protected:
	/**
	 * @brief �ϊ����e�[�u������f�[�^�ǂݍ��݁i�������z�֐��j
	 */
	virtual bool loadData() = 0;

	/**
	 * @brief ����ϊ��i���z�֐��j
	 * @retval true ��������
	 * @retval false �������s
	 */
	virtual bool convSpecial();

	/**
	 * @brief Shapefile�փf�[�^�o�́i�������z�֐��j
	 */
	virtual bool outputData() = 0;

	/** 
	 * @brief �t�B�[���h�C���f�b�N�X�쐬�i�������z�֐��j
	 */
	virtual bool createFieldIndex() = 0;

	/** 
	 * @brief �o�͐�Shapefile�̃t�B�[���h�ݒ�i�������z�֐��j
	 */
	virtual bool createFields(IFieldsPtr& ipFields) = 0;

protected:
	/** 
	 * @brief �o�͐��Shapefile�쐬
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool createShapefile();

	/**
	 * @brief �����t�B�[���h�ǉ�
	 * @note �Ή����Ă���͕̂�����^�A�����^�̂�
	 * @param strTableName [in] �e�[�u�����i���O�o�͗p�j
	 * @param strFieldName [in] �t�B�[���h��
	 * @param eFieldType [in] �t�B�[���h�^�C�v
	 * @param vaIsEditable [in] �ҏW����
	 * @param vaIsNullable [in] NULL����
	 * @param lLength [in] ����
	 * @param lPrecision [in] ���x
	 * @param ipFields [out] �t�B�[���h�Q
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool addAttrField(const CString&strTableName, const CString& strFieldName, esriFieldType eFieldType, VARIANT_BOOL vaIsEditable, VARIANT_BOOL vaIsNullable, long lLength, long lPrecision, IFieldsPtr& ipFields);

	/**
	 * @brief �`��t�B�[���h�ǉ�
	 * @param strTableName [in] �e�[�u�����i���O�o�͗p�j
	 * @param eGeoType [in] �`��^�C�v
	 * @param ipFields [out] �t�B�[���h�Q
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool addShapeField(const CString&strTableName, esriGeometryType eGeoType, IFieldsPtr& ipFields);

	/**
	 * @brief �t�B�[���h�C���f�b�N�X�擾
	 * @param strTableName [in] �e�[�u�����i���O�o�͗p�j
	 * @param ipTable [in] �e�[�u��
	 * @param mapFieldIndex [out] �t�B�[���h�C���f�b�N�X
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool setFieldIndex(const CString&strTableName, const ITablePtr& ipTable, std::map<CString,long>& mapFieldIndex);

	/**
	 * @brief ������Shapefile�폜�p
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool deleteShapefile();

	/**
	 * @brief �e�[�u���폜
	 * @note �t�B�[�`���N���X�ł��폜�\
	 * @param ipOutdirWork [in] �폜�Ώۂ̃e�[�u���������[�N�X�y�[�X
	 * @param strTableName [in] �e�[�u����
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool deleteTable(const IWorkspacePtr ipOutdirWork, const CString& strTableName);

protected:
	CString m_strOutDir;			//!< �o�͐�f�B���N�g��
	IWorkspacePtr m_ipOutWork;		//!< �o�͐�f�B���N�g���̃��[�N�X�y�[�X
	ITablePtr m_ipOutShapefile;		//!< �o��Shapefile�e�[�u���i�t�B�[�`���N���X�j

	bool m_bFeatureClass;			//!< �o��Shapefile���t�B�[�`���N���X���ǂ���
	CString m_strShapefileName;		//!< �o��Shapefile��
	std::map<CString,long> m_mapFieldIndexOutShapefile;	//!< �ϊ���iShapefile�j�e�[�u���̃t�B�[���h�C���f�b�N�X
};
