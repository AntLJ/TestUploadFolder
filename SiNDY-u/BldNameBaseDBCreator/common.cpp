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

#include "common.h"
#include "LogSys.h"
#include <boost/tokenizer.hpp>

long BldNamePtInfo::GetContentsCode() const
{
	long contentsCode = -1;
	switch( e_srcType )
	{
	case sindy::schema::sj::buildingname_src_point::source_type::kAnnoPOI:		// 注記POI（特定カテゴリ）
	case sindy::schema::sj::buildingname_src_point::source_type::kCompAnnoPOI:	// 注記POI（補完カテゴリ）
		{
			std::vector< long > tokens;
			if( TokenizeSrcID( tokens ) ) {
				contentsCode = tokens[0] * 100000 + tokens[1];
			} else {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("出典資料IDからコンテンツコードを取得できません : ") + e_srcID );
			}
		}
	}
	return contentsCode;
}

long BldNamePtInfo::GetContentsSeq() const
{
	long contentsSeq = -1;
	switch( e_srcType )
	{
	case sindy::schema::sj::buildingname_src_point::source_type::kAnnoPOI:		// 注記POI（特定カテゴリ）
	case sindy::schema::sj::buildingname_src_point::source_type::kCompAnnoPOI:	// 注記POI（補完カテゴリ）
		{
			std::vector< long > tokens;
			if( TokenizeSrcID( tokens ) ) {
				contentsSeq = tokens[2];
			} else {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("出典資料IDからコンテンツ内シーケンスを取得できません : ") + e_srcID );
			}
		}
	}
	return contentsSeq;
}

bool BldNamePtInfo::TokenizeSrcID( std::vector< long > &tokens ) const
{
	// 出典資料IDが半角数字と半角ハイフンのみで構成されるかチェック
	if( 0 != e_srcID.Compare( e_srcID.SpanIncluding( _T("0123456789-") ) ) )
		return false;

	// 半角ハイフンを区切文字としてトークンに分割
	std::string strSrcID = CT2A( e_srcID );
	boost::tokenizer< boost::char_separator< char > > strTokens( strSrcID, boost::char_separator< char >( "-" ) );
	for( const auto &itr : strTokens ) {
		tokens.push_back( atol( itr.c_str() ) );
	}

	// トークンの数が3であることをチェック
	if( 3 != tokens.size() )
		return false;

	return true;
}

namespace common_proc {
	bool GetValue(_IRowPtr ipRow, LPCTSTR talbleName, LPCTSTR fieldName, const std::map<CString, long>& indeces, CComVariant* val)
	{
		CString errMsg;
		CString temp = fieldName;
		temp.MakeLower();
		std::map<CString, long>::const_iterator fieldItr = indeces.find(temp);
		if (fieldItr == indeces.end()) {
			errMsg.Format(_T("%sフィールドのインデックス取得に失敗しました"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, talbleName);
			return false;
		}
		if (FAILED(ipRow->get_Value(fieldItr->second, val))) {
			errMsg.Format(_T("%sフィールドの値取得に失敗しました"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, talbleName);
			return false;
		}
		return true;
	}

	bool SetValue(IRowBufferPtr ipBuffer, LPCTSTR tableName, LPCTSTR fieldName, const std::map<CString, long>& indeces, const CComVariant& val)
	{
		CString errMsg;
		CString temp = fieldName;
		temp.MakeLower();
		std::map<CString, long>::const_iterator fieldItr = indeces.find(temp);
		if (fieldItr == indeces.end()) {
			errMsg.Format(_T("%sフィールドのインデックス取得に失敗しました(属性付与時)"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, tableName);
			return false;
		}
		if (FAILED(ipBuffer->put_Value(fieldItr->second, val))) {
			errMsg.Format(_T("%sフィールドの値付与に失敗しました"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, tableName);
			return false;
		}	
		return true;
	}

	bool SelectHeadNameFeature(const IDFeatureMap& tgtFeatures, LPCTSTR tableName, LPCTSTR fieldName, const std::map<CString, long>& indeces, long* headID, IFeaturePtr& ipHeadFeature)
	{
		// 対象フィーチャ数が０の場合 → エラー
		if (0 == tgtFeatures.size()) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("調査対象ポイント選定対象フィーチャ数が0件でした"), tableName);
			return false;
		}

		// 対象フィーチャ数が１の場合
		if (1 == tgtFeatures.size()) {
			*headID = tgtFeatures.begin()->first;
			ipHeadFeature = tgtFeatures.begin()->second.m_T;
			return true;
		}

		// 対象フィーチャ数が複数の場合
		long minLength = LONG_MAX;
		for (const auto &feature : tgtFeatures) {
			IFeaturePtr ipFeature = feature.second.m_T;
			//　名称の長さ取得
			CComVariant val;
			if (! ::common_proc::GetValue((_IRowPtr)ipFeature, tableName, fieldName, indeces, &val)) 
				return false;
			long length = _tcslen((CString)val.bstrVal);
			// 文字列が短い場合、更新。
			// 文字列長が一致した場合は、OBJECTIDが小さいほうが優先される。tgtFeaturesはOBJECTID順なので、更新しなれければOBJECTIDが小さいほうが採用される。
			if (length < minLength) {
				minLength = length;
				*headID = feature.first;
				ipHeadFeature = ipFeature;
			}
		}
		return true;
	}

	bool SearchByWhereClause(IFeatureClassPtr ipFeatureClass, IFeatureCursorPtr& ipFeatureCursor, const CString& whereClause, bool bRecycling)
	{
		CString errMsg;

		// フィーチャクラス名称取得（ログ出力用）
		CComBSTR strName;
		((IDatasetPtr)ipFeatureClass)->get_Name(&strName);

		// フィルタ作成
		IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
		if (FAILED(ipQueryFilter->put_WhereClause(CComBSTR(whereClause)))) {
			errMsg.Format(_T("フィルタの作成に失敗しました : %s"), whereClause);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, (CString)strName);
			return false;
		}

		// 検索
		if (FAILED(ipFeatureClass->Search(ipQueryFilter, bRecycling ? VARIANT_TRUE : VARIANT_FALSE, &ipFeatureCursor))) {
			errMsg.Format(_T("カーソルの取得に失敗しました : %s"), whereClause);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, (CString)strName);
			return false;
		}

		return true;
	}

	bool SearchByWhereClause(ITablePtr ipTable, _ICursorPtr& ipCursor, const CString& whereClause, bool bRecycling)
	{
		CString errMsg;

		// テーブル名称取得（ログ出力用）
		CComBSTR strName;
		((IDatasetPtr)ipTable)->get_Name(&strName);

		// フィルタ作成
		IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
		if (FAILED(ipQueryFilter->put_WhereClause(CComBSTR(whereClause)))) {
			errMsg.Format(_T("フィルタの作成に失敗しました : %s"), whereClause);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, (CString)strName);
			return false;
		}

		// 検索
		if (FAILED(ipTable->Search(ipQueryFilter, bRecycling ? VARIANT_TRUE : VARIANT_FALSE, &ipCursor))) {
			errMsg.Format(_T("カーソルの取得に失敗しました : %s"), whereClause);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, (CString)strName);
			return false;
		}

		return true;
	}

	bool SearchByGeometry(IFeatureClassPtr ipFeatureClass, IFeatureCursorPtr& ipFeatureCursor, const IGeometryPtr& ipGeometry, esriSpatialRelEnum spRel, bool bRecycling)
	{
		// フィーチャクラス名称取得（ログ出力用）
		CComBSTR strName;
		((IDatasetPtr)ipFeatureClass)->get_Name(&strName);

		// フィルタ作成
		ISpatialFilterPtr ipSpFilter(CLSID_SpatialFilter);
		if (FAILED(ipSpFilter->putref_Geometry(ipGeometry)) || FAILED(ipSpFilter->put_SpatialRel(spRel))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("フィルタの作成に失敗しました"), (CString)strName);
			return false;
		}
		// カーソル取得
		if (FAILED(ipFeatureClass->Search(ipSpFilter, bRecycling ? VARIANT_TRUE : VARIANT_FALSE, &ipFeatureCursor))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("カーソルの取得に失敗しました"), (CString)strName);
			return false;
		}

		return true;
	}

	bool GetCodedValues(const ITablePtr& ipTable, LPCTSTR talbleName, LPCTSTR fieldName, const std::map<CString, long>& indeces, std::set<long>& codedValues)
	{
		CString errMsg;
		CString temp = fieldName;
		temp.MakeLower();
		std::map<CString, long>::const_iterator fieldItr = indeces.find(temp);
		if (fieldItr == indeces.end()) {
			errMsg.Format(_T("%sフィールドのインデックス取得に失敗しました"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, talbleName);
			return false;
		}
		IFieldsPtr ipFields;
		if (FAILED(ipTable->get_Fields(&ipFields))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("フィールド列の取得に失敗しました"), talbleName);
			return false;
		}
		IFieldPtr ipField;
		if (FAILED(ipFields->get_Field(fieldItr->second, &ipField))) {
			errMsg.Format(_T("%sフィールドの取得に失敗しました"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, talbleName);
			return false;
		}
		IDomainPtr ipDomain;
		if (FAILED(ipField->get_Domain(&ipDomain))) {
			errMsg.Format(_T("%sフィールドのドメイン取得に失敗しました"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, talbleName);
			return false;
		}
		esriDomainType domainType;
		if (FAILED(ipDomain->get_Type(&domainType))) {
			errMsg.Format(_T("%sフィールドのドメインタイプ取得に失敗しました"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, talbleName);
			return false;
		}
		if (esriDTCodedValue != domainType) {
			errMsg.Format(_T("%sフィールドのドメインタイプがコード値ドメインではありません"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, talbleName);
			return false;
		}
		ICodedValueDomainPtr ipCodedValueDomain(ipDomain);
		long codeCount = 0;
		if (FAILED(ipCodedValueDomain->get_CodeCount(&codeCount))) {
			errMsg.Format(_T("%sフィールドのコード値数取得に失敗しました"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, talbleName);
			return false;
		}
		for(long i = 0; i < codeCount; ++i) {
			CComVariant varValue;
			if (FAILED(ipCodedValueDomain->get_Value(i, &varValue))) {
				errMsg.Format(_T("%sフィールドのコード値取得に失敗しました"), fieldName);
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, talbleName);
				return false;
			}
			codedValues.insert(varValue.lVal);
		}
		return true;
	}
}
