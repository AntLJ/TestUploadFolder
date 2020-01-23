#pragma once
#include "ConvBase.h"
#include "ConvRelZipAddr.h"
#include "Define.h"

/**
 * @brief �s�撬����\�|�C���g�ϊ��p�N���X
 */
class CConvRep : public CConvBase
{
public:
	CConvRep(const CString& strOutDir, const ITablePtr& ipRelAddr, const CString& strAddrDB, const CADOBase& cADOBase){
		m_strOutDir = strOutDir;
		m_ipRelAddr = ipRelAddr;
		m_strAddrDB = strAddrDB;
		m_cADOBase = &cADOBase;
		m_bFeatureClass = true;
		m_strShapefileName = zip_pol::schema::zip_rep::kTableName;
	}
	~CConvRep(){
		m_mapFieldIndexRelAddr.clear();
		m_mapZipRep.clear();
	}
public:

	void clearData(){
		m_mapZipRep.clear();
	}

private:
	/** 
	 * @brief �t�B�[���h�C���f�b�N�X�쐬
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool createFieldIndex() override;

	/** 
	 * @brief �o�͐�Shapefile�̃t�B�[���h�ݒ�
	 * @param ipFields [out] �t�B�[���h�Q
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool createFields(IFieldsPtr& ipFields) override;

	/**
	 * @brief �ϊ����e�[�u������f�[�^�ǂݍ���
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool loadData() override;

	/**
	 * @brief Shapefile�փf�[�^�o��
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool outputData() override;

private:
	CString m_strAddrDB;			//!< �Z��DB�ڑ����
	const CADOBase* m_cADOBase;		//!< �Z��DB�R�l�N�V����
	ITablePtr m_ipRelAddr;			//!< �X�֔ԍ��ɕR�t���Z���R�[�h�i�ϊ����e�[�u���j
	std::map<CString,long> m_mapFieldIndexRelAddr;	//!< �ϊ����iSDE,���v���J�j�e�[�u���̃t�B�[���h�C���f�b�N�X

	std::map<CString,std::map<std::pair<CString,CString>,IPointPtr>> m_mapZipRep;	//!< �X�֔ԍ��Ǝs�撬����\�|�C���g�̃}�b�v
};
