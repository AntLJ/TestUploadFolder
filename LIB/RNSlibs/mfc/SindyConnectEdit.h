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

// $Id: SindyConnectEdit.h,v 1.1.1.1 2003/09/12 07:26:04 f_kiku Exp $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYCONNECTEDIT_H__5DE197E3_2501_46F9_B36A_21FEB5972B61__INCLUDED_)
#define AFX_SINDYCONNECTEDIT_H__5DE197E3_2501_46F9_B36A_21FEB5972B61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SindyConnectEdit.h : �w�b�_�[ �t�@�C��
//

#include "SindyConnectProperty.h"

/////////////////////////////////////////////////////////////////////////////
// CSindyConnectEdit �E�B���h�E

class CSindyConnectEdit : public CEdit
{
// �R���X�g���N�V����
public:
	CSindyConnectEdit();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSindyConnectEdit)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CSindyConnectEdit();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CSindyConnectEdit)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
// �ݒ菈��
	void setProperties(const CSindyConnectProperty& rProperty);

// �擾����
	void getPropertiesTo(CSindyConnectProperty& rProperty);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SINDYCONNECTEDIT_H__5DE197E3_2501_46F9_B36A_21FEB5972B61__INCLUDED_)
