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

#ifndef __RNS_OPTION__
#define	__RNS_OPTION__

/* ************************************************

 	���e :
 	���� :  
 	�ǋL : 
 
 	���� :	Fukuta.K
 	
************************************************* */

/* ----- ��` ----- */

#include <RNSObject.h>
#include <RNSString.h>

/* ////// �N���X��`  ////// */

/**
 *	�I�v�V�����L�^�N���X
**/
class RNSOption : public RNSObject {
	char			mSimbol;		// �I�v�V�������ʎq ("-a"�� a �Ƃ��j
	RNSStringPtr	mLongName;		// ���g�p
	bool			mNeadValue;		// -a 150 �̂悤�ɁA�l��K�v�Ƃ��邩

	bool			mFlag;			// �I�v�V�����������Ă��܂�
	RNSStringPtr	mValue;			// �l

	// �S���O��
	void init();

 public:
	RNSOption();
	RNSOption(char iSimbol, bool iNeadValue/* = false*/);

	/// �I�v�V�������w�肳��܂����I
	void		setFlag(bool iFlag = true);
	/// �l�ݒ�
	void		setValue(const char *iValue);

	/// ���̃I�v�V�����͎w�肳��܂������H
	bool		flag() const;
	/// �l���擾
	const char* value() const;

	/// �l��K�v�Ƃ���I�v�V�����ł����H
	bool		isNeadValue() const;

	/// �I�y���[�^
	bool		operator < (const RNSOption &iOption) const;
				operator char () const;
				operator bool () const;
};

_RNSOBJECT_TYPEDEF(RNSOption);

#endif /* __RNS_OPTION__ */
