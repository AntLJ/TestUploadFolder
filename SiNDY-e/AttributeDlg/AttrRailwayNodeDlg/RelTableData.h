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
 * @note  鉄道ノード紐月テーブルのレコード群に相当する情報
 */
class CRelTableData
{
public:

	/**
	 *	@brief	路線リスト1レコード 
	 */
	struct LineInfoRecord
	{
		long nJoinLineId;			//!< 乗り入れ路線OID
		CString strStationName;		//!< 駅名
		CString strRailwayName;		//!< 路線名
		CString strStationNumber;	//!< 駅番号

		LineInfoRecord() : nJoinLineId(0) {}
		~LineInfoRecord(){}

		bool equalStaNumber(const LineInfoRecord& rec) const
		{
			return ( strStationNumber.Compare(rec.strStationNumber) == 0 );
		}
	};

public:
	typedef std::vector<LineInfoRecord>::iterator Iter;
	typedef std::vector<LineInfoRecord>::const_iterator CIter;

public:
	CRelTableData(void) {};
	~CRelTableData(void) {};

	/**
	 *	@brief	路線リストデータの先頭を返す 
	 */
	CIter begin() const { return m_LineInfoData.begin(); }

	/**
	 *	@brief	路線リストデータの末尾を返す 
	 */
	CIter end() const { return m_LineInfoData.end(); }

	/**
	 *	@brief	路線リストデータのクリア 
	 */
	void clear() { m_LineInfoData.clear(); }

	/**
	 *	@brief	路線リストデータが空か否か
	 *	@retval true   データが空
	 *	@retval false  データが空ではない
	 */
	bool empty() const { return m_LineInfoData.empty(); }

	/**
	 *	@brief	レコード数を返す
	 *  @return レコードのサイズ
	 */
	int size() const { return m_LineInfoData.size(); }

	/**
	 *	@brief	指定した乗り入れ路線IDのレコードが存在するか
	 *  @param   [in]  nJoinLineId  乗り入れ路線ID
	 *  @retval  true   存在する
	 *  @retval  false  存在しない
	 */
	bool isExist( int nJoinLineId ) const;

	/**
	 *	@brief	IDが重複するレコード数を取得
	 *  @param  [in]  比較先のレコード群
	 *  @return 重複レコード数
	 */
	int getDupliCount( const CRelTableData& cData ) const;

	/**
	 *	@brief	乗り入れ路線IDでのレコード検索
	 *  @param  [in]  nJoinLineId  乗り入れ路線ID
	 *  @return 路線リストレコードのイテレータ
	 */
	Iter find( int nJoinLineId );

	/**
	 *	@brief	レコードの追加 
	 *  @param  [in]  cRecord  路線リストのレコード
	 */
	void add(LineInfoRecord cRecord){ m_LineInfoData.push_back(cRecord); }

	/**
	 *	@brief	レコードの削除 
	 *  @param  [in]  iter  削除レコードのイテレータ
	 */
	void erase(CIter iter){ m_LineInfoData.erase(iter); };

	/**
	 *	@brief	レコードの削除(JOIN_LINE_ID 指定) 
	 *  @param  [in]  lid  削除するレコードのJOIN_LINE_ID
	 */
	void erase(long lid);

private:

	std::vector<LineInfoRecord> m_LineInfoData;	//!< 路線リストのデータ群
};

