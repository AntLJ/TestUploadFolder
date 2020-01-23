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

// RNSComException.h: RNSComException �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if(0) // �g�p�s�� //

#if !defined(AFX_RNSCOMEXCEPTION_H__54853C52_E782_4A56_B6DC_853F5CEB94E8__INCLUDED_)
#define AFX_RNSCOMEXCEPTION_H__54853C52_E782_4A56_B6DC_853F5CEB94E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RNSException.h"

/**
 * RNS_COM_CALL ���G���[�𔭌������Ƃ��ɔ��������O
 */
class RNSComException : public RNSException  
{
public:
	/// �R���X�g���N�^
	RNSComException(HRESULT cHr, const char *iMessage = NULL, const char *iLocation = NULL);

	/// �f�X�g���N�^
	virtual ~RNSComException();

	// ----- �擾 ----- //
	/// ���U���g�擾
	HRESULT hr() const;

protected:
	/// �G���[���e
	virtual const char* circumstance() const { return "com error issued"; }

private:
	HRESULT	mHr;

};

#endif // !defined(AFX_RNSCOMEXCEPTION_H__54853C52_E782_4A56_B6DC_853F5CEB94E8__INCLUDED_)

#endif
