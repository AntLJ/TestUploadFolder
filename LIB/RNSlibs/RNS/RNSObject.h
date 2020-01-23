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

#ifndef __RNS_OBJECT__
#define __RNS_OBJECT__

/* ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆

	RNS基礎クラスライブラリ 
 	
 	初稿 : 
 	改訂 : 
 
 	制作 :	Fukuta.K

☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆ */

/* ------------------------------------------------------------
	クラス定義
------------------------------------------------------------ */

/**
 * 参照カウンタを搭載した、全てのRNSクラスシリーズの基底クラス
 *
 * オブジェクトが参照されている個所数を記憶し、参照が全て解かれた時点で自らを delete する。
 *
 * - new により生成されたオブジェクトの参照カウンタは 0 を示している
 * - retain メソッドを実行することで、参照カウンタは１増える
 * - release メソッドを実行することで、参照カウンタは１減る
 * - 参照カウンタが１以下のときに release メソッドを実行すると、オブジェクトは delete される
 *
 * これらの機能を活用するため、RNSクラスには次のようなルールが存在する
 *
 * - new で生成されたRNSオブジェクトを「オーナーシップルールを持つオブジェクト」と呼ぶ
 * - 自動変数として生成されたオブジェクトを「オーナーシップルールを持たないオブジェクト」と呼ぶ
 * - オーナーシップとは「そのオブジェクトを保持する権利」である\n
 *	 それは言い換えれば「オブジェクトを動的にdeleteできる」ことを示す
 * - オーナーシップを持つオブジェクトは、RNSPtrに格納することでガベージコレクトが成立する
 * - オーナーシップを持たないオブジェクトは、RNSPtrに格納してはならない\n
 *   これは「自動変数にdeleteをかけることになる」からである
 * - オーナーシップを持つオブジェクトへのポインタを、そのオブジェクトのオーナーと呼ぶ
 */

class RNSObject {
	mutable int __retain_counter__;	///< 参照カウンタ

 public:
	/* ----- 初期化 ----- */
	/// デフォルトコンストラクタ
	RNSObject()
	{
		__retain_counter__ = 0;
	}

	/// デストラクタ
	virtual ~RNSObject()
	{
	}

	/* ----- 処理 ----- */
	/// 参照カウンタを１増やす
	void retain() const
	{
		++__retain_counter__;
	}

	/// 参照カウンタを１減らし、参照されなくなったら解放する
	void release() const
	{
		if( --__retain_counter__ <= 0) {
			delete this;
		}
	}

	/// 参照カウンタを１減らす
	void dummyRelease() const {	--__retain_counter__; }

	/// 参照カウントを取得する
	int retainCount() const
	{
		return __retain_counter__;
	}
};

/**
 * RNSクラスオブジェクトポインタ保持クラス
 *
 * RNSクラスオブジェクトの参照カウンタの操作を自動で行う
 *
 * - RNSクラスオブジェクトのポインタを受けると、そのオブジェクトを retain して所持する
 * - オブジェクトを所持しているときに、別のポインタを受けると<BR>
 *   現在の オブジェクト を release し、新しく受けた オブジェクト を retain する
 * - RNSPtr が破棄されるとき、内部の オブジェクト は release される
 */
template<typename T>
class _RNSPtr {
public:
	// ----- 初期化 ----- //
	/// コンストラクタ
	_RNSPtr()
	{
		mObj = 0;
	}

	/// コンストラクタ
	_RNSPtr(T* iObj)
	{
		mObj = 0;
		setObj(iObj);
	}

	/// コンストラクタ
	_RNSPtr(const _RNSPtr<T>& iPtr)
	{
		mObj = 0;
		setObj(const_cast<T*>(iPtr.obj()));
	}

	/// デストラクタ
	~_RNSPtr()
	{
		release();
	}

	// ----- 設定 ----- //
	/// オブジェクトを設定する
	void setObj(T* iObj)
	{
		release();
		mObj = iObj;
		retain();
	}

	// ----- 取得 ----- //
	/// オブジェクトを取得
	T* obj() const					{ return mObj; }

	// ----- オペレータ ----- //

	/// 参照取得
	T& operator * ()				{ return *mObj; }
	/// 参照取得（const）
	const T& operator * () const	{ return *mObj;	}

	/// ポインタ取得
	operator T* ()					{ return mObj; }
	/// ポインタ取得（const）
	operator const T* () const		{ return mObj; }

	/// 矢印
	T* operator -> ()				{ return &(operator*()); }
	/// 矢印（const）
	const T* operator -> () const	{ return &(operator*()); }

	/// 比較演算子（等号）
	bool operator == (const T* iTgt) const	{ return mObj == iTgt; }
	/// 比較演算子（不等号）
	bool operator != (const T* iTgt) const	{ return !(mObj == iTgt); }

protected:
	/// 保持する
	void retain() const {
		if( mObj ) {
			mObj->retain();
		}
	}

	/// 解放する
	void release() {
		if( mObj ) {
			const T* tmpObj = mObj;
			mObj = 0;
			tmpObj->release();
		}
	}

private:
	T* mObj;
};

template<typename T>
class RNSPtr : public _RNSPtr<T> {
public:
	/* --- コンストラクタ・デストラクタ --- */
	/// デフォルトコンストラクタ
	RNSPtr() {}

	/// コピーコンストラクタ
	RNSPtr(RNSPtr<T>& iPtr) : _RNSPtr<T>(iPtr) {}

	/// コンストラクタ（オブジェクト代入）
	RNSPtr(T* iObj) : _RNSPtr<T>(iObj) {}

	template<typename T2> 
	RNSPtr(const RNSPtr<T2>& iPtr) : _RNSPtr<T>(iPtr.obj()) {}

	/// ポインタ代入
	RNSPtr<T>& operator = (T* iPtr)
	{
		setObj(iPtr);
		return *this;
	}

	RNSPtr<T>& operator = (const RNSPtr<T>& iPtr) 
	{
		setObj(iPtr.obj());
		return *this;
	}

	bool operator < (const RNSPtr<T>& iPtr) const	{ return obj() < iPtr.obj(); }
};

#define	_RNSPTR_TYPEDEF(Type)	\
	typedef	RNSPtr<Type>		\
		Type ## Ptr;

_RNSPTR_TYPEDEF(RNSObject);

/**
 * RNSクラスオブジェクト参照クラス
 *
 * RNSPtrと同様に、動的なRNSObjectのポインタを保持する。\n
 * 相違点は、RNSPtrの比較演算がポインタが示すアドレスの比較であるのに対し、RNSRefは実体の比較を行う。\n
 * setやmapに格納する場合はこちらの方が便利。
 */
template <typename T>
class RNSRef : public _RNSPtr<T> {
public:
	/* --- コンストラクタ・デストラクタ --- */
	/// デフォルトコンストラクタ
	RNSRef() {}

	/// コピーコンストラクタ
	RNSRef(RNSRef<T>& iRef) : _RNSPtr<T>(iRef) {}

	/// コンストラクタ（オブジェクト代入）
	RNSRef(T* iObj) : _RNSPtr<T>(iObj) {}

	template<typename T2> 
	RNSRef(const RNSRef<T2>& iRef) : _RNSPtr<T>(iRef.obj()) {}

	RNSRef<T>& operator = (T* iRef)
	{
		setObj(iRef);
		return *this;
	}

	RNSRef<T>& operator = (RNSRef<T>& iRef) 
	{
		setObj(iRef.obj());
		return *this;
	}
	
	template <typename T2>
	RNSRef<T>& operator = (const RNSRef<T2>& iRef)
	{
		setObj(iRef.obj());
		return *this;
	}

	// --- オペレータ --- //
	bool operator < (const T& iRef) const { return **this < *iRef; }
	bool operator > (const T& iRef) const { return **this > *iRef; }
	bool operator == (const T& iRef) const { return **this == *iRef; }
	bool operator != (const T& iRef) const { return **this != *iRef; }

	bool operator < (const _RNSPtr<T>& iRef) const { return **this < *iRef; }
	bool operator > (const _RNSPtr<T>& iRef) const { return **this > *iRef; }
	bool operator == (const _RNSPtr<T>& iRef) const { return **this == *iRef; }
	bool operator != (const _RNSPtr<T>& iRef) const { return **this != *iRef; }
};

#define _RNSREF_TYPEDEF(Type)	\
	typedef RNSRef<Type>		\
		Type ## Ref;			

#define _RNSOBJECT_TYPEDEF(Type)	\
	_RNSPTR_TYPEDEF(Type);	\
	_RNSREF_TYPEDEF(Type)

_RNSOBJECT_TYPEDEF(RNSObject);

/**
 * RNSクラスオブジェクト（自動変数）保護クラス
 *
 * 	自動変数を勝手にreleaseされてしまいそうなときはこれにしまっておく<BR>
 *	代入時にretain、破棄時にdummyRelease（参照カウンタが0を指してもdeleteしないrelease）を行う
 */

template<typename T>
class _RNSRetainPtr {
	mutable const T* mObj;
	
 public:
	/* --- コンストラクタ・デストラクタ --- */
	/// コンストラクタ
	_RNSRetainPtr(const T* iObj)
		{ mObj = iObj; mObj->retain(); }
	/// デストラクタ
	~_RNSRetainPtr()
		{ mObj->dummyRelease(); }
};

typedef _RNSRetainPtr<RNSObject>		RNSRetainPtr;

#define RNSReleaseProtect(e)	RNSRetainPtr __protecter__(&e)

#endif /* __RNS_OBJECT__ */
