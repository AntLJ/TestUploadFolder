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
#include <string>
#include <stdio.h>
#include <algorithm>
#define _USE_SHAPELIB_STATIC
#include <libshape.h>
#include <DBFHandle.h>
#include <shapelib\SHPHandle.h>

#define ON_DEFAULT			0x00
#define	ON_NORMAL_MODE		0x01
#define	ON_CITYCODE_MODE	0x02
#define	ON_ROADCODE_MODE	0x04

using namespace std;

struct FieldInfo
{
	int				iID;		// フィールド番号
	DBFFieldType	dType;		// フィールドタイプ(0:FTString,1:FTInteger,2:FTDouble,3:FTLogical,4:FTInvalid)
	string			sName;		// フィールド名
	int				iWidth;		// フィールドサイズ
	int				iDecimals;	// フィールド精度
	int				iIgnoreFlag;	// 変換無視フィールド(1:string型でも変換しない)

	FieldInfo()
	{
		iID			= 0;
		dType		= FTInvalid;
		iWidth		= 0;
		iDecimals	= 0;
		iIgnoreFlag	= 0;
	}
	~FieldInfo()
	{
	}

	bool	operator<(const FieldInfo temp_FieldInfo) const
	{
		if( iID < temp_FieldInfo.iID )
			return true;
		else
			return false;
	}
	bool	operator=(const FieldInfo temp_FieldInfo) const
	{
		if( iID == temp_FieldInfo.iID )
			return true;
		else
			return false;
	}
};

class	CConvSJIStoUTF8
{

public:

	// コンストラクタ
	CConvSJIStoUTF8()
	{
		m_iExeMode	= ON_DEFAULT;
		m_DbfMode = false;
	}

	// デクストラクタ
	~CConvSJIStoUTF8()
	{
	}

	/**
	 * @brief	使用法
	 */
	static void	printUsage();

	/**
	 * @brief	引数チェック
	 * @param	argc	[in]	引数の数
	 * @param	argv	[in]	引数
	 * @return	bool
	 */
	bool	arg_check( int argc, char* argv[] );

	/**
	 * @brief	初期化
	 * @param	argc	[in]	引数の数
	 * @param	argv	[in]	引数
	 * @return	bool
	 */
	bool	init( int argc, char* argv[] );

	/**
	 * @brief	実行
	 * @return	bool
	 */
	bool	execute();

private:

	/**
	 * @brief	リストファイル読み込み
	 * @param	lpszFile		[in]	読み込みたいリスト名
	 * @param	vsVectorList	[out]	読込先Vector(1フィールドごと)
	 * @return	bool
	 */
	bool	loadlist( LPCTSTR lpszFile, vector<string>& vsVectorList );

	/**
	 * @brief	dbfファイルのUTF8変換
	 * @param	メッシュ	[in]	変換メッシュ
	 * @param	パス		[in]	入力ファイルのフルパス
	 * @param	パス		[in]	出力ファイルのフルパス
	 * @return	bool
	 */	
	bool	ConvertDBF( string Mesh, string fullpass, string destfullpass );

	/**
	 * @brief	メッシュ単位ファイル名の作成
	 * @param	MeshCode	[in]	メッシュコード
	 * @param	cPathName	[in]	ファイル基点パス
	 * @return	ファイルのフルパス
	 */
	string	make_mesh_fname( string MeshCode,	char* cpPathName );

	/**
	 * @brief	変換対象レイヤから、文字列フィールドのインデックスを取得
	 * @param	fullpass	[in]	ファイルのフルパス
	 * @return	bool
	 */
	bool	GetFieldInfo( string sFullPass );

	/**
	 * @brief	文字コードがShift-JISか判定する。
	 * @param	cChars	[in]	文字列
	 * @return	bool
	 */
	bool	CheckShiftJIS( const char* cChars );

	string				m_sFilePass;				// 変換対象Shapeファイルパス(基点)
	string				m_sDestFilePass;			// 変換先Shapeファイルパス

	string				m_sLayerName;				// 処理対象レイヤ名
	string				m_sDataSet;					// データセット名(Citymap,Road等)

	vector<string>		m_vsMesh;					// メッシュリスト1行ずつ格納
	vector<string>		m_vsAllLayer;				// 05Shape存在レイヤ全てを格納
	vector<string>		m_vsIgnoreField;			// 変換無視レイヤ格納(stringだが、数値(+,)しか持たないフィールド)

	vector<FieldInfo>	m_vfFieldInfo;				// フィールド番号,フィールドタイプ,フィールド名,フィールドサイズ,精度,変換無視フラグを持つ構造体のvector

	int					m_iExeMode;					// 実行モード(citycode, roadcode, DBFのみ, 通常レイヤ)

	ofstream			m_ErrorLog;					// ログ出力用
	SYSTEMTIME			m_st;						// 開始・終了時間取得用

	bool				m_DbfMode;					// dbf変換モード(通常はshapeであること前提)
};
