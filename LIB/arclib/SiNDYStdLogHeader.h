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

#ifndef __SINDY_STD_LOG_HEADER_H__
#define __SINDY_STD_LOG_HEADER_H__

#include "SiNDYLogHeader.h"

namespace sindy {

/**
 * @brief �W���`�����O�w�b�_�N���X
 */
class StdLogHeader : public SiNDYLogHeader
{
public:
// ��`
	/**
	 * @brief �W���`�����O�w�b�_�\�z�N���X
	 */
	class Builder
	{
	public:
	// ����
		void operator()(SiNDYLogHeader& rLogHeader);

	private:
	// �ϐ�
		static const LPCTSTR m_rgszColumn[];
	};

// �\�z/����
	StdLogHeader();

// ���z�֐��̃I�[�o�[���C�h
	virtual LPCTSTR getId() const { return id(); }
	virtual LPCTSTR getDescription() const { return _T("�W���`�����O"); }
	virtual LPCTSTR getSeps() const { return seps(); }

// �ÓI�֐�
	static LPCTSTR id() { return _T("SINDYSTDLOG"); }
	static LPCTSTR seps() { return _T("\t,|"); }
};

} // namespace sindy

#endif // __SINDY_STD_LOG_HEADER_H__
