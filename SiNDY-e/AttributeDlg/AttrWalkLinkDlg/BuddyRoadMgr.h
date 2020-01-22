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

#pragma once


/**
* @brief 紐付く道路リンク管理用クラス（bug 11283）
*/
class CBuddyRoad : private boost::operators<CBuddyRoad>
{
	CBuddyRoad(){}
public:
	CBuddyRoad( const IFeaturePtr& roadLink, const _IRowPtr& linkRel );
	~CBuddyRoad();

	long m_objectID;		//!< LINK_RELATIONテーブル自体のオブジェクトID
	long m_roadID;          //!< RoadLinkのOID
	IGeometryPtr m_geom;    //!< RoadLinkの形状
	IFeaturePtr m_roadLink; //!< RoadLink
	_IRowPtr m_linkRel;     //!< LINK_RELATIONのレコード
	bool m_addF;            //!< 追加フラグ
	bool m_deleteF;         //!< 削除フラグ


	/**
	* @brief 変更チェック
	* @note 追加または削除のどちらか一方だけが立っていたら変更有りと見なす
	*       両立は許可（実際は追加も削除もしないで無視）
	* @return 値が変更されていればtrue
	*/
	bool IsChanged() const { return (m_addF ^ m_deleteF); }

	/// LINK_RELATIONテーブル自体のオブジェクトIDと道路リンクIDで判定
	/// オブジェクトIDだけだと、新規追加時に同じと判定され、
	/// 道路リンクIDだけだと、重複して入っている場合に同じと判定されるため
	bool operator==(const CBuddyRoad& other) const { 
		return m_objectID == other.m_objectID && m_roadID == other.m_roadID;
	}
};

typedef std::vector<CBuddyRoad> RoadList; //!< 道路のリスト定義


/**
* @brief 紐付く道路リンク群管理用クラス
*/
class CBuddyRoadMgr
{
public:
	CBuddyRoadMgr();
	~CBuddyRoadMgr();
	
	/**
	* @brief 初期化
	*
	* @note walkLinkと同じユーザのLINK_RELATIONテーブルから、既に紐付いている道路リンクを探します
	* @param [in] walkLink 対象の歩行者リンク
	* @param [in] ah ArcHelper
	*/
	void Init( std::list<CFeatureDef>* featureDefs, const CArcHelper& ah );
	
	/**
	* @brief 道路紐付け処理
	*
	* LINK_RELATIONテーブルから、既に紐付いている道路リンクを探し、リストボックスに表示します
	*/
	void AddFromSearchedRoads( long walkID );

	/**
	* @brief 紐付いているか
	* @param [in] walkID 歩行者リンクID
	* @param [in] roadID 道路リンクID
	* @return walkIDとroadIDが紐付いていればtrue
	*/
	bool IsBuddy( long walkID, long roadID );

	/**
	* @brief 紐付く道路リンクが存在するか
	* @note 削除フラグの立っている道路リンクはカウント対象になりません
	* @param [in] walkID 対象の歩行者リンクID
	* @return 紐付く道路リンクが１つでも存在すればtrue
	*/
	bool HasBuddy( long walkID );
	
	/**
	* @brief 変更チェック
	* @param [in] walkID 対象の歩行者リンクID
	* @return 紐付く道路リンク群が変更されていればtrue
	*/
	bool IsChanged( long walkID );
	
	/**
	* @brief 論理チェック
	* @param [out] stringID エラーメッセージのリソースID
	* @retval true エラーなし
	* @retval false エラーあり
	*/
	bool CheckLogic( UINT& stringID );
	
	/**
	* @brief LINK_RELATIONテーブルを更新する
	* @note 変更分のレコード追加、削除を行い、Storeまで行います
	* @return 保存に成功すればtrue
	*/
	bool UpdateLinkRelTable();

	/**
	* @brief buddy編集内容の破棄
	*
	* @note m_buddyRoadsをクリアします。
	*   buddyRoadsの変更を保存・破棄した後に呼ばないと、前回の編集が残ってしまうことがあります。
	*/
	void ClearLinkMap();

	/**
	* @brief 周辺検索
	* @note 周辺の道路リンクを検索し、結果は内部でも保存します
	* @param [in] walkLink 対象の歩行者リンク
	* @param [in] dRange 検索範囲（メートル）
	* @return 検索結果の道路リンク群
	*/
	RoadList& ArroundSearch( IFeaturePtr walkLink, double dRange);
	
	/**
	* @brief 紐付いている道路リンク取得
	* @note 削除フラグは考慮されません
	* @param [in] walkID 対象の歩行者リンクID
	* @return Buddy関係にある道路リンク群
	*/
	RoadList& GetBuddyRoads( long walkID );
	
	/**
	* @brief 周辺検索済み道路リンク取得
	* @return 検索結果の道路リンク群
	*/
	RoadList& GetSearchedRoads(){ return m_searchedRoads; }

private:
	ITablePtr m_LinkRelT;                  //!< LINK_RALATIONテーブル
	IFeatureClassPtr m_roadLinkT;          //!< 道路リンクテーブル
	std::map<long, RoadList> m_buddyRoads; //!< key:WalkLinkのID val:紐付く道路のリスト
	RoadList m_searchedRoads;              //!< 周辺検索された道路リンク群
};

