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
 * @file RelAnnotation.cpp
 * @brief <b>CRelAnnotationクラス実装ファイル</b>
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @vertion $Id$
 */
#include "stdafx.h"
#include "AnnotationClass.h"
#include "RelAnnotation.h"
#include "Table.h"
#include "util.h"
#include "ErrorObject.h"
#include "AnnoGlobals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;

double g_dTotalSetDispLineTime = 0.0;	//!< SetDispLineにかかる総時間

// 表示位置をセットする
sindyErrCode CRelAnnotation::SetDispLine( sindyTableType::ECode emTableType, IFeature* ipDispLineFeature, bool bDetatch/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNameString/* = CSPTableNameString()*/ )
{
	double dStart = GetTickCount();	// SetDispLine開始時間（トレース用）

	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	LOGASSERT_IF( ipDispLineFeature, sindyErr_COMInterfaceIsNull )
	{
		CSPDispLine cDisp( new CDispLine( ipDispLineFeature, emTableType, false, spFields, spNameString ) );
		if( bDetatch )
			cDisp->CreateCache( true, spFields, spNameString );
		push_front( cDisp, false ); // ユニークかどうかはここで判定しない（スピード重視）
	}

	g_dTotalSetDispLineTime += GetTickCount()-dStart;	// SetDispLineにかかる時間を計測

	return emErr;
}

// 表示タイプをチェックする
errorcode::sindyErrCode CRelAnnotation::CheckDispType() const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	switch( GetTableType() )
	{
		case sindyTableType::city_annotation:
			emErr = CAnnotation::CheckDispType( sindyTableType::city_disp_line );
			break;
		case sindyTableType::d1_annotation:
			emErr = CAnnotation::CheckDispType( sindyTableType::c_sc1disp_line );
			break;
		case sindyTableType::base_annotation:
			if( sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::b_sc4disp_line ) ) &&
				sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::b_sc3disp_line ) ) &&
				sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::b_sc2disp_line ) ) &&
				sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::b_sc1disp_line ) ) )
				emErr = sindyErr_NoErr;
			break;
		case sindyTableType::middle_annotation:
			if( sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::m_sc4disp_line ) ) &&
				sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::m_sc3disp_line ) ) &&
				sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::m_sc2disp_line ) ) &&
				sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::m_sc1disp_line ) ) )
				emErr = sindyErr_NoErr;
			break;
		case sindyTableType::top_annotation:
			if( sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::t_sc4disp_line ) ) &&
				sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::t_sc3disp_line ) ) &&
				sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::t_sc2disp_line ) ) &&
				sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::t_sc1disp_line ) ) )
				emErr = sindyErr_NoErr;
			break;
		default:
			emErr = sindyErr_UnknownTableType;
			break;
	}
	return emErr;
}

// 注記文字列と真位置の位置関係をチェックする
sindyErrCode CRelAnnotation::CheckPosition() const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	CPoint cAnnoPoint( (IPointPtr)(IGeometry*)*GetShape() );		// 真位置ポイント
	schema::annotation::annotation_code::ECode emAnnoCode = GetAnnoCode();	// 注記種別

	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator itDisp = it->second.begin(); itDisp != it->second.end(); ++itDisp )
		{
			const CDispLine& cDisp( static_cast<const CDispLine&>(**itDisp) );
			// パラメータ計算
			long lSecondLineStartIndex = 0;
			WKSEnvelope pRect;
			IGeometryPtr ipGeom;
			sindyAnnotationLineType emLineType = cDisp.CalcParameter( cDisp.IsAltitudeValue( emAnnoCode ), &lSecondLineStartIndex, NULL, &pRect, &ipGeom );	// ここでパラメータ計算するのは無駄だなぁ…

			// 表示位置タイプ取得
			sindyAnnotationPosition emPos = cDisp.CalcPosition( cAnnoPoint, emLineType, pRect );

			sindyErrCode emErr = sindyErr_AnnoDispPositionInvalid;
			switch( emAnnoCode )
			{
				case schema::annotation::annotation_code::kPass:		// 峠
					if( emPos == sindyAnnoPos_UpperVertical  ||
						emPos == sindyAnnoPos_LowerVertical  ||
						emPos == sindyAnnoPos_RightHorizonal || 
						emPos == sindyAnnoPos_LeftHorizonal )
						emErr = sindyErr_NoErr;
					break;
				case schema::annotation::annotation_code::kMountain:	// 山岳
					if( emPos == sindyAnnoPos_UpperHorizonal ||
						emPos == sindyAnnoPos_LowerHorizonal ||
						emPos == sindyAnnoPos_RightHorizonal || 
						emPos == sindyAnnoPos_LeftHorizonal )
						emErr = sindyErr_NoErr;
					break;
				default:
					if( emPos != (sindyAnnotationPosition)-1 )
						emErr = sindyErr_NoErr;
					break;
			}
			if( emErr != sindyErr_NoErr ) break;	// エラーなら途中でやめる
		}
	}
	return emErr;
}

// キャッシュを作成する
sindyErrCode CRelAnnotation::CreateCache( bool bDetach/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNameString/* = CSPTableNameString()*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	LOGASSERT_IF( sindyErr_NoErr == ( emErr = CRow::CreateCache( bDetach, spFields, spNameString ) ), emErr )
	{
		// 共有オブジェクト作成
		boost::shared_ptr<CFieldMap> spFields;
		boost::shared_ptr<CTableNameString> spDispNameString;
		for( iterator it = begin(); it != end(); ++it )
		{
			for( rows_iterator itDisp = it->second.begin(); itDisp != it->second.end(); ++itDisp )
			{
				if( sindyErr_NoErr != ( emErr = (*itDisp)->CreateCache( bDetach ) ) )
				{
					ERRORLOG(emErr);
					SASSERT(false);
					break;
				}
			}
		}
	}
	return emErr;
}

// スケールにあった表示位置フィーチャの個数を取得する
long CRelAnnotation::GetDispLineCount( sindyTableType::ECode emTableType ) const // TODO: CContainer::size
{
	const_iterator it = find( emTableType );
	if( it != end() )
		return (long)it->second.size();
	return 0;
}

// 表示位置を取得する
CDispLine* CRelAnnotation::GetDispLine( long lIndex, sindyTableType::ECode emTableType ) // TODO: CContainer::findか？そもそもインデックス指定する？
{
	iterator it = find( emTableType );
	if( it != end() )
	{
		if( (long)it->second.size() > lIndex )
		{
			rows_iterator itDisp = it->second.begin();
			std::advance( itDisp, lIndex );
			return static_cast<CDispLine*>(&**itDisp);
		}
	}
	ERRORLOG(sindyErr_ArgLimitOver);
	SASSERT(false);

	return NULL;
}

// 表示位置を取得する
const CDispLine* CRelAnnotation::GetDispLine( long lIndex, sindyTableType::ECode emTableType ) const  // TODO: CContainer::findか？そもそもインデックス指定する？
{
	const_iterator it = find( emTableType );
	if( it != end() )
	{
		if( (long)it->second.size() > lIndex )
		{
			const_rows_iterator itDisp = it->second.begin();
			std::advance( itDisp, lIndex );
			return static_cast<const CDispLine*>(&**itDisp);
		}
	}
	ERRORLOG(sindyErr_ArgLimitOver);
	SASSERT(false);

	return NULL;
}

// スケールにあった注記文字列を取得する
LPCTSTR CRelAnnotation::GetNameString( long lIndex, sindyTableType::ECode emTableType, int nLen, LPTSTR lpszName ) const
{
	const CDispLine* pDisp = GetDispLine( lIndex, emTableType );

	LOGASSERT_IF( pDisp, sindyErr_ArgLimitOver )
	{
		switch( pDisp->GetTargetStr() )
		{
			case schema::disp_line::target_str::kStr1: GetNameString1( nLen, lpszName ); break;
			case schema::disp_line::target_str::kStr2: GetNameString2( nLen, lpszName ); break;
			case schema::disp_line::target_str::kStr3: GetNameString3( nLen, lpszName ); break;
			default: ERRORLOG(sindyErr_AnnoDispTargetStrInvalid); break; // ターゲット文字列コードがおかしい
		}
	}
	return lpszName;
}

// スケールにあった注記文字数を取得する
long CRelAnnotation::GetStringNum( long lIndex, sindyTableType::ECode emTableType ) const
{
	long lNum = -1;
	const CDispLine* pDisp = GetDispLine( lIndex, emTableType );

	LOGASSERT_IF( pDisp, sindyErr_ArgLimitOver )
	{
		switch( pDisp->GetTargetStr() )
		{
			case schema::disp_line::target_str::kStr1: lNum = GetStringNum1(); break;
			case schema::disp_line::target_str::kStr2: lNum = GetStringNum2(); break;
			case schema::disp_line::target_str::kStr3: lNum = GetStringNum3(); break;
			default: ERRORLOG(sindyErr_AnnoDispTargetStrInvalid); break; // ターゲット文字列コードがおかしい
		}
	}
	return lNum;
}

// コピー注記が存在するかどうか
bool CRelAnnotation::HasCopyDispLine( sindyTableType::ECode emDispTableType ) const
{
	// 同じテーブルタイプを持つものが複数存在するかどうかで判定
	const_iterator it = find( emDispTableType );
	if( it != end() )
		return ( 1 < it->second.size() ) ? true : false;

	return false;
}

// 指定オブジェクトを削除する
sindyErrCode CRelAnnotation::Delete( _IRow* ipRow, CErrorObjects& cErrs )
{
	_ASSERTE(false); // 未実装
	// 引数チェック
	if( ! ipRow )
		return sindyErr_ArgIsNull;

	// 指定オブジェクトが真位置？
	if( *this == ipRow )
	{
		CAnnotation::Delete();
		// 全ての表示位置を削除対象に
		CContainer::Delete( NULL, true );
	}
	else {
		// 表示位置？
		CSPRow cRow = find( ipRow );
		if( cRow )
		{
			// 表示位置が削除される場合、コピー注記が無ければ真位置が持っている表示タイプ
			// は記号のみ又は表示無しとなる（記号のみの方が優先度高）。
			// 同じスケール内にコピーがあるなら表示タイプの変更なし
			// 同じスケール内にコピーがないなら表示タイプを変更
			// 表示タイプを変更した場合は、中抜けの整合性を確認する
			//
			// コピーの存在を確認
			if( HasCopyDispLine( cRow->GetTableType() ) )
			{
				// コピーがあるならそのまま削除
				cRow->Delete();
			}
			// コピーがない場合
			else {
				// 削除後の表示タイプを調べる
				schema::annotation::disp_type::ECode emAfterDispType = (schema::annotation::disp_type::ECode)-1; // 削除後の表示タイプ
				const annotation_class::CAnnotationClass* pRule = GetRule();
				LOGASSERTE_IF( pRule, sindyErr_RuleNotFound )
				{
					schema::annotation_class::scaledisp_rule::ECode emDispRuleCode = pRule->GetDispRule( cRow->GetTableType() ); // 表示タイプルール
					if( AheCheckDispTypeCodeAndScaleDispRule( schema::annotation::disp_type::kSym, emDispRuleCode ) )
						emAfterDispType = schema::annotation::disp_type::kSym;
					else if( AheCheckDispTypeCodeAndScaleDispRule( schema::annotation::disp_type::kNone, emDispRuleCode ) )
						emAfterDispType = schema::annotation::disp_type::kNone;
				}
				if( (schema::annotation::disp_type::ECode)-1 != emAfterDispType )
				{
					bool bConsistencyOK = false; // 表示タイプの整合が取れているかどうか
					if( ! pRule->GetExceptScale( m_emTableType ) )
					{
						// 中抜けできないなら上位スケールの表示タイプと比較して中抜けにならないかチェック
						// 上位スケールの表示タイプを取得
						schema::annotation::disp_type::ECode emUpperDispType = GetDispType( *(++DispIterator( cRow->GetTableType() )) );
						if( AheCheckDispTypeAndUpperDispType( emAfterDispType, emUpperDispType ) )
							bConsistencyOK = true;
					}
					else
						bConsistencyOK = true;

					// 削除しても整合が保てるなら削除
					if( bConsistencyOK )
					{
						// 表示タイプ属性を変更
						SetLongValueByFieldName( schema::annotation::kDispType, emAfterDispType );
						// 削除
						cRow->Delete();
					}
					else
						// 中抜け状態になるので削除不可
						cErrs.push_back( CErrorObject( *cRow, CErrorInfo( sindyErr_AnnoDispExceptExist, sindyErrLevel_ERR ) ) );
				}
				else
					// 表示タイプを変更することができないので削除不可
					cErrs.push_back( CErrorObject( *cRow, CErrorInfo( sindyErr_AnnoDispLineShouldExist, sindyErrLevel_ERR ) ) );
			}
			// 真位置属性
		}
		else
			return sindyErr_ObjectNotExist; // ターゲットが見つからない
	}

	return cErrs.GetDefaultErrCode();
}

// 注記真位置に合った表示位置レイヤ数を返す
int CRelAnnotation::GetDispLayerCount() const
{
	int nRet = -1;	// 返り値

	switch( GetTableType() )
	{
	case sindyTableType::city_annotation:
	case sindyTableType::d1_annotation:   nRet = 1; break;
	case sindyTableType::base_annotation:
	case sindyTableType::middle_annotation:
	case sindyTableType::top_annotation:  nRet = 4; break;
	default:
		LOGASSERT(false,sindyErr_UnknownTableType); break;
	}
	return nRet;
}

// 表示位置レイヤのテーブルタイプを返す
sindyTableType::ECode CRelAnnotation::GetDispLayer( int index ) const
{
	sindyTableType::ECode emType = sindyTableType::unknown;	// 返り値

	switch( GetTableType() )
	{
	case sindyTableType::city_annotation: emType = sindyTableType::city_disp_line; break;
	case sindyTableType::d1_annotation:   emType = sindyTableType::c_sc1disp_line; break;
	case sindyTableType::base_annotation:
	{
		switch( index )
		{
		case 0: emType = sindyTableType::b_sc4disp_line; break;
		case 1: emType = sindyTableType::b_sc3disp_line; break;
		case 2: emType = sindyTableType::b_sc2disp_line; break;
		case 3: emType = sindyTableType::b_sc1disp_line; break;
		default: LOGASSERT(false,sindyErr_ArgLimitOver); break;
		}
		break;
	}
	case sindyTableType::middle_annotation:
	{
		switch( index )
		{
		case 0: emType = sindyTableType::m_sc4disp_line; break;
		case 1: emType = sindyTableType::m_sc3disp_line; break;
		case 2: emType = sindyTableType::m_sc2disp_line; break;
		case 3: emType = sindyTableType::m_sc1disp_line; break;
		default: LOGASSERT(false,sindyErr_ArgLimitOver); break;
		}
		break;
	}
	case sindyTableType::top_annotation:
	{
		switch( index )
		{
		case 0: emType = sindyTableType::t_sc4disp_line; break;
		case 1: emType = sindyTableType::t_sc3disp_line; break;
		case 2: emType = sindyTableType::t_sc2disp_line; break;
		case 3: emType = sindyTableType::t_sc1disp_line; break;
		default: LOGASSERT(false,sindyErr_ArgLimitOver); break;
		}
		break;
	}
	default:
		LOGASSERT(false,sindyErr_UnknownTableType); break;
	}

	return emType;
}

} // sindy
