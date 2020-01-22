#include "stdafx.h"
#include "Utility.h"
#include "Logger.h"

namespace mid_addr_schema
{
	namespace addr_point
	{
		const _TCHAR kGeoBFid[]    = _T("GEOBFID");       //!< GEOSPACE建物ID
		const _TCHAR kIpcObjId[]   = _T("IPCOBJID");      //!< 中間ファイルオブジェクトID
		const _TCHAR kPlaceCd[]    = _T("PLACE_CD");      //!< 住所11桁コード
		const _TCHAR kStreetCd[]   = _T("STREET_CD");     //!< 通り名フラグ
		const _TCHAR kAddress1[]   = _T("ADDRESS1");      //!< 住所文字列1
		const _TCHAR kAddress2[]   = _T("ADDRESS2");      //!< 住所文字列2
		const _TCHAR kAddress3[]   = _T("ADDRESS3");      //!< 住所文字列3
		const _TCHAR kSourceCd[]   = _T("SOURCE_CD");     //!< 出典フラグ
		const _TCHAR kPtnF[]       = _T("PTN_F");         //!< 処理パターンフラグ
		const _TCHAR kOrderCd[]    = _T("ORDER_CD");      //!< 採用順フラグ
		const _TCHAR kJudgCd[]     = _T("JUDG_CD");       //!< 採用判定理由フラグ
		const _TCHAR kGeoSf[]      = _T("GEO_SF");        //!< GEOSPACE住所の信頼度フラグ
		const _TCHAR kIpcSf[]      = _T("IPC_SF");        //!< インクリメントP住所の信頼度フラグ
		const _TCHAR kShinraiF[]   = _T("SHINRAI_F");     //!< 信頼度フラグ（手作業入力時）
		const _TCHAR kHyoki[]      = _T("HYOUKI");        //!< 図面表記上の地番
	}
}
