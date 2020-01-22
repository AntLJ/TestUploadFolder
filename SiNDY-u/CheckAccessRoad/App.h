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

/** @file
 * CApp �N���X�̃C���v�������e�[�V����
 *
 * @author	shimano
 * @date	2003/04/09
 *
 * $Log: App.h,v $
 * Revision 1.3  2003/11/17 01:09:03  shimano
 * Ver.1.6�i�A�N�Z�X���H���X�g�o�̓t�H�[�}�b�g�C���j�Ή�
 *
 * Revision 1.2  2003/11/15 01:35:57  shimano
 * �A�N�Z�X���H�����m�F�t�@�C���o�͑Ή��iDebug���[�h�j
 *
 * Revision 1.1.1.2  2003/04/24 09:46:44  shimano
 * �V�K�쐬
 *
 * Revision 1.0  2003/04/09 15:13:43  shimano
 * beta8�Ή��B
 *
 */

#if !defined(AFX_APP_H__90BEEEFE_3B9D_486C_B608_D08B434A610E__INCLUDED_)
#define AFX_APP_H__90BEEEFE_3B9D_486C_B608_D08B434A610E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	"doc.h"
#define	CONFIGFILE	"config.txt"		//���̃c�[���̂r�c�d�ڑ��p�����^�t�@�C��

class CApp  
{
public:
	CApp();
	virtual ~CApp();
	bool	init(int, TCHAR* cArgv[]);
	bool	getenv(LPCTSTR);
	int	showMenu(void);
	void	getTime(CString &);
	int		run(void);
	int	selectjobnum(char);
	int		m_JobNo;			//���X�g�쐬�ԍ�

// �A�g���r���[�g
protected:
	CDoc	   *m_pDoc;				//�h�L�������g�Ǘ��N���X
	CString		m_aMeshcodeFile;	//�S�������b�V���R�[�h���X�g�t�@�C��
};

#endif // !defined(AFX_APP_H__90BEEEFE_3B9D_486C_B608_D08B434A610E__INCLUDED_)
