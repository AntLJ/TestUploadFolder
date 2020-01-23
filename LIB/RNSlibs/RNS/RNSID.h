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

#ifndef __RNS_ID__
#define __RNS_ID__

/* ************************************************

 	初稿 :
 	改訂 : 
 
 	制作 :	Fukuta.K
 	
************************************************* */

#include "RNS.h"

#include <functional>

#include "RNSObject.h"

// ///// ポインタセット・マップ ///// //

/**
 * @brief	ポインタ比較クラス
 *
 * ポインタの内容を比較するクラス。
 */
template <typename T>
struct _RNSLess : std::binary_function<T, T, bool>
{
	bool operator()(const T p1, const T p2) const
	{
		return *T < *T;
	}
};

#define pointer_set(T)		std::set< T, _RNSLess<T> >		///< ポインタの内容でソートするstd::set
#define pointer_map(T, T2)	std::map< T, T2, _RNSLess<T> >	///< ポインタの内容でソートするstd::map


/* ///// クラス宣言 ///// */

/**
 * @brief	ID 基底クラス
 *
 * オブジェクトにIDを持たせる時は、このクラスから派生したクラスを用いる。
 */
class RNSID : public RNSObject {
 public:
	/// 大小比較を行う
	virtual bool operator < (const RNSID& iID) const = 0;
};

_RNSOBJECT_TYPEDEF(RNSID);

#if(0)
/**
 * RNSID 型オブジェクトの保持ポインタの２項述語オブジェクト
 *
 * RNSID 型オブジェクトを格納したポインタ同士で大小比較をおこなう
 */
struct RNSIDPtrLess : std::binary_function<RNSIDConstPtr, RNSIDConstPtr, bool>
{
	bool operator()(const RNSIDConstPtr& p1, const RNSIDConstPtr& p2) const
	{
		return *p1 < *p2;
	}
};
#endif

/**
 * 単純型指定
 */
template<typename _Type>
class _RNSID : virtual public RNSID {
	/// ID（指定型の値）
	_Type	mID;

	/// 指定型のIDを設定する
 	void setID(_Type iID)
	{
		mID = iID;
	}

 public:
	/// コンストラクタ（ID設定）
	_RNSID(_Type iID) 	{ setID(iID); }

	/// デストラクタ
	virtual ~_RNSID(){}

	/* ----- 取得 ----- */
	/// 指定型のIDを返す
	_Type id() const
	{
		_Type tmpid = mID;
		return tmpid;
	}

	/* ----- オペレータ ----- */
	/// 指定型のIDを返す
	operator _Type () const
	{
		return mID;
	}

	/// 大小比較を行う
	virtual bool operator < (const RNSID& iID) const
	{
		const _RNSID* pID = dynamic_cast<const _RNSID*>(&iID);
		return pID ? (*this < *pID) : 0;
	}

	bool operator < (const _RNSID& iID) const
	{
		return id() < iID.id();
	}
};

typedef _RNSID<int>	RNSIntID;
typedef _RNSID<MeshCode>	RNSMeshCodeID;

_RNSOBJECT_TYPEDEF(RNSIntID);

#endif /* __RNS_ID__ */
