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
#include "Util.h"
#include "BusPointCommon/Connect.h"
#include <WinLib/ExcelHelper.h>


/**
 * @class   CImportBusPoint
 * @brief   バスデータのインポートのためのメインクラス
 *          TODO: バスライブラリ活用
 */
class CImportBusPoint
{
public:
	/**
	 * @brief	コンストラクタ
 	 * @param	args [in] 各種オプション
	 */
	CImportBusPoint( const Arguments& args ){
		m_strInFile  = (args.in_list).c_str();
		m_strImpLog  = (args.import_log).c_str();	
		m_strARPLog  = (args.auto_rpoint_log).c_str();
		m_strBusDB   = (args.bus_db).c_str();
		m_strRoadDB  = (args.road_db).c_str();

		m_import     = args.bImport;
		m_autoRPoint = args.bAutoRPoint;
	};

	/**
	 * @brief	デストラクタ
	 */
	~CImportBusPoint(){};

	/**
	 * @brief	各種処理を実行するために最初に呼ばれる関数
	 * @retval	true  処理成功
	 * @retval	false 処理失敗
	 */
	bool execute();

private:

	typedef std::map<CString, CString>       BUSCONTAINER; //!< バスレコード管理用のフィールド名・値map

	typedef std::pair<CString, CString>      CSTRPAIR;     //!< CStringのペア

	typedef std::map<CSTRPAIR, BUSCONTAINER> BUSPOINTMAP;  //!< BUS_STOP_POINTインポート用map
	typedef std::map<CSTRPAIR, BUSCONTAINER> BUSMASTERMAP; //!< BUS_LINE_MASTERインポート用map
	typedef std::vector<BUSCONTAINER>        BUSSEQVEC;    //!< BUS_STOP_SEQインポート用vector(兼Excel読み込み先)

	/**
	 * @brief   各種初期化
	 * @retval  true  初期化成功
	 * @retval  false 初期化失敗
	 */
	bool Init();

	/**
	 * @brief   調査結果ファイルリストを読み込み、ファイルパス1行ずつをリストに格納
	 * @retval  true  処理成功
	 * @retval  false 処理失敗
	 */
	bool LoadList();

	/**
	 * @brief   列名をリストに格納
	 * @note    各シートや各ファイル間で万一列名が揺れた場合を考え、独自に格納する
	 */
	void SetColumnName();

	/**
	 * @brief   BUS_LINE_MASTERから既存の系統番号（ROUTE_NUMBER）を全て取得する
	 */
	void GetExistRouteNumber();

	/**
	 * @brief   BUS_STOP_POINTから既存の停留所コードと標柱番号のペアを全て取得する
	 */
	void GetExistStopAndPole();

	/**
	 * @brief   調査結果ファイルから、インポート用リスト作成
	 * @note    それぞれのリストのユニーク条件に応じてリストを作成する
	 * @param   busStopPoints  [out] バス停ポイントインポート用リスト
	 * @param   busLineMasters [out] バス路線マスタインポート用リスト
	 * @param   busStopSeqs    [out] バス停車順管理インポート用リスト
	 * @retval  true  作成成功
	 * @retval  false 作成失敗
	 */
	bool LoadExcelData( BUSPOINTMAP& busStopPoints, BUSMASTERMAP& busLineMasters, BUSSEQVEC& busStopSeqs );

	/**
	 * @brief   調査結果ファイルの1行から、コンテナに読み込み
	 * @param   ipWorkSheet [in]  調査結果ワークシート
	 * @param   lineIdx     [in]  行番号
	 * @param   busInput    [out] 調査結果1行格納コンテナ
	 * @retval  true  1行読み込み成功
	 * @retval  false 1行読み込み失敗
	 */
	bool GetLineFromSheet( const Excel::_WorksheetPtr& ipWorkSheet, const long lineIdx, BUSCONTAINER& busInput );

	/**
	 * @brief   セルの値を取得
	 * @param   sa        [in]  Excelシート1行分のSafeArray
	 * @param   Idx       [in]  取得したいセルのインデックス
	 * @retval  セルの値
	 */
	 CString GetCellStr( CComSafeArray<VARIANT>& sa, long idx )
	 {
		CComVariant vaValue;
		long cellIdx[2] = {1, idx};
		sa.MultiDimGetAt( cellIdx, vaValue );
		return CellToString( vaValue );
	 };

	/**
	 * @brief   セルの値をCStringで返す
	 * @param   vaValue [in] セルの情報を持ったCComVariant型
	 * @retval  CString型のセルの値
	 */
	CString CellToString( const CComVariant& vaValue )
	{
		CString retStr;
		if( VT_R8 == vaValue.vt){
			// 小数点以下無し（四捨五入されるが、整数のみ指定フォーマットのため、問題なし）
			retStr.Format(_T("%.lf"), vaValue.dblVal);
		}
		else{
			retStr = (VT_BSTR == vaValue.vt)? vaValue.bstrVal : _T("");
		}
		return retStr;
	};

	/**
	 * @brief   バス停ポイントインポート用リストへの値セット
	 * @param   busInput     [in]  調査結果1行
	 * @param   busStopPoint [out] バス停ポイントインポート1行
	 * @retval  true  セット成功
	 * @retval  false セット失敗
	 */
	bool SetBusStopPoint( const BUSCONTAINER& busInput, BUSCONTAINER& busStopPoint );

	/**
	 * @brief   バス路線マスタインポート用リストへの値セット
	 * @param   busInput      [in]  調査結果1行
	 * @param   companyName   [in]  調査結果ファイル(事業者)名
	 * @param   busLineMaster [out] バス路線マスタインポート1行
	 * @retval  true  セット成功
	 * @retval  false セット失敗
	 */
	bool SetBusLineMaster( const BUSCONTAINER& busInput, const CString& companyName, BUSCONTAINER& busLineMaster );

	/**
	 * @brief   バス停車順管理インポート用リストへの値セット
	 * @param   busInput     [in]  調査結果1行
	 * @param   busStopPoint [out] バス停車順管理インポート1行
	 * @retval  true  セット成功
	 * @retval  false セット失敗
	 */
	bool SetBusStopSeq( const BUSCONTAINER& busInput, BUSCONTAINER& busStopSeq );

	/**
	 * @brief   調査結果からSDEのバス関連テーブルにインポート
	 * @param   busStopPoints  [in] バス停ポイントインポート用リスト
	 * @param   busLineMasters [in] バス路線マスタインポート用リスト
	 * @param   busStopSeqs    [in] バス停車順管理インポート用リスト
	 * @retval  true  インポート成功
	 * @retval  false インポート失敗
	 */
	bool ImportBusPoint( const BUSPOINTMAP& busStopPoints, const BUSMASTERMAP& busLineMasters, const BUSSEQVEC& busStopSeqs );

	/**
	 * @brief   バス停ポイントインポートリストレコードから、形状・属性をFeatureBufferにセット
	 * @param   busStopPoint         [in]  バス停ポイントインポートリストレコード
	 * @param   ipBusStopPointBuffer [out] インポート用FeatureBuffer
	 * @retval  true  形状・属性のセット成功
	 * @retval  false 失敗
	 */
	bool SetWriteBusStopPoint( const BUSCONTAINER& busStopPoint, IFeatureBufferPtr& ipBusStopPointBuffer );

	/**
	 * @brief   バス路線マスタテーブルインポートリストレコードから、属性をRowBufferにセット
	 * @param   busLineMaster         [in]  バス路線マスタテーブルインポートリストレコード
	 * @param   ipBusLineMasterBuffer [out] インポート用RowBuffer
	 * @retval  true  属性のセット成功
	 * @retval  false 失敗
	 */
	bool SetWriteBusLineMaster( const BUSCONTAINER& busLineMaster, IRowBufferPtr& ipBusLineMasterBuffer );

	/**
	 * @brief   バス停車順管理テーブルインポートリストレコードから、属性をRowBufferにセット
	 * @param   busStopSeq         [in]  バス停車順管理テーブルインポートリストレコード
	 * @param   ipBusStopSeqBuffer [out] インポート用RowBuffer
	 * @retval  true  属性のセット成功
	 * @retval  false 失敗
	 */
	bool SetWriteBusStopSeq( const BUSCONTAINER& busStopSeq, IRowBufferPtr& ipBusStopSeqBuffer );

	/**
	 * @brief   標柱ポイントに対応するルート探索用ポイントを追加
	 * @retval  true  追加成功
	 * @retval  false 追加失敗
	 */
	bool CreateRPoints();

	/**
	 * @brief   作成対象のバス停ポイントObjectIDリストを作成する
	 * @note    ログ出力用にOIDと合わせて、バス停ポイントの情報も取得する
	 * @param   busPoints [out] バス停ポイントリスト
	 * @retval  true  リスト作成成功
	 * @retval  false 失敗
	 */
	bool CreateOIDList( std::set<std::list<CString>>& busPoints );

	/**
	 * @brief   ルート探索用ポイントの追加対象か調べる
	 * @param   busPointOID [in] バス停ポイントObjectID
	 * @retval  true  追加対象
	 * @retval  false 追加非対象（既にルート探索用ポイントが存在）
	 */
	bool IsRPointTarget( const long busPointOID );

	/**
	 * @brief   ルート探索用ポイント作成対象のバス停ポイント情報を取得する
	 * @param   ipFeature [in]  バス停ポイント
	 * @param   busPoint    [out] バス停ポイントのフィールド情報
	 * @retval  true  情報取得成功
	 * @retval  false 失敗
	 */
	bool GetBusPointInfo( const IFeaturePtr& ipFeature, std::list<CString>& busPoint ); 

	/**
	 * @brief   バス停ポイントのOIDから、バスルート探索用ポイント追加用FeatureBufferに形状・属性をセット
	 * @param   busStopPointOID      [out] バス停ポイントObjectID
	 * @param   ipBusStopPointBuffer [out] インポート用FeatureBuffer
	 * @retval  true  形状・属性セット成功
	 * @retval  false 失敗
	 */
	bool SetWriteBusRoutePoint( const long busStopPointOID, IFeatureBufferPtr& ipBusStopPointBuffer );

	/**
	 * @brief   最近傍リンク上の最短距離の点を求める
	 * @param   ipFeature [in] バス停ポイント
	 * @retval  IPointPtr バスルート探索用ポイント作成位置（NULL：見つからなかった）
	 */
	IPointPtr GetNearestPoint(const IFeaturePtr& ipFeature);

private:

	CString            m_strInFile;                  //!< 調査結果ファイルリスト名
	CString            m_strImpLog;                  //!< インポートモードログファイル名
	CString            m_strARPLog;                  //!< ルート探索用ポイントモードログファイル名

	CString            m_strBusDB;                   //!< バス関連データベース接続名
	CString            m_strRoadDB;                  //!< 道路データベース接続名

	std::ofstream      m_ofsImp;                     //!< インポートログ
	std::ofstream      m_ofsARP;                     //!< ルート探索用ポイントログ

	std::vector<std::string> m_fileList;             //!< 調査結果ファイルリスト

	bool               m_import;                     //!< インポートモード
	bool               m_autoRPoint;                 //!< ルート探索用ポイントモード

	IWorkspaceEditPtr  m_ipBusWorkspace;             //!< バス関連ワークスペース
	IWorkspacePtr      m_ipRoadWorkspace;            //!< 道路ワークスペース
	
	IFeatureClassPtr   m_ipBusStopPointFC;           //!< バス停ポイントフィーチャクラス
	ITablePtr          m_ipBusLineMasterTable;       //!< バス路線マスタテーブル
	ITablePtr          m_ipBusStopSeqTable;          //!< バス停車順管理テーブル
	IFeatureClassPtr   m_ipBusRoutePointFC;          //!< バスルート探索用ポイントフィーチャクラス
	IFeatureClassPtr   m_ipRoadLinkFC;               //!< 道路リンクフィーチャクラス

	CFieldMap          m_busStopPointFM;             //!< バス停ポイントフィールドマップ
	CFieldMap          m_busLineMasterFM;            //!< バス路線マスタフィールドマップ
	CFieldMap          m_busStopSeqFM;               //!< バス停車順管理フィールドマップ
	CFieldMap          m_busRoutePointFM;            //!< バスルート探索用ポイントフィールドマップ

	std::list<CString> m_columnNameList;             //!< カラム名格納用リスト

	std::set<long>     m_existRouteNumbers;          //!< 既にBUS_LINE_MASTERに存在する系統番号リスト
	std::set<std::pair<long, long>> m_existStopPole; //!< 既にBUS_STOP_POINTに存在する停留所コードと標柱番号のペアリスト
};
