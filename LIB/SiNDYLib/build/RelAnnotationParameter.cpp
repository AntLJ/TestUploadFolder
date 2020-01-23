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

/**
 * @file RelAnnotationParameter.cpp
 * @brief <b>CRelAnnotationParameterクラス実装ファイル</b>
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @vertion $Id$
 */
#include "stdafx.h"
#include "RelAnnotationParameter.h"
#include "GlobalRules.h"
#include "GlobalFunctions.h"
#include "AnnoGlobals.h"
#include "Rule.h"
#include "ErrorObject.h"
#include "AheGeometryConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR NUMBER_OF_CHARACTERS   = _T("%s 文字数:%d");
	LPCTSTR STRINGS_ARE_DUPLICATE  = _T("(一つの真位置に紐付く表示文字列同士が重複)");
	LPCTSTR ANGLE                  = _T("角度[%.1lf]");
	LPCTSTR CHARACTERS_INFO        = _T("文字列:%s 文字数:%d 期待文字数:%d");
	LPCTSTR THRESHOLD_AND_DISTANCE = _T("%s:%d 閾値：%.20lf-%.20lf 距離：%.20lf（%.1lfpx）");
	LPCTSTR CLASS_CODE             = _T("%s:%d 種別コード:%d 種別:%s");
	LPCTSTR CITY_MESH              = _T("都市地図");
	LPCTSTR DISPPOINT_SHAPE_NULL   = _T("表示位置の形状がNULL、又は1点しかありません");
#else
	LPCTSTR NUMBER_OF_CHARACTERS   = _T("%s number of characters:%d");
	LPCTSTR STRINGS_ARE_DUPLICATE  = _T("(Display character strings associated with one true position are duplicate)");
	LPCTSTR ANGLE                  = _T("Angle[%.1lf]");
	LPCTSTR CHARACTERS_INFO        = _T("Character string:%s Number of characters:%d Expected number of characters:%d");
	LPCTSTR THRESHOLD_AND_DISTANCE = _T("%s:%d Threshold：%.20lf-%.20lf Distance：%.20lf(%.1lfpx)");
	LPCTSTR CLASS_CODE             = _T("%s:%d Class code:%d Class %s");
	LPCTSTR CITY_MESH              = _T("City mesh");
	LPCTSTR DISPPOINT_SHAPE_NULL   = _T("The shape of display point is NULL or only one point");
#endif // SINDY_FOR_ENGLISH
} // namespace

namespace sindy {
using namespace errorcode;

#ifdef SINDYLIB_CALC_TIME
double g_dCRelAnnotationParameter_CopyTime = 0.0;		//!< オブジェクトコピー時間
double g_dCreateParameterCache_AllTime = 0.0;			//!< CreateParameterCacheの総時間
double g_dCreateParameterCache_RuleCacheTime = 0.0;		//!< CreateParameterCacheのルールキャッシュ時間
double g_dCreateParameterCache_AnnotationTime = 0.0;	//!< CreateParameterCacheの注記真位置記号矩形作成時間
double g_dCreateParameterCache_DispLineTime = 0.0;		//!< CreateParameterCacheの注記表示位置バウンディングボックス作成時間

void CRelAnnotationParameter::TraceTime()
{
	TRACEMESSAGE( _T("                          ======================================================\n"));
	TRACEMESSAGE( _T("CRelAnnotationParameter : 計測時間結果 :\n") );
	TRACEMESSAGE( _T("                          ------------------------------------------------------\n"));
	TRACEMESSAGE( _T("                        : CreateParameterCache 総時間             : %.5lf秒\n"), g_dCreateParameterCache_AllTime/1000.0 );
	TRACEMESSAGE( _T("                        : CreateParameterCache ルールキャッシュ   :   %.5lf秒\n"), g_dCreateParameterCache_RuleCacheTime/1000.0 );
	TRACEMESSAGE( _T("                        : CreateParameterCache 真位置キャッシュ   :   %.5lf秒\n"), g_dCreateParameterCache_AnnotationTime/1000.0 );
	TRACEMESSAGE( _T("                        : CreateParameterCache 表示位置キャッシュ :   %.5lf秒\n"), g_dCreateParameterCache_DispLineTime/1000.0 );
	TRACEMESSAGE( _T("                          ------------------------------------------------------\n"));
	TRACEMESSAGE( _T("CRelAnnotationParameter : CopyCRelAnnotationParameter 総時間      : %.5lf秒\n"), g_dCRelAnnotationParameter_CopyTime/1000.0 );
	TRACEMESSAGE( _T("                          ======================================================\n"));
}
#endif // ifdef SINDYLIB_CALC_TIME

void CRelAnnotationParameter::DeleteCache( bool bRecursive/* = false*/ )
{
	CRelAnnotation::DeleteCache(bRecursive);
	DeleteParameter();
}

// メンバ変数を初期化する
void CRelAnnotationParameter::Init()
{
	m_bIsCached = false;
}

// パラメータキャッシュを削除する
void CRelAnnotationParameter::DeleteParameter()
{
	m_listAnnoRect.clear();
	for( iterator it = begin(); it != end(); ++it )
	{
		for( rows_iterator itRows = it->second.begin(); itRows != it->second.end(); ++itRows )
			static_cast<CDispLine&>(**itRows).ClearParameterCache();
	}
	m_pAnnoRule = m_pAnnoRule2 = NULL;
	m_bIsCached = false;
}

// コピーする
sindyErrCode CRelAnnotationParameter::CopyTo( CRow& dest, bool bIsCopyCache/*=true*/, bool bIsCopyRow/*=true*/, bool bIsCopyFlag/*=true*/, bool bIsCopyContainer/*=true*/ ) const
{
#ifdef SINDYLIB_CALC_TIME
	double dStartFunc = GetTickCount();
#endif // SINDYLIB_CALC_TIME

	// CRelAnnotationParameter以外は通さない
	CRelAnnotationParameter* pDest = dynamic_cast<CRelAnnotationParameter*>( &dest );
	if( ! pDest )
		return sindyErr_ArgIsConsistency;

	sindyErrCode emErr = CRelAnnotation::CopyTo( dest, bIsCopyCache, bIsCopyRow, bIsCopyFlag, bIsCopyContainer );
	pDest->m_bIsCached = m_bIsCached;
	pDest->m_pAnnoRule = m_pAnnoRule;
	pDest->m_pAnnoRule2 = m_pAnnoRule2;
	pDest->m_listAnnoRect = m_listAnnoRect;

#ifdef SINDYLIB_CALC_TIME
	g_dCRelAnnotationParameter_CopyTime += (double)GetTickCount() - dStartFunc;
#endif // SINDYLIB_CALC_TIME

	return emErr;
}

// パラメータキャッシュを作成する
sindyErrCode CRelAnnotationParameter::UpdateParameter( bool bForce/* = false */ )
{
	sindyErrCode emErr = sindyErr_NoErr; // 戻り値

#ifdef SINDYLIB_CALC_TIME
	double dStartFunc = GetTickCount();
#endif // ifdef SINDYLIB_CALC_TIME

	static const bool bCalcFlagGeom = false; // 旗揚げ形状が欲しい場合はCDispLine::CalcStringBoundsを個別に呼ぶこと

	if( bForce )
		DeleteParameter();

	if( ! IsCreatedParameterCache() )
	{
		const annotation_class::CAnnotationClass* pRule = GetRule();
		if( pRule )
		{
#ifdef SINDYLIB_CALC_TIME
		g_dCreateParameterCache_RuleCacheTime += (double)GetTickCount() - dStartFunc;
		double dStartAnno = GetTickCount();
#endif // ifdef SINDYLIB_CALC_TIME

			// 真位置矩形キャッシュ
			for( int nDispLayerCount = 0; nDispLayerCount < GetDispLayerCount(); ++nDispLayerCount )
			{
				WKSEnvelope cRect;
				sindyTableType::ECode emDispLayerType = GetDispLayer( nDispLayerCount );
				emErr = CalcSymbolBounds( CalcMarkLength( emDispLayerType, true ), CalcMarkLength( emDispLayerType, false ), cRect );
				if( sindyErr_NoErr != emErr )
					ERRORLOG( emErr );
				m_listAnnoRect.push_back( cRect );
			}
			
#ifdef SINDYLIB_CALC_TIME
			g_dCreateParameterCache_AnnotationTime += (double)GetTickCount() - dStartAnno;
			double dStartDisp = GetTickCount();
#endif // ifdef SINDYLIB_CALC_TIME

			// 表示位置矩形・形状キャッシュ TODO: CDispLineに実装するほうが良い
			long lSecondLineStartIndex;
			WKSEnvelope cLineRect = {0.0}, cStringRect = {0.0};
			sindyAnnotationLineType emLineType = (sindyAnnotationLineType)-1;

			for( const_iterator it = begin(); it != end(); ++it )
			{
				for( const_rows_iterator itDisp = it->second.begin(); itDisp != it->second.end(); ++itDisp )
				{
					IGeometryPtr ipGeom;
					CDispLine& cDisp = *static_cast<CDispLine*>(itDisp->get());
					int nPointCount = cDisp.GetShape()->GetPointCount();
					WKSPoint* pStringRects = NULL;
					LOGASSERTEM_IF( nPointCount > 1, sindyErr_GeometryNotFound, DISPPOINT_SHAPE_NULL ) // 形状が無いデータが来る場合がある（bug 5300） TODO: OIDを出せるようにしておくこと
					{
						pStringRects = new WKSPoint[(nPointCount-1)*5]; // 最後の点は含まない
						// 表示位置パラメータ計算
						emLineType = cDisp.CalcParameter( cDisp.IsAltitudeValue( GetAnnoCode() ), &lSecondLineStartIndex, pStringRects, &cLineRect, &ipGeom );
						// 表示タイプが線状の場合はラインタイプも線状とする（bug 6537）
						if( pRule->GetDispType() == schema::annotation_class::disptype_rule::kLinear )
							emLineType = sindyAnnoLine_Linear;
					}
					// 旗揚げする種別かどうか
					bool bIsFlag = ( pRule->GetDispType() == schema::annotation_class::disptype_rule::kFlag ) ? true : false;

					// コピー可能かどうか
					bool bCanCopy = pRule ->GetCopyPaste( cDisp.GetTableType() );

					// 旗揚げの場合は旗揚げ形状を付与
					if( bCalcFlagGeom && bIsFlag )
					{
						emErr = cDisp.CalcStringBounds( (IPoint*)GetShape()->GetPoint(0), ipGeom ); // TODO: 真位置ポイント無駄…
						if( sindyErr_NoErr != emErr )
							ERRORLOG( emErr );
					}
					// 表示位置バウンディングボックスの矩形を取得
					if( ipGeom )
					{
						IEnvelopePtr ipEnv;
						ipGeom->get_Envelope( &ipEnv );
						ipEnv->QueryCoords( &cStringRect.XMin, &cStringRect.YMin, &cStringRect.XMax, &cStringRect.YMax );
					}
					// キャッシュをセット
					cDisp.SetParameterCache( nPointCount - 1, lSecondLineStartIndex, pStringRects, cStringRect, ipGeom, emLineType, bIsFlag, bCanCopy );
				}
			}
#ifdef SINDYLIB_CALC_TIME
			g_dCreateParameterCache_DispLineTime += (double)GetTickCount() - dStartDisp;
#endif // ifdef SINDYLIB_CALC_TIME

			// キャッシュ終了
			m_bIsCached = true;
		}
		else {
			emErr = sindyErr_RuleCantCached;
			ERRORLOG( emErr );	// ルールキャッシュが作成されていない
			SASSERT( m_pAnnoRule );
		}
	}

#ifdef SINDYLIB_CALC_TIME
	g_dCreateParameterCache_AllTime += (double)GetTickCount() - dStartFunc;
#endif // ifdef SINDYLIB_CALC_TIME

	return emErr;
}

// 真位置と表示位置のセットでの単体・複合チェックを行う
//sindyErrCode CRelAnnotationParameter::Check( std::list<CErrorObject>& listErr, bool bDoUnitCheck/* = true*/, bool bDoComplexCheck/* = true*/, double dGapThreshold/* = 0.5*/ ) const
sindyErrCode CRelAnnotationParameter::CheckLogic( CErrorObjects& cErrObjects, bool bDoUnitCheck/* = true*/, bool bDoComplexCheck/* = true*/, const double& dThresholdMinPx/* = 0.0*/, const double& dThresholdMaxPx/* = 16.0*/ ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	if( g_bDisableCheckToNoSubstance )
	{
		// 実体がないもの（新規作成されたものを除く）は
		// チェック対象から外す bug 6779
		if( ! ( p || m_bCreated ) )
			return sindyErr_NoErr;
	}

	if( bDoUnitCheck && bDoComplexCheck )
	{
		const annotation_class::CAnnotationClass* pRule = GetValidRule(); // 種別単位のルール

		// 真位置
		if( bDoUnitCheck )
		{
			// 真位置単体チェック
			TCHAR lpszNameString[_NAMESTRING_MAX_LEN+1];
			if( sindyErr_NoErr != ( emErr = CheckString1() ) )		cErrObjects.push_back( CErrorObject( *this, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, GetDispScale(0), NUMBER_OF_CHARACTERS, GetNameString1( _NAMESTRING_MAX_LEN, lpszNameString ), GetStringNum1() ) ) );
			if( sindyErr_NoErr != ( emErr = CheckString2() ) )		cErrObjects.push_back( CErrorObject( *this, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, GetDispScale(0), NUMBER_OF_CHARACTERS, GetNameString2( _NAMESTRING_MAX_LEN, lpszNameString ), GetStringNum2() ) ) );
			if( sindyErr_NoErr != ( emErr = CheckString3() ) )		cErrObjects.push_back( CErrorObject( *this, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, GetDispScale(0), NUMBER_OF_CHARACTERS, GetNameString3( _NAMESTRING_MAX_LEN, lpszNameString ), GetStringNum3() ) ) );
//			if( sindyErr_NoErr != ( emErr = CheckStringLen() ) )	cErrObjects.push_back( CErrorObject( *this, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, GetDispScale(0) ) ) ); <- [bug 3035] いらない
			if( sindyErr_NoErr != ( emErr = CheckAnnoCode() ) )		cErrObjects.push_back( CErrorObject( *this, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, GetDispScale(0) ) ) );
			if( sindyErr_NoErr != ( emErr = CheckDispType() ) )		cErrObjects.push_back( CErrorObject( *this, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, GetDispScale(0) ) ) );//, _T("%s:%d 種別コード:%d 種別:%s 表示ルール:%d 表示タイプ:%d"), GetAnnoCode(), AheAnnoCode2CodeName( GetAnnoCode() ), CalcDispRuleCode( GetTableType() ), GetDispTypeCode( GetTableType() ) ) ) );

		}

		// 表示位置
		bool bHasS5 = false, bHasS4 = false, bHasS3 = false, bHasS2 = false, bHasS1 = false;			// 各スケール用の表示位置存在確認用（中抜けチェック・表示種別チェック用）
		bool bHasS4MtName = false, bHasS3MtName = false, bHasS2MtName = false, bHasS1MtName = false;	// 山岳名称存在確認用（種別が山岳の場合のみ使用、超特別対応！！！ [bug 4049]）
		bool bHasS4MtAlt  = false, bHasS3MtAlt  = false, bHasS2MtAlt  = false, bHasS1MtAlt  = false;	// 山岳標高存在確認用（種別が山岳の場合のみ使用、超特別対応！！！ [bug 4049]）
		TCHAR strNameString[_NAMESTRING_MAX_LEN+1] = _T("");						// 対応文字列格納バッファ

		schema::annotation::annotation_code::ECode emAnnoCode = GetAnnoCode();	// 注記種別
		for( const_iterator it = begin(); it != end(); ++it )
		{
			//-------------------------------------------------------------------------------------------
			// [bug 9221],[bug 9223]対応で追加
			for( const_rows_iterator itDispSrc = it->second.begin(); itDispSrc != it->second.end(); ++itDispSrc )
			{
				const CDispLine& cDispSrc( static_cast<const CDispLine&>(**itDispSrc) ); // 比較元の表示ライン
				IGeometryPtr	ipGeomSrc = cDispSrc.GetStringBounds();

				IRelationalOperatorPtr ipRelOpe(ipGeomSrc);

				for( const_rows_iterator itDispTgt = itDispSrc; itDispTgt != it->second.end(); ++itDispTgt )
				{
					if( itDispTgt == itDispSrc ) continue;
					const CDispLine& cDispTgt( static_cast<const CDispLine&>(**itDispTgt) ); // 比較先の表示ライン
					VARIANT_BOOL	vOverlap;
					LOGASSERTE_IF( SUCCEEDED( ipRelOpe->Disjoint ( cDispTgt.GetStringBounds(), &vOverlap ) ), sindyErr_GeometryFunctionFailed )
					{
						if( vOverlap == VARIANT_FALSE ) {
							cErrObjects.push_back( CErrorObject( cDispSrc, CErrorInfo( sindyErr_AnnoDispBoundsIsOverlaped, sindyErrLevel_ERR, NULL, NULL, cDispSrc.GetMaxScale(), STRINGS_ARE_DUPLICATE ) ) );
						}
					}
				}
			}
			//-------------------------------------------------------------------------------------------
			for( const_rows_iterator itDisp = it->second.begin(); itDisp != it->second.end(); ++itDisp )
			{
				const CDispLine& cDisp( static_cast<const CDispLine&>(**itDisp) );
				// 表示位置単体チェック 
				if( bDoUnitCheck )
				{
					double dErrAngle = 0.0;
					if( sindyErr_NoErr != ( emErr = cDisp.CheckAnnoID() ) )									cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, cDisp.GetMaxScale(), _T("AnnoID[%d]"), cDisp.GetAnnoID() ) ) );
					if( sindyErr_NoErr != ( emErr = cDisp.CheckTargetStr() ) )								cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, cDisp.GetMaxScale(), _T("TargetStr[%d]"), cDisp.GetTargetStr() ) ) );
					if( sindyErr_NoErr != ( emErr = cDisp.CheckAngle( cDisp.m_emLineType, &dErrAngle ) ) )	cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, cDisp.GetMaxScale(), ANGLE, dErrAngle ) ) );
					if( sindyErr_NoErr != ( emErr = cDisp.CheckLine( pRule->GetDispType() ) ) )             cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, cDisp.GetMaxScale() ) ) ); // (bug 6537)
				}

				// 表示文字列取得
				schema::disp_line::target_str::ECode emTargetStrCode = cDisp.GetTargetStr();	// 表示文字番号
				CAnnotation::GetNameString( emTargetStrCode, _NAMESTRING_MAX_LEN, strNameString );

				// フォントサイズチェック（以前は単体チェックに入っていたが、表示文字列が必要なので、複合チェックに移動）
				if( sindyErr_NoErr != ( emErr = cDisp.CheckFontSize( strNameString ) ) ) cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( emErr, ( sindyErr_AnnoDispFontSizeInvalidWarn == emErr ) ? sindyErrLevel_WARN : sindyErrLevel_ERR, NULL, -1, cDisp.GetMaxScale(), _T("%d"), cDisp.GetFontSize() ) ) );

				// 文字数と構成点数が合っているかどうかチェック（bug 3778）
				// 国道・県道番号は常に1文字として数える
				long nVertexNum = cDisp.GetShape()->GetPointCount();
				long nStringNum = CString_GetLength2( CString(strNameString) );

				switch( emAnnoCode )
				{
					case schema::annotation::annotation_code::kNationalRouteNumber:
					case schema::annotation::annotation_code::kMainPrefRouteNumber: nStringNum = 1; break;
					default: break;
				}
				if( sindyErr_NoErr != ( emErr = ( nStringNum != nVertexNum - 1 ) ? sindyErr_AnnoStringNumNotEqualVertexNum : sindyErr_NoErr ) ) 
					cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1,cDisp.GetMaxScale(), CHARACTERS_INFO, strNameString, nStringNum, nVertexNum-1 ) ) );

				// 中抜けチェック（都市地図以外）
				// ついでにログ表示用のS1～S4の文字列も作る
				switch( cDisp.GetTableType() )
				{
					case sindyTableType::city_disp_line: bHasS5 = true; break;
					case sindyTableType::c_sc1disp_line: bHasS5 = true; break;
					case sindyTableType::b_sc4disp_line:
					case sindyTableType::m_sc4disp_line:
					case sindyTableType::t_sc4disp_line:
						bHasS4 = true;
						if( schema::annotation::annotation_code::kMountain == emAnnoCode )		// 山岳名称存在確認用（種別が山岳の場合のみ使用、超特別対応！！！ [bug 4049]）
						{
							switch( emTargetStrCode )
							{
								case schema::disp_line::target_str::kStr1:	bHasS4MtName = true; break;
								case schema::disp_line::target_str::kStr3: bHasS4MtAlt  = true; break;
								default: break;
							}
						}
						break;
					case sindyTableType::b_sc3disp_line:
					case sindyTableType::m_sc3disp_line:
					case sindyTableType::t_sc3disp_line:
						bHasS3 = true;
						if( schema::annotation::annotation_code::kMountain == emAnnoCode )		// 山岳名称存在確認用（種別が山岳の場合のみ使用、超特別対応！！！ [bug 4049]）
						{
							switch( emTargetStrCode )
							{
								case schema::disp_line::target_str::kStr1:	bHasS3MtName = true; break;
								case schema::disp_line::target_str::kStr3: bHasS3MtAlt  = true; break;
								default: break;
							}
						}
						break;
					case sindyTableType::b_sc2disp_line:
					case sindyTableType::m_sc2disp_line:
					case sindyTableType::t_sc2disp_line:
						bHasS2 = true;
						if( schema::annotation::annotation_code::kMountain == emAnnoCode )		// 山岳名称存在確認用（種別が山岳の場合のみ使用、超特別対応！！！ [bug 4049]）
						{
							switch( emTargetStrCode )
							{
								case schema::disp_line::target_str::kStr1:	bHasS2MtName = true; break;
								case schema::disp_line::target_str::kStr3: bHasS2MtAlt  = true; break;
								default: break;
							}
						}
						break;
					case sindyTableType::b_sc1disp_line:
					case sindyTableType::m_sc1disp_line:
					case sindyTableType::t_sc1disp_line:
						bHasS1 = true;
						if( schema::annotation::annotation_code::kMountain == emAnnoCode )		// 山岳名称存在確認用（種別が山岳の場合のみ使用、超特別対応！！！ [bug 4049]）
						{
							switch( emTargetStrCode )
							{
								case schema::disp_line::target_str::kStr1:	bHasS1MtName = true; break;
								case schema::disp_line::target_str::kStr3: bHasS1MtAlt  = true; break;
								default: break;
							}
						}
						break;

					default: break;
				}

				// 真位置から表示文字列ポリゴンまでの距離チェック（記号表示できるものは全て）
				if( CanShowMark( cDisp.GetTableType() ) )
				{
					double dDistance = 0.0;
					if( sindyErr_NoErr != ( emErr = CheckToStringBoundsLength( cDisp.m_ipGeom, cDisp.GetTableType(), dThresholdMinPx, dThresholdMaxPx, &dDistance ) ) )
					{
						// ログのために閾値を計算（CAnnotation::CheckToStringBoundsLengthより）
						double d1PXLen = AheCalcDispLineType21PXLen( cDisp.GetTableType(), true );
						double dMarkHalfLen = ( d1PXLen * (double)CalcMarkSize( cDisp.GetTableType() ) ) / (double)2.0;	// 記号矩形の半分の距離（基本）
						double dThresholdMin = dMarkHalfLen + dThresholdMinPx * d1PXLen;			// 最小閾値
						double dThresholdMax = dMarkHalfLen + dThresholdMaxPx * d1PXLen;			// 最大閾値
						double dDifferencePx = ( dDistance - dMarkHalfLen ) / AheCalcDispLineType21PXLen( cDisp.GetTableType(), true );	// 差（ピクセル）
						cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, cDisp.GetMaxScale(), THRESHOLD_AND_DISTANCE, GetTableName(), GetOID(), dThresholdMin, dThresholdMax, dDistance, dDifferencePx ) ) );
					}
				}
				// 文字列のみでコピー不可、且つ都市地図の場合は文字列ポリゴンの中に真位置があるかどうかチェック
				// 旗揚げの場合は文字列ポリゴンの中に真位置がないかどうかチェック
				bool bCanShowStrOnly = CanShowStrOnly( cDisp.GetTableType() );  // <- これってルールじゃなくて、今の属性だよね？ここはルールでチェックすべき <- コメントアウトになってたけど、今の属性じゃないとだめでしょ？？？
	/*			bool bCanShowStrOnly = false;	// 文字列のみの表示ができるルールかどうかのフラグ
				switch( CalcDispRuleCode( cDisp.GetTableType() ) )
				{
					case annotation_class::scaledisprule_code::kOXOXX:
					case annotation_class::scaledisprule_code::kStrOnly:
					case annotation_class::scaledisprule_code::kStrOnlyMust: bCanShowStrOnly = true; break;
					default: bCanShowStrOnly = false; break;
				}*/
				if( ( bCanShowStrOnly && ! cDisp.m_bCanCopy && ( cDisp.GetTableType() == sindyTableType::city_disp_line /* || cDisp.GetTableType() == sindyTableType::c_sc1disp_line*/ /* || cDisp.GetTableType() == sindyTableType::b_sc4disp_line || cDisp.GetTableType() == sindyTableType::m_sc4disp_line [bug 3035] いらない*/ ) ) || 
					cDisp.m_bIsFlag )
				{
					IRelationalOperatorPtr ipRelOpe;
					LOGASSERTE_IF( SUCCEEDED( cDisp.m_ipGeom->QueryInterface( IID_IRelationalOperator, (void**)&ipRelOpe ) ), sindyErr_COMQueryInterfaceFailed )
					{
						VARIANT_BOOL vbDisjoint = VARIANT_FALSE;
						LOGASSERTE_IF( SUCCEEDED( ipRelOpe->Disjoint( (IGeometryPtr)((IPoint*)GetShape()->GetPoint(0)), &vbDisjoint ) ), sindyErr_GeometryFunctionFailed )
						{
							// 旗揚げも文字列のみなので、先に旗揚げをチェックする必要がある
							// 都市地図の場合は、旗揚げの種別は文字矩形で囲むのみで、文字列内に真位置が存在するので下のチェック（ルールがおかしいと思う…）
							if( cDisp.m_bIsFlag && cDisp.GetTableType() != sindyTableType::city_disp_line )
							{
								if( !vbDisjoint )
								{
									emErr = sindyErr_AnnoFlagButStringBoundsInside;
									cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, cDisp.GetMaxScale(), CLASS_CODE, GetTableName(), GetOID(), GetAnnoCode(), AheAnnoCode2CodeName( GetAnnoCode() ) ) ) );
								}
							}
							else if( bCanShowStrOnly )
							{
								if( vbDisjoint )
								{
									emErr = sindyErr_AnnoNoMarkButStringBoundsOutside;
									cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, cDisp.GetMaxScale(), CLASS_CODE, GetTableName(), GetOID(), GetAnnoCode(), AheAnnoCode2CodeName( GetAnnoCode() ) ) ) );
								}
							}
						}
					}
				}
			}
		}
		// 中抜けチェック（都市地図以外）
		if( GetTableType() != sindyTableType::city_annotation )
		{
			LOGASSERTE_IF( pRule, sindyErr_RuleNotFound )
			{
				if( ! pRule->GetExceptScale( m_emTableType ) )
				{
					if( ( schema::annotation::annotation_code::kMountain != emAnnoCode && ( ( !bHasS4 && ( bHasS3 || bHasS2 || bHasS1 ) ) || ( bHasS4 && !bHasS3 && ( bHasS2 || bHasS1 ) ) || ( bHasS4 && bHasS3 && (! bHasS2 ) && bHasS1 ) ) ) ||
						( schema::annotation::annotation_code::kMountain == emAnnoCode && ( ( !bHasS4MtName && ( bHasS3MtName || bHasS2MtName || bHasS1MtName ) ) || ( bHasS4MtName && !bHasS3MtName && ( bHasS2MtName || bHasS1MtName ) ) || ( bHasS4MtName && bHasS3MtName && (! bHasS2MtName ) && bHasS1MtName ) ) ) ||
						( schema::annotation::annotation_code::kMountain == emAnnoCode && ( ( !bHasS4MtAlt  && ( bHasS3MtAlt  || bHasS2MtAlt  || bHasS1MtAlt  ) ) || ( bHasS4MtAlt  && !bHasS3MtAlt  && ( bHasS2MtAlt  || bHasS1MtAlt  ) ) || ( bHasS4MtAlt  && bHasS3MtAlt  && (! bHasS2MtAlt  ) && bHasS1MtAlt  ) ) ) )
					{
						CString strMsg;
						if( schema::annotation::annotation_code::kMountain != emAnnoCode )
						{
							if( !bHasS4 && (  bHasS3 ||   bHasS2 || bHasS1 ) ) strMsg = _T("S4");
							else if(  bHasS4 &&   !bHasS3 && ( bHasS2 || bHasS1 ) ) strMsg = _T("S3");
							else if( bHasS4 && bHasS3 && (! bHasS2 ) && bHasS1 ) strMsg = _T("S2");
						}
						else {
							if( ( !bHasS4MtName && (  bHasS3MtName || bHasS2MtName || bHasS1MtName ) ) || ( !bHasS4MtAlt && (  bHasS3MtAlt || bHasS2MtAlt || bHasS1MtAlt ) ) ) strMsg = _T("S4");
							else if( ( bHasS4MtName && !bHasS3MtName && ( bHasS2MtName || bHasS1MtName ) ) || ( bHasS4MtAlt && !bHasS3MtAlt && ( bHasS2MtAlt || bHasS1MtAlt ) ) ) strMsg = _T("S3");
							else if( ( bHasS4MtName && bHasS3MtName && (! bHasS2MtName ) && bHasS1MtName ) || ( bHasS4MtAlt && bHasS3MtAlt && (! bHasS2MtAlt ) && bHasS1MtAlt ) ) strMsg = _T("S2");
						}
						emErr = sindyErr_AnnoDispExceptExist;
						cErrObjects.push_back( CErrorObject( *this, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, GetDispScale(0), _T("%s"), strMsg ) ) );
					}
				}
			}
		}

		// 真位置で記号のみあるいは表示無しになっているのに、文字列ラインが存在するものがないかどうかチェックする
		// 文字列ラインがないのに真位置で文字列のみや記号＋文字列になっているものがないかどうかチェックする [bug 4000]
		for( scale_iterator itScale = scale_begin(); itScale != scale_end(); ++itScale )
		{
			bool bHasDispLine = false;
			int nShowLevel = 0;
			CString strMsg;

			switch( *itScale )
			{
				case sindyTableType::b_sc1disp_line:
				case sindyTableType::m_sc1disp_line:
				case sindyTableType::t_sc1disp_line: bHasDispLine = bHasS1; nShowLevel = 3; strMsg = _T("S1"); break;
				case sindyTableType::b_sc2disp_line:
				case sindyTableType::m_sc2disp_line:
				case sindyTableType::t_sc2disp_line: bHasDispLine = bHasS2; nShowLevel = 2; strMsg = _T("S2"); break;
				case sindyTableType::b_sc3disp_line:
				case sindyTableType::m_sc3disp_line:
				case sindyTableType::t_sc3disp_line: bHasDispLine = bHasS3; nShowLevel = 1; strMsg = _T("S3"); break;
				case sindyTableType::b_sc4disp_line:
				case sindyTableType::m_sc4disp_line:
				case sindyTableType::t_sc4disp_line: bHasDispLine = bHasS4; nShowLevel = 0; strMsg = _T("S4"); break;
				case sindyTableType::c_sc1disp_line: bHasDispLine = bHasS5; nShowLevel = 0; strMsg = _T("D1"); break;
				case sindyTableType::city_disp_line: bHasDispLine = bHasS5; nShowLevel = 0; strMsg = CITY_MESH; break;
					break;
				default:
					_ASSERTE(false);
					break;
			}
			schema::annotation::disp_type::ECode emDispTypeCode = GetDispType( *itScale );	// 表示種別取得

			// 真位置で記号のみあるいは表示無しになっているのに、文字列ラインが存在するかどうかのチェック
			if( ( schema::annotation::disp_type::kSym == emDispTypeCode || schema::annotation::disp_type::kNone == emDispTypeCode ) && bHasDispLine )
			{
				emErr = sindyErr_AnnoDispLineShouldNotExist;
				cErrObjects.push_back( CErrorObject( *this, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, GetDispScale(nShowLevel), _T("%s"), strMsg ) ) );
			}
			// 文字列ラインがないのに真位置で文字列のみや記号＋文字列になっているものがないかどうかチェックする
			if( ( schema::annotation::disp_type::kStr == emDispTypeCode || schema::annotation::disp_type::kSymStr == emDispTypeCode || schema::annotation::disp_type::kPoiStr == emDispTypeCode ) && ( ! bHasDispLine ) )
			{
				emErr = sindyErr_AnnoDispLineShouldExist;
				cErrObjects.push_back( CErrorObject( *this, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, GetDispScale(nShowLevel), _T("%s"), strMsg ) ) );
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

extern IGeometryPtr _WKSPoints2Polygon( long lPointCount, WKSPoint* pPoints, ISpatialReference* ipSpRef );

// 現在格納されているフィーチャの形状のコピーを返す
IGeometryPtr CRelAnnotationParameter::GetShapeCopy( bool bLatest/* = true*/, bool bOriginal/* = false*/, bool bFeatureChangedOriginal/* = false*/, bool bDiff/* = false*/, long lParam/* = -1*/ ) const
{
	// スケールが指定されていない場合はCRow::GetShapeCopyに処理をまわす
	if( 0 > lParam )
		return CRow::GetShapeCopy( bLatest, bOriginal, bFeatureChangedOriginal, bDiff );

	// 記号形状・文字列ポリゴンを返す
	sindyTableType::ECode emDispTableType = sindyTableType::unknown; // 対象の表示位置
	long lIndex = 0; // 記号矩形のインデックス
	for( scale_iterator it = scale_begin(); it != scale_end(); ++it, ++lIndex )
	{
		if( AheTableType2AnnotationScale( *it ) == (sindyAnnotationScale)lParam )
		{
			emDispTableType = *it;
			break;
		}
	}

	if( emDispTableType == sindyTableType::unknown ) // lParamと真位置がマッチしない場合（e.x. lParam=3125, table type=base_annotation）
		return IGeometryPtr();

	ISpatialReferencePtr ipSpRef( GetShape()->GetSpatialReference() );
	IGeometryCollectionPtr ipEnumGeom( CLSID_GeometryBag ); // ConstructUnion用
	((IGeometryPtr)ipEnumGeom)->putref_SpatialReference( ipSpRef );

	// 真位置矩形ポリゴン	
	IGeometryPtr ipAnnoPoly;
	{
		annorect_const_iterator it = annorect_begin();
		std::advance( it, lIndex );
		IEnvelopePtr ipEnv( CLSID_Envelope );
		ipEnv->putref_SpatialReference( ipSpRef );
		ATLTRY( SUCCEEDED( ipEnv->PutWKSCoords( const_cast<WKSEnvelope*>(&*it) ) ) );
		ipAnnoPoly = ToPolygon( ipEnv );
		ATLTRY( SUCCEEDED( ipEnumGeom->AddGeometry( ipAnnoPoly ) ) ); // 追加
	}
	// 表示位置ポリゴン
	const_iterator it = find( emDispTableType );
	if( it != end() )
	{
		for( const_rows_iterator itDisp = it->second.begin(); itDisp != it->second.end(); ++itDisp )
		{
			const CDispLine& cDisp = static_cast<const CDispLine&>(**itDisp);
			ATLTRY( SUCCEEDED( ipEnumGeom->AddGeometry( _WKSPoints2Polygon( cDisp.m_nStringRectsSize * 5, cDisp.m_pStringRects, ipSpRef ) ) ) ); // 追加
		}
	}
	IGeometryPtr ipRet( AheCreateClone( ipAnnoPoly ) ); // 便宜的に真位置ポリゴンから作成
	ATLTRY( SUCCEEDED( ((ITopologicalOperatorPtr)ipRet)->ConstructUnion( (IEnumGeometryPtr)ipEnumGeom ) ) ); // UNION

	return ipRet;
}

// 適切なルールキャッシュを返す
const annotation_class::CAnnotationClass* CRelAnnotationParameter::GetValidRule() const
{
	const annotation_class::CAnnotationClass* pRule = NULL;	// 返り値

	schema::annotation::annotation_code::ECode emCode = GetAnnoCode();
	if( schema::annotation::annotation_code::kSubwaySta == emCode || schema::annotation::annotation_code::kSubwaySubSta == emCode )
	{
		if( m_pAnnoRule && m_pAnnoRule->IsValidRule( m_emTableType ) )
			pRule = m_pAnnoRule;
		else if( m_pAnnoRule2 && m_pAnnoRule2->IsValidRule( m_emTableType ) )
			pRule = m_pAnnoRule2;
		else {
			LOGASSERT(false,sindyErr_RuleNotFound);
		}
	}
	else {
		LOGASSERTE_IF( m_pAnnoRule, sindyErr_RuleNotFound )
			pRule = m_pAnnoRule;
	}

	return pRule;
}

} // sindy
