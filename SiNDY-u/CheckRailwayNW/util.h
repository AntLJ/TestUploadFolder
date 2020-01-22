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

#include <algorithm>
#include <vector>
#include <map>

namespace util {

	/**
	 * @brief 整数を列挙したファイルを読み込む
	 * @note 典型的にはメッシュリストファイルの読み込みに使う
	 * @param fileName [in] リストファイル名
	 * @param integerList [out] 出力リストのset
	 * @retval true 読み込み成功
	 * @retval false 読み込み失敗
	 */
	inline bool loadMeshList(const CString& fileName, std::set<long>& list)
	{
		std::ifstream ifs(fileName);
			
		// ストリームを取得できなければfalse
		if( !ifs ){
			return false;
		}
		std::copy(
			std::istream_iterator<long>(ifs),
			std::istream_iterator<long>(),
			inserter(list, list.end())
		);
		if(ifs.fail() && !ifs.eof()){
			return false;
		}
		return true;
	};

	inline void GetDomainMap( const CString& fieldName, const sindy::CFieldMap& fieldMap, std::map<long, CString>& classMap )
	{
		std::map<CString, long> domainMap = fieldMap.GetDomain( fieldName );
		for( auto& domain : domainMap )
			classMap[domain.second] = domain.first;
	};

	/**
	 * @brief	現在時刻取得用
	 * @retval	CString  「YYYY/MM/DD hh:mm:ss」 
	 */
	inline CString GetCurTime(){
		SYSTEMTIME st;
		::GetLocalTime( &st );
		CString strTime;
		strTime.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
		return strTime;
	};


/// 3点の内積を求める
inline double innerproduct( double x1, double y1, double x2, double y2, double x3, double y3 )
{
	return ( x1 - x2 ) * ( x3 - x2 ) + ( y1 - y2 ) * ( y3 - y2 );
};

/// 2点の長さを求める
inline double length( double x1, double y1, double x2, double y2 )
{
	double dx = x2 - x1;
	double dy = y2 - y1;

	return sqrt( dx*dx + dy*dy );
};

/**
	* @brief 3点からなる線分の角度を求める
	*
	* @param x1	[in]	DOUBLE
	* @param y1	[in]	DOUBLE
	* @param x2	[in]	DOUBLE
	* @param y2	[in]	DOUBLE
	* @param x3	[in]	DOUBLE
	* @param y3	[in]	DOUBLE
	*
	* @retval 角度（-180～180度）
	*/
inline double angle( double x1, double y1, double x2, double y2, double x3, double y3 ){
	DOUBLE dInnerProduct = util::innerproduct( x1, y1, x2, y2, x3, y3 );
	DOUBLE d = util::length( x1, y1, x2, y2 ) * util::length( x2, y2, x3, y3 );

	return ( acos( dInnerProduct / d ) * (double)180.0 ) / (double)M_PI;
};

}
