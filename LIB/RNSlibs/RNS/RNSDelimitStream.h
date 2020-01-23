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

// RNSDelimitStream.h: RNSDelimitStream �N���X�̃C���^�[�t�F�C�X
//
// $Id: RNSDelimitStream.h,v 1.3 2003/07/18 04:14:33 f_kiku Exp $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RNSDELIMITSTREAM_H__0F35D20F_43DF_47CF_89BA_B90D1225E1B0__INCLUDED_)
#define AFX_RNSDELIMITSTREAM_H__0F35D20F_43DF_47CF_89BA_B90D1225E1B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <iostream>
#include <stdexcept>

/**
 * @brief �x���f���~�^�o�̓X�g���[��
 *
 * �K�v�ƂȂ�܂Ńf���~�^���o�͂��Ȃ��X�g���[���B
 */
class RNSDelimitStream  
{
public:
// �\�z/����
	RNSDelimitStream(std::ostream* pStream, const char* cDelimiter = "\t");

// �ݒ�
	void setStream(std::ostream* pStream);
	void setDelimiter(const char* cDelimiter) { mDelimiter = cDelimiter; }

// �擾
	std::ostream& stream() { return *mStream; }
	int countDelimiter() { return mDelimNum; }

// ����
	RNSDelimitStream& operator ++ ();

// �o��
	/**
	 * @brief �f���~�^��S�ďo�͂���
	 */
	void	outputDelimiter()
	{
		for(int i = 0; i < mDelimNum; ++i) {
			stream() << mDelimiter;
		}
		mDelimNum = 0;
	}

	/**
	 * @brief std::endl�Ȃǂ̏���
	 *
	 * std::ostream�̃}�j�s�����[�^�͕��͂̏I�����������̂������������Ǝv���̂ŁA�f���~�^�o�͂͂Ȃ��B\n
	 * ���Ⴂ��������C�����悤�B
	 *
	 * @param std::ostream�̃}�j�s�����[�^
	 * @return ����
	 */
	RNSDelimitStream& operator <<( std::ostream& (*fcn)(std::ostream&) )
	{
		stream() << fcn;
		return *this;
	}

	/**
	 * @brief std::ios�}�j�s�����[�^
	 *
	 * std::ios�̃}�j�s�����[�^�͏����ύX���s�����̂������������Ǝv���̂ŁA�f���~�^�o�͂͂Ȃ��B\n
	 * ���Ⴂ��������C�����悤�B
	 *
	 * @param std::ios�̃}�j�s�����[�^
	 * @return ����
	 */
	RNSDelimitStream& operator <<( std::ios& (*fcn)(std::ios&) )
	{
		stream() << fcn;
		return *this;
	}

	/**
	 * @brief �f���~�^�o�͌�AcArg���o��
	 *
	 * @param cArg [in] �o�͑Ώ�
	 * @retrurn ����
	 */
	template<typename T>
	RNSDelimitStream& operator << (T cArg)
	{
		outputDelimiter();
		stream() << cArg;

		return *this;
	}

private:
	int mDelimNum; ///< �X�^�b�N���Ă���f���~�^��
	std::string mDelimiter; ///< �f���~�^
	std::ostream* mStream; ///< �o�̓X�g���[��
};

#endif // !defined(AFX_RNSDELIMITSTREAM_H__0F35D20F_43DF_47CF_89BA_B90D1225E1B0__INCLUDED_)
