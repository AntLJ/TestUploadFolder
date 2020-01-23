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
 *	@file AppropriateAttribute.h
 *	@brief 路線コード指定ファイルクラス 定義部
 *	@author	F.Adachi
 *	@date	2005/08/10		新規作成
 *	$ID$
 */

#ifndef ___APPROPRIATEATTRIBUTE___
#define ___APPROPRIATEATTRIBUTE___

#include <map>
#include <boost/utility.hpp>

/// <b>路線コード指定ファイルクラス</b>
class AppropriateAttribute : boost::noncopyable
{
public:
	AppropriateAttribute(void);
	~AppropriateAttribute();

	void	read( const char* cpFilePath );						///< 路線コード指定ファイルの読み込み
	void	disp( std::ostream& crStream ) const;				///< データ全体をストリーム出力
	
	const std::map<long, long>* getTable(void) const { return &m_Table; }

	long	getRoadClass( long cRoadCode ) const;
protected:

private:
	std::map<long, long>	m_Table;		///< 路線コードと道路標示種別の対応テーブル

};

#endif
