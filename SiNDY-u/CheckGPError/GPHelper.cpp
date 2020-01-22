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

#include "stdafx.h"
#include "GPHelper.h"

#include <SiNDYLib/ErrorObject.h>
#include <ArcHelperEx/GlobalFunctions.h>

#include <AddrLib/CodeConverter.h>

#include <regex>

static const tr1::wregex g_re( L"[0-9\\-]*" );
// 移動か追加かの判定に利用
const bool MOVEMODE = true;
const bool INSERTMODE = false;

/**
 * @brief	左側の0除去関数
 * @return	0を除去した文字列(全て0の場合は、0を返す)
 */
inline CString CString_TrimLeft0( CString strData )
{
	return strData.TrimLeft(_T("0")).IsEmpty()? _T("0") : strData;
}

// 号ポイント検索 ([Bug 6181]優先フラグ付きが最優先ポイントにする)
EResultStat CGPHelper::findPoint( CLRec& cRec, bool bAddMode )
{
	CSPGouPoint pGou;			// 一時号ポイント情報格納用
	CSPGouPoint pNotPriorGou;	// 優先フラグなし号ポイント情報保持用

	// まずは16桁で行政界検索
	EResultStat emStatus = kO_NOT_FOUND;

	m_cGous.SelectByWhereClause( MakeWhereClause(cRec, kCTAddr16) );
	if( m_cGous.size(sindyTableType::city_admin) > 0 )
	{
		if( cRec.m_strAddrCodeNew.Mid(16, 4) == "0000" )
		{
			// ラベルポイントの取得
			const boost::shared_ptr<CCityAdmin> pAdmin = boost::static_pointer_cast<CCityAdmin>(*m_cGous.begin(sindyTableType::city_admin));

			IPointPtr ipLabelPoint;
			((IAreaPtr)pAdmin->GetShapeCopy())->get_LabelPoint( &ipLabelPoint );

			gf::ConvPoint2MXY( ipLabelPoint, &cRec.m_cMXY_Ref );
			return kW_GP_SAME_ADMIN;
		}

		emStatus = findGPByAdmin( cRec, pGou, kCTAddr16 );
		if( kO_MATCH_PRIOR_GP == emStatus )
			return getDetailStatus( cRec, pGou, bAddMode );
		else if ( kW_GP_DUPLI_PRIOR == emStatus )
			return kW_GP_DUPLI_PRIOR;
		else if( kO_MATCH_GP == emStatus )
			pNotPriorGou = pGou;	// 優先フラグなし号情報保持
	}

	// 次に11桁で検索
	m_cGous.SelectByWhereClause( MakeWhereClause(cRec, kCTAddr11) );
	if( m_cGous.size(sindyTableType::city_admin) > 0 )
	{
		emStatus = findGPByAdmin( cRec, pGou, kCTAddr11 );
		if( kO_MATCH_PRIOR_GP == emStatus )
			return getDetailStatus( cRec, pGou, bAddMode );
		else if ( kW_GP_DUPLI_PRIOR == emStatus )
			return kW_GP_DUPLI_PRIOR;
		else if( kO_MATCH_GP == emStatus && !pNotPriorGou )
			pNotPriorGou = pGou;	// 優先フラグなし号情報保持(上位行政界検索で見つかっていないとき)		
	}

	// 次に11桁＆［拡張号番号フラグ：ON］で検索
	bool bNotPriorExFlag = false;	// 拡張号で優先フラグなし号見つかったときに一時的に使用するフラグ
	m_cGous.SelectByWhereClause( MakeWhereClause(cRec, kCTAddrEX) );
	if( m_cGous.size(sindyTableType::city_admin) > 0 )
	{
		emStatus = findGPByAdmin( cRec, pGou, kCTAddrEX );
		if( kO_MATCH_PRIOR_GP == emStatus )
		{
			cRec.m_lExFlag = 1;	// 拡張号番号フラグ = Onで見つかったので
			return getDetailStatus( cRec, pGou, bAddMode );
		}
		else if ( kW_GP_DUPLI_PRIOR == emStatus )
			return kW_GP_DUPLI_PRIOR;
		else if( kO_MATCH_GP == emStatus && !pNotPriorGou )
		{
			bNotPriorExFlag = true;
			pNotPriorGou = pGou;	// 優先フラグなし号情報保持(上位行政界検索で見つかっていないとき)
		}
	}

	// 次にADDRCODE2で検索(Bug8166)
	m_cGous.SelectByWhereClause( MakeWhereClause(cRec, kCTAddrCM) );
	if( m_cGous.size(sindyTableType::city_admin) > 0 )
	{
		emStatus = findGPByAdmin( cRec, pGou, kCTAddrCM );
		if( kO_MATCH_PRIOR_GP == emStatus )
			return getDetailStatus( cRec, pGou, bAddMode );
		else if ( kW_GP_DUPLI_PRIOR == emStatus )
			return kW_GP_DUPLI_PRIOR;
		else if( kO_MATCH_GP == emStatus && !pNotPriorGou )
			pNotPriorGou = pGou;	// 優先フラグなし号情報保持(上位行政界検索で見つかっていないとき)
	}

	// 拡張号で優先フラグなし号と一致していたら、ここでリストレコードの拡張フラグ1にする
	if( bNotPriorExFlag )
		cRec.m_lExFlag = 1;

	// どの行政界レベルでも優先なし号と一致していなければ、一致なし
	return !pNotPriorGou? kO_NOT_FOUND : getDetailStatus(cRec, pNotPriorGou, bAddMode);
}

// 号ポイント移動
EResultStat CGPHelper::movePoint( CLRec& cRec, bool bPriorF )
{
	CGouPoint cGou( m_cGouPointFC.GetFeature(cRec.m_lEditPointID), sindyTableType::gou_point, false, m_cGouPointFC.GetFieldMap(), m_cGouPointFC.GetNameString() );

	// 移動先ポイント生成
	IPointPtr ipPoint = gf::CreateMXYPoint( cRec.m_cMXY );
	if( cGou && ipPoint )
	{		
		// 移動前後の行政界をチェック
		ISpatialFilterPtr ipOldSpFilter( AheInitSpatialFilter(NULL, cGou.GetShapeCopy()) );
		ISpatialFilterPtr ipNewSpFilter( AheInitSpatialFilter(NULL, ipPoint) );

		long lOldCount = m_cCityAdminFC.FeatureCount(ipOldSpFilter), lNewCount = m_cCityAdminFC.FeatureCount(ipNewSpFilter);
		IFeatureCursorPtr ipOldCursor( m_cCityAdminFC.Search(ipOldSpFilter, VARIANT_FALSE) ), ipNewCursor( m_cCityAdminFC.Search(ipNewSpFilter, VARIANT_FALSE) );
		if( 1 == lOldCount && 1 == lNewCount && ipOldCursor && ipNewCursor )
		{
			IFeaturePtr ipOldAdminF, ipNewAdminF;
			if( ipOldCursor->NextFeature( &ipOldAdminF ) == S_OK && ipNewCursor->NextFeature( &ipNewAdminF ) == S_OK )
			{
				CCityAdmin cOldAdmin( ipOldAdminF, sindyTableType::city_admin, false, m_cCityAdminFC.GetFieldMap(), m_cCityAdminFC.GetNameString() );
				CCityAdmin cNewAdmin( ipNewAdminF, sindyTableType::city_admin, false, m_cCityAdminFC.GetFieldMap(), m_cCityAdminFC.GetNameString() );

				// 行政界が持つ住所コード比較
				if( cOldAdmin.GetAddrCode(cRec.IsCharAddrCode()) != cNewAdmin.GetAddrCode(cRec.IsCharAddrCode()) )
					return kW_GP_MOVED_DIFF_ADMIN;	// 【要確認】移動前後で行政界の住所コードが異なる

				// ここからが移動処理
				// 属性値の設定
				cGou.SetValueByFieldName( ipc_table::kOperator, CComVariant(gf::GetCurAccount()) );			// 最終更新者
				cGou.SetValueByFieldName( ipc_table::kPurpose, CComVariant(cRec.m_lPurpose) );				// 作業目的
				cGou.SetValueByFieldName( ipc_table::kUpdateType, CComVariant(3L) );						// 最終更新内容(形状変更)
				cGou.SetValueByFieldName( ipc_table::kProgModifyDate, CComVariant(gf::GetCurDateStr()) );	// 最終プログラム更新日時
				cGou.SetValueByFieldName( ipc_table::kModifyProgName, CComVariant(PROGRAM_NAME) );	// 最終更新プログラム名
				cGou.SetValueByFieldName( ipc_table::kUserClaim, CComVariant(1L) );							// ユーザクレームフラグ
				cGou.SetValueByFieldName( ipc_table::kSource, CComVariant(cRec.m_strClaimNo) );				// 情報ソース
				cGou.SetValueByFieldName( gou_point::kPriority, CComVariant(1L) );							// 優先フラグ
				cGou.SetValueByFieldName( gou_point::kGouType, CComVariant(1L) );							// 号タイプ[Bug6318]
				cGou.SetShape( ipPoint );																	// 形状

				// 更新元DBを参照して、移動後としてあるべきポイントを取得、比較する (bug 9300)
				if( !checkGPByReferenceDB( cRec, cNewAdmin, cGou, MOVEMODE ) )
						return kE_MOVED_REF_FAIL;
		
			}
			else
				return kW_GP_MOVED_FAIL;
		}
		else
			return kW_GP_MOVED_FAIL;

		CErrorObjects cErrObjs;
		if( sindyErr_NoErr == cGou.Store(cErrObjs) )
			return bPriorF? kW_GP_MOVED : kW_GP_MOVED_NOT_PRIOR;
		else
			return kW_GP_MOVED_FAIL;
	}
	else
		return kW_GP_MOVED_FAIL;
}

// 号ポイント追加
EResultStat CGPHelper::insertPoint( CLRec& cRec )
{
	// フィーチャの作成と移動先ポイントの生成
	CRow cRow;
	CGouPoint cGou;
	cGou.SetObject( m_cGouPointFC.CreateRow(cRow), sindyTableType::gou_point, true, m_cGouPointFC.GetFieldMap(), m_cGouPointFC.GetNameString() );
	IPointPtr ipPoint = gf::CreateMXYPoint( cRec.m_cMXY );
	if( cGou && ipPoint )
	{
		// 座標位置の行政界を検索
		ISpatialFilterPtr ipSpFilter( AheInitSpatialFilter(NULL, ipPoint) );

		long lCount = m_cCityAdminFC.FeatureCount( ipSpFilter );
		IFeatureCursorPtr ipFeatureCursor( m_cCityAdminFC.Search(ipSpFilter, VARIANT_FALSE) );
		if( lCount == 1 && ipFeatureCursor )
		{
			IFeaturePtr	ipAdminFeature;
			ipFeatureCursor->NextFeature( &ipAdminFeature );

			CCityAdmin cAdmin( ipAdminFeature, sindyTableType::city_admin, false, m_cCityAdminFC.GetFieldMap(), m_cCityAdminFC.GetNameString() );

			CString	strGouNo;
			long lExFlag = 0;

			// 文字付き地番の場合を考慮
			CString strTmpGouNo1;
			if( cRec.IsCharPntCode() )
			{
				strTmpGouNo1 = CCodeConverter::GetCodeConverter().GetPntName(cRec.m_strAddrCodeNew.Left(11), cRec.m_strAddrCodeNew.Mid(11, 5));
				if( strTmpGouNo1.IsEmpty() )
					return kE_PNTNAME_GET_FAIL;	// 文字地番名称取得失敗
			}
			else
				strTmpGouNo1 = CString_TrimLeft0(cRec.m_strAddrCodeNew.Mid(11, 5));

			// 文字付き号の場合を考慮
			CString strTmpGouNo2;
			if( cRec.IsCharGouCode() )
			{
				strTmpGouNo2 = CCodeConverter::GetCodeConverter().GetPntName(cRec.m_strAddrCodeNew.Left(11), cRec.m_strAddrCodeNew.Mid(16, 4));
				if( strTmpGouNo2.IsEmpty() )
					return kE_GOUNAME_GET_FAIL;	// 文字号名称取得失敗
			}
			else
				strTmpGouNo2 = CString_TrimLeft0(cRec.m_strAddrCodeNew.Mid(16, 4));

			// 行政界の16ケタと一致するか？
			if( cAdmin.GetAddrCode(cRec.IsCharAddrCode()).GetLength() == 16 )
			{
				if( cRec.m_strAddrCodeNew.Left(16) != cAdmin.GetAddrCode(cRec.IsCharAddrCode()) )
				{
					lExFlag = 1;	// 拡張号として追加
					if( cRec.m_strAddrCodeNew.Mid(16, 4) != "0000" )
						strGouNo = strTmpGouNo1 + "-" + strTmpGouNo2;
					else
						strGouNo = strTmpGouNo1;
				}
				else
					strGouNo = strTmpGouNo2;
			}
			else
			{
				if( cRec.m_strAddrCodeNew.Mid(16, 4) != "0000" )
					strGouNo = strTmpGouNo1 + "-" + strTmpGouNo2;
				else
					strGouNo = strTmpGouNo1;
			}

			// 号番号に文字が含まれる場合(半角数字と[-]以外が入っていたら)は、先頭に[#]を挿入
			if( ! tr1::regex_match((const wchar_t*)strGouNo, g_re) )
				strGouNo = _T("#") + strGouNo;

			// 属性値の設定
			cGou.SetValueByFieldName( gou_point::kOperator, CComVariant(gf::GetCurAccount()) );
			cGou.SetValueByFieldName( gou_point::kPurpose, CComVariant(cRec.m_lPurpose) );
			cGou.SetValueByFieldName( gou_point::kUpdateType, CComVariant(1L) );
			cGou.SetValueByFieldName( gou_point::kProgModifyDate,	CComVariant(gf::GetCurDateStr()) );
			cGou.SetValueByFieldName( gou_point::kModifyProgName,	CComVariant(PROGRAM_NAME) );
			cGou.SetValueByFieldName( gou_point::kUserClaim, CComVariant(1L) );
			cGou.SetValueByFieldName( gou_point::kSource, CComVariant(cRec.m_strClaimNo) );
			cGou.SetValueByFieldName( gou_point::kPriority, CComVariant(1L) );
			cGou.SetValueByFieldName( gou_point::kExpGouNo, CComVariant(lExFlag) );
			cGou.SetValueByFieldName( gou_point::kGouType, CComVariant(1L) );
			cGou.SetValueByFieldName( gou_point::kGouNo, CComVariant(strGouNo) );
			cGou.SetShape( ipPoint );

			// 更新元DBを参照して、移動後としてあるべきポイントを取得、比較する (bug 9300)
			if( !checkGPByReferenceDB( cRec, cAdmin, cGou, INSERTMODE ) )
				return kE_INSERT_REF_FAIL;
			
			CErrorObjects cErrObjs;
			if( sindyErr_NoErr == cGou.Store(cErrObjs) )
			{
				cRec.m_lEditPointID = cGou.GetOID();
				cRec.m_lExFlag = lExFlag;
				return lExFlag? kW_GP_INSERT_EX : kW_GP_INSERT;
			}
			else
				return lExFlag? kE_GP_INSERT_EX_FAIL : kE_GP_INSERT_FAIL;
		}
	}

	return kE_GP_INSERT_FAIL;
}

/////////////////////////////////////
//	内部関数
/////////////////////////////////////

// チェックタイプごとの行政界Where句作成
CString CGPHelper::MakeWhereClause( const CLRec& cRec, ECheckType emCType )
{
	CString strWhere;
	strWhere.Format( _T("%s='%s'"), city_admin::kCityCode, cRec.m_strAddrCodeNew.Left(5) );
	switch( emCType )
	{
	case kCTAddr16:
		strWhere.AppendFormat( _T(" and %s='%s'"), city_admin::kAddrCode, cRec.m_strAddrCodeNew.Mid(5, 6) );
		if( !cRec.IsCharPntCode() )
			strWhere.AppendFormat( _T(" and %s=%d"), city_admin::kGaikuFugo, _tstol(cRec.m_strAddrCodeNew.Mid(11, 5)) );
		else
			strWhere.AppendFormat( _T(" and %s='%s'"), city_admin::kExtGaikuFugo, CCodeConverter::GetCodeConverter().GetPntName(cRec.m_strAddrCodeNew.Left(11), cRec.m_strAddrCodeNew.Mid(11, 5)) );
		break;
	case kCTAddr11:
		strWhere.AppendFormat( _T(" and %s='%s' and %s is null"), city_admin::kAddrCode, cRec.m_strAddrCodeNew.Mid(5, 6), city_admin::kGaikuFugo );
		if( cRec.IsCharAddrCode() )
			strWhere.AppendFormat( _T(" and %s is null"), city_admin::kExtGaikuFugo );
		break;
	case kCTAddrEX:
		strWhere.AppendFormat( _T(" and %s='%s'"), city_admin::kAddrCode, cRec.m_strAddrCodeNew.Mid(5, 6) );
		if( !cRec.IsCharAddrCode() )
			strWhere.AppendFormat( _T(" and %s is not null"), city_admin::kGaikuFugo );
		else
			strWhere.AppendFormat( _T(" and (%s is not null or %s is not null)"), city_admin::kGaikuFugo, city_admin::kExtGaikuFugo );
		break;
	case kCTAddrCM:
		strWhere.AppendFormat( _T(" and %s='%s'"), city_admin::kAddrCode2, cRec.m_strAddrCodeNew.Mid(5, 6) );
		break;
	default:
		return NULL;
	}

	return strWhere;
}

// 指定行政界内の号ポイント確認
EResultStat CGPHelper::findGPByAdmin( const CLRec& cRec, CSPGouPoint& rGou, ECheckType emCType )
{
	std::list<CSPGouPoint> listGou;		// 20桁一致+優先フラグなし
	std::list<CSPGouPoint> listPriorGou;	// 20桁一致+優先フラグ用

	CCityAdmin::const_rows_iterator itAdminEnd = m_cGous.end(sindyTableType::city_admin);
	for( CCityAdmin::const_rows_iterator itAdmin = m_cGous.begin(sindyTableType::city_admin); itAdmin != itAdminEnd; ++itAdmin )
	{
		const boost::shared_ptr<CCityAdmin> pAdmin = boost::static_pointer_cast<CCityAdmin>(*itAdmin);

		CGouPoint::const_rows_iterator itGouEnd = pAdmin->end(sindyTableType::gou_point);
		for( CGouPoint::const_rows_iterator itGou = pAdmin->begin(sindyTableType::gou_point); itGou != itGouEnd; ++itGou )
		{
			const boost::shared_ptr<CGouPoint> pGou = boost::static_pointer_cast<CGouPoint>(*itGou);

			// 11桁+拡張号検索時に拡張号でないなら飛ばす
			if( kCTAddrEX == emCType && !pGou->IsExGouNo() )
				continue;

			// 20桁コードが一致？
			CString strGouCode( pGou->GetAddrCode(cRec.IsCharAddrCode()) );
			if( strGouCode.GetLength() == 20 )
			{
				// 通称名特殊対応の場合、大字・字コードはADDRCODE2を採用
				if( kCTAddrCM == emCType )
					strGouCode.Format( _T("%s%s%s"), strGouCode.Left(5), pAdmin->GetOazaAzaCode2(), strGouCode.Mid(11) );

				if( strGouCode == cRec.m_strAddrCodeNew )
				{
					if( pGou->IsPrior() )	listPriorGou.push_back( pGou );
					else					listGou.push_back( pGou );
				}
			}
		}
	}

	// 優先フラグありと一致した？
	if( !listPriorGou.empty() )
	{
		rGou = *(listPriorGou.begin());
		return listPriorGou.size() == 1? kO_MATCH_PRIOR_GP : kW_GP_DUPLI_PRIOR;
	}
	else if( !listGou.empty() )	// 優先フラグなしと一致した？
	{
		rGou = *(listGou.begin());
		return kO_MATCH_GP;
	}
	else
		return kO_NOT_FOUND;
}

// レコードと一致した号ポイントの関係から詳細ステータス取得
EResultStat CGPHelper::getDetailStatus( CLRec& cRec, const CSPGouPoint& pGou, bool bAddMode )
{
	// 情報ソースは、無条件で取得するようにした
	CComVariant	vaValue;
	pGou->GetValueByFieldName(ipc_table::kSource, vaValue );
	cRec.m_strSource = (vaValue.vt == VT_BSTR)? CString(vaValue.bstrVal) : _T("");

	// 先にMeshXYも取得
	CMeshXY cTmpMXY;
	gf::ConvPoint2MXY( pGou->GetShapeCopy(), &cTmpMXY );

	// 優先フラグありなしで場合わけ
	if( pGou->IsPrior() )
	{
		if( bAddMode )
		{
			cRec.m_cMXY = cTmpMXY;
			return (1 == pGou->GetGouType())? kO_GP_ADDED_PRIOR : kW_GP_ADDED_PRIOR_TPG;
		}
		else
		{
			if( cRec.m_cMXY == cTmpMXY )
			{
				return (1 == pGou->GetGouType())? kO_GP_OK : kW_GP_PRIOR_TPG;
			}
			else
			{
				cRec.m_cMXY_Ref = cTmpMXY;
				cRec.m_lEditPointID = pGou->GetOID();
				return kW_GP_MOVED;
			}
		}
	}
	else
	{
		cRec.m_cMXY_Ref = cTmpMXY;
		cRec.m_lEditPointID = pGou->GetOID();
		return kW_GP_NOT_PRIOR;
	}
}

// 移動・追加予定ポイントと参照DBのポイントとの比較 (bug9300)
bool CGPHelper::checkGPByReferenceDB( CLRec& cRec, CCityAdmin& cAdmin, CGouPoint& cGou, bool bIsMovePoint )
{
	// 参照行政界との比較
	IPointPtr ipPointRefGou = gf::CreateMXYPoint( cRec.m_cMXY );	
	if( !ipPointRefGou )
		return false;
	
	// 座標位置の行政界を検索
	ISpatialFilterPtr ipRefAdminFilter( AheInitSpatialFilter(NULL, ipPointRefGou) );
	long lRefAdminCount = m_cCityAdminFC_Ref.FeatureCount( ipRefAdminFilter );
	if( lRefAdminCount != 1 )
		return false;

	IFeatureCursorPtr ipRefAdminCursor( m_cCityAdminFC_Ref.Search(ipRefAdminFilter, VARIANT_FALSE) );
	if( !ipRefAdminCursor )
		return false;

	IFeaturePtr	ipRefAdminFeature;
	if( S_OK == ipRefAdminCursor->NextFeature( &ipRefAdminFeature ) && ipRefAdminFeature )
	{
		CCityAdmin cRefAdmin( ipRefAdminFeature, sindyTableType::city_admin, false, m_cCityAdminFC_Ref.GetFieldMap(), m_cCityAdminFC_Ref.GetNameString() );

		// CITYCODEチェック
		if( 0 != cAdmin.GetCityCode().CompareNoCase( cRefAdmin.GetCityCode() ) )
			return false;

		// ADDRCODEチェック
		if( 0 != cAdmin.GetOazaAzaCode().CompareNoCase( cRefAdmin.GetOazaAzaCode() ) )
			return false;
		
		// EXTGAIKUFUGOチェック
		if( 0 != cAdmin.GetExGaikuFugo().CompareNoCase( cRefAdmin.GetExGaikuFugo() ) )
			return false;
			
		// ADDRCODE2チェック
		if( 0 != cAdmin.GetOazaAzaCode2().CompareNoCase( cRefAdmin.GetOazaAzaCode2() ) )
			return false;

		// ADDRCLASS_C は移動時のみチェック
		if( bIsMovePoint )
		{
			if( cAdmin.GetAddrClass() != cRefAdmin.GetAddrClass() )
				return false;
		}
	}
	else
		return false;
	

	// 参照ポイントとの比較
	ISpatialFilterPtr ipRefGouFilter( AheInitSpatialFilter(NULL, ipPointRefGou) );
	long lRefGouCount = m_cGouPointFC_Ref.FeatureCount(ipRefGouFilter);
	if( 1 > lRefGouCount )
		return false;

	IFeatureCursorPtr ipRefGouCursor( m_cGouPointFC_Ref.Search(ipRefGouFilter, VARIANT_FALSE) );
	if( !ipRefGouCursor )
		return false;
	
	IFeaturePtr ipRefGou;
	while( S_OK == ipRefGouCursor->NextFeature( &ipRefGou ) && ipRefGou )
	{
		CGouPoint cRefGou( sindyTableType::gou_point, ipRefGou );
			
		// PRIORITY_Fチェック
		if( cGou.IsPrior() == cRefGou.IsPrior() )
		{
			// GAIKUFUGO、GOU_NO、EXTGOU_Fチェック
			// 取得できる20桁コード(文字付考慮する)が一致するかどうかであわせて判定
			if( 0 == cGou.GetAddrCode(true).CompareNoCase( cRefGou.GetAddrCode(true) ) )
				// 両方のチェックが通るポイントが見つかればその時点でreturnする
				return true;
		}
	}

	// 号ポイントのチェック通らず
	return false;
}
