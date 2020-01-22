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

/*
** TOwnerPointer.h ���L�����������I�[�g�|�C���^
**   Programmed by Takashi Togawa
**   EDITION: 2001/10/16 �쐬
**
*/

#ifndef ___T_OWNER_POINTER_H___
#define ___T_OWNER_POINTER_H___

#define _TOP_NULL 0
#define _TOP_TRUE true
#define _TOP_FALSE false

/*
** �C���N���[�h�t�@�C��
*/

#include <functional>

/*!
  delete �̒��ۉ�
*/

template <class T>
struct _top_delete : std::unary_function<T, void>
{
	void operator()(T* cPointer) const
		{ delete cPointer; }
};

template <class T>
struct _top_delete_n : std::unary_function<T, void>
{
	void operator()(T* cPointer) const
		{ delete [] cPointer; }
};

/*!
  ���L���t���|�C���^
  =���Z�q�g�p���ɏ��L���͈�ؕύX���Ȃ��̂ŁA
  �g�p���ɖ����I�ɕύX����K�v������B
*/

template <class T, class Bool = bool, class Destruct = _top_delete<T> >
class TOwnerPointer
{
	T* mSubstance; //!< ���̃|�C���^
	Bool mOwner;   //!< ���L��

public:
	typedef T* pointer;
	typedef T& reference;

	//! �f�t�H���g�R���X�g���N�^
	TOwnerPointer() : mSubstance(_TOP_NULL), mOwner(_TOP_FALSE) {}
	//! ���̎w��R���X�g���N�^
	TOwnerPointer(pointer cSubstance) :
		mSubstance(cSubstance), mOwner(_TOP_TRUE) {}
	//! ���́��t���O�w��R���X�g���N�^
	TOwnerPointer(pointer cSunstance, Bool cOwner) :
		mSubstance(cSubstance), mOwner(cOwner) {}
	//! �R�s�[�R���X�g���N�^
	TOwnerPointer(const TOwnerPointer& cPointer) :
		mSubstance(cPointer.mSubstance), mOwner(cPointer.cOwner) {}

	//! �f�X�g���N�^
	~TOwnerPointer() { if(mOwner) Destruct()(mSubstance); }

	//! ���L���̕ύX
	void setOwner(Bool cOwner) { mOwner = cOwner; }
	//! �|�C���^�̕ύX
	void setSubstance(pointer cSubstance)
	{
		if(mOwner) delete mSubstance;
		mSubstance = cSubstance;
	}
	//! = ���Z�q
	TOwnerPointer& operator = (pointer cSubstance)
		{ setSubstance(cSubstance); return *this; }
	TOwnerPointer& operator = (const TOwnerPointer& cPointer)
		{ setSubstance(cSubstance); return *this; }

	//! ���̂̎擾
	T* getSubstance() { return mSubstance; }
	const T* getSubstance() const { return mSubstance; }
	// �|�C���^�̎擾
	T* getPointer() { return getSubstance(); }
	const T* getPointer() const { return getSubstance(); }
	//! pointer�ւ̃L���X�g(����ς�_��) 2001/12/25
	//operator pointer () { return getSubstance(); }
	//! reference�ւ̃L���X�g
	// operator reference () { return *getSubstance(); }
	//! *
	T& operator * () { return *getSubstance(); }
	const T& operator * () const { return *getSubstance(); }
	//! ->
	T* operator -> () { return getSubstance(); }
	const T* operator -> () const { return getSubstance(); }
	// void*
	//operator void* () const { return (void*)mSubstance; }
	// bool
	operator bool () const { return mSubstance != _TOP_NULL; }

	//! == ���Z�q
	bool operator == (TOwnerPointer& cOwnerPointer)
		{ return getSubstance() == cOwnerPointer.getSubstance(); }
	//! != ���Z�q
	bool operator != (TOwnerPointer& cOwnerPointer)
		{ return getSubstance() != cOwnerPointer.getSubstance(); }
	//! < ���Z�q
	bool operator < (TOwnerPointer& cOwnerPointer)
		{ return this < &cOwnerPointer; }
	//! > ���Z�q
	bool operator > (TOwnerPointer& cOwnerPointer)
		{ return this > &cOwnerPointer; }
	//! <= ���Z�q
	bool operator <= (TOwnerPointer& cOwnerPointer)
		{ return this <= &cOwnerPointer; }
	//! >= ���Z�q
	bool operator >= (TOwnerPointer& cOwnerPointer)
		{ return this >= &cOwnerPointer; }
};

#endif // ___T_OWNER_POINTER_H___
