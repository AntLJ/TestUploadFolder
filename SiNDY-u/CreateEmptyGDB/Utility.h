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
namespace util
{

/**
 * @brief �Ώۂ̃X�L�[�}�Ƀ��b�N��������N���X
 */
class SchemaGuard
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param target [in] ���b�N�Ώ�
	 */
	SchemaGuard(ISchemaLockPtr target):
		m_target(target)
	{
		lock();
	}

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~SchemaGuard()
	{
		releaseLock();
	}

protected:
	/**
	 * @brief �Ώۂ̃X�L�[�}�����b�N����
	 */
	void lock();

	/**
	 * @brief �Ώۂ̃X�L�[�}�̃��b�N����������
	 */
	void releaseLock();

	ISchemaLockPtr m_target; //!< ���b�N�Ώ�
};

/**
 * @brief ������̑啶���������𖳎����Ĕ�r
 * @retval true  �����͈�v���Ă���
 * @retval false �����͈�v���Ă��Ȃ�
 */
inline bool eq( const uh::tstring& s1, const uh::tstring& s2 )
{
	return 0 == _tcsicmp( s1.c_str(), s2.c_str() );
}

/**
 * @param �t�H���_���ǂ������肷��
 * @note FGDB�t�H���_�̓t�H���_�Ƃ݂Ȃ��Ȃ�
 * @param path [in] ����p�X
 * @retval true  �t�H���_
 * @retval false �t�H���_�ȊO
 */
bool isDirectory( const uh::tstring& path );

/**
 * @param �t�@�C�����ǂ������肷��
 * @note FGDB�t�H���_�̓t�@�C���Ƃ݂Ȃ�
 * @param path [in] ����p�X
 * @retval true  �t�@�C��
 * @retval false �t�@�C���ȊO
 */
bool isFile( const uh::tstring& path );

/**
 * @brief ��̋�ԎQ�Ƃ��������ǂ����𔻒肷��
 * @param src [in] ��r��
 * @param dst [in] ��r��
 * @retval true  ��v���Ă���
 * @retval false ��v���Ă��Ȃ�
 */
bool isEqual(ISpatialReferencePtr src, ISpatialReferencePtr dst);

/**
 * @brief ���[�N�X�y�[�X����w��̃f�[�^�Z�b�g�Q���擾
 * @param workspace [in] �f�[�^�Z�b�g���擾�Ώۂ̃��[�N�X�y�[�X
 * @param type      [in] �擾����f�[�^�Z�b�g�̃^�C�v
 * @return �擾�����f�[�^�Z�b�g�Q
 * @retval null �擾�Ɏ��s
 */
IEnumDatasetPtr getDatasets( IWorkspacePtr workspace, esriDatasetType type );

/**
 * @brief UID�̃R�s�[���擾
 * @param srcUID [in] �R�s�[��
 * @return �R�s�[���ꂽUID
 * @retval null �R�s�[�Ɏ��s
 */
IUIDPtr copyUID(IUIDPtr srcUID);

/**
 * @brief �����񂪐��������ō\������Ă��邩����
 * @retval true  �����̂�
 * @retval false �����ȊO���܂܂�Ă���
 */
bool isNumber( const uh::tstring& str );

/**
 * @brief �I�[�i�[���̕������폜�����������Ԃ�
 */
uh::tstring eraseOwnerName( const uh::tstring& str);

/**
 * @brief �t�B�[���h�}�b�v�𐶐�
 * @detail �t�B�[���h���͑啶���ɂȂ�
 * @param layer      [in]  �t�B�[���h�}�b�v�����Ώ�
 * @param shapeIndex [out] �`��t�B�[���h�̃C���f�b�N�X�ԍ����o�͂����(�Ȃ��ꍇ-1)
 * @return �t�B�[���h�}�b�v
 */
std::map<uh::tstring,long> createFieldMap(ITablePtr layer, long& shapeIndex);

} // util
