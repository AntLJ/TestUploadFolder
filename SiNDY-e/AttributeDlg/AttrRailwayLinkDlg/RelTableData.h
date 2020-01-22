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
 * @brief 路線リスト群を管理するクラス
 * @note  鉄道リンク紐月テーブルのレコード群に相当する情報
 */
class CRelTableData
{
public:

	/**
	 *	@brief	路線リスト1レコード 
	 */
	struct RailwayLineRecord
	{
		long lRailwayLineOid;		//!< 鉄道路線テーブルのOID
		CString strRailwayName;		//!< 路線名
		CString strCorpName;		//!< 会社名

		RailwayLineRecord(){}
		~RailwayLineRecord(){}
	};

public:
	typedef std::vector<RailwayLineRecord>::iterator Iter;
	typedef std::vector<RailwayLineRecord>::const_iterator CIter;

public:
	CRelTableData(void) {};
	~CRelTableData(void) {};

	/**
	 *	@brief	路線リストデータの先頭を返す 
	 */
	CIter begin() const { return m_RailwayLineData.begin(); }

	/**
	 *	@brief	路線リストデータの末尾を返す 
	 */
	CIter end() const { return m_RailwayLineData.end(); }

	/**
	 *	@brief	路線リストデータのクリア 
	 */
	void clear() { m_RailwayLineData.clear(); }

	/**
	 *	@brief	路線リストデータが空か否か
	 *	@retval true   データが空
	 *	@retval false  データが空ではない
	 */
	bool empty() const { return m_RailwayLineData.empty(); }

	/**
	 *	@brief	レコード数を返す
	 *  @return レコードのサイズ
	 */
	int size() const { return m_RailwayLineData.size(); }

	/**
	 *	@brief	 指定した鉄道路線テーブルOIDのレコードが存在するか
	 *  @param   [in]   lId  鉄道路線テーブルのOID
	 *  @retval  true   存在する
	 *  @retval  false  存在しない
	 */
	bool isExist( long lId ) const;

	/**
	 *	@brief	IDが重複するレコード数を取得
	 *  @param  [in]  比較先のレコード群
	 *  @return 重複レコード数
	 */
	int getDupliCount( const CRelTableData& cData ) const;

	/**
	 *	@brief	鉄道路線テーブルのOIDでのレコード検索
	 *  @param  [in]  nJoinLineId  乗り入れ路線ID
	 *  @return 路線リストレコードのイテレータ
	 */
	Iter find( long lId );

	/**
	 *	@brief	レコードの追加 
	 *  @param  [in]  cRecord  路線リストのレコード
	 */
	void add(RailwayLineRecord cRecord){ m_RailwayLineData.push_back(cRecord); }

	/**
	 *	@brief	レコードの削除 
	 *  @param  [in]  iter  削除レコードのイテレータ
	 */
	void erase(CIter iter){ m_RailwayLineData.erase(iter); };

	/**
	 *	@brief	レコードの削除(鉄道路線テーブルのOIDを指定) 
	 *  @param  [in]  lid  削除するレコードの鉄道路線テーブルのOID
	 */
	void erase(long lid);

	/**
	 *	@brief	路線リストデータのコピー
	 *	@note	コピー先のコンテナは初期化数
	 *  @param  [out]  cData  コピー先のコンテナ
	 */
	void copyData(CRelTableData& cData) const;
	
private:

	std::vector<RailwayLineRecord> m_RailwayLineData;	//!< 路線リストのデータ群
};

