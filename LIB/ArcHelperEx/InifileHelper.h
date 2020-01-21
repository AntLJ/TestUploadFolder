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
* @file InifileHelper.h
*
* @brief IniFile関連を操作するクラス群の定義ファイル
*
* @author 古川 貴宏
*
* @date   2001/12/10
*/
#if !defined(AFX_INIFILEHELPER_H__7746FE5E_5FFC_44A0_9BEC_76C538AAF0D0__INCLUDED_)
#define AFX_INIFILEHELPER_H__7746FE5E_5FFC_44A0_9BEC_76C538AAF0D0__INCLUDED_	//!< 重複インクルード防止

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
* @class CInifileHelperRoot
*
* @brief iniファイルへの基本アクセスを提供するクラス
*/
class CInifileHelperRoot
{
public:
	CInifileHelperRoot( LPCTSTR lpcszPath = NULL);
	~CInifileHelperRoot();

	/**
	* @brief 読み込み対象のINIファイルが設定済みかテストする
	*
	* @retval TRUE iniファイルは未設定
	* @retval FALSE iniファイルは設定済み
	*/
	virtual BOOL IsEmpty();

	/**
	* @brief 固有設定ファイルからの数値取得関数
	*
	* @param lpAppName [in]
	* @param lpKeyName [in]
	* @param nDefault [in]
	*
	* @return 取得データ
	*/
	int GetPrivateProfileInt( LPCTSTR lpAppName, LPCTSTR lpKeyName, int nDefaule);

	/**
	* @brief 固有設定ファイルからの文字列取得関数
	*
	* @param lpAppName [in]
	* @param lpKeyName [in]
	* @param lpDefault [in]
	* @param cString [out]
	* @param nSize [in]
	*
	* @return 取得データ長
	*/
	DWORD GetPrivateProfileString( LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, CString& cString, int nSize = DEFAULT_ALLOC);

	/**
	* @brief 固有設定ファイルへの数値設定関数
	*
	* @param lpAppName [in]
	* @param lpKeyName [in]
	* @param nDefault [in]
	*
	* @retval ０(Zero) 成功
	* @retval ０以外 失敗
	*/
	DWORD WritePrivateProfileInt( LPCTSTR lpAppName, LPCTSTR lpKeyName, int nData);

	/**
	* @brief 固有設定ファイルへの文字列設定関数
	*
	* @param lpAppName [in]
	* @param lpKeyName [in]
	* @param lpcszString [in]
	*
	* @retval ０(Zero) 成功
	* @retval ０以外 失敗
	*/
	DWORD WritePrivateProfileString( LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpcszString);

	/**
	* @brief 管理対象のINIファイル取得関数
	*
	* @param cStrPath [out] 管理対象INIファイルへのパス
	*/
	void GetPrivateProfilePath( CString& cStrPath);

	/**
	* @brief 管理対象のINIファイル設定関数
	*
	* @param lpszPath [in] 管理対象にするINIファイルへのパス
	*/
	void SetPrivateProfilePath( LPCTSTR lpszPath);

private:
	enum MAX_LENGTH
	{
		DEFAULT_ALLOC = 512	//!< デフォルトの文字列取得エリアサイズ
	};
	CString m_cStrInifile;	//!< 管理対象iniファイルのパス
};

/**
* @class CRasterInifileHelper
*
* @brief ラスタファイル読み込みコマンド設定INIファイ用クラス。
*
* @note INIファイルの例
* @note [rasterfile]
* @note basedir=C:\TChiba\ArcWORK\ラスタデータ\
* @note purpose0=地形図ラスタ
* @note 
* @note [地形図ラスタ]
* @note ; 入力コードのタイトル
* @note codealias=メッシュコード
* @note ; 入力コードの最大長最小長
* @note ; 0以下設定では無チェック
* @note codemin=8
* @note codemax=8
* @note ; コードからのパス変換
* @note ; %.[文字列長]s
* @note ; %sですべて
* @note code2path=%.4s\%.6s\C%s.tif
* @note subdir=地形図\
* @note ; C:\TChiba\ArcWORK\ラスタデータ\地形図\xxxx\xxxxxx\Cxxxxxxxx.tif
**/
class CRasterInifileHelper : public CInifileHelperRoot
{
public:
	CRasterInifileHelper( LPCTSTR lpszPath = NULL);
	~CRasterInifileHelper();

	/**
	* @brief ベースディレクトリパス取得関数
	*
	* @param cStrBaseDir [out] ベースディレクトリ
	*/
	void GetBaseDir( CString& cStrBaseDir);

	/**
	* @brief ベースディレクトリパス設定関数
	*
	* @note ラスタファイル読み込みコマンド用INIファイルへ、ベースディレクトリを設定
	*
	* @param lpcszBaseDir [in] ベースディレクトリ
	*/
	void WriteBaseDir( LPCTSTR lpcszBaseDir);

	/**
	* @brief ラスタの種類取得関数
	*
	* @note ラスタファイル読み込みコマンド用INIファイルから、ラスタ種別を取得
	*
	* @param nIndex [in] Purpose[n]のインデックス値
	* @param cStrPurpose [out] ラスタ種別名称
	*/
	void GetPurpose( int nIndex, CString& cStrPurpose);

	/**
	* @brief ラスタの種類設定関数
	*
	* @note ラスタファイル読み込みコマンド用INIファイルへ、ラスタ種別を設定
	*
	* @param nIndex [in] Purpose[n]のインデックス値
	* @param lpcszPurpose [in] ラスタ種別名称
	*/
	void WritePurpose( int nIndex, LPCTSTR lpcszPurpose);

	/**
	* @brief ラスタの最大表示枚数取得関数
	*
	* @note ラスタファイル読み込みコマンド用INIファイルから、ラスタ最大表示枚数を取得
	*
	* @param nMax [out] ラスタ最大表示枚数
	*/
	void GetRasterMax( int& nMax);

	/**
	* @brief 入力コード名称取得関数
	*
	* @note 入力コードタイトルを取得
	*
	* @param lpcszPurpose [in] ラスタ名称
	* @param cStrAlias [out] 入力コードタイトル
	*/
	void GetAlias( LPCTSTR lpcszPurpose, CString& cStrAlias);

	/**
	* @brief 入力コード名称設定関数
	*
	* @note 入力コードタイトルをINIファイルへ保存
	*
	* @param lpcszPurpose [in] ラスタ名称
	* @param lpcszAlias [in] 入力コードタイトル
	*/
	void WriteAlias( LPCTSTR lpcszPurpose, LPCTSTR lpcszAlias);

	/**
	* @brief 入力コード最大長最小長取得関数
	*
	* @note コード入力をサポートするため、指定の範囲に該当する場合のみＯＫボタンをEnableにするために利用する。
	*
	* @param lpcszPurpose [in] ラスタ名称
	* @param nMax [out] 最大文字列長
	* @param nMin [out] 最小文字列長
	*/
	void GetMaxMin( LPCTSTR lpcszPurpose, int& nMax, int& nMin);

	/**
	* @brief 入力コード最大長最小長設定関数
	*
	* @param lpcszPurpose [in] ラスタ名称
	* @param nMax [in] 最大文字列長
	* @param nMix [in] 最小文字列長
	*/
	void WriteMaxMin( LPCTSTR lpcszPurpose, int nMax, int nMin);

	/**
	* @brief コードパス変換書式取得関数
	*
	* @note 入力された文字列をパスへ変換するための、変換種を取得する関数。
	* @note spritfの%sを活用
	*
	* @param lpcszPurpose [in] ラスタ名称
	* @param cStrSeed [out] 変換種文字列
	*/
	void GetSeed( LPCTSTR lpcszPurpose, CString& cStrSeed);

	/**
	* @brief ラスタファイル拡張子取得関数
	*
	* @note ラスタファイル拡張子を取得する関数。
	*
	* @param lpcszPurpose [in] ラスタ名称
	* @param cStrExt [out] ラスタファイル拡張子
	*/
	void GetFileExtension( LPCTSTR lpcszPurpose, CString& cStrExt);

	/**
	* @param コードパス変換書式設定関数
	*
	* @param lpcszPurpose [in] ラスタ名称
	* @param lpcszSeed [in] 変換種文字列
	*/
	void WriteSeed( LPCTSTR lpcszPurpose, LPCTSTR lpcszSeed);

	/**
	* @brief サブディレクトリ取得関数
	*
	* @note 指定コードから指定ファイルへのパスに変換する再に利用する、サブディレクトリ値
	* @note [BaseDir]\[Subdir]\[Seed]への変換時に利用
	*
	* @param lpcszPurpose [in] ラスタ名称
	* @param cStrSubdir [out] 変換時に利用されるサブディレクトリ文字列
	*/
	void GetSubdir( LPCTSTR lpcszPurpose, CString& cStrSubdir);

	/**
	* @brief サブディレクトリ設定関数
	*
	* @param lpcszPurpose [in] ラスタ名称
	* @param lpcszSubdir [in] サブディレクトリ文字列
	*/
	void WriteSubdir( LPCTSTR lpcszPurpose, LPCTSTR lpcszSubdir);

   // void CRasterInifileHelper::GetPyramid( LPCTSTR lpcszPurpose, int *nPyramid);
	void GetPyramid(LPCTSTR lpcszPurpose, int& nPyramid);
	void WritePyramid(LPCTSTR lpcszPurpose, int nPyramid);
	void GetTransmissivity(LPCTSTR lpcszPurpose, int& nTransmissivity);
	void GetGroupLayer(LPCTSTR lpcszPurpose, int& nGroupLayer);
	


private:
	/**
	* @brief INIファイルからのデータ取得時に利用するバッファサイズ
	*
	* @note 設定は十分な値を適宜設定のこと
	*/
	enum MAX_LENGTH
	{
		_MAX_BASEDIR = 512,		//!< GetBaseDirで利用されるバッファサイズ
		_MAX_PURPOSE = 512,		//!< GetPurposeで利用されるバッファサイズ
		_MAX_CODEALIAS = 512,	//!< GetAliasで利用されるバッファサイズ
		_MAX_CODE2PATH = 512,	//!< GetSeedで利用されるバッファサイズ
		_MAX_SUBDIR = 512		//!< GetSubdirで利用されるバッファサイズ
	};
};

/**
* @class CInifileHelper
*
* @brief SiNDY-e Toolの設定ファイルにアクセスする、ヘルパクラス
*
* @note このクラスは、レジストリとiniファイルへのアクセスを包括的に行うクラス。
*
* @note INIファイルの例
* @note [currentdata]
* @note work=foobar
* @note user=Nanno,Darebeh
* @note [rasterfile]
* @note filepath=C:\TChiba\ArcWORK\Raster.ini
* @note [worklfile]
* @note filepath=C:\TChiba\ArcWORK\Work.ini
* @note [デフォルト]
* @note layerfile0=C:\TChiba\ArcWORK\defaultsymbol\DBTEST.Intersection.lyr
* @note layerfile1=C:\TChiba\ArcWORK\defaultsymbol\DBTEST.Node.lyr
* @note layerfile2=C:\TChiba\ArcWORK\defaultsymbol\DBTEST.Road_Link.lyr
* @note layerfile3=C:\TChiba\ArcWORK\defaultsymbol\DBTEST.Parking_link.lyr
* @note layerfile4=C:\TChiba\ArcWORK\defaultsymbol\DBTEST.Grad.lyr
* @note layerfile5=C:\TChiba\ArcWORK\defaultsymbol\DBTEST.Segment_Attr.lyr
* @note [Tools]
* @note ;編集対象検索範囲
* @note Tolerance=8
* @note [SketchTool]
* @note SplitRoadByRoad=1
* @note SplitRoadByNode=1
* @note [EditTool]
* @note SplitRoadByRoad=0
* @note SplitRoadByNode=0
* @note ;ターゲットレイヤは、|(OR)で区切ります。先頭と末尾にも必要で、１だけ指定する場合にも同様です。
* @note ;TargetLayers=|Node|Road_Link|
* @note TargetLayers=|Node|Road_Link|Parking_link|Segment_Attr|Grad|Vics|Turn_Regulation|Guide|Direction_Guide|Lane|Base_Station|Base_Site|Base_Railway|Base_Line|
* @note ;移動後のチェック対象レイヤ
* @note CheckLayers=|Node|Road_Link|Parking_link|Segment_Attr|Grad|Vics|Turn_Regulation|Guide|Direction_Guide|Lane|Base_Station|Base_Site|Base_Railway|Base_Line|
* @note [DeleteTool]
*/
class CInifileHelper  
{
public:
	CInifileHelper();
	virtual ~CInifileHelper();
	
	/**
	* @brief SiNDY-eのインストールルートパスを取得
	*
	* @param strInifilePath [out] INIファイルのパス
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetInstallRoot( CString& strInstallRoot );

	/**
	* @brief INIファイルのファイル名をフルパスで取得
	*
	* @param strInifilePath [out] INIファイルのパス
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetInifilePath( CString& strInifilePath );

	/**
	* @brief データベースサーバ名をINIファイルから取得
	*
	* @param strDBServerName [out] データベースサーバ名
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetSDEServerName( CString& strDBServerName );

	/**
	* @brief ラスタファイル定義ファイルへのアクセスヘルパクラスを初期化
	*
	* @param cStrRasterfilePath [out] 初期化するラスタファイル定義ファイルアクセスヘルパ
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetRasterfile( CRasterInifileHelper& cRasterfileHelper);

	/**
	* @brief ラスタファイル定義ファイルのパスを取得
	*
	* @param cRasterfileHelper [out] ラスタファイル定義ファイルのパス
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetRasterfilePath( CString& cRasterfileHelper);

	/**
	* @brief シンボル変更用定義ファイルのパスを取得
	*
	* @param strSymbolfilePath [out] シンボル変更用ファイルのパス
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetSymbolfilePath( CString& strSymbolfilePath);

	BOOL GetAddLayerfilePath( CString& strAddLayerPath);

	/**
	* @brief ユーザのワーニングレベル取得
	*
	* @return ワーニングレベル
	*/
	int GetWarningLevel( void);

	/**
	* @brief 選択エリア半径値取得
	*
	* @note 指定されている値はPixel単位。
	*
	* @return 選択エリア半径
	*/
	UINT GetSelectTolerance( void);

	/**
	* @brief 傾斜レイヤファイル
	*
	* @param cGrandLayerFile [out] 傾斜レイヤー用のレイヤーファイル
	*
	* return 傾斜レイヤー用のレイヤーファイル文字列長
	*/
	int GetGradLayerFile( CString& cGradLayerFile);
	/**
	* @brief 傾斜レイヤ
	*
	* @param cGradLayer [out] 傾斜レイヤーをレイヤーリストに追加する際のレイヤー名
	*
	* @return 傾斜レイヤーをレイヤーリストに追加する際のレイヤー名文字列長
	*/
	int GetGradLayer( CString& cGradLayer);


	/*
	/// シンボル変更ツールのクラスIDを取得
	BOOL GetChangeSymbolToolCLSID( LPCTSTR lpszSection, CString& strCLSID );
	/// 目的別シンボル変更用のレイヤファイル名を返す。無ければ FALSE。
	BOOL GetChangeSymbolLayerFileName( LPCTSTR lpszSection, LONG index, CString& strLayerFilePath );
	*/
private:
	/**
	* @brief イニシャライズ
	*
	* @note レジストリを参照し、設定ファイルのパス取得等をおこなう。
	*/
	void Initialize();

	/// レジストリ関係

	/**
	* @brief レジストリの設定
	*/
	void  SetRegistryKey( LPCTSTR lpszRegistryKey );

	/**
	* @brief レジストリへint値の書き込み
	*/
	BOOL  WriteRegInt( LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue );

	/**
	* @brief レジストリからint値の読み込み
	*/
	UINT  GetRegInt( LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault );

	/**
	* @brief レジストリへ文字列の書き込み
	*/
	BOOL  WriteRegString( LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue );

	/**
	* @brief レジストリへ文字列の読み込み
	*/
	void  GetRegString( LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault, 
						LPTSTR lpReturnedString, DWORD nSize );
private:
	HKEY				m_hKey;				//!< SiNDYの設定を保存しているレジストリへのハンドル
	CString				m_strInifilePath;	//!< 設定ファイルへのぱす
};

#include "InifileHelper.inl"

#endif // !defined(AFX_INIFILEHELPER_H__7746FE5E_5FFC_44A0_9BEC_76C538AAF0D0__INCLUDED_)
