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

class RelLinkInfos
{
private:
	/**
	 *	関連車線リンク情報
	 */
	struct RelLink
	{
	public:
		long m_LinkOid;				//!< リンクのOID
		CString m_LinkGid;			//!< リンクのGID
		IPolylinePtr m_Polyline;	//!< リンク形状

		RelLink(): m_LinkOid(-1){}
		RelLink(long oid, CString gid, IPolylinePtr polyline): 
			m_LinkOid(oid), m_LinkGid(gid), m_Polyline(polyline) {}
		~RelLink(){}

		// OID でユニークになる想定
		bool operator <(const RelLink& rel_node ) const{
			return m_LinkOid < rel_node.m_LinkOid;
		}

		bool operator ==(const RelLink& rel_node ) const{
			return m_LinkOid == rel_node.m_LinkOid;
		}
	};


public:
	RelLinkInfos(void) : m_error(false) {};
	RelLinkInfos(ITablePtr rel_table, ITablePtr link_table, const CString& roadmark_gid) : 
		m_relTable(rel_table), 
		m_linkTable(link_table),
		m_roadmarkGid(roadmark_gid)
		{}
	virtual ~RelLinkInfos(void){};

public:
	/**
	 *  @brief 関連テーブルの情報を読み込み
	 *  @retval true  読み込み成功
	 *  @retval false 読み込み失敗
	 */
	bool read();

	/**
	 *  @brief  再読み込みを行う
	 *  @param[in] oids 再読み込みを行うリンクのOID群
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
	 *  @brief リンクの形状群(ポリライン)を返す
	 *  @return ポリライン群
	 */
	std::vector<IPolylinePtr> getLinkPolylines() const;

	/**
	 *  @brief 変更があるか
	 *  @retval true  変更あり
	 *  @retval false 変更なし
	 */
	bool isChange() const;

	/**
	 *  @brief 指定したOIDに該当するリンクの形状を取得する
	 *  @param[in] oids  取得したいリンクのOID群
	 *  @return    ノードのポイント群
	 */
	std::vector<IPolylinePtr> getPolylines(const std::vector<long>& oids) const;

	/**
	 *  @brief エラーがあるか
	 *  @detail ダイアログにツリーにぶら下がっている各フィーチャについて
	 *          関連ノードのエラー状態を保持しておく必要があるために、
	 *          用意している関数
	 *  @retval true  変更あり
	 *  @retval false 変更なし
	 */
	bool hasError() const { return m_error; }

private:
	/**
	 *  @brief レコードの存在確認
	 *  @param[in]    検索対象のレコード
	 *  @retval true  レコードが見つかった
	 *  @retval false レコードが見つからなかった
	 */
	bool isExist(const RelLink& rel_link);

private:
	CString m_roadmarkGid;		//!< 道路表示エリアGID

	std::set<RelLink> m_relLinks;		//!< 関連リンク情報群
	std::set<RelLink> m_prevRelLinks;	//!< 変更前の関連リンク情報群

	ITablePtr m_relTable;	//!< 関連テーブル
	ITablePtr m_linkTable;	//!< 車線リンクフィーチャクラス

	bool m_error;		//!< エラーがあるか
};

