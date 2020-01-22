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

#include "stdafx.h"
#include "TDC/useful_headers/str_util.h"

namespace util
{
	typedef std::map<long, std::vector<CString>>     vec_map;
	typedef std::vector<std::pair<std::string, std::string>> pair_vec;
	typedef std::map<CString, std::vector<long>>     strKey_map; //!< 文字列のキーに複数の整数値を紐づけるmap

	/**
	 * @brief   リストファイルの読み込み
	 * @param   fileName [in]  リストファイル名
	 * @param   lSet    [out] 読み込み先（longのセット）
	 * @retval  true  読み込み成功
	 * @retval  false 読み込み失敗
	 */
	inline bool loadLongSet( const CString& fileName, std::set<long>& lSet )
	{
		std::ifstream ifs( fileName );

		// ストリームを取得できなければfalse
		if( !ifs ){
			return false;
		}
		std::copy(
			std::istream_iterator<long>(ifs),
			std::istream_iterator<long>(),
			std::inserter( lSet, lSet.end() )
			);

		if(ifs.fail() && !ifs.eof()){
			return false;
		}
		return true;
	};

	/**
	 * @brief   リストファイルの読み込み
	 * @param   fileName [in]  リストファイル名
	 * @param   strList  [out] 読み込み先（std::stringのベクタ）
	 * @retval  true  読み込み成功
	 * @retval  false 読み込み失敗
	 */
	inline bool loadStringList( const CString& fileName, std::vector<std::string>& strList )
	{
		std::ifstream ifs( fileName );

		// ストリームを取得できなければfalse
		if( !ifs ){
			return false;
		}

		std::copy(
			std::istream_iterator<std::string>(ifs),
			std::istream_iterator<std::string>(),
			std::back_inserter( strList )
			);

		if(ifs.fail() && !ifs.eof()){
			return false;
		}
		return true;
	};

	/**
	* @brief   リストファイルの読み込み
	* @param   fileName [in]  リストファイル名
	* @param   cStrSet  [out] 読み込み先（std::stringのセット）
	* @retval  true  読み込み成功
	* @retval  false 読み込み失敗
	*/
	inline bool loadStringSet(const CString& fileName, std::set<std::string>& cStrSet)
	{
		std::ifstream ifs(fileName);

		// ストリームを取得できなければfalse
		if (!ifs) {
			return false;
		}

		std::copy(
			std::istream_iterator<std::string>(ifs),
			std::istream_iterator<std::string>(),
			std::inserter(cStrSet, cStrSet.end() )
		);

		if (ifs.fail() && !ifs.eof()) {
			return false;
		}
		return true;
	};

	/**
	 * @brief   リストファイルの読み込み
	 * @param   fileName [in]  リストファイル名
	 * @param   cStrSet  [out] 読み込み先（CStringのセット）
	 * @retval  true  読み込み成功
	 * @retval  false 読み込み失敗
	 */
	inline bool loadPairVec( const CString& fileName, pair_vec& pairVec )
	{
		std::ifstream ifs( fileName );

		// ストリームを取得できなければfalse
		if( !ifs ){
			return false;
		}

		std::string line;
		while( std::getline( ifs, line ) ){
			
			// 空の行や行頭「#」は無視
			if( line.size() == 0 || line.at(0) == '#' )
				continue;

			std::vector<std::string> columns;
			boost::algorithm::split(columns, line, boost::is_any_of("\t"));

			if( columns.size() < 2 )
				continue;

			pairVec.push_back( std::make_pair( columns[0], columns[1] ) );
		}

		if(ifs.fail() && !ifs.eof()){
			return false;
		}
		return true;
	};

	/**
	 * @brief   リストファイルの読み込み
	 * @param   fileName [in]  リストファイル名
	 * @param   vecMap   [out] 読み込み先
	 * @retval  true  読み込み成功
	 * @retval  false 読み込み失敗
	 */
	inline bool loadVecMap( const CString& fileName, vec_map& vecMap )
	{
		std::ifstream ifs( fileName );

		// ストリームを取得できなければfalse
		if( !ifs ){
			return false;
		}

		std::string line;
		while( std::getline( ifs, line ) ){
			
			// 空の行や行頭「#」は無視
			if( line.size() == 0 || line.at(0) == '#' )
				continue;

			std::vector<std::string> columns;
			boost::algorithm::split(columns, line, boost::is_any_of("\t"));

			if( columns.size() < 2 )
				continue;

			vecMap[stol(columns[0])].push_back( columns[1].c_str() );

		}

		if(ifs.fail() && !ifs.eof()){
			return false;
		}
		return true;
	};

	/**
	* @brief   リストファイルの読み込み
	* @param   fileName  [in]  リストファイル名
	* @param   strKeyMap [out] 読み込み先
	* @retval  true  読み込み成功
	* @retval  false 読み込み失敗
	*/
	inline bool loadStrKeyMap( const CString& fileName, strKey_map& strKeyMap )
	{
		std::ifstream ifs( fileName );

		// ストリームを取得できなければfalse
		if( !ifs ){
			return false;
		}

		std::string line;
		while( std::getline( ifs, line ) ){

			// 空の行や行頭「#」は無視
			if( line.size() == 0 || line.at( 0 ) == '#' )
				continue;

			std::vector<std::string> columns;
			boost::algorithm::split( columns, line, boost::is_any_of( "\t" ) );

			if( columns.size() < 2 )
				continue;

			strKeyMap[ columns[ 1 ].c_str() ].push_back( stol( columns[ 0 ] ) );

		}

		if( ifs.fail() && !ifs.eof() ){
			return false;
		}
		return true;
	}

	/**
	 * @brief	現在時刻取得用
	 * @retval	CString  「YYYY/MM/DD hh:mm:ss」 
	 */
	inline CString GetCurTime(){
		SYSTEMTIME st;
		::GetLocalTime( &st );
		CString strTime = uh::str_util::ToString( &st );
		return strTime;
	};

}
