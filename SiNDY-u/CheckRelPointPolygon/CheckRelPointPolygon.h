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


class CCheckRelPointPolygon
{
public:

	/**
	 * @brief �|�C���g�ƃ|���S���̊֌W���`�F�b�N����
	 * @param args	[in]	�R�}���h���C������
	 * @retval true	��������
	 * @retval false	�������s
	 */
	bool Check(const Arguments& args);

	/**
	 * @brief ������
	 * @param args	[in]	�R�}���h���C������
	 * @retval true	����
	 * @retval false	���s
	 */
	bool Init(const Arguments& args);

	/**
	 * @brief ���[�N�X�y�[�X�擾
	 * @param ipPointWork	[out]	�|�C���g�̐ڑ���
	 * @param ipPolyWork	[out]	�|���S���̐ڑ���
	 * @retval true �擾����
	 * @retval false �擾���s
	 */
	bool GetWorkspace(IWorkspacePtr& ipPointWork, IWorkspacePtr& ipPolyWork);

	/**
	 * @brief �t�B�[�`���N���X�擾
	 * @param ipPointWork	[in]	�|�C���g�̐ڑ���
	 * @param ipPolyWork	[in]	�|���S���̐ڑ���
	 * @param ipPointFC	[out]	�|�C���g�̃t�B�[�`���N���X
	 * @param ipPolyFC	[out]	�|���S���̃t�B�[�`���N���X
	 * @retval true �擾����
	 * @retval false �擾���s
	 */
	bool GetFeatureClass(IWorkspacePtr ipPointWork, IWorkspacePtr ipPolyWork, IFeatureClassPtr& ipPointFC, IFeatureClassPtr& ipPolyFC);

	/**
	 * @brief �w�肳�ꂽ�t�B�[�`�����w�肳�ꂽ�t�B�[�`���N���X�ɏd�Ȃ��Ă��邩���`�F�b�N����
	 * @param ipFeature	[in]	�Ώۂ̃I�u�W�F�N�g
	 * @param ipFeatureClass	[in]	�ڐG���Ă��邩���ׂ�t�B�[�`��
	 * @param strErrMsg	[in]	�G���[���b�Z�[�W
	 * @param bIsMyself	[in]	�Ώۂ̃I�u�W�F�N�g�ƐڐG���������t�B�[�`���������t�B�[�`�����ǂ���
	 */
	void CheckIntersects(IFeaturePtr ipFeature, IFeatureClassPtr ipFeatureClass, const CString& strErrMsg, bool bIsMyself = false);

	/**
	 * @brief �J�n���̃��O����������
	 * @param args	[in]	�R�}���h���C������
	 */
	void WriteStartLog(const Arguments& args);

	/**
	 * @brief �G���[���O����������
	 * @param strObject	[in]	�I�u�W�F�N�g��
	 * @param OID	[in]	�I�u�W�F�N�gID
	 * @param strErrMsg	[in]	�G���[���b�Z�[�W
	 */
	void WriteErrLog(const CString& strObject, long OID, const CString& strErrMsg);

	/**
	 * @brief �I�����̃��O����������
	 * @param success	[in]	������������true�A�������s����false
	 */
	void WriteEndLog(bool success = true);

private:
	uh::tstring m_PointConnect;  //!< �|�C���g�̐ڑ���
	uh::tstring m_PolyConnect;   //!< �|���S���̐ڑ���
	uh::tstring m_Point;         //!< �ݒ�t�@�C����POINT�Ŏw�肳�ꂽ�t�B�[�`����
	uh::tstring m_Poly;          //!< �ݒ�t�@�C����POLYGON�Ŏw�肳�ꂽ�t�B�[�`����
	std::ofstream m_RunLog;      //!< ���s���O
	std::ofstream m_ErrLog;      //!< �G���[���O
	CString m_PointSubF;        //!< �|�C���g��SubFields
	CString m_PointWhere;       //!< �|�C���g��WHERE��
	CString m_PolySubF;         //!< �|���S����SubFields
	CString m_PolyWhere;        //!< �|���S����WHERE��
};
