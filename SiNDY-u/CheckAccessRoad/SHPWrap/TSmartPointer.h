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
** TSmartPointer.h 参照カウントを持ったポインタ
**   Programmed by Takashi Togawa
**   EDITION: 2001/10/12 作成
**            2001/10/15 deleteの抽象化
**            2002/02/06 setNULLの追加
**
*/

#ifndef ___T_SMART_POINTER_H___
#define ___T_SMART_POINTER_H___

#define _TSP_NULL 0
#define _TSP_ZERO (Count)0

/*
** インクルードファイル
*/

#include <functional>

/*!
  deleteの抽象化
*/

template <class T>
struct _tsp_delete : std::unary_function<T, void>
{
	void operator()(T* cPointer) const
		{ delete cPointer; }
};

template <class T>
struct _tsp_delete_n : std::unary_function<T, void>
{
	void operator()(T* cPointer) const
		{ delete [] cPointer; }
};

/*!
  スマートポインタ
  delete は抽象化してあるけど、++Count --Countは抽象化していない。
*/

template <class T, class Count = int, class Destruct = _tsp_delete<T> >
class TSmartPointer
{
public:
	// T* T& の typedef
	typedef T* pointer;
	typedef T& reference;

	//! スマートポインタのアイテムボックス
	class tsp_box
	{
		Count mCount;       //! カウンタ
		pointer mSubstance; //! 実体へのポインタ
	public:
		//! デフォルトコンストラクタ
		tsp_box() : mCount(_TSP_ZERO), mSubstance(_TSP_NULL) {}
		//! 実体指定コンストラクタ
		/*!
		  @param cSubstance [in] 実体へのポインタ
		*/
		tsp_box(pointer cSubstance) : mCount(_TSP_ZERO), mSubstance(cSubstance) {}

		//! デストラクタ
		~tsp_box() { Destruct()(mSubstance); }

		//! ポインタをNULLに指定
		void setNULL() { mSubstance = _TSP_NULL; }
		//! カウンタ１加算
		void increment() { ++mCount; }
		//! カウンタ１減算
		void decrement()
		{
			if(mCount > _TSP_ZERO)
			{
				--mCount; //カウンタ減算
				if(mCount <= _TSP_ZERO)
				{
					Destruct()(mSubstance); // 実体消去
					mSubstance = _TSP_NULL;
				}
			}
		}
		//! ポインタの取得
		pointer getSubstance() { return mSubstance; }
		//! カウンタの取得
		Count getCount() const { return mCount; }
		//! カウンタは０か？
		bool isZero() const { return mCount == _TSP_ZERO; }
		//! voidポインタの取得
		void* getPointer() const { return (void*)mSubstance; }
	};
	typedef tsp_box box;

private:
	box* mBox; //! アイテムボックスへのポインタ

public:
	//! デフォルトコンストラクタ
	TSmartPointer() : mBox(_TSP_NULL) {}
	//! 実体指定コンストラクタ
	/*!
	  @param cSubstance [in] 実体へのポインタ
	*/
	TSmartPointer(pointer cSubstance) : mBox(_TSP_NULL)
	{
		if(cSubstance)
		{
			mBox = new box(cSubstance);
			if(mBox) mBox->increment();
		}
	}
	//! アイテム指定コンストラクタ
	/*!
	  @param cBox [in] アイテムへのポインタ
	*/
	TSmartPointer(box* cBox) : mBox(cBox)
		{ if(mBox) mBox->increment(); }
	//! コピーコンストラクタ
	/*!
	  @param cSmartPointer [in] コピーするスマートポインタ
	*/
	TSmartPointer(const TSmartPointer& cSmartPointer) :
		mBox(cSmartPointer.mBox)
		{ if(mBox) mBox->increment(); }
	//! デストラクタ
	~TSmartPointer()
	{
		if(mBox)
		{
			mBox->decrement();
			if(mBox->isZero()) _tsp_delete<box>()(mBox);
		}
	}

	//! ポインタをNULLにする
	void setNULL()
	{
		if(mBox)
		{
			mBox->setNULL();
			delete mBox;
			mBox = _TSP_NULL;
		}
	}

	//! = (実体ポインタの代入)
	/*!
	  @param cSubstance [in] 実体へのポインタ
	  @return 自分の参照
	*/
	TSmartPointer& operator = (pointer cSubstance)
	{
		if(mBox)
		{
			mBox->decrement();
			if(mBox->isZero()) _tsp_delete<box>()(mBox);
		}
		if(cSubstance != _TSP_NULL)
		{
			mBox = new box(cSubstance);
			if(mBox) mBox->increment();
		}
		else
		{
			mBox = NULL;
		}
		return *this;
	}
	//! = (スマートポインタの代入)
	/*!
	  @param cSmartPointer [in] コピーするスマートポインタ
	  @return 自分の参照
	*/
	TSmartPointer& operator = (const TSmartPointer& cSmartPointer)
	{
		if(mBox)
		{
			mBox->decrement();
			if(mBox->isZero()) _tsp_delete<box>()(mBox);
		}
		mBox = cSmartPointer.mBox;
		if(mBox) mBox->increment();
		return *this;
	}

	//! ポインタの取得
	T* getPointer()
	{
		return mBox ? mBox->getSubstance() : (T*)_TSP_NULL;
	}
	const T* getConstPointer() const
	{
		return mBox ? mBox->getSubstance() : (T*)_TSP_NULL;
	}
	// 2001/12/25 コメント化 
	// ポインタの取得(キャストは作らない方がいいらしい)
	//operator pointer ()
	//{
	//	if(mBox) return mBox->getSubstance();
	//	return (pointer)_TSP_NULL;
	//}
	//! 参照の取得
	// operator reference () { return *(pointer)*this; }
	//! *演算子
	T& operator * () { return *getPointer(); }
	const T& operator * () const { return *getConstPointer(); }
	//! ->演算子
	T* operator -> () { return getPointer(); }
	const T* operator -> () const { return getConstPointer(); }

	//! 参照カウンタの取得
	Count getCount() const
	{
		if(mBox) return mBox->getCount();
		return _TSP_ZERO;
	}
	//! 参照アイテムの取得
	box* getBox()
	{
		if(mBox) return mBox;
		return _TSP_NULL;
	}
	//! スワップ
	void swap(TSmartPointer& cSmartPointer)
	{
		if(mBox != cSmartPointer.mBox)
		{
			box* aTemp = mBox;
			mBox = cSmartPointer.mBox;
			cSmartPointer.mBox = aTemp;
		}
	}

	//! void*へのキャスト
	//operator void* () const
	//{
	//	if(mBox) return mBox->getPointer();
	//	return _TSP_NULL;
	//}
	//! boolへのキャスト
	operator bool () const
	{
		return ((mBox ? mBox->getPointer() : NULL) != NULL);
	}

	//! == 演算子
	bool operator == (TSmartPointer& cSmartPointer)
		{ return getPointer() == cSmartPointer.getPointer(); }
	//! != 演算子
	bool operator != (TSmartPointer& cSmartPointer)
		{ return getPointer() != cSmartPointer.getPointer(); }
	//! < 演算子
	bool operator < (TSmartPointer& cSmartPointer)
		{ return this < &cSmartPointer; }
	//! > 演算子
	bool operator > (TSmartPointer& cSmartPointer)
		{ return this > &cSmartPointer; }
	//! <= 演算子
	bool operator <= (TSmartPointer& cSmartPointer)
		{ return this <= &cSmartPointer; }
	//! >= 演算子
	bool operator >= (TSmartPointer& cSmartPointer)
		{ return this >= &cSmartPointer; }
};

#endif // ___T_SMART_POINTER_H___
