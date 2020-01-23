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

#include <boost/program_options.hpp>

/**
 * @brief イラストファイル名対応表レコードクラス
 */
class IllustNameRec
{
public:
	/**
	 * @brief コンストラクタ
	 */
	IllustNameRec() {}

	/**
	 * @brief コンストラクタ
	 */
	IllustNameRec( int nIllustClass, int nCustomer, long lOID, const CString& strFileName ) :
		m_nIllustClass( nIllustClass ), m_nCustomer( nCustomer ), m_lOID( lOID ), m_strFileName( strFileName ) {}

	/**
	 * @brief デストラクタ
	 */
	~IllustNameRec() {}

	/**
	 * @brief 比較演算子
	 */
	bool operator < ( const IllustNameRec& rec ) const
	{
		if( m_nIllustClass != rec.m_nIllustClass )	return m_nIllustClass < rec.m_nIllustClass;
		if( m_nCustomer != rec.m_nCustomer )		return m_nCustomer < rec.m_nCustomer;
		return m_lOID < rec.m_lOID;
	}

public:
	int		m_nIllustClass;	//!< イラスト種別コード
	int		m_nCustomer;	//!< 仕向先コード
	long	m_lOID;			//!< イラストリンクOID
	CString	m_strFileName;	//!< イラストファイル名（整備用）
};

/**
 * @brief イラストファイル名対応表クラス
 */
class IllustNameTable : public std::set< IllustNameRec, std::less< IllustNameRec > >
{
public:
	/**
	 * @brief コンストラクタ
	 */
	IllustNameTable();

	/**
	 * @brief デストラクタ
	 */
	~IllustNameTable();

	/**
	 * @brief 出力ファイルをオープン
	 * @param strFileName [in] イラストファイル名対応表のファイル名
	 * @param varMap [in] オプション格納マップ
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool open( const CString& strFileName, const boost::program_options::variables_map& varMap );

	/**
	 * @brief 全レコードをファイルに書き込み
	 */
	void write();

private:
	/**
	 * @brief ヘッダ情報を出力
	 */
	void outputHeader( const boost::program_options::variables_map& varMap );

private:
	std::ofstream	m_ofsIllustNameTable;	//!< イラストファイル名対応表ファイル出力ストリーム
};

/**
 * @brief イラストファイル名変換用レコードクラス
 */
class IllustNameConverterRec
{
public:
	/**
	 * @brief コンストラクタ
	 */
	IllustNameConverterRec() {}

	/**
	 * @brief コンストラクタ
	 * @param illustNameRec [in] イラストファイル名対応表のレコード
	 */
	IllustNameConverterRec( const IllustNameRec& illustNameRec ) :
		m_nIllustClass( illustNameRec.m_nIllustClass ), m_nCustomer( illustNameRec.m_nCustomer ), m_strFileName( illustNameRec.m_strFileName ) {}

	/**
	 * @brief デストラクタ
	 */
	~IllustNameConverterRec() {}

	/**
	 * @brief 比較演算子
	 */
	bool operator < ( const IllustNameConverterRec& rec ) const
	{
		if( m_nIllustClass != rec.m_nIllustClass )	return m_nIllustClass < rec.m_nIllustClass;
		if( m_nCustomer != rec.m_nCustomer )		return m_nCustomer < rec.m_nCustomer;
		return m_strFileName < rec.m_strFileName;
	}

public:
	int		m_nIllustClass;	//!< イラスト種別コード
	int		m_nCustomer;	//!< 仕向先コード
	CString	m_strFileName;	//!< イラストファイル名（整備用）
};

/**
 * @brief イラストファイル名変換クラス
 * @note 「イラスト種別コード」「仕向先コード」「イラストファイル名（整備用）」に対してユニークとなるようなリリース用イラストファイル名を振る
 */
class IllustNameConverter : private std::map< IllustNameConverterRec, int >
{
private:
	/**
	 * @brief コンストラクタ
	 * @note private化してあるので使用不可能
	 */
	IllustNameConverter();

public:
	/**
	 * @brief コンストラクタ
	 * @param illustNameTable [in] イラストファイル名対応表クラス
	 */
	IllustNameConverter( const IllustNameTable& illustNameTable );

	/**
	 * @brief デストラクタ
	 */
	~IllustNameConverter();

	/**
	 * @brief 対応するリリース用イラストファイル名の取得
	 * @param illustNameRec [in] イラストファイル名対応表のレコード情報
	 * @return リリース用イラストファイル名
	 */
	CString getReleaseFileName( const IllustNameRec& illustNameRec );
};
