#pragma once

#include "stdafx.h"
#include "AddrRec.h"

#include <WinLib/ADOBase.h>
#include <TDC/sindylib_base/RowContainer.h>

namespace addr
{

// 文字付き住所コード変換TB/文字付き住所コード変換仮TB スキーマ定義
namespace cv_schema
{
	namespace cv_common
	{
		const static TCHAR kAddrCode[]		= _T("AddrCode");
		const static TCHAR kRegistDate[]	= _T("RegistDate");
	}

	namespace pnt_conv
	{
		const static TCHAR kTableName[]	= _T("Pnt_CodeConv");
		const static TCHAR kPntCode[]	= _T("PntCode");
		const static TCHAR kPntName[]	= _T("PntName");
		using namespace ::addr::cv_schema::cv_common;		
	}

	namespace gou_conv
	{
		const static TCHAR kTableName[]	= _T("Gou_CodeConv");
		const static TCHAR kGouCode[]	= _T("GouCode");
		const static TCHAR kGouName[]	= _T("GouName");
		using namespace ::addr::cv_schema::cv_common;
	}
}

/**
 * @brief 区切り文字（号番号分割用）

 */
static const CString SEP_HYPHEN = _T("-");

namespace AddrRecHelperEnum
{

/**
 * @brief 住所ステータス（CharAddrConvertから流用）
 */
enum AddrStatus
{
	kNormal,			//!< 通常住所
	kExtGaiku,			//!< 拡張街区符号あり
	kMojiGou,			//!< 号番号に文字あり
	kMojiGou_ExtGaiku,	//!< 号番号に文字あり(拡張街区符号内)
	kSepOver,			//!< 号番号の区切りが多い
	kSepOver_ExtGaiku,	//!< 号番号の区切りが多い(拡張街区符号内)
	kNoOver,			//!< 地番・号で表現しきれない番号
	kNoOver_ExtGaiku 	//!< 地番・号で表現しきれない番号(拡張街区符号内)
};

}

/**
 * @brief 住所レコード（CRowCotainer付き）
 */
typedef std::pair<AddrRec, sindy::CRowContainer> AddrRecPair;

/**
 * @brief 住所レコードリスト
 */
typedef std::vector<AddrRecPair> AddrRecPairs;

/**
 * @brief 属性名・属性値のペア
 */
typedef std::pair<CString, CString> AttributeValue;

using namespace AddrRecHelperEnum;

/**
 * @brief 住所レコード生成補助クラス
 */
class AddrRecHelper
{
public:

	/**
	 * @brief コンストラクタ
	 */
	AddrRecHelper() {}

	/**
	 * @brief 文字付き住所コード変換テーブル接続
	 * @param[in] charConvFile 文字付き住所コード変換テーブルファイル
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool connectCharConvFile(const CString& charConvFile);

	/**
	 * @brief 文字付き住所コード変換仮テーブル接続
	 * @param[in] charConvTmpFile 文字付き住所コード変換仮テーブル
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool connectCharConvTmpFile(const CString& charConvTmpFile);

	/**
	 * @brief 地番名称・号名称取得
	 * @param[in] record 住所レコードコンテナ
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool makeAddrNames(AddrRec& record);

	/**
	 * @brief 地番名称取得
	 * @param[in] record 住所レコードコンテナ
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool makeChibanName(AddrRec& record);

	/**
	 * @brief 号名称取得
	 * @param[in] record 住所レコードコンテナ
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool makeGouName(AddrRec& record);

	/**
	 * @brief 都市地図行政界ポリゴンレコード取得
	 * @param[in]  cityAdminT     テーブルコンテナ（都市地図行政界ポリゴン）
	 * @param[in]  addrcode11     対象の住所レコード（11桁）
	 * @param[out] caAddrRecPairs 住所レコードリスト（都市地図行政界ポリゴンから取得したもの）
	 */
	void getCityAdminRecs(
			sindy::CTableContainer& cityAdminT,
			const CString& addrcode11,
			AddrRecPairs& caAddrRecPairs);

	/**
	 * @brief 住所レコードリスト（号ポイント）生成
	 * @param[in]  gouPointT     テーブルコンテナ（号ポイント）
	 * @param[in]  csAdrRecPairs 住所レコードリスト（号ポイント）
	 * @param[out] gpAdrRecs     住所レコード（都市地図行政界ポリゴン）
	 */
	void makeAdrRecordsFromGp(
			sindy::CTableContainer& gouPointT,
			const AddrRecPairs& csAdrRecPairs,
			AddrRecs& gpAdrRecs);

	/**
	 * @brief 住所レコードリスト（号ポイント）生成
	 * @param[in]  gouPointT  テーブルコンテナ（号ポイント）
	 * @param[in]  addrcode11 対象住所コード（11桁）
	 * @param[in]  caRow      Rowコンテナ（都市地図行政界ポリゴン）
	 * @param[out] gpAdrRecs  住所レコードリスト（号ポイント）
	 */
	void makeAdrRecordsFromGp(
			sindy::CTableContainer& gouPointT,
			const CString& addrcode11,
			const sindy::CRowContainer& caRow,
			AddrRecs& gpAdrRecs);

	/**
	 * @brief 住所レコードリスト（号ポイント）生成
	 * @param[in]  caRow      Rowコンテナ（都市地図行政界ポリゴン）
	 * @param[in]  gpRow      Rowコンテナ（号ポイント）
	 * @param[in]  addrcode11 対象住所コード（11桁）
	 * @param[out] gpAdrRecs  住所レコードリスト（号ポイント）
	 */
	void makeAdrRecordsFromGp(
			const sindy::CRowContainer& caRow,
			const sindy::CRowContainer& gpRow,
			const CString& addrcode11,
			AddrRecs& gpAdrRecs);

	/**
	 * @brief 住所レコードリスト（CSポイント）生成
	 * @param[in]  csPointT   テーブルコンテナ（CSポイント）
	 * @param[in]  rec        住所レコード（都市地図行政界ポリゴン）
	 * @param[out] gpAdrRecs  住所レコードリスト（CSポイント）
	 */
	void makeAdrRecordsFromCs(
			sindy::CTableContainer& csPointT,
			const CString& addrcode11,
			std::map<CString, AddrRecs>& addrRecs);

private:

	/**
	 * @brief Accessへの接続
	 * @param[in] ado DB接続管理クラス
	 * @param[in] file Accessファイルパス
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool openDb(CADOBase& ado, const CString& file);

	/**
	 * @brief Access内のテーブル検索
	 * @note 検索結果の1フィールド目の値を取得する（かつ、フィールドは文字列に限る）
	 * @note また、検索結果が複数レコードの場合、取得結果の1レコード目の値を採用する
	 * @param[in]  ado    接続管理クラス
	 * @param[in]  sqlStr 検索用SQL文字列
	 * @param[out] value  取得結果（文字列）
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool searchStrValue(const CADOBase& ado, const LPCSTR& sqlStr, CString& value);

	/**
	 * @brief 文字付き住所名称取得
	 * @param[in]   sql          取得用SQL
	 * @param[out]  resultName   取得した文字付き住所名称
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool getName(const uh::tstring& sql, CString& resultName);

	/**
	 * @brief 住所ステータス取得
	 * @param[in] gaikuFugo    街区符号
	 * @param[in] extGaikuFugo 拡張街区符号
	 * @param[in] gouNo        号番号
	 * @param[in] expGouNo     拡張号番号フラグ
	 * @retval 住所ステータス
	 */
	AddrStatus getAddrStatus(
				const CString& gaikuFugo,
				const CString& extGaikuFugo, 
				const CString& gouNo,
				bool expGouNo);

	/**
	 * @brief 住所レコード（通常住所版）取得
	 * @param[out] addrRecs    住所レコードリスト
	 * @param[in]  addrcode    住所コード（11桁）
	 * @param[in]  gaikuFugo   街区符号
	 * @param[in]  gouNo       号番号
	 * @param[in]  expGouNo    拡張号番号フラグ
	 * @param[in]  geo          形状（号ポイント）
	 */
	void makeNormalAdrRecordFromGp(
			AddrRecs& addrRecs,
			const CString& addrcode,
			const CString& gaikuFugo,
			const CString& gouNo1,
			bool expGouNo,
			IGeometryPtr& geo);

	/**
	 * @brief 住所レコード（文字付き住所版）取得
	 * @param[out] addrRecs     住所レコードリスト
	 * @param[in]  addrcode     住所コード（11桁）
	 * @param[in]  gaikuFugo    街区符号
	 * @param[in]  extGaikuFugo 拡張街区符号
	 * @param[in]  gouNo        号番号
	 * @param[in]  expGouNo     拡張号番号フラグ
	 * @param[in]  geo          形状（号ポイント）
	 */
	void makeCharAdrRecordFromGp(
			AddrRecs& addrRecs,
			const CString& addrcode,
			const CString& extGaikuFugo,
			const CString& gaikuFugo,
			const CString& gouNo,
			bool expGouNo,
			IGeometryPtr& geo
		);

	/**
	 * @brief 通常住所を生成するかを判定
	 * @param[in] addrClass    住所整備種別
	 * @param[in] gaikuFugo    街区符号
	 * @retval true 生成する
	 * @retval false 生成しない
	 */
	bool isMakeNormalAddrRec(int addrClass, const CString& gaikuFugo);

	/**
	 * @brief 文字付き住所を生成するかを判定
	 * @param[in] addrClass    住所整備種別
	 * @param[in] gaikuFugo    街区符号
	 * @param[in] extGaikuFugo 拡張街区符号
	 * @param[in] gouNo        号番号
	 * @param[in] expGouNo     拡張号番号フラグ
	 * @retval true 生成する
	 * @retval false 生成しない
	 */
	bool isMakeCharAddrRec(
			int addrClass,
			const CString& gaikuFugo,
			const CString& extGaikuFugo, 
			const CString& gouNo,
			bool expGouNo);

private:

	CADOBase m_charConvAdo;     //!< DB接続管理クラス（文字付き住所コード変換テーブル用）
	CADOBase m_charConvTmpAdo;  //!< DB接続管理クラス（文字付き住所コード変換仮テーブル用）
};

} // namespace addr
