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

#ifndef __RNS_GROUP__
#define __RNS_GROUP__

#include "RNS.h"
#include "RNSObject.h"

#include <map>

#include "RNSID.h"

typedef std::map<RNSIDRef, RNSObjectPtr>	RNSObjectMap;
typedef RNSObjectMap::iterator				RNSObjectMapItr;
typedef RNSObjectMap::const_iterator		RNSObjectMapConstItr;

/**
 * 拡張std::mapクラス
 *
 * - グループはmapと同様に値（メンバという）を格納する。\n
 * - グループは、格納している全てのメンバをシーケンシャルに取得できる。\n
 * - グループは、以下のメソッドをオーバーライドすることでグループを格納できるようになる。
 *		-# isCommunity ... true を返すようにオーバーライドする。
 *		-# extractID ... メンバIDから、現グループでの分類IDを取得する。
 *		-# newGroup ... 格納するグループの作り方。（これは必須ではない）
 */
class RNSGroup : public RNSObjectMap,
				 virtual public RNSObject
{
	RNSIDRef	mID;		///< グループID
	RNSGroup*	mParent;	///< 親グループ

	RNSGroup*	mNext;		///< 次のグループ
	RNSGroup*	mPrev;		///< 前のグループ

 public:
	/* ----- least_iterator の定義 ----- */
	/**
	 * グループ内にある全てのメンバをシーケンシャルに取得する反復子
	 */
	 class least_iterator : public RNSObjectMap::iterator {
		friend RNSGroup;

		RNSGroup*	mCommunity;	///< 反復子を呼び出したグループ
		RNSGroup*	mGroup;		///< 現在反復子が読み込んでいるグループ
		bool		mIsLast;	///< 反復子が最後のメンバに達したか

		// ----- 初期設定 ----- //
		/// コンストラクタ（コミュニティやグループを設定）
		least_iterator(RNSGroup* iCommunity, RNSGroup* iGroup);

		// ----- 設定 ----- //
		void setCommunity(RNSGroup* iCommunity);
		void setGroup(RNSGroup* iGroup);
		void setIsLast(bool iIsLast);

	 public:
		// ----- 初期設定 ----- //
		 /// デフォルトコンストラクタ
		least_iterator();

		/// デストラクタ
		~least_iterator(){}

		// ----- 取得 ----- //
		/// 反復子がアクセスする全てのメンバが格納されているグループを取得
		RNSGroup*	community();

		/// 現在反復子がアクセスしているメンバが属するグループを取得
		RNSGroup*	group();

		/// 反復子がコミュニティの最後のメンバに到達した
		bool	isLast() const;

		// ----- オペレータ ----- //
		/// 次のメンバへ
		least_iterator& operator ++ ();
	};

	/* ----- 初期設定 ----- */
	/// デフォルトコンストラクタ
	RNSGroup();

	/// コンストラクタ（グループID設定）
	RNSGroup(RNSIDRef iGroupID);

	/// コンストラクタ（グループID、親グループ設定）
	RNSGroup(RNSIDRef iGroupID, RNSGroup* iParent);

	/// デストラクタ
	virtual ~RNSGroup(){}

	/// 同世代の、次のグループを設定する
	void	setNextGroup(RNSGroup* iGroup);

	/// 同世代の、前のグループを設定する
	void	setPrevGroup(RNSGroup* iGroup);

	/* ----- 追加 ----- */
	/// メンバを追加する
	bool add(RNSIDRef iMemberID, RNSObjectPtr iProf);

	/// グループを追加して、ポインタを取得する
	RNSGroup* addGroup(RNSIDRef iGroupID);

	/// グループを追加して、ポインタを取得する
	RNSGroup* addGroup(RNSIDRef iGroupID, RNSPtr<RNSGroup> iGroup);

	/* ----- 削除 ----- */
	/// メンバを削除する
	void remove(RNSIDRef iMemberID);

	/// グループを削除する
	void removeGroup(RNSIDRef iGroupID);

	/* ----- 取得 ----- */
	/// グループ識別番号取得する
	RNSIDRef		id();

	/// 同世代の、次のグループを取得する
	RNSGroup*		nextGroup();

	/// 同世代の、前のグループを取得する
	RNSGroup*		prevGroup();

	/// 親グループ取得する
	RNSGroup*		parent();
	
	/// 最上位のグループを取得する
	RNSGroup*	master();

	/// 最初のメンバを示す least_iterator を取得する
	least_iterator	first();

	/// 最後のメンバを示す iterator を取得する
	least_iterator	last();

	/// メンバの反復子を取得する
	least_iterator	search(RNSIDRef iMemberID);

	/// メンバを取得する
	RNSObject*	member(RNSIDRef iMemberID);

	/// グループを取得する
	RNSGroup*	group(RNSIDRef iGroupID);

	// ----- 仮想関数 ----- //
	/**
	 * メンバIDから、このグループでの識別IDを取得する
	 *
	 * @param	iMemeberID	メンバID
	 * @return	このグループ内での識別IDへの動的ポインタ\n
	 *          このグループにメンバが属する場合、０が返る\n
	 *			オーナーシップは譲渡される
	 */
	virtual RNSIDRef extractID(RNSIDRef iMemberID);

	/**
	 * グループIDから、新しいグループを生成する
	 *
	 * @param	iGroupID	グループID
	 * @return	このグループ内での識別IDへの動的ポインタ\n
	 *          このグループにメンバが属する場合、０が返る\n
	 *			オーナーシップは譲渡される
	 */
	virtual RNSPtr<RNSGroup> newGroup(RNSIDRef iGroupID);

	/**
	 * コミュニティ（直下にメンバが存在しないグループ）か？
	 *
	 * @retval	true	コミュニティである
	 * @retval	false	コミュニティではない
	 */
	virtual bool isCommunity() const;
};

_RNSOBJECT_TYPEDEF(RNSGroup);

template<typename _BaseClass, typename _Member>
class _least_iterator : public _BaseClass::least_iterator {
public:
	_least_iterator(RNSGroup::least_iterator& iSrc)
	{ *(RNSGroup::least_iterator*)this = iSrc; }

	_Member* member()
	{ return isLast() ? 0 : dynamic_cast<_Member*>((*this)->second.obj()); }
};

/**
 * RNSGroup格納クラス
 * コミュニティはグループを複数格納できる。\n
 * コミュニティは、格納している全てのグループのメンバをシーケンシャルに取得できる。\n
 * RNSGroupのisCommunityをオーバーライドしただけのクラス
 */
class RNSCommunity: public RNSGroup {
public:
	/* ----- 初期設定 ----- */
	/// デフォルトコンストラクタ
	RNSCommunity()
		: RNSGroup(){}

	/// コンストラクタ（グループID設定）
	RNSCommunity(RNSIDRef iGroupID)
		: RNSGroup(iGroupID){}

	/// コンストラクタ（グループID、親グループ設定）
	RNSCommunity(RNSIDRef iGroupID, RNSGroup* iParent)
		: RNSGroup(iGroupID, iParent){}

	// ----- 仮想関数のオーバーライド ----- //
	/// コミュニティか？
	virtual bool isCommunity() const;
};

_RNSOBJECT_TYPEDEF(RNSCommunity);

#endif /* __RNS_MAP_GROUP__ */
