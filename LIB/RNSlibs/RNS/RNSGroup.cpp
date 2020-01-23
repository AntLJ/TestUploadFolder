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

/** @file
 *
 * @author	f_kik
 * @date	2002/04/26
 */

#include "RNSException.h"
#include "RNSGroup.h"

/*
 * **************************************************
 *      RNSGroup
 * **************************************************
 */
// ===== public メソッド ===== //
// ----- 初期設定 ----- //
/// デフォルトコンストラクタ
RNSGroup::RNSGroup()
{
	mID = 0; mParent = 0; mNext = 0; mPrev = 0;
}

/**
 * コンストラクタ
 */
RNSGroup::RNSGroup(RNSIDRef iID)
{
	mID = iID; mParent = 0; mNext = 0; mPrev = 0;
}

/**
 * コンストラクタ
 */
RNSGroup::RNSGroup(RNSIDRef iID, RNSGroup* iParent)
{
	mID = iID; mParent = iParent; mNext = 0; mPrev = 0;
}

/**
 * 同世代の、次のグループを設定する
 */
void RNSGroup::setNextGroup(RNSGroup* iGroup)
{
	mNext = iGroup;
}

/**
 * 同世代の、前のグループを設定する
 */
void RNSGroup::setPrevGroup(RNSGroup* iGroup)
{
	mPrev = iGroup;
}

// ----- 取得 ----- //
/// グループ識別番号取得
RNSIDRef RNSGroup::id()
{
	return mID; 
}

/// 親グループ取得
RNSGroup* RNSGroup::parent()
{
	return mParent;
}

/// 同世代の、次のグループを取得
RNSGroup* RNSGroup::nextGroup()
{
	return mNext;
}

/// 同世代の、前のグループを取得
RNSGroup* RNSGroup::prevGroup()
{
	return mPrev;
}

/**
 * 最上位のグループを取得する
 *
 * @return	グループへのポインタ
 */
RNSGroup* RNSGroup::master()
{
	RNSGroup*	pParent = parent();
	return pParent ? pParent->master() : this;
}

/// 最初のメンバを示す反復子を取得する
RNSGroup::least_iterator RNSGroup::first()
{
	using namespace std;
	least_iterator itr;

	(iterator&)itr = begin();

	if(empty()) {
		// 空データの反復子
		itr.setCommunity(this);
		itr.setGroup(this);
		itr.setIsLast(true);
	}
	else {
		// グループが別のグループを格納している場合、格納されているグループの最初のメンバを反復子が示す

		if(isCommunity()) {
			RNSGroup* pGroup = dynamic_cast<RNSGroup*>(itr->second.obj());
			itr = pGroup->first();
		}
		else {
			itr.setGroup(this);
			itr.setIsLast(false);
		}
		itr.setCommunity(this);
	}

	return itr;
}

/// 最後のメンバを示す反復子を取得する
RNSGroup::least_iterator RNSGroup::last()
{
	least_iterator itr;

	(iterator&)itr = end();

	if(empty()) {
		// 空データの反復子
		itr.setCommunity(this);
		itr.setGroup(this);
		itr.setIsLast(true);
	}
	else {
		--itr;
		if(isCommunity()) {
			RNSGroup* pGroup = dynamic_cast<RNSGroup*>(itr->second.obj());
			itr = pGroup->last();
		}
		else {
			itr.setGroup(this);
			itr.setIsLast(false);
		}
		itr.setCommunity(this);
	}

	return itr;
}

// メンバの iterator を探索して取得
RNSGroup::least_iterator RNSGroup::search(RNSIDRef iMemberID)
{
	least_iterator	itr;

	RNSIDRef	pID = extractID(iMemberID);

	if(isCommunity()) {
		if(! pID) {
			itr = last();
		}
		else {
			RNSGroup*	pGroup = group(pID);
			if(pGroup) {
				itr = pGroup->search(iMemberID);
			}
			else {
				itr = last();
			}
		}
	}
	else {
		RNSIDRef	pMemberID = ! pID ? iMemberID : pID;

		(iterator&)itr = find(pMemberID);

		if((iterator&)itr == end()) {
			itr.setGroup(this);
			itr.setIsLast(true);
		}
		else {
			itr.setGroup(this);
			itr.setIsLast(false);
		}
	}

	itr.setCommunity(this);
	return itr;
}

// メンバを取得する
RNSObject*	RNSGroup::member(RNSIDRef iMemberID)
{
	least_iterator itr = search(iMemberID);
	return itr.isLast() ? 0 : itr->second;
}

/**
 * 指定したIDのグループを取得する
 *
 * @return	グループへのポインタ\n
 *			当クラスがコミュニティではない場合、または当コミュニティのIDを所有しない場合は自己へのポインタ\n
 *			該当するグループが存在しなかった場合は NULL
 *
 */
RNSGroup* RNSGroup::group(RNSIDRef iGroupID)
{
	RNSGroup*		pResult = 0;
	
	if(isCommunity()) {
		RNSIDRef	pID = extractID(iGroupID);

		if(! pID) {
			// IDが取得できなかった場合は自己へのポインタ
			pResult = this;
		}
		else {
			// 対応するグループを探す
			// 存在しなかったら NULL
			iterator itr = find(pID);
			pResult = (itr == end() ? 0 : dynamic_cast<RNSGroup*>(itr->second.obj())->group(iGroupID));
		}
	}
	else {
		// コミュニティではなかった場合自己へのポインタ
		pResult = this;
	}

	return pResult;
}

/* ----- 追加 ----- */
/// メンバ追加
bool RNSGroup::add(RNSIDRef iMemberID, RNSObjectPtr iMember)
{
	bool			result = false;

	RNSIDRef	pID = extractID(iMemberID);

	if(isCommunity()) {
		// 現クラスがコミュニティであったら、該当する最小グループを探索（あるいは生成）してそこに登録
		RNSGroup* pGroup = addGroup(pID);
		result = pGroup->add(iMemberID, iMember);
	}
	else {
		// コミュニティでなかったら、このグループに登録
		RNSIDRef	pMemberID = pID ? pID : iMemberID;

		std::pair<iterator, bool>	aPair;
		aPair = insert(RNSObjectMap::value_type(pMemberID, iMember));
		result = aPair.second;
	}

	return result;
}

/**
 * グループ追加
 *
 * @param	iGroupID	追加するグループのグループID\n
 *						グループ内グループも追加可能
 * @return	追加されたグループへのポインタ\n
 *			既に該当するグループが存在していた場合は、その追加せずにそのグループへのポインタを返す。
 */
RNSGroup* RNSGroup::addGroup(RNSIDRef iGroupID)
{
	RNSGroup*		result = 0;

	// コミュニティでなければこのメソッドは無効
	if(isCommunity()) {
		RNSIDRef	pID = extractID(iGroupID);

		// グループIDを取得できたら対応グループを探索
		// 対応グループが見つからなかったら生成、追加
		if(pID) {
			iterator	itr;
			
			if((itr = find(pID)) != end()) {
				RNSGroup* son = dynamic_cast<RNSGroup*>(itr->second.obj());
				result = son->addGroup(iGroupID);
				if(! result) {
					result = son;
				}
			}
			else {
				result = addGroup(iGroupID, newGroup(pID));
			}
		}
	}

	return result;
}

/// グループ追加
RNSGroup* RNSGroup::addGroup(RNSIDRef iGroupID, RNSPtr<RNSGroup> iGroup)
{
	RNSGroup*		result = 0;

	// コミュニティでなければこのメソッドは無効
	if(isCommunity()) {
		RNSIDRef	pID = extractID(iGroupID);

		if(pID) {
			iterator	itr;
			
			if((itr = find(pID)) != end()) {
				RNSGroup* son = dynamic_cast<RNSGroup*>(itr->second.obj());
				result = son->addGroup(iGroupID, iGroup);
				if(! result) {
					result = son;
				}
			}
			else {
				using namespace std;
				pair<iterator, bool>	aPair;
				aPair = RNSObjectMap::insert(RNSObjectMap::value_type(pID, iGroup));
				
				if(aPair.second) {
					iterator aThis;
					iterator aPrev;
					iterator aNext;
					
					result = dynamic_cast<RNSGroup*>((aThis = aPrev = aNext = aPair.first)->second.obj());
					--aPrev;
					++aNext;
					
					RNSGroup*	aThisGroup = dynamic_cast<RNSGroup*>(aThis->second.obj());
					
					if(aThis != begin()) {
						RNSGroup*	aPrevGroup = dynamic_cast<RNSGroup*>(aPrev->second.obj());
						aPrevGroup->setNextGroup(aThisGroup);
						aThisGroup->setPrevGroup(aPrevGroup);
					}
					else {
						aThisGroup->setPrevGroup(0);
					}
					
					if(aNext != end()) {
						RNSGroup*	aNextGroup = dynamic_cast<RNSGroup*>(aNext->second.obj());
						aNextGroup->setPrevGroup(aThisGroup);
						aThisGroup->setNextGroup(aNextGroup);
					}
					else {
						aThisGroup->setNextGroup(0);
					}
				}
			}
		}
	}

	return result;
}

// ----- 削除 ----- //
/// 指定したメンバを削除する
void RNSGroup::remove(RNSIDRef iMemberID)
{
	RNSIDRef	pID = extractID(iMemberID);
	RNSGroup*		pGroup;

	if(pID) {
		pGroup = group(iMemberID);
	}
	else {
		pGroup = this;
	}

//	RNSReleaseProtect(iMemberID);
	pGroup->erase(iMemberID);
}

/// 指定したグループを削除する
void RNSGroup::removeGroup(RNSIDRef iGroupID)
{
	// 指定されたグループを探す
	RNSGroup*	pGroup = group(iGroupID);

	if(pGroup) {
		// グループを発見したら、その親グループを探す
		RNSGroup* pParent = pGroup->parent();

		// 親グループが存在しなかったら例外発生
		if(! pParent)
			throw RNSException("cant remove group");

		// 親グループに削除依頼を申請する
		RNSIDRef	pID = pParent->extractID(iGroupID);

		if(! pID)
			throw RNSException("cant remove group");

		pParent->erase(pID);
	}
}

// ----- 仮想関数 ----- //
/// メンバIDから、このグループでの識別IDを取得する
RNSIDRef RNSGroup::extractID(RNSIDRef iMemberID)
{
	return 0;
}

/// 新規グループ生成
RNSPtr<RNSGroup> RNSGroup::newGroup(RNSIDRef iGroupID)
{
	return new RNSGroup(iGroupID, this);
}

// ===== public メソッド ===== //
/// デフォルトコンストラクタ/// コミュニティか？
bool RNSGroup::isCommunity() const
{
	return false;
}

/*
 * **************************************************
 *      RNSGroup::least_iterator
 * **************************************************
 */


RNSGroup::least_iterator::least_iterator()
{
	mCommunity = 0;
	mGroup = 0;
	mIsLast = true;
}

// ----- 取得 ----- //
/**
 * コミュニティ（反復子がアクセスする全てのメンバが格納されたグループ）を取得する
 *
 * @return	コミュニティ
 */
RNSGroup* RNSGroup::least_iterator::community()
{
	return mCommunity;
}

/**
 * 反復子が現在アクセスしているメンバが属するグループを取得する
 *
 * @param	iGroup	グループ
 */
RNSGroup* RNSGroup::least_iterator::group()
{
	return mGroup;
}

/**
 * 反復子がコミュニティの最後のメンバにアクセスし終わったかを取得する
 *
 * @retval	true	アクセス終了
 * @retval	false	アクセス未終了
 */
bool RNSGroup::least_iterator::isLast() const
{
	return mIsLast;
}

// ----- オペレータ ----- //
/**
 * 次のメンバへ移動
 */
RNSGroup::least_iterator& RNSGroup::least_iterator::operator ++ ()
{
	using namespace std;

	// 継承元の反復子の要領で次のメンバを取得
	++((iterator&)*this);

	if((iterator&)(*this) == group()->end()){
		setIsLast(true);

		// 現在アクセスしているグループの最後のデータに到達したら、次のグループへ移動する
		if(group() == community()) {
			// 現在アクセスしているグループがコミュニティそのものだったら、最後まで到達した
			least_iterator tmpItr = community()->last();
			tmpItr.setCommunity(community());
			tmpItr.setIsLast(true);
			*this = tmpItr;
			return *this;
		}

		// 次のグループが存在していたらそちらへ反復子を移動、なければ終了
		RNSGroup* pNextGroup = group()->nextGroup();
		if(! pNextGroup) {
			RNSGroup* pParentGroup = group()->parent();
			if(pParentGroup && pParentGroup != community()) {
				pNextGroup = pParentGroup->nextGroup();
			}
		}

		if(pNextGroup) {
			least_iterator tmpItr = pNextGroup->first();
			tmpItr.setCommunity(community());
			*this = tmpItr;
		}
	}
	else {
		// まだ最後のデータではなかったら、そのまま続行
		mIsLast = false;
	}

	return *this;
}

// ===== private メソッド ===== //
// ----- 初期設定 ----- //
/**
 * コンストラクタ（コミュニティ、グループ指定）
 *
 * @param	iCommunity	コミュニティ
 * @param	iGroup		反復子が現在アクセスしているメンバが属するグループ
 */
RNSGroup::least_iterator::least_iterator(RNSGroup* iCommunity, RNSGroup* iGroup)
{
	mCommunity = iCommunity;
	mGroup = iGroup;
	mIsLast = false;
}

// ----- 設定 ----- //
/**
 * コミュニティを設定する
 *
 * @param	iCommunity	コミュニティ
 */
void RNSGroup::least_iterator::setCommunity(RNSGroup* iCommunity)
{
	mCommunity = iCommunity;
}

/**
 * 反復子が現在アクセスしているメンバが属するグループを設定する
 *
 * @param	iGroup	グループ
 */
void RNSGroup::least_iterator::setGroup(RNSGroup* iGroup)
{
	mGroup = iGroup;
}

/**
 * 反復子がコミュニティの最後のメンバにアクセスし終わったかを設定する
 *
 * @param	iIsLast	アクセスし終わったか？
 */
void RNSGroup::least_iterator::setIsLast(bool iIsLast)
{
	mIsLast = iIsLast;
}

/*
 * **************************************************
 *      RNSCommunity
 * **************************************************
 */

/// コミュニティか？
bool RNSCommunity::isCommunity() const
{
	return true;
}
