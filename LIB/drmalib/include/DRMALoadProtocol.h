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

// DRMALoadProtocol.h: DRMALoadProtocol �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMALOADPROTOCOL_H__E2D2CEE1_E922_4921_8B75_F226DAC89A6D__INCLUDED_)
#define AFX_DRMALOADPROTOCOL_H__E2D2CEE1_E922_4921_8B75_F226DAC89A6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <iostream>

/**
 * @brief DRMA�f�[�^�ǂݎ��v���g�R��
 */
class DRMALoadProtocol  
{
public:
	DRMALoadProtocol();

	virtual bool load(const char* cDirectoryName, int cMeshCode, const char* cSuffix = ".mt") throw(std::runtime_error);
	virtual bool load(const char* cFileName) throw(std::runtime_error);
	virtual bool load(std::istream& cStream) throw(std::runtime_error);

	void close();

protected:
	/**
	 * @brief �ǂݍ��݊����錾
	 *
	 * �S���R�[�h�𑖍�����O��load()���I�����������ꍇ�Aappend()�����炱�̊֐����Ă�
	 */
	void setComplete() { mLoadComplete = true; }
	
// �������z�֐�
	/**
	 * @brief DRMA��1���R�[�h���n���h�����O����
	 *
	 * @param cRecordId [in] ���R�[�hID
	 * @param cOriginalRecord [in] ���R�[�h
	 * @retval true �o�^�ɐ��������A���邢�͓o�^���Ȃ�����
	 * @retval false ���邢�͓o�^�Ɏ��s����
	 */
	virtual bool append(int cRecordId, const char* cOriginalRecord) = 0;

	/**
	 * @brief �n���h�����O�����S�Ẵ��R�[�h���������
	 */
	virtual void clear() = 0;

private:
// �v���p�e�B
	bool mLoadComplete;
};

#endif // !defined(AFX_DRMALOADPROTOCOL_H__E2D2CEE1_E922_4921_8B75_F226DAC89A6D__INCLUDED_)
