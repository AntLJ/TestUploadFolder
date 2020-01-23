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

/* ************************************************
	���e :
	���� :  
	�ǋL : 
 
	���� :  Fukuta.K
************************************************* */

/* ----- �C���N���[�h�t�@�C�� ----- */

#include <iostream>

#include "RNSOption.h"

// ===== public ���\�b�h ===== //

// --- ������ --- //

/// �f�t�H���g�R���X�g���N�^
RNSOption::RNSOption()
{
	init();
}

/// �R���X�g���N�^
RNSOption::RNSOption(char iSimbol, bool iNeadValue)
{
	init();
	mSimbol = iSimbol;
	mNeadValue = iNeadValue;
}

/// ������
void RNSOption::init()
{
	mSimbol = 0;
	mNeadValue = false;
	mFlag = false;
}

// --- �ݒ�@--- //

/// �I�v�V�������w�肳��܂����I
void RNSOption::setFlag(bool iFlag)
{
	mFlag = iFlag;
}

/// �l�ݒ�
void RNSOption::setValue(const char *iValue)
{
	mValue = new RNSString(iValue);
	setFlag();
}

// --- �擾 --- //

/// ���̃I�v�V�����͎w�肳��܂������H
bool RNSOption::flag() const 
{
	return mFlag;
}

/// �l���擾
const char* RNSOption::value() const 
{
	return mValue ? mValue->c_str() : 0;
}

// --- ���� --- //
/// �l��K�v�Ƃ���I�v�V�����ł����H
bool RNSOption::isNeadValue() const {return mNeadValue;}

// --- �I�y���[�^ --- //
/// �I�v�V�����擾
RNSOption::operator char () const
{
	return mSimbol;
}

/// �I�v�V�������Z�b�g���ꂽ������
RNSOption::operator bool () const
{
	return mFlag;
}

/// �I�v�V�����̑召����
bool RNSOption::operator < (const RNSOption &iOption) const
{
	return (char)*this < (char)iOption;
}

