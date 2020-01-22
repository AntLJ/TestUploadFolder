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

#include <crd_cnv/coord_converter.h>


namespace adam_v2
{
/**
 * �f�[�^�A�N�Z�X���N���X
 */
class DataAccessor
{
public:
	DataAccessor(void) :
		_path(_T(""))
	{}
	virtual ~DataAccessor(void){}

	/**
	 * �f�[�^�N���A
	 */
	virtual void Clear() = 0;
	/**
	 * �f�[�^�ǂݍ���
	 * @param[in]	fileName	CSV�t�@�C����
	 * @retval	true:	����
	 * @retval	false:	���s
	 */
	virtual bool Read(const uh::tstring& fileName) = 0;
	/**
	 * �f�[�^��������
	 * @param[in]	tableName	ADAM�e�[�u����
	 * @retval	true:	����
	 * @retval	false:	���s
	 */
	virtual bool Write(const uh::tstring& tableName) = 0;

	/**
	 * �p�����[�^�ݒ�
	 *
	 * @param param	[in] ����DB��
	 */
	void SetParam(const uh::tstring& param)
	{
		_path = param;
	}

	/**
	 * @brief ���W�ϊ��p�p�����[�^�t�@�C���̓ǂݍ���
	 *
	 * @param fileName [in] �t�@�C����
	 * @retval true �ǂݍ��ݐ���
	 * @retval false �ǂݍ��ݎ��s
	 */
	bool ReadCrdParam(const uh::tstring& fileName);

	/**
	 * �o�[�W�������p�L���ݒ�
	 *
	 * @param[in]	flg	true:���p���Ȃ�
	 */
	void SetUnuseVersion(bool flg)
	{
		_unuse_version = flg;
	}

	/**
	 * �o�[�W�������p�L���擾
	 *
	 * @return	true:���p���Ȃ�
	 */
	bool GetUnuseVersion()
	{
		return _unuse_version;
	}


protected:
	uh::tstring _path;				///< �p�X
	crd_cnv		_cnv;				//!< ���W�ϊ��p�����[�^
	bool		_unuse_version;		//!< �o�[�W�������p���Ȃ�
};

typedef std::shared_ptr<DataAccessor> DataAccessorPtr;

};	//namespace adam_v2