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

// WaitCursor.h: CWaitCursor �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAITCURSOR_H__745CA646_1660_4819_AA58_C7B4A7B06CA0__INCLUDED_)
#define AFX_WAITCURSOR_H__745CA646_1660_4819_AA58_C7B4A7B06CA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// �N���X��`
//////////////////////////////////////////////////////////////////////

/**
 * @brief �҂��J�[�\���N���X
 */
class CWaitCursor
{
public:
// �\�z/����
	CWaitCursor();
	~CWaitCursor();

// ����
	void Restore();

private:
// �J�[�\��
	HCURSOR m_curWait;
	HCURSOR m_curPrev;
};

//////////////////////////////////////////////////////////////////////
// CWaitCursor�N���X����

inline CWaitCursor::CWaitCursor()
{
	m_curWait = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_WAIT));
	m_curPrev = 0;

	m_curPrev = ::GetCursor();
	::SetCursor(m_curWait);
}

inline CWaitCursor::~CWaitCursor()
{
	Restore();
}

inline void CWaitCursor::Restore()
{
	if(m_curPrev) {
		::SetCursor(m_curPrev);
		m_curPrev = 0;
	}
}

#endif // !defined(AFX_WAITCURSOR_H__745CA646_1660_4819_AA58_C7B4A7B06CA0__INCLUDED_)
