#pragma once

#include "CheckBase.h"

class CheckPolygon
	: public CheckBase
{
public:
	CheckPolygon();
	virtual ~CheckPolygon();

	/**
	 * @brief ������
	 * @retval true ����
	 * @retval false ���s
	 */
	bool Init();

	/**
	 * @brief �`�F�b�N���s
	 * @retval true �G���[�Ȃ�
	 * @retval false �G���[����
	 */
	bool Check();

private:
	/**
	 * @brief �X�֔ԍ��������|���S�����אڂ��Ă��Ȃ������`�F�b�N
	 * @param meshGeometry [in] ���b�V���W�I���g��
	 * @retval true �G���[�Ȃ�
	 * @retval false �G���[����
	 */
	bool CheckPolygonDuplication(const IGeometryPtr& meshGeometry);

private:
	std::list<long> m_meshes; //!< �����Ώۃ��b�V���R�[�h�i�[
	IFeatureClassPtr m_ipZipCodeArea; //!< �X�֔ԍ��|���S���t�B�[�`���N���X
	ISpatialReferencePtr m_ipSpRef; //!< �X�֔ԍ��|���S����ԎQ��(���b�V���`��쐬�p)
	sindy::CFieldMap m_zipCodeAreaFieldMap; //!< �X�֔ԍ��|���S���t�B�[���h�}�b�v
};

