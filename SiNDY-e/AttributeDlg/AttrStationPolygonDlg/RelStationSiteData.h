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
 * @brief 駅ポリゴン紐付きテーブルの情報を管理するクラス
 */
class CRelStationSiteData
{
public:

	/**
	 *	@brief	駅ポリゴン紐付きテーブル1レコード
	 */
	struct RelStationSiteRecord
	{
		long lGroupId;			//!< 駅ID
		long lLayerNoC;			//!< レイヤー番号
		long lLayerOid;			//!< レイヤー内OID

		RelStationSiteRecord(): lGroupId(0L), lLayerNoC(0L), lLayerOid(0L) {}
		~RelStationSiteRecord(){}
	};

public:
	typedef std::vector<RelStationSiteRecord>::iterator Iter;
	typedef std::vector<RelStationSiteRecord>::const_iterator CIter;

public:
	CRelStationSiteData(void) {};
	~CRelStationSiteData(void) {};

	/**
	 *	@brief	駅ポリゴン紐付きテーブルデータの先頭を返す 
	 *  @return データの先頭
	 */
	CIter begin() const { return m_RelStationData.begin(); }

	/**
	 *	@brief	駅ポリゴン紐付きテーブルデータの末尾を返す 
	 *  @return データの末尾
	 */
	CIter end() const { return m_RelStationData.end(); }

	/**
	 *	@brief	駅ポリゴン紐付きテーブルデータのクリア 
	 */
	void clear() { m_RelStationData.clear(); }

	/**
	 *	@brief	駅ポリゴン紐付きテーブルのデータが空か否か
	 *	@retval true   データが空
	 *	@retval false  データが空ではない
	 */
	bool empty() const { return m_RelStationData.empty(); }

	/**
	 *	@brief	レコード数を返す
	 *  @return レコードのサイズ
	 */
	int size() const { return m_RelStationData.size(); }

	/**
	 *	@brief	 駅ポリゴン紐付きテーブルのレコードが存在するか
	 *  @param   [in]   record     駅ポリゴン紐付きテーブルのレコード
	 *  @retval  true   存在する
	 *  @retval  false  存在しない
	 */
	bool isExist( const RelStationSiteRecord& record ) const;

	/**
	 *	@brief	重複するレコード数を取得する
	 *  @param  [in]  cData  比較先のレコード群
	 *  @return 重複レコード数
	 */
	int getDupliCount( const CRelStationSiteData& cData ) const;

	/**
	 *	@brief   駅ポリゴン紐付きテーブルのレコード検索
	 *  @param   [in]   record  駅ポリゴン紐付きテーブルのレコード
	 *  @return  駅ポリゴン紐付きレコードのイテレータ
	 */
	Iter find( const RelStationSiteRecord& record );

	/**
	 *	@brief       駅ポリゴン紐付きテーブルのレコード検索
	 *  @attension   保持しているレコード内でグループIDがユニークな前提
	 *  @param       [in]   lGroupId  グループID
	 *  @return      駅ポリゴン紐付きレコードのイテレータ
	 */
	Iter find( long lGroupId );

	/**
	 *	@brief	レコードの追加 
	 *  @param  [in]  cRecord  路線リストのレコード
	 */
	void add( RelStationSiteRecord cRecord ){ m_RelStationData.push_back(cRecord); }

	/**
	 *	@brief	レコードの削除 
	 *  @param  [in]  iter  削除レコードのイテレータ
	 */
	void erase( CIter iter ){ m_RelStationData.erase(iter); };

	/**
	 *	@brief	レコードの削除 
	 *  @param  [in]  lGroupId
	 *  @attension 保持しているコンテナ内でグループIDがユニークな前提
	 */
	void erase( long lGroupId );

private:

	std::vector<RelStationSiteRecord> m_RelStationData;		//!< 駅ポリゴン紐付きテーブルのデータ
};

