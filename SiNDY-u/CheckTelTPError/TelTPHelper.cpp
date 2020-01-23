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
#include "TelTPHelper.h"

#include <AddrLib/CSAddrPoint.h>

#include <SiNDYLib/ErrorObject.h>
#include <ArcHelperEx/GlobalFunctions.h>

using namespace addr;

// TelTPポイント検索
EResultStat CTelTPHelper::findPoint( CLRec& cRec, bool bAddMode )
{
	CString strWhere;
	strWhere.Format( _T("%s = '%s' and OBJECTID = '%s'"), teltp_point::kSequence, cRec.m_strSequence, cRec.m_strObjectID );

	IQueryFilterPtr ipQuery( AheInitQueryFilter(NULL, NULL, strWhere) );

	long lTelTPCount = m_cTelTPPointFC.FeatureCount( ipQuery );
	IFeatureCursorPtr ipFeatureCursor( m_cTelTPPointFC.Search(ipQuery, VARIANT_FALSE) );

 	if( lTelTPCount > 0 && ipFeatureCursor )
	{
		// フィーチャ取得
		ipFeatureCursor->NextFeature( &m_ipFeature );

		CRow	cTelTPPoint( m_ipFeature, sindyTableType::unknown, false, m_cTelTPPointFC.GetFieldMap(), m_cTelTPPointFC.GetNameString() );

		// 座標取得・メッシュＸＹに変換
		CMeshXY cMXY;
		gf::ConvPoint2MXY( cTelTPPoint.GetShapeCopy(), &cMXY );

		// 座標付与種別(ADDXY_C)取得
		long lAddXY;
		CComVariant vaValue;
		cTelTPPoint.GetValueByFieldName( _T("ADDXY_C"), vaValue );
		lAddXY = (vaValue.vt == VT_I4)? vaValue.lVal : -1;
		if( lAddXY == -1 )
			return kE_FAIL_GET_ADDXY;

		// TelTPから各種情報取得
		if( !getInfo( cRec, cTelTPPoint ) )
		{
			return kE_FAIL_GET_INFO;
		}

		if( bAddMode )	// 付与モード
		{	
			cRec.m_cMXY		= cMXY;
			cRec.m_lAddXY	= lAddXY;
			return kO_TELTP_ADDED;
		}
		else			// チェックモード
		{
			// 同一シーケンスのAddXY_Cチェック
			CString strWhereSameSeq;
			strWhereSameSeq.Format( _T("%s = '%s'"), teltp_point::kSequence, cRec.m_strSequence );
			IQueryFilterPtr ipQuerySameSeq( AheInitQueryFilter(NULL, NULL, strWhereSameSeq) );

			long lTelTPCountSameSeq = m_cTelTPPointFC.FeatureCount( ipQuerySameSeq );
			if( lTelTPCountSameSeq > 1 )
			{
				vector<long>	vlAddXY;
				bool			bDupliFlag = false;
				cRec.m_strAddComment.Format( _T("(同一シーケンス:AddXY_C<") );

				IFeatureCursorPtr ipFeatureCursorSameSeq( m_cTelTPPointFC.Search(ipQuerySameSeq, VARIANT_FALSE) );
				IFeaturePtr	ipFeature;
				while( ipFeatureCursorSameSeq->NextFeature( &ipFeature ) == S_OK )
				{
					CRow	cTelTPPointSameSeq( ipFeature, sindyTableType::unknown, false, m_cTelTPPointFC.GetFieldMap(), m_cTelTPPointFC.GetNameString() );
					long	lAddXYSameSeq	= 0;
					cTelTPPointSameSeq.GetValueByFieldName( _T("ADDXY_C"), vaValue );
					lAddXYSameSeq	= (vaValue.vt == VT_I4)? vaValue.lVal : -1;
					cRec.m_strAddComment.AppendFormat( _T("%d "), lAddXYSameSeq );
					if( !vlAddXY.empty() )
					{
						vector<long>::iterator itr = find( vlAddXY.begin(), vlAddXY.end(), lAddXYSameSeq );
						if( itr != vlAddXY.end() )
							bDupliFlag = true;
					}
					vlAddXY.push_back( lAddXYSameSeq );
				}
				cRec.m_strAddComment.Append( _T(">)") );
				if( bDupliFlag == true )
					return kW_DUPLI_TELTP;
			}

			// 座標が移動していないか？
			if( cRec.m_cMXY == cMXY )
			{
				// ADDXY_Cが指定と違う (指定がない場合はクレーム扱い)
				if( cRec.m_lAddXY != lAddXY )
				{
					return kW_TELTP_FAIL_ADDXY;
				}

				// ADDXY_C=1 かつ POINTSTAT_C != 2 (クレーム対象なのに「該当施設あり」以外)
				if( cRec.m_lAddXY == 1 && cRec.m_lPointStat != 2 )
				{
					return kW_TELTP_FAIL_PSTAT;
				}
			}
			// 座標移動
			else
			{
				cRec.m_cMXY_Ref = cMXY;
				return kW_TELTP_MOVED;
			}
		}
		return kO_TELTP_OK;
	}
	else
	{
		return kW_TELTP_NOT_FOUND;
	}
}

// TelTPポイントの情報取得
bool CTelTPHelper::getInfo( CLRec& cRec, CRow cTelTPPoint )
{
	// TelTPから各種情報を取得
	CComVariant vaValue;

	// ユーザクレームフラグ
	cTelTPPoint.GetValueByFieldName( _T("USERCLAIM_F"), vaValue );
	cRec.m_lUserClaimF		= (vaValue.vt == VT_I4)? vaValue.lVal : -1;

	// 電話番号
	cTelTPPoint.GetValueByFieldName( _T("TEL_NUM"), vaValue );
	cRec.m_strTelNum		= (vaValue.vt == VT_BSTR)? CString(vaValue.bstrVal) : _T("");

	// 掲載住所
	cTelTPPoint.GetValueByFieldName( _T("ADDRNAME"), vaValue );
	cRec.m_strAddrName		= (vaValue.vt == VT_BSTR)? CString(vaValue.bstrVal) : _T("");

	// 住所コード
	cTelTPPoint.GetValueByFieldName( _T("ADDRCODE"), vaValue );
	cRec.m_strAddrCode		= (vaValue.vt == VT_BSTR)? CString(vaValue.bstrVal) : _T("");

	// 方書
	cTelTPPoint.GetValueByFieldName( _T("KATAGAKINAME"), vaValue );
	cRec.m_strKatagaki		= (vaValue.vt == VT_BSTR)? CString(vaValue.bstrVal) : _T("");

	// 掲載名称
	cTelTPPoint.GetValueByFieldName( _T("KEISAINAME"), vaValue );
	cRec.m_strKeisaiName	= (vaValue.vt == VT_BSTR)? CString(vaValue.bstrVal) : _T("");

	// 調査結果コード
	cTelTPPoint.GetValueByFieldName( _T("POINTSTAT_C"), vaValue );
	cRec.m_lPointStat		= (vaValue.vt == VT_I4)? vaValue.lVal : -1;
		
	// 情報ソース
	cTelTPPoint.GetValueByFieldName( ipc_table::kSource, vaValue );
	cRec.m_strSource		= (vaValue.vt == VT_BSTR)? CString(vaValue.bstrVal) : _T("");

	return true;
}

// 指定種別のドメイン取得(現状未使用、使用するためにはCheckTelTPError::outputLogにCTelTPHelperを追加する。)
const std::string CTelTPHelper::getDomain(LPCTSTR lpszDomainName, long lDomainCode) const
{
	// 座標付与種別
	IFieldsPtr ipFields;
	m_ipFeature->get_Fields( &ipFields );
	if( ipFields == NULL )
		return "";
	LONG lFID;
	ipFields->FindField( CComBSTR(lpszDomainName), &lFID );
	IFieldPtr ipField;
	ipFields->get_Field( lFID, &ipField );
	if( ipField == NULL )
		return "";

	IDomainPtr ipDomain;
	ipField->get_Domain( &ipDomain );
	if( ipDomain == NULL )
		return "";

	ICodedValueDomainPtr ipCVDoamin( ipDomain );
	if( ipCVDoamin == NULL )
		return "";

	USES_CONVERSION;

	LONG lDomainCount = 0;
	ipCVDoamin->get_CodeCount( &lDomainCount );
	for( LONG i=0;i<lDomainCount;++i )
	{
		CComVariant vaValue;
		ipCVDoamin->get_Value( i, &vaValue );
		if( static_cast<LONG>(lDomainCode) == vaValue.lVal ){
			CComBSTR bstr;
			ipCVDoamin->get_Name( i, &bstr );
			return OLE2A(bstr);
		}
	}
	
	return "";
}
