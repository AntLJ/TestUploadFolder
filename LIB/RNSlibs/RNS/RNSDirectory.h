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

#ifndef __RNS_DIRECTORY__
#define	__RNS_DIRECTORY__

/* ************************************************

 	���e :
 	���� :  
 	�ǋL : 
 
 	���� :	Fukuta.K
 	
************************************************* */

#ifdef _WIN32
#pragma comment(lib, "imagehlp.lib")
#endif

/* ----- ��` ----- */

#include "RNS.h"
#include "RNSString.h"

/* ////// �N���X��`  ////// */

/**
 * �f�B���N�g�����̃N���X
 *
 * ��ɍŌ���Ƀf�B���N�g����؂�L����t���ĕ�������Ǘ����Ă��� 
 */
class RNSDirectory : public RNSString {
public:

// �\�z/����
	RNSDirectory();
	RNSDirectory(const char *iStr);
	RNSDirectory(const std::string &iStr);
	virtual ~RNSDirectory();

// ����
	virtual RNSString& append(const RNSString &iStr);

	bool	mkdir() const;

// ����
	bool	exists() const;

// �I�y���[�^
	virtual RNSString& operator = (const std::string &iStr);
	virtual RNSString& operator = (const char *iStr);

protected:

	virtual void	appendEpiEffect();

	RNSDirectory&	dirComplete(char iTrMark = _pathChr);

};

_RNSOBJECT_TYPEDEF(RNSDirectory);

#endif /* __RNS_DIRECTORY__ */
