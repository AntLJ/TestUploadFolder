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
** TOwnerPointer.h 所有権を持ったオートポインタ
**   Programmed by Takashi Togawa
**   EDITION: 2001/10/16 作成
**
*/

#ifndef ___T_OWNER_POINTER_H___
#define ___T_OWNER_POINTER_H___

#define _TOP_NULL 0
#define _TOP_TRUE true
#define _TOP_FALSE false

/*
** インクルードファイル
*/

#include <functional>

/*!
  delete の抽象化
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
  所有権付きポインタ
  =演算子使用時に所有権は一切変更しないので、
  使用時に明示的に変更する必要がある。
*/

template <class T, class Bool = bool, class Destruct = _top_delete<T> >
class TOwnerPointer
{
	T* mSubstance; //!< 実体ポインタ
	Bool mOwner;   //!< 所有権

public:
	typedef T* pointer;
	typedef T& reference;

	//! デフォルトコンストラクタ
	TOwnerPointer() : mSubstance(_TOP_NULL), mOwner(_TOP_FALSE) {}
	//! 実体指定コンストラクタ
	TOwnerPointer(pointer cSubstance) :
		mSubstance(cSubstance), mOwner(_TOP_TRUE) {}
	//! 実体＆フラグ指定コンストラクタ
	TOwnerPointer(pointer cSunstance, Bool cOwner) :
		mSubstance(cSubstance), mOwner(cOwner) {}
	//! コピーコンストラクタ
	TOwnerPointer(const TOwnerPointer& cPointer) :
		mSubstance(cPointer.mSubstance), mOwner(cPointer.cOwner) {}

	//! デストラクタ
	~TOwnerPointer() { if(mOwner) Destruct()(mSubstance); }

	//! 所有権の変更
	void setOwner(Bool cOwner) { mOwner = cOwner; }
	//! ポインタの変更
	void setSubstance(pointer cSubstance)
	{
		if(mOwner) delete mSubstance;
		mSubstance = cSubstance;
	}
	//! = 演算子
	TOwnerPointer& operator = (pointer cSubstance)
		{ setSubstance(cSubstance); return *this; }
	TOwnerPointer& operator = (const TOwnerPointer& cPointer)
		{ setSubstance(cSubstance); return *this; }

	//! 実体の取得
	T* getSubstance() { return mSubstance; }
	const T* getSubstance() const { return mSubstance; }
	// ポインタの取得
	T* getPointer() { return getSubstance(); }
	const T* getPointer() const { return getSubstance(); }
	//! pointerへのキャスト(やっぱりダメ) 2001/12/25
	//operator pointer () { return getSubstance(); }
	//! referenceへのキャスト
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

	//! == 演算子
	bool operator == (TOwnerPointer& cOwnerPointer)
		{ return getSubstance() == cOwnerPointer.getSubstance(); }
	//! != 演算子
	bool operator != (TOwnerPointer& cOwnerPointer)
		{ return getSubstance() != cOwnerPointer.getSubstance(); }
	//! < 演算子
	bool operator < (TOwnerPointer& cOwnerPointer)
		{ return this < &cOwnerPointer; }
	//! > 演算子
	bool operator > (TOwnerPointer& cOwnerPointer)
		{ return this > &cOwnerPointer; }
	//! <= 演算子
	bool operator <= (TOwnerPointer& cOwnerPointer)
		{ return this <= &cOwnerPointer; }
	//! >= 演算子
	bool operator >= (TOwnerPointer& cOwnerPointer)
		{ return this >= &cOwnerPointer; }
};

#endif // ___T_OWNER_POINTER_H___
