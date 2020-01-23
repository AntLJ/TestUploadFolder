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

// RNSConsoleApp.h: RNSConsoleApp �N���X�̃C���^�[�t�F�C�X
//
// $Id: RNSConsoleApp.h,v 1.2 2003/07/14 01:21:01 f_kiku Exp $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RNSCONSOLEAPP_H__832C4327_E892_46FC_B182_999FC8EA742D__INCLUDED_)
#define AFX_RNSCONSOLEAPP_H__832C4327_E892_46FC_B182_999FC8EA742D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <iostream>

#include "RNSOptionSet.h"

/**
 * @brief	�R���\�[���A�v���P�[�V�����N���X
 */
class RNSConsoleApp  
{
public:
// �\�z/����
	RNSConsoleApp(const char* cAppCaption);
	virtual ~RNSConsoleApp();

// �ݒ�
	bool setArgs(int argc, char* argv[]);

// �擾
	const std::string&	appCaption() const { return mAppCaption; }
	const std::string&	appName() const	{ return mAppName; }
	const RNSOptionSet&	optionSet() const { return mOptionSet; }
	const RNSArgList&	argList() const { return mArgList; }

// ����
	int run();

protected:
// �I�[�o�[���C�h�����ݒ�
	virtual const char* optionSimbols() { return 0; }

// �I�[�o�[���C�h����锻��
	virtual bool		isNeedPrintUsage() { return false; }

// �I�[�o�[���C�h����鏈��
	virtual void		printUsage(std::ostream& cStream);
	virtual int			callEntrust();
	virtual bool		preEntrust(){return true;}
	virtual int			entrust() = 0;
	virtual bool		epiEntrust(){return true;}

private:
// �ݒ�
	void setAppCaption(const char* cAppCaption) { mAppCaption = cAppCaption; }
	void setAppName(const char* cAppName)		{ mAppName = RNSPlant::fileNameOf(cAppName); }

// ����

// �v���p�e�B
	std::string		mAppCaption;	///< �A�v���P�[�V������
	std::string		mAppName;		///< �A�v���P�[�V�������s�t�@�C����
	RNSOptionSet	mOptionSet;		///< �K��I�v�V�����ꗗ
	RNSArgList		mArgList;		///< �K��I�v�V�����O�����ꗗ

	bool			mAlreadyRun;	///< ���s�J�n�ς݃t���O
};

/** @example example_RNSConsoleApp.cpp
 * �ʏ�ARNSConsoleApp�N���X���̂̃C���X�^���X���쐬���邱�Ƃ͂Ȃ��B\n
 * ��t�@�C���ɂ�����RNSConsoleApp�̃C���X�^���X���쐬����Ă���
 */

#endif // !defined(AFX_RNSCONSOLEAPP_H__832C4327_E892_46FC_B182_999FC8EA742D__INCLUDED_)
