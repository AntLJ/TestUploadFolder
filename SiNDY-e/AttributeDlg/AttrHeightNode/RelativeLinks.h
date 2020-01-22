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
#include <boost/operators.hpp>

/**
* @brief リンク番号（リンク１、リンク２）区別用
*/
namespace linkNo{
	enum eCode{
		unkown, //!< 不明
		link1,  //!< ID1
		link2   //!< ID2
	};
}

/**
* @brief 相対高さ用
*/
namespace height_info{
	enum eCode{
		flat,    //!< 相対高さなし
		higher,  //!< 高い
		lower    //!< 低い
	};
}

/**
* @brief リンクと高さ管理用
*/
struct CLinkHeight : private boost::operators<CLinkHeight>
{
	CLinkHeight() : m_id(-1), m_height(0){}
	CLinkHeight( const IFeaturePtr link );
	~CLinkHeight(){}

	CString m_layer;        //!< レイヤ名
	long m_id;              //!< OID
	CString m_walkclass;    //!< 歩行者種別
	long m_height;          //!< （相対？）高さ
	IFeaturePtr m_feature;  //!< リンクのフィーチャ

	/// 高さを比べる
	height_info::eCode Compare ( const CLinkHeight& other) const;

	/// 高さで判定
	bool operator == (const CLinkHeight& other) const { 
		return m_height == other.m_height; 
	}
	bool operator < (const CLinkHeight& other) const { 
		return m_height < other.m_height; 
	}
};

class CRelativeLinks
{
public:
	CRelativeLinks();
	~CRelativeLinks();
	
	/**
	* @brief 初期化
	* @param heightNode [in] 交点となる相対高さノード
	* @param ah [in] ArcHelper
	* @param err [out] エラーメッセージ
	*/
	bool Reset( const IFeaturePtr& heightNode, const CArcHelper& ah, CString& err );
	
	/**
	* @brief 交差する２リンクを検索する
	* @param heightnNode [in] 交点の高さノード
	* @return 道路リンク、または歩行者リンクのリスト
	*/
	std::list< CAdapt<IFeaturePtr> > GetCrossedLinks( const IFeaturePtr& heightNode );
	
	/**
	* @brief （相対？）高さを変更する
	* @param linkNo [in] リンク番号（ID1 or ID2）
	* @param height [in] 設定する高さ（-2 ～ 2）
	*/
	void UpdateHeight( linkNo::eCode linkNo, long height ){ m_links[linkNo].m_height = height; }
	
	/**
	* @brief リンク情報の存在判定
	* @return 1つでもあればtrue
	*/
	bool IsExist() const{ return !m_links.empty(); }
	
	/**
	* @brief 新規もの判定
	* @return SketchToolで新規に作られた（であろう）ものならtrue
	*/
	bool IsCreated() const { return m_createF;}
	
	/**
	* @brief 1つ分のリンク情報取得
	* @param linkNo [in] リンク番号（ID1 or ID2）
	*/
	const CLinkHeight& GetLinkInfo(linkNo::eCode linkNo ) {return m_links[linkNo];}
	
	/**
	* @brief すべてのリンク情報取得
	*/
	const std::map<linkNo::eCode, CLinkHeight>& GetLinkInfos() {return m_links;}
	
	/**
	* @brief 相対高さ取得
	* @note linkNoのリンクの他方に対する相対高さを返す
	* @param linkNo [out] 相対高さ取得対象のリンク
	* @return higher linkNoは他方よりも高い
	* @return lower linkNoは他方よりも低い
	* @return flat 両リンクの高さは等しい
	*/
	height_info::eCode GetRelativeHeight( linkNo::eCode linkNo );
	
	/**
	* @brief 論理チェック
	* @param msg [out] エラーメッセージ。なければ空文字
	* @return エラーがなければtrue
	*/
	bool CheckLogic( CString& msg );

	/// mapのラップ用
	typedef std::map<linkNo::eCode, CLinkHeight> LinkMap;
	LinkMap::const_iterator begin() {return m_links.begin();}
	LinkMap::const_iterator end() {return m_links.end();}
	const CLinkHeight& operator []( linkNo::eCode linkNo ){ return m_links[linkNo]; }

private:
	/**
	* @brief 初期化
	* @param links [in] 交差する2リンク
	*/
	void Reset( const std::list< CAdapt<IFeaturePtr> >& links );

	bool m_createF;  //!< 新規ものフラグ
	std::map<linkNo::eCode, CLinkHeight> m_links;   //<! リンク番号ごとの高さ情報

	IFeaturePtr m_heightNode;   //!< 重なり高さフィーチャ
	IFeatureClassPtr m_roadT;   //!< 道路リンクフィーチャクラス
	IFeatureClassPtr m_walkT;   //!< 歩行者リンクフィーチャクラス
};

