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

// TODO: str_util�Ɏ�������
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
 * @brief �Ώۃ��C���̃C���f�b�N�X�Q���擾
 * @param layer [in] ���C��
 * @return �Ώۃ��C���̃C���f�b�N�X�Q
 */
IIndexesPtr getIndexes(ITablePtr layer);

/**
 * @brief �Ώۃ��C������w��̃t�B�[���h���擾
 * @param fieldName [in] �t�B�[���h��
 * @param layer     [in] ���C��
 * @return �擾�����t�B�[���h��
 */
IFieldPtr getField(const uh::tstring& fieldName, ITablePtr layer);

} // util
