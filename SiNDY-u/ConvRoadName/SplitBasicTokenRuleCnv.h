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
#include "BaseCnv.h"
#include <functional>

// 分割種別コード
namespace split_func {
	enum ECode {
		kNormal = 1,
		kParen = 2,
		kHwy = 3,
	};
}

/**
* @brief 分割名称格納クラス
*/
class SplitElement
{
public:
	SplitElement(const CStringW str, const split_class::ECode& cls, bool isParen)
		:m_str(str), m_class(cls), m_isParen(isParen) {}
	~SplitElement() {}

public:
	CStringW m_str;
	split_class::ECode  m_class;
	bool m_isParen;
};

/**
* @brief 名称分割ベースクラス
*/
class SplitBasicTokenRuleCnv :
	public BaseCnv
{
public:
	SplitBasicTokenRuleCnv(const std::map<CString, std::vector<CString>>& dictionary, const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList, const ITablePtr& ipRoadCodeList, const ITablePtr& ipSubNameTable)
		:BaseCnv(ipRoadLink, ipRoadNameList, ipSubNameTable), m_ipRoadCodeList(ipRoadCodeList), m_dictionary(dictionary){}
	~SplitBasicTokenRuleCnv() {}

protected:
	bool setRoadLinkFieldIndex() override;
	bool setSubTblFieldIndex() override;
	bool loadSubTable() override;
	virtual bool cnvSplitName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, const RoadNameRecord& roadNameRecord, std::set<SpltNameRecord>& spltRdNameRecs) override;
	
	/**
	 * @brief 辞書チェック
	 * @note 変換対象文字列が辞書に登録されている場合は、辞書に従い分割する。
	 * @param roadNameRecord [in] 変換対象名称データ
	 * @param spltRdNameRecs [out] 道路名称分割データレコード変換結果
	 * @return true 辞書に登録されたデータ有り
	 * @return false 辞書に登録されたデータ無し
	 *
	 */
	bool chkDictionary(const RoadNameRecord& roadNameRecord, std::set<SpltNameRecord>& spltRdNameRecs);

	/**
	 * @brief 変換対象道路リンクの路線コード取得
	 * @note 道路リンクに関連付く路線コードから取得する。
	 * @param linkOID [in] 変換対象リンクOBJECTID
	 * @param ipLink [in] 変換対象リンク
	 * @param lineCls [out] 路線コード
	 * @retval true 成功
	 * @retval fasle 失敗
	 */
	bool getLineCls(const long& linkOID, const IFeaturePtr& ipLink, long& lineCls);
	
	/**
	 * @brief Basic Tokenization Ruleに基づく道路名称分割変換
	 * @note Basic Tokenization Ruleについては、開発文書参照
	 * @param tgtID [in] 分割元名称データを持つレコードのOBJECTID
	 * @param tgtTblName [in] 分割元名称データをもつテーブル名
	 * @param nameStr [in] 分割対象名称
	 * @param yomiStr [in] 分割対象読み
	 * @param mainNameSpltElem [out] 名称分割結果
	 * @param mainYomiSpltElem [out] 読み分割結果
	 */
	void basicTokenRuleCnv(const long& tgtID, 
		const CString& tgtTblName,
		const long& lineCls,
		const CString& nameStr, 
		const CString& yomiStr, 
		std::list<SplitElement>& mainNameSpltElem, 
		std::list<SplitElement>& mainYomiSpltElem);
	/*
	* @brief 道路リンクに関連する路線コードレコード取得
	* @param linkOID [in] 変換対象リンクOBJECTID
	* @param ipLink [in] 変換対象リンク
	* @param codeOID [out] 関連する路線コードOBJECTID
	* @param ipCode [out] 関連する路線コードレコード
	*/
	bool getLinkRelRoadCode(const long& linkOID, const IFeaturePtr& ipLink, long& codeOID, _IRowPtr& ipCode);

private:
	split_class::ECode getSplitClass(const CString& splitStr);

	/**
	 * @brief 名称読み分割全体関数
	 * @param funcCode [in] 関数コード(使用する文字列分割関数を決めるコード)
	 * @param tgtID [in] 分割元名称データを持つレコードのOBJECTID
	 * @param tgtTblName [in] 分割元名称データをもつテーブル名
	 * @param spltNameWord [in] 名称用分割基準文字列
	 * @param spltYomiWord [in] 読み用分割基準文字列
	 * @param mainNameSpltElem [in,out] 名称分割結果(分割対象文字列も本コンテナに格納されている)
	 * @param mainYomiSpltElem [in,out] 読み分割結果(分割対象文字列も本コンテナに格納されている)
	 */
	void splitNameYomi(const split_func::ECode& funcCode,
		const long& tgtID,
		const CString& tgtTblName,
		const CStringW& spltNameWord,
		const CStringW& spltYomiWord,
		std::list<SplitElement>& mainNameSpltElem,
		std::list<SplitElement>& mainYomiSpltElem);

	void splitNameYomiByRoadNo(const long& tgtID,
		const CString& tgtTblName,
		const CStringW& spltBaseNameWord,
		const CStringW& spltBaseYomiWord,
		std::list<SplitElement>& mainNameSpltElem,
		std::list<SplitElement>& mainYomiSpltElem);
	
	/*
	 * @brief 文字列分割関数
	 * @note funcCodeに合わせて、各種文字列分割関数を呼び出すだけの関数
	 * @param funcCode [in] 関数コード(使用する文字列分割関数を決めるコード)
	 * @param tgtID [in] 分割元名称データを持つレコードのOBJECTID
	 * @param tgtTblName [in] 分割元名称データをもつテーブル名
	 * @param spltWord [in] 分割基準文字列
	 * @param spltCls [in] 分割属性(分割基準文字列に付与される属性)
	 * @param tgtElem [in] 分割対象文字列(分割属性等も持つ)
	 * @param spltElem [out] 分割結果
	 */
	void splitStr(const split_func::ECode& funcCode, 
		const long& tgtID,
		const CString& tgtTblName,
		const CStringW& spltWord,
		const SplitElement& tgtElem,
		std::vector<SplitElement>& spltElem);
	
	/*
	 * @brief ノーマル分割関数
	 * @param spltWord [in] 分割基準文字列
	 * @param spltCls [in] 分割属性(分割基準文字列に付与される属性)
	 * @param tgtElem [in] 分割対象文字列(分割属性等も持つ)
	 * @param spltElem [out] 分割結果
	 */
	void splitStrNormal(const CStringW& spltWord, const SplitElement& tgtElem, std::vector<SplitElement>& spltElem);
	
	/*
	 * @brief ()文字列分割関数
	 * @param tgtID [in] 分割元名称データを持つレコードのOBJECTID
	 * @param tgtTblName [in] 分割元名称データをもつテーブル名
	 * @param tgtElem [in] 分割対象文字列(分割属性等も持つ)
	 * @param spltElem [out] 分割結果
	 */
	void splitStrByParen(const long& tgtID, const CString& tgtTblName, const SplitElement& tgtElem, std::vector<SplitElement>& spltElem);
	
	/*
	 * @brief 高速文字列分割関数
	 * @param spltWord [in] 分割基準文字列
	 * @param tgtElem [in] 分割対象文字列(分割属性等も持つ)
	 * @param spltElem [out] 分割結果
	 */
	void splitStrByHwy(const CStringW& spltWord, const SplitElement& tgtElem, std::vector<SplitElement>& spltElem);
	
	/*
	 * @brief 道路No文字列分割関数
	 * @param spltBaseWord [in] 分割基準文字列
	 * @param tgtElem [in] 分割対象文字列(分割属性等も持つ)
	 * @param spltNo [out] 実際に使用した分割数字群
	 * @param spltElem [out] 分割結果
	 */
	void splitStrByRoadNo(const CStringW& spltBaseWord, const SplitElement& tgtElem, std::vector<CString>& spltNo, std::vector<SplitElement>& spltElem);

	/*
	 * @brief 分割結果のマージ関数
	 * @param spltElem [in] ある文字列の分割結果
	 * @param itr [in] 全体の分割結果の中のある文字列(分割前)を指すイテレータ
	 * @param mainSplitElem[in,out] 全体の分割結果
	 */
	void mergeElementsToMain(const std::vector<SplitElement>& spltElem, std::list<SplitElement>::iterator& itr, std::list<SplitElement>& mainSpltElem);
	
protected:
	ITablePtr m_ipRoadCodeList;						//!< 道路路線リストテーブル
	
	long m_roadLinkRoadCodeIndex;					//!< 道路リンクROAD_CODEフィールドインデックス
	long m_roadCodeListRoadCodeIndex;				//!< 路線コードリストROAD_CODEフィールドインデックス
	long m_roadCodeListLineClsIndex;				//!< 路線コードリストLINECLASS_Cフィールドインデックス
	
	std::map<long, _IRowPtr> m_tgtRdCodeListRecs;	//!< 変換対象路線リストレコード キー：路線コード

	const std::map<CString, std::vector<CString>> m_dictionary; //!< 特殊変換用辞書
};

