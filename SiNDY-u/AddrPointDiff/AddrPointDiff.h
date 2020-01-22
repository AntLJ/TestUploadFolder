#pragma once

#include "Arguments.h"
#include "stdafx.h"
#include "AddrPointRec.h"
#include "ErrInfoRec.h"
#include "AddressSort.h"
#include "Utility.h"

#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGlobalsMisc.h>
#include <ArcHelperEx/AheGeometryOp.h>
#include <TDC/sindylib_base/TableFinder.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>

/**
 * @brief エラーログヘッダリスト
 */
const std::vector<CString> ERRLOG_HEADER_LIST = boost::assign::list_of
		( _T("FLAG") )
		( _T("LAYER") )
		( _T("OBJECTID") )
		( _T("1:LAYER") )
		( _T("1:OBJECTID") )
		( _T("エラーレベル") )
		( _T("差分パターン") )
		( _T("Geospace建物ID") )
		( _T("iPC住所:11桁コード") )
		( _T("iPC住所:住所文字列") )
		( _T("住所中間F:11桁コード") )
		( _T("住所中間F:住所文字列") )
		( _T("距離") )
		;

/**
 * @brief   住所ポイント存在種別パターン
 */
typedef std::pair<AddrExistAttr, CString> PointExistAttrPattern;

/**
 * @brief   メインクラス
 */
class CAddrPointDiff
{

public:

	/**
	 * @brief   コンストラクタ
	 * @param   args [in]  引数
	 */
	CAddrPointDiff( const Arguments& args ) : m_args( args ){}

	/**
	 * @brief   デストラクタ
	 */
	~CAddrPointDiff(){};

	/**
	 * @brief   実行
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool execute();

private:

	/**
	 * @brief   初期化
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool init();

	/**
	 * @brief テーブルオープン
	 * @param[in]  conn      接続文字列
	 * @param[in]  tableName 対象テーブル名
	 * @param[out] table     接続テーブル（ITablePtr）
	 * @retval true 成功
	 * @retval false エラー
	 */
	bool openTable(const CString& conn, const CString& tableName, ITablePtr& table);

	/**
	 * @brief テーブルファインダ初期化
	 * @param[in]  finder  テーブルファインダ
	 * @param[in]  conn    接続先
	 * @param[out] table   テーブル名
	 * @retval true 成功
	 * @retval false エラー
	 */
	bool initCollection(sindy::CTableFinder& finder, const CString& conn, const CString& table);

	/**
	 * @brief iPC建物上に存在するかチェックする（中間ファイル住所ポイント用）
	 * @param[in]      buildingT      テーブルコンテナ（BUILDING）
	 * @param[in/out]  addrPointRecs  住所ポイントセット
	 */
	void checkInBuilding(
				sindy::CTableContainer& buildingT,
				AddrPointRecs& addrPointRecs
			);

	/**
	 * @brief iPC建物上に存在するかチェックする（号ポイント/CSポイント用）
	 * @param[in]      buildingT      テーブルコンテナ（BUILDING）
	 * @param[in/out]  addrPointRecs  住所ポイントセット
	 * @param[in]      targetGeo      BUILDING取得時に接触判定に使用する形状
	 */
	void checkInBuilding(
				sindy::CTableContainer& buildingT,
				AddrPointRecs& addrPointRecs,
				const IGeometryPtr& targetGeo
			);

	/**
	 * @brief 住所ポイントコンテナを生成する
	 * @param[in]      type          住所ポイント種別
	 * @param[in]      addrcode11    住所11桁コード
	 * @param[in]      targetRow     Rowコンテナ（データ取得元）
	 * @param[in]      gaikuFugo     街区符号
	 * @param[in]      extGaikuFugo  拡張街区符号
	 * @param[in]      addrClass     住所整備種別
	 * @param[in/out]  addrPointRecs 住所ポイントセット
	 */
	void makeAddrPoint(
				AddrPointType type,
				const CString& addrcode11,
				const sindy::CRowContainer& targetRow,
				const CString& gaikuFugo,
				const CString& extGaikuFugo,
				int addrClass,
				AddrPointRecs& addrPointRecs
			);

	/**
	 * @brief 住所ポイントコンテナを生成する
	 * @param[in]      type          住所ポイント種別
	 * @param[in]      addrcode11    住所11桁コード
	 * @param[in]      targetRow     Rowコンテナ（データ取得元）
	 * @param[in/out]  addrPointRecs 住所ポイントセット
	 */
	void makeAddrPoint(
				AddrPointType type,
				const CString& addrcode11,
				const sindy::CRowContainer& targetRow,
				AddrPointRecs& addrPointRecs
			);

	/**
	 * @brief 住所ポイントの都市地図エリア内外判定を行う
	 * @param[in]   cityAdminT             テーブルコンテナ（CITY_ADMIN）
	 * @param[in]   addrPointRecs          住所ポイントセット
	 * @param[in]   targetGeo              CITY_ADMIN取得時に接触判定に使用する形状
	 * @param[out]  urbanAreaAddrPoints    住所ポイントセット管理コンテナ（都市地図エリア内、11桁コード単位）
	 * @param[out]  nonUrbanAreaAddrPoints 住所ポイントセット管理コンテナ（都市地図エリア外、11桁コード単位）
	 * @retval true 成功
	 * @retval false エラー
	 */
	bool assortAddrPointsByArea(
				sindy::CTableContainer& cityAdminT,
				const AddrPointRecs& addrPointRecs,
				const IGeometryPtr& targetGeo,
				std::map<CString, AddrPointRecSet>& urbanAreaAddrPoints,
				std::map<CString, AddrPointRecSet>& nonUrbanAreaAddrPoints
			);

	/**
	 * @brief 取得した住所ポイントセットを、addrcodd2を考慮して住所ポイントセット管理コンテナに格納する
	 * @param[in]      addrPointRecs  住所ポイントセット
	 * @param[in]      addrcode2      住所11桁コード（addrcode2考慮）
	 * @param[in/out]  addrPointSets  住所ポイントセット管理コンテナ（11桁コード単位）
	 */
	void setAddrPointRecs(
				const AddrPointRecs& addrPointRecs,
				const CString& addrcode2,
				std::map<CString, AddrPointRecSet>& addrPointSets
			);
			
	/**
	 * @brief 号ポイントから住所ポイントを生成し、住所ポイントセット管理コンテナに格納する
	 * @param[in]      citycode               市区町村コード
	 * @param[in]      cityAdminT             テーブルコンテナ（CITY_ADMIN）
	 * @param[in]      buildingT              テーブルコンテナ（BUILDING）
	 * @param[in/out]  addrcode11List         住所11コードセット
	 * @param[in/out]  urbanAreaAddrPoints    住所ポイントセット管理コンテナ（都市地図エリア内、11桁コード単位）
	 * @param[in/out]  nonUrbanAreaAddrPoints 住所ポイントセット管理コンテナ（都市地図エリア外、11桁コード単位）
	 */
	void getAddrPointsFromGouPoint(
				const CString& citycode,
				sindy::CTableContainer& cityAdminT,
				sindy::CTableContainer& buildingT,
				std::set<CString>& addrcode11List,
				std::map<CString, AddrPointRecSet>& urbanAreaAddrPoints,
				std::map<CString, AddrPointRecSet>& nonUrbanAreaAddrPoints
			);
			
	/**
	 * @brief CSポイントから住所ポイントを生成し、住所ポイントセット管理コンテナに格納する
	 * @param[in]      citycode               市区町村コード
	 * @param[in]      cityAdminT             テーブルコンテナ（CITY_ADMIN）
	 * @param[in]      buildingT              テーブルコンテナ（BUILDING）
	 * @param[in/out]  addrcode11List         住所11コードセット
	 * @param[in/out]  urbanAreaAddrPoints    住所ポイントセット管理コンテナ（都市地図エリア内、11桁コード単位）
	 * @param[in/out]  nonUrbanAreaAddrPoints 住所ポイントセット管理コンテナ（都市地図エリア外、11桁コード単位）
	 * @retval true 成功
	 * @retval false エラー
	 */
	bool getAddrPointsFromCsPoint(
				const CString& citycode,
				sindy::CTableContainer& cityAdminT,
				sindy::CTableContainer& buildingT,
				std::set<CString>& addrcode11List,
				std::map<CString, AddrPointRecSet>& urbanAreaAddrPoints,
				std::map<CString, AddrPointRecSet>& nonUrbanAreaAddrPoints
			);
			
	/**
	 * @brief 中間ファイル住所ポイントから住所ポイントを生成し、住所ポイントセット管理コンテナに格納する
	 * @param[in]      citycode               市区町村コード
	 * @param[in]      cityAdminT             テーブルコンテナ（CITY_ADMIN）
	 * @param[in]      buildingT              テーブルコンテナ（BUILDING）
	 * @param[in/out]  addrcode11List         住所11コードセット
	 * @param[in/out]  urbanAreaAddrPoints    住所ポイントセット管理コンテナ（都市地図エリア内、11桁コード単位）
	 * @param[in/out]  nonUrbanAreaAddrPoints 住所ポイントセット管理コンテナ（都市地図エリア外、11桁コード単位）
	 * @retval true 成功
	 * @retval false エラー
	 */
	bool getAddrPointsFromMidAddrPoint(
				const CString& citycode,
				sindy::CTableContainer& cityAdminT,
				sindy::CTableContainer& buildingT,
				std::set<CString>& addrcode11List,
				std::map<CString, AddrPointRecSet>& urbanAreaAddrPoints,
				std::map<CString, AddrPointRecSet>& nonUrbanAreaAddrPoints
			);

	/**
	 * @brief 都市地図エリア内のiPC住所/中間ファイル住所ポイントを比較し、差分を取得する
	 * @param[in]      addrcode11             住所11桁コード
	 * @param[in/out]  urbanAreaAddrPoints    住所ポイントセット管理コンテナ（都市地図エリア内、11桁コード単位）
	 * @param[in/out]  nonUrbanAreaAddrPoints 住所ポイントセット管理コンテナ（都市地図エリア外、11桁コード単位）
	 */
	void diffUrbanAreaPoints(
			const CString& addrcode11,
			std::map<CString, AddrPointRecSet>& urbanAreaAddrPoints,
			std::map<CString, AddrPointRecSet>& urbanAreaMidAddrPoints
			);

	/**
	 * @brief 都市地図エリア外のiPC住所/中間ファイル住所ポイントを比較し、差分を取得する
	 * @param[in]      addrcode11             住所11桁コード
	 * @param[in/out]  urbanAreaAddrPoints    住所ポイントセット管理コンテナ（都市地図エリア内、11桁コード単位）
	 * @param[in/out]  nonUrbanAreaAddrPoints 住所ポイントセット管理コンテナ（都市地図エリア外、11桁コード単位）
	 */
	void diffNonUrbanAreaPoints(
			const CString& addrcode11,
			std::map<CString, AddrPointRecSet>& nonUrbanAreaAddrPoints,
			std::map<CString, AddrPointRecSet>& urbanAreaMidAddrPoints
			);

	/**
	 * @brief 仕分けた中間ファイル住所ポイント群を基に、差分パターンを決定する
	 * @param[in]   midPointRecs       仕分けを行った中間ファイル住所ポイント
	 * @param[out]  outMidPointRecs    決定した差分パターンに紐付く住所ポイント
	 * @return 住所ポイント存在種別パターン
	 */
	PointExistAttrPattern decideDiffPattern(
				std::map<AddrExistAttr, std::vector<AddrPointWithSortKey>>& midPointRecs,
				std::vector<AddrPointWithSortKey>& outMidPointRecs
			);

	/**
	 * @brief エラーログにiPC住所/中間ファイル住所の比較結果を書き込む
	 */
	void CAddrPointDiff::writeErrLog();

private:

	Arguments m_args;                                //!< 引数
	sindy::CTableFinder m_addrFinder;                //!< テーブルファインダ（SiNDY（住所））
	sindy::CTableFinder m_bgFinder;                  //!< テーブルファインダ（SiNDY（背景））
	sindy::CTableFinder m_midAddrPointFinder;        //!< テーブルファインダ（中間ファイル住所）

	std::vector<ErrInfoRec> errInfoRecs;             //!< エラー情報リスト
};


