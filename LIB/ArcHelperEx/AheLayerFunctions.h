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
 * @file LayerFunctions.h
 * @brief <b>ArcObjectsグローバルヘルパー関数定義ファイル</b>\n
 * このファイルには、主にレイヤ系のグローバル関数が定義されています。
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _LAYERFUNCTIONS_H_
#define _LAYERFUNCTIONS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "exportimport.h"

//namespace sindy {

/**
* @brief 名称比較対照
*/
enum FINDTYPE
{
	_FIND_TABLE = 0,	//!< テーブル名
	_FIND_ALIAS = 1,	//!< エイリアス名
	_FIND_MXDNAME = 2	//!< MXDファイル定義レイヤ名称
};

/**
 * @brief <b>レイヤからテーブルを取得する</b>\n
 * @param ipLayer	[in]	レイヤ
 * @return テーブル系のレイヤではない場合はNULL
 */
ITablePtr /*SINDYLIB_API*/ AheLayer2Table( ILayer* ipLayer );

/**
 * @brief <b>編集可能なテーブルかどうかをチェックする</b>\n
 * SDEの非バージョン対応レイヤは編集不可になります。
 * それ以外のテーブルは、編集開始されている場合は編集可になります。
 * SiNDY特有のルールは考慮しませんので注意して下さい。
 * @return 編集可ならtrue
 */
BOOL /*SINDYLIB_API*/ AheIsEditableTable( ITable* ipTable );

/// レイヤがデータベース的に編集可能かどうかをチェックする
BOOL AheIsEditableLayer( ILayer* ipLayer );

/**
 * @brief <b>テーブルが同じ物かどうかをチェックする</b>\n
 * テーブルのオーナー名とテーブル名のみでの比較になります。
 * @return 同じならtrue
 */
bool /*SINDYLIB_API*/ AheIsEqualTable( ITable* ipTable1, ITable* ipTable2 );

/**
 * @brief <b>レイヤからオーナー名.テーブル名を取得する</b>\n
 * この関数はCStringを返すため、DLL版では非公開です。
 * @param ipLayer	[in]	レイヤ
 * @return テーブル系のオブジェクトではない場合は空文字列を返す
 */
inline CString AheGetOwnerTableName( ITable* ipTable )
{
	CString strName;
	IDatasetPtr ipDataset( ipTable );
	if( ipDataset != NULL )
	{
		BSTR bstrName = 0;
		ipDataset->get_Name( &bstrName );
		strName = bstrName;
		SysFreeString(bstrName);
	}
	return strName;
}

//////////////////////////////////////////////////////////////////////
// レイヤ名・テーブル名取得に関する関数
//////////////////////////////////////////////////////////////////////
/**
 * @brief 接続ユーザ名を取得する
 * @param ipUnk [in] Row,Feature,Table,FeatureClass,FeatureLayer
 * return CString
 */
CString AheGetConnectUserName( IUnknownPtr ipUnk );

/**
 * Owner.Class名を取得する
 *
 * @param ipUnk			[in]			IFeature、IFeatureClass、ILayerインターフェースを持つオブジェクト
 *
 * @return CString
 */
CString AheGetOwnerDotClassName( IUnknownPtr ipUnk );

/**
 * フィーチャクラス名を取得する
 *
 * @param ipUnk			[in]			IFeature、IFeatureClass、ILayerインターフェースを持つオブジェクト
 *
 * @note AheGetLayerTableName() では ***.*** の形で返ることがありますが、
 * @note この関数は必ずフィーチャクラス名のみ返します
 *
 * @return フィーチャクラス名
 */
CString AheGetFeatureClassName( IUnknownPtr ipUnk );

/**
 * フィーチャクラスのオーナー名を取得する
 *
 * @param ipUnk			[in]			IFeature、IFeatureClass、ILayerインターフェースを持つオブジェクト
 *
 * @return フィーチャクラスのオーナー名
 */
CString AheGetFeatureClassOwnerName( IUnknownPtr ipUnk );

/**
 * レイヤのテーブル名を取得する
 *
 * @param ipUnk			[in]			IFeature、IFeatureClass、ILayerインターフェースを持つオブジェクト
 * @param bstrLayerName	[out]			レイヤのテーブル名
 *
 * @retval TURE テーブル名を取得できた
 * @retval FALSE テーブル名を取得できなかった
 */
BOOL AheGetLayerTableName( IMapPtr piMap, IUnknownPtr ipUnk, BSTR* bstrLayerName );	

/**
 * レイヤのエイリアス名を取得する
 *
 * @param ipUnk				[in]			IFeature、IFeatureClass、ILayerオブジェクトのうちのどれか
 * @param bstrLayerName		[out]			レイヤのテーブル名
 *
 * @retval TURE エイリアス名を取得できた
 * @retval FALSE エイリアス名を取得できなかった
 */
BOOL AheGetLayerAliasName( IMapPtr piMap, IUnknownPtr ipUnk, BSTR* bstrLayerName );

/**
 * レイヤのエイリアス名を取得する
 *
 * @param ipUnk				[in]			IFeature、IFeatureClass、ILayerオブジェクトのうちのどれか
 *
 * @return レイヤエイリアス名
 */
CString AheGetLayerAliasName( IUnknownPtr ipUnk );

/**
 * *.mxdファイルに保存されているレイヤ名を取得する
 *
 * @param ipUnk				[in]			IFeature、IFeatureClass、ILayerオブジェクトのうちのどれか
 * @param bstrLayerName		[out]			レイヤのテーブル名
 *
 * @retval TURE レイヤ名を取得できた
 * @retval FALSE レイヤ名を取得できなかった
 */
BOOL AheGetLayerMXDName( IMapPtr piMap, IUnknownPtr ipUnk, BSTR* bstrLayerName );
CString AheGetLayerMXDName( IMapPtr piMap, IUnknownPtr ipUnk);

/**
 * @brief IUnknown からレイヤ名を取得
 *
 * @note IFeatureClassが取得できない場合は*.mxdファイルに保存されている名前を使用
 *
 * @param piMap			[in]			IMap のインスタンス
 * @param ipUnk			[in]			IUnknown のインスタンス
 * @param bstrName		[out]			取得されたエイリアス名
 * @param type			[in]			取得したいエイリアスのタイプ
 *
 * @retval TURE 適合する座標を取得できた
 * @retval FALSE 適合する座標を取得できなかった
 */
BOOL AheGetLayerName( IMapPtr piMap, IUnknownPtr ipUnk, BSTR* bstrName, FINDTYPE type = _FIND_TABLE );

/**
 * @brief IUnknown からレイヤ名を取得
 *
 * @note IFeatureClassが取得できない場合は*.mxdファイルに保存されている名前を使用
 *
 * @param piMap			[in]			IMap のインスタンス
 * @param ipUnk			[in]			IUnknown のインスタンス
 * @param bstrName		[out]			取得されたエイリアス名
 * @param type			[in]			取得したいエイリアスのタイプ
 *
 * @retval TURE 取得できた
 * @retval FALSE 取得できなかった
 */
BOOL AheGetLayerName( IMapPtr piMap, IUnknownPtr ipUnk, LPCTSTR& lpszName, FINDTYPE type = _FIND_TABLE );

/**
 * @brief IUnknown からレイヤ名を取得
 *
 * @note IFeatureClassが取得できない場合は*.mxdファイルに保存されている名前を使用
 *
 * @param piMap			[in]			IMap のインスタンス
 * @param ipUnk			[in]			IUnknown のインスタンス
 * @param bstrName		[out]			取得されたエイリアス名
 * @param type			[in]			取得したいエイリアスのタイプ
 *
 * @retval 取得したレイヤ名（取得できなかった場合は空文字列）
 */
CString AheGetLayerName( IMapPtr piMap, IUnknownPtr ipUnk, FINDTYPE type = _FIND_TABLE );
//////////////////////////////////////////////////////////////////////
// レイヤ・テーブル・フィーチャクラス取得に関する関数
//////////////////////////////////////////////////////////////////////
/**
 * レイヤのテーブル名から ILayer を取得する（type = 0）
 *
 * @param ipUnk				[in]			IFeature、IFeatureClass、ILayerオブジェクトのうちのどれか
 * @param ipFeatureClass	[out]			取得された IFeatureClass オブジェクト
 * @param bEditalbeOnly		[in,optional]	編集可能なもののみを対象とする場合はTRUE（デフォルト：FALSE）
 *
 * @retval TURE IFeatureClass を取得できた
 * @retval FALSE IFeatureClass を取得できなかった
 */
BOOL AheGetLayerByTableName( IMapPtr piMap, LPCTSTR lpszLayerName, ILayer** ipRetLayer, BOOL bEditableOnly = FALSE );

/**
 * レイヤのテーブル名から ILayer を取得する（type = 0）
 *
 * @param ipMap				[in]			IMapPtr
 * @param lpcszLayerName	[in]			取得したいレイヤのテーブル名
 * @param bEditableOnly		[in,optional]	編集可能なもののみを対象とする場合はTRUE（デフォルト：FALSE）
 *
 * @return ILayerPtr
 */
ILayerPtr AheGetLayerByTableName( IMapPtr ipMap, LPCTSTR lpcszLayerName, BOOL bEditableOnly = FALSE );

/**
 * レイヤのエイリアス名から ILayer を取得する（type = 1）
 *
 * @param ipUnk				[in]			IFeature、IFeatureClass、ILayerオブジェクトのうちのどれか
 * @param ipFeatureClass	[out]			取得された IFeatureClass オブジェクト
 *
 * @retval TURE IFeatureClass を取得できた
 * @retval FALSE IFeatureClass を取得できなかった
 */
BOOL AheGetLayerByAliasName( IMapPtr piMap, LPCTSTR lpszLayerName, ILayer** ipRetLayer );

/**
 * レイヤのエイリアス名から ILayer を取得する（type = 1）
 *
 * @param ipMap				[in]			IMapPtr
 * @param lpcszLayerName	[in]			取得したいレイヤのエイリアス名
 *
 * @return ILayerPtr
 */
ILayerPtr AheGetLayerByAliasName( IMapPtr ipMap, LPCTSTR lpszLayerName );

/**
 * *.mxdファイルに保存されているレイヤ名から ILayer を取得する（type = 2）
 *
 * @param ipUnk				[in]			IFeature、IFeatureClass、ILayerオブジェクトのうちのどれか
 * @param ipFeatureClass	[out]			取得された IFeatureClass オブジェクト
 *
 * @retval TURE IFeatureClass を取得できた
 * @retval FALSE IFeatureClass を取得できなかった
 */
BOOL AheGetLayerByMXDName( IMapPtr piMap, LPCTSTR lpszLayerName, ILayer** ipRetLayer );

/**
 * *.mxdファイルに保存されているレイヤ名から ILayer を取得する（type = 2）
 *
 * @param ipMap				[in]			IMapPtr
 * @param lpcszLayerName	[in]			取得したいレイヤのMXDレイヤ名
 *
 * @return ILayerPtr
 */
ILayerPtr AheGetLayerByMXDName( IMapPtr ipMap, LPCTSTR lpszLayerName );

/**
 * レイヤ名から ILayer を取得する
 *
 *  type = 0 : レイヤのテーブル名で検索
 *  type = 1 : レイヤのエイリアス名で検索
 *  type = 2 : MXDに保存されているレイヤ名で検索
 *
 * @param lpszLayerName		[in]			取得したいレイヤのレイヤ名
 * @param ipRetLayer		[out]			取得された ILayer オブジェクト
 * @param type				[in,optional]	0 ～ 2 の値を取る。デフォルトは 0
 * @param bEditableOnly		[in,optional]	編集可能なもののみを対象とする場合はTRUE（デフォルト：FALSE）
 * @param bFullName			[in,optional]	テーブル名で検索時、Owner名を含める場合はTRUE（デフォルト：FALSE）
 *
 * @retval TURE ILayer を取得できた
 * @retval FALSE ILayer を取得できなかった
 */
BOOL AheGetLayerByName( IMapPtr piMap, LPCTSTR lpszLayerName, ILayer** ipRetLayer, FINDTYPE type = _FIND_TABLE, BOOL bEditableOnly = FALSE, BOOL bFullName = FALSE );

/**
 * レイヤ名から ILayer を取得する
 *
 *  type = 0 : レイヤのテーブル名で検索
 *  type = 1 : レイヤのエイリアス名で検索
 *  type = 2 : MXDに保存されているレイヤ名で検索
 *
 * @param lpszLayerName		[in]			取得したいレイヤのレイヤ名
 * @param type				[in]			0 ～ 2 の値を取る。デフォルトは 0
 * @param bEditableOnly		[in,optional]	編集可能なもののみを対象とする場合はTRUE（デフォルト：FALSE）
 * @param bFullName			[in,optional]	テーブル名で検索時、Owner名を含める場合はTRUE（デフォルト：FALSE）
 *
 * @return ILayerPtr
 */
ILayerPtr AheGetLayerByName( IMapPtr ipMap, LPCTSTR lpszLayerName, FINDTYPE type = _FIND_TABLE, BOOL bEditableOnly = FALSE, BOOL bFullName = FALSE );

/**
 * @brief TOCで現在選択されているレイヤ取得
 *
 * @note 基本的にTOCで現在選択しているレイヤを返す。
 *		グループレイヤ選択されている場合には、そのなかの編集可能されているレイヤを取得してくる
 *		そのグループレイヤの中に複数の編集可能レイヤがある場合には、選択しているグループレイヤを返します。
 *
 * @param piMxDoc		[in]	IMxDocument のインスタンス
 * @param ipRetLayer	[out]	取得された ILayer オブジェクト
 * @param bIsEditable	[in]　	編集開始されているレイヤをグループレイヤの下のレイヤから拾ってくるか？(デフォルト：true)
 *
 * @retval TURE ILayer を取得できた
 * @retval FALSE ILayer を取得できなかった
 */
BOOL AheGetSelectedLayer( IMxDocumentPtr piMxDoc, ILayer** ipRetLayer, bool bIsEditable = true);
 
/**
 * @brief テーブル名からITableを取得する
 *
 * @param piMap				[in]			IMap のインスタンス
 * @param lpszTableName		[in]			取得したい ITable 名
 * @param ipRetTable		[out]			取得された ITable のインスタンス
 * @param bReference		[in,optional]	REFERENCE. を取得したい場合は TRUE（デフォルト：FALSE）
 * @param bEditableOnly		[in,optional]	編集開始されているテーブルのみ取得する場合はTRUE（デフォルト：FALSE）
 *
 * @see AheOpenTableByTableName
 *
 * @retval TURE ITable を取得できた
 * @retval FALSE ITable を取得できなかった
 */
BOOL AheGetTableByTableName( IMapPtr piMap, LPCTSTR lpszTableName, ITable** ipTable, BOOL bReference = FALSE, BOOL bEditableOnly = FALSE );

/**
 * @brief テーブル名からITableを取得する
 *
 * @param ipMap				[in]			IMapPtr
 * @param lpszTableName		[in]			取得したいテーブル名
 * @param bReference		[in,optional]	REFERENCE. を取得したい場合は TRUE（デフォルト：FALSE）
 * @param bEditableOnly		[in,optional]	編集開始されているテーブルのみ取得する場合はTRUE（デフォルト：FALSE）
 *
 * @see AheOpenTableByTableName
 *
 * @retval NULL テーブル取得失敗
 * @retval ITablePtr テーブル
 */
ITablePtr AheGetTableByTableName( IMapPtr ipMap, LPCTSTR lpszTableName, BOOL bReference = FALSE, BOOL bEditableOnly = FALSE  );

/**
 * @brief <b>データフレームに読み込まれているテーブル・フィーチャクラスを取得する</b>\n
 * nFindCountの値が1以上の場合は、取り扱いに注意して下さい。
 * @param ipMap					[in]			IMapPtr
 * @param lpcszOwnerTableName	[in]			オーナー.テーブル名
 * @param nFindCount			[out]			見つかった件数（同じテーブルの場合はカウントしない）
 * @param bEditableOnly			[in,optional]	編集可能なもののみ取得したい場合はtrue
 * @return テーブル・フィーチャクラスが取得できない場合はNULL
 */
ITablePtr /*SINDYLIB_API*/ AheGetTableByTableName2( IMap* ipMap, LPCTSTR lpcszOwnerTableName, int& nFindCount, bool bEditableOnly = true );

/**
 * @brief テーブル名からITableを取得する
 *
 * @param ipWorkspace		[in]			IWorkspacePtr
 * @param lpszTableName		[in]			取得したいテーブル名
 * @param bReference		[in,optional]	REFERENCE. を取得したい場合は TRUE（デフォルト：FALSE）
 *
 * @retval NULL テーブル取得失敗
 * @retval ITablePtr テーブル
 */
ITablePtr AheOpenTableByTableName( IWorkspacePtr ipWorkspace, LPCTSTR lpszTableName, BOOL bReference = FALSE );

/**
 * @brief 同じオーナーのテーブルをオープンする
 * 
 * @param ipTable			[in]			オープンしたいテーブルと同じオーナーのテーブル
 * @param lpcszTableName	[in]			オープンしたいテーブル名
 *
 * return ITablePtr
 */
ITablePtr AheOpenSameOwnerTable( IWorkspace* ipWork, ITable* ipTable, LPCTSTR lpcszTableName );
ITablePtr AheOpenSameOwnerTable( ITable* ipTable, LPCTSTR lpcszTableName );

/**
 * @brief 参照用テーブルを取得する
 *
 * @param ipMap				[in]			IMapPtr
 * @param lpcszTableName	[in]			取得したいテーブル名
 *
 * @see AheGetTableByTableName
 *
 * @retval NULL テーブル取得失敗
 * @retval ITablePtr テーブル
 */
ITablePtr AheGetReferenceTable( IMapPtr ipMap, LPCTSTR lpcszTableName );

/**
 * フィーチャクラスを取得する
 *
 * @param ipUnk				[in]			IFeature、IFeatureClass、ILayerオブジェクトのうちのどれか
 * @param ipFeatureClass	[out]			取得された IFeatureClass オブジェクト
 *
 * @retval TURE IFeatureClass を取得できた
 * @retval FALSE IFeatureClass を取得できなかった
 */
BOOL AheGetFeatureClass( IMapPtr piMap, IUnknownPtr ipUnk, IFeatureClass** ipFeatureClass );			//!< フィーチャクラスを取得

/**
 * フィーチャクラスを取得する
 * @param ipUnk				[in]	Row,Feature,Table,FeatureClass,FeatureLayerオブジェクトのうちのどれか
 * @retval ITablePtr
 */
ITablePtr AheGetTable( IUnknownPtr ipUnk );

/**
 * フィーチャクラスを取得する
 * @param ipUnk				[in]	Row,Feature,Table,FeatureClass,FeatureLayerオブジェクトのうちのどれか
 * @retval IFeatureClassPtr
 */
IFeatureClassPtr AheGetFeatureClass( IUnknownPtr ipUnk );
//} // sindy

#endif // _LAYERFUNCTIONS_H_

inline BOOL AheIsEditableLayer( ILayer* ipLayer ) {
	return AheIsEditableTable( AheGetTable( ipLayer ) );
}

inline IFeatureClassPtr AheGetFeatureClass( IUnknownPtr ipUnk ) {
	return AheGetTable( ipUnk );
}
