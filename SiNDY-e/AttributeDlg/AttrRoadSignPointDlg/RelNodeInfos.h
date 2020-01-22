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

class RelNodeInfos
{
private:
	/**
	 *	関連ノード情報
	 */
	struct RelNode
	{
	public:
		long m_NodeOid;		//!< ノードのOID
		CString m_NodeGid;	//!< ノードのGID
		IPointPtr m_Point;	//!< ポイント

		RelNode(): m_NodeOid(-1){}
		RelNode(long oid, CString gid, IPointPtr point): 
			m_NodeOid(oid), m_NodeGid(gid), m_Point(point) {}
		~RelNode(){}

		// OID でユニークになる想定
		bool operator <(const RelNode& rel_node ) const{
			return m_NodeOid < rel_node.m_NodeOid;
		}

		bool operator ==(const RelNode& rel_node ) const{
			return m_NodeOid == rel_node.m_NodeOid;
		}
	};


public:
	RelNodeInfos(void) : m_Error(false) {};
	RelNodeInfos(ITablePtr rel_table, ITablePtr node_table, const CString& roadsign_gid) : 
		m_RelTable(rel_table), 
		m_NodeTable(node_table),
		m_RoadsignGid(roadsign_gid)
		{}
	virtual ~RelNodeInfos(void){};

public:
	/**
	 *  @brief 関連テーブルの情報を読み込み
	 *  @retval true  読み込み成功
	 *  @retval false 読み込み失敗
	 */
	bool read();

	/**
	 *  @brief  再読み込みを行う
	 *  @param[in] oids 再読み込みを行うノードのOID群
	 *  @param[out] err_oids 読み込みエラーとなったOID群
	 *  @retval true  再読み込み成功
	 *  @retval false 再読み込み失敗
	 */
	bool reloadData(const std::vector<long>& oids, std::vector<long>& err_oids);

	/**
	 *  @brief  変更点をテーブルへ反映する
	 *  @retval true  テーブル更新成功
	 *  @retval false テーブル更新失敗
	 */
	bool updateTable();

	/**
	 *  @brief 関連テーブルのOID群を取得
	 *  @return GID群
	 */
	std::vector<long> getOIDs() const;

	/**
	 *  @brief ノードのポイント群を返す
	 *  @return ポイント群
	 */
	std::vector<IPointPtr> getNodePoints() const;

	/**
	 *  @brief 変更があるか
	 *  @retval true  変更あり
	 *  @retval false 変更なし
	 */
	bool isChange() const;

	/**
	 *  @brief 指定したOIDに該当するノードの形状を取得する
	 *  @param[in] oids  取得したいノードのOID群
	 *  @return    ノードのポイント群
	 */
	std::vector<IPointPtr> getPoints(const std::vector<long>& oids) const;

	/**
	 *  @brief エラーがあるか
	 *  @detail ダイアログにツリーにぶら下がっている各フィーチャについて
	 *          関連ノードのエラー状態を保持しておく必要があるために、
	 *          用意している関数
	 *  @retval true  変更あり
	 *  @retval false 変更なし
	 */
	bool hasError() const { return m_Error; }

private:

	/**
	 *  @brief レコードの存在確認
	 *  @param[in]    検索対象のレコード
	 *  @retval true  レコードが見つかった
	 *  @retval false レコードが見つからなかった
	 */
	bool isExist(const RelNode& rel_node);

private:


	CString m_RoadsignGid;		//!< 道路標識ポイントGID

	std::set<RelNode> m_RelNodes;		//!< 関連ノード情報群
	std::set<RelNode> m_prevRelNodes;	//!< 変更前の関連ノード情報群

	ITablePtr m_RelTable;	//!< 関連テーブル
	ITablePtr m_NodeTable;	//!< 関連するノードのフィーチャクラス

	bool m_Error;		//!< エラーがあるか
};

