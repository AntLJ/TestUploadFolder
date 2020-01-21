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

#include <TDC/useful_headers/tstring.h>

class crd_cnv;
extern crd_cnv g_cnv;
class CTimeTrace;
extern CTimeTrace g_trace;

std::list<long> set_intersection( const std::list<long>& in1, const std::list<long>& in2 );
inline std::list<long> set_intersection( const std::list<long>& in1, const std::list<long>& in2 )
{
	std::list<long> out;
	out.assign( max( in1.size(), in2.size() ), LONG_MAX );
	std::set_intersection( in1.begin(), in1.end(), in2.begin(), in2.end(), out.begin() );
	out.remove(LONG_MAX);
	return out;
}

/// 形状コピー取得モード
enum ShapeMode {
	enumLatest = 1, //!< 変更後形状
	enumOriginal = 2, //!< 変更前形状
	enumAll = 3, //!< enumLatest|enumOriginal
	enumFeatureChangedOriginal = 4, //!< IFeatureChangedインターフェースから取得できる変更前形状
	enumDiff = 8, //!< 変更差分（LatestとOriginal）
};

/**
 * @brief 渡された形状群をUNIONする
 * 
 * 複数形状を一つの形状にまとめます。
 * 別次元の形状が存在する場合はGeometryBagと
 * して返します。同次元の形状のみ存在する場合は
 * その次元の形状で返します（MultiPoint、Polyline、Polygon）。
 */
IGeometryPtr AheConstructUnion( _ISetPtr& ipGeometrySet );

// SDEに入れるための値とした時の同じかどうかの判定用関数
bool IsEqualSDEValue( esriFieldType type, const CComVariant& va1, const CComVariant& va2 );

/**
 * @brief targetをsepで分解し、listに格納する
 * @param target [in] 分解対象文字列
 * @param sep_char [in] セパレータ文字列
 * @param values [out] 分解後の文字列リスト
 */
void get_separated_values( const uh::tstring& target, const uh::tstring& sep_char, std::list<uh::tstring>& values );

/// CRowBase::Store()時の形状比較の判定方法（通常はCGeometry::Equals（IClone::IsEqual）を使用する）
extern bool g_bStoreShapeJudge_Native; //!< CRowBase::Store()時の形状比較にIRelationalOperator::Equalsを使う
extern bool g_bStoreShapeJudge_Exact;  //!< CRowBase::Store()時の形状比較にCGeometry::EqualsVertex（構成点比較）を使う
extern bool g_bStoreShapeJudge_PointZ_Exact; //!< CRowBase::Store()時の形状比較でZ値を持つポイントの場合にZ値の許容値を考慮しない（デフォルト：false)
extern bool g_bShapeChanged_UseRelationalOperator; //!< CRowBase::ShapeChangedでIRelationalOperator::Equalsを使用するかどうか（デフォルト：false）