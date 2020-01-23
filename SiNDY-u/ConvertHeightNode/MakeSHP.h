#pragma once
#include "CnvData.h"

class MakeSHP
{
public:
	MakeSHP(void);
	~MakeSHP(void);

	/*
	 *	@brief	static�����o�ϐ��̏������E��Shape�쐬
	 *	@param	outputDir [in] �ėp���΍���Shape�o�̓f�B���N�g��
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Init(LPCTSTR outputDir);

	/*
	 *	@brief�@���R�[�h�ϊ�
	 *	@note	�ϊ��f�[�^�R���e�i�̂����Aobjectid == startID�ł��郌�R�[�h����A���R�[�h��ϊ�����
	 *			startID == 0�̏ꍇ�́A�R���e�i�̍ŏ�����ϊ�����B
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool InsertRecord(const CnvData& cnvData, const long startID = 0);

	/*
	 *	@brief	�t�@�C�����l�[��
	 *	@note	�쐬����Shape�t�@�C������̏ꍇ�̂ݍs��
	 *	@param	outputDir [in] �ėp���΍���Shape�o�̓f�B���N�g��
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool ReName(LPCTSTR outputDir);


private:
	/*
	 *	@brief	HeightNode��Shape�쐬
	 *	@param	shpName
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool MakeVacuityShp(LPCTSTR shpName);
	/*
	 *	@brief	�����t�B�[���h�쐬
	 *	@param	fieldName [in] �t�B�[���h��
	 *	@param	fieldType [in] �t�B�[���h�^�C�v
	 *	@param	editalbe [in] �ҏW��
	 *	@param	nullable [in] NULL����
	 *	@param	fieldLength [in] �t�B�[���h��
	 *	@param	precision [in] ���x
	 *	@param	ipFields [out] �t�B�[���h�|�C���^
	 *	@retval	true
	 *	@retval false
	 */
	bool MakeAttrField(LPCTSTR fieldName,
					   const esriFieldType fieldType,
					   const VARIANT_BOOL editable,
					   const VARIANT_BOOL nullable,
					   const long fieldLength,
					   const long precision,
					   IFieldsEditPtr& ipFields);
	/*
	 *	@brief	�`��t�B�[���h�쐬
	 *	@param	geometryType	[in] �W�I���g���^�C�v
	 *	@param	ipFields [out]	�t�B�[���h�|�C���^
	 *	@retval	true
	 *	@retval false
	 */
	bool MakeShapeField(const esriGeometryType geoType, IFieldsEditPtr& ipFields);

	/*
	 *	@brief	�t�B�[���h�C���f�b�N�X�擾
	 *	@retval true ����
	 *	@retval	false ���s
	 */
	bool SetFieldIndex();


private:
	IWorkspacePtr m_ipWorkspace;
	IFeatureClassPtr m_ipFeatureClass;
	long m_fileCount;

	long m_objectidIndex;
	long m_linkid1Index;
	long m_divid1Index;
	long m_relHeight1Index;
	long m_linkid2Index;
	long m_divid2Index;
	long m_relHeight2Index;

	long m_shpTotalSize;
	long m_dbfTotalSize;
};

