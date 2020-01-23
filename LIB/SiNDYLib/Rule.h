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
 * @file Rule.h
 * CRuleクラス定義ファイル
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _RULE_H_
#define _RULE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AnnotationClassTable.h"
#include "MovableLayersTable.h"
#include "DeletableLayersTable.h"
#include "EditAttrLayersTable.h"
#include "CreatableLayersTable.h"
#include "WorkPurposeTable.h"
#include "LayerNamesTable.h"
#include "SiNDYeSettings.h"
#include "BackGroundClassTable.h"
#include "ConnectUserTable.h"
#include "DispNaviRelationTable.h"
#include "DispOtherRelationTable.h"
#include "EditHistoryTable.h"
#include "OperatorTable.h"
#include "RoadAttributeTable.h"
#include "RoadDispClassTable.h"
#include "RoadNaviClassTable.h"
#include "SharedDeleteLayersTable.h"
#include "SharedMoveLayersTable.h"
#include "LockAreaTable.h"
#include "LockMeshTable.h"
#include "EditMeshTable.h"
#include "RuleCommunity.h"
#include "AutoValue.h"

class CListString;
class CListLong;
class CSiNDYRuleObj;
namespace sindy {
/**
 * @class CRule
 * @brief <b>ルール管理クラス</b>\n
 * このクラスは、ルールをキャッシュして使用することが前提に
 * なっていますので、使用する前に必ずCreateRuleCacheを呼ぶ
 * 必要があります。ただし、以下の場合は自動的に作成されます。
 * @li ログインダイアログを表示した時
 * @li sindy::CApplicationを初期化した時
 * @li sindy::CWorkspacesで接続を管理する場合（バッチ系）
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CRule
{
public:
	CRule();
	virtual ~CRule();

	friend class CSiNDYRuleObj;

	/**
	 * @brief <b>ルールのキャッシュを作成する</b>\n
	 * キャッシュを作成した後は、接続を切ります。
	 * 既にキャッシュが存在する場合は何もしません。
	 * キャッシュを作り直したい場合はRecreateRuleCacheを
	 * 呼んでください。
	 * 
	 * @note 特定の作業目的だけキャッシュしたい場合はlPurposeIDに作業目的IDをセットします。
	 * デフォルトの状態では全作業目的をキャッシュします。
	 *
	 * @param bAnnoRule	[in,optional] 注記のルールを読み込む場合はtrue（デフォルト：true）
	 * @param bEditRule [in,optional] 編集ルールを読み込む場合はtrue（デフォルト：true）
	 * @param bBGClass [in,optional] 背景種別ルールを読み込む場合はtrue（デフォルト：true）
	 * @param lPurposeID [in,optional] 作業目的を限定してキャッシュを作成する（デフォルト：-1）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CreateRuleCache( bool bAnnoRule = true, bool bEditRule = true, bool bBGClass = true, long lPurposeID = -1 );

	/**
	 * @brief <b>ルールのキャッシュを作成しなおす</b>\n
	 * キャッシュを作成した後は、接続を切ります。
	 * 既にキャッシュが存在する場合はキャッシュを削除してから作成します。
	 * 
	 * @note 特定の作業目的だけキャッシュしたい場合はlPurposeIDに作業目的IDをセットします。
	 * デフォルトの状態では全作業目的をキャッシュします。
	 *
	 * @param bAnnoRule	[in,optional] 注記のルールを読み込む場合はtrue（デフォルト：true）
	 * @param bEditRule [in,optional] 編集ルールを読み込む場合はtrue（デフォルト：true）
	 * @param bBGClass [in,optional] 背景種別ルールを読み込む場合はtrue（デフォルト：true）
	 * @param lPurposeID [in,optional] 作業目的を限定してキャッシュを作成する（デフォルト：-1）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode RecreateRuleCache( bool bAnnoRule = true, bool bEditRule = true, bool bBGClass = true, long lPurposeID = -1 );

	/**
	 * @brief ルールキャッシュをクリアする
	 *
	 * @param isFinal [in,optional] CFieldMapやCTableNameStringまでクリアする場合はtrue（デフォルト：false）
	 * @note isFinal引数はC++/CLIでどうしても必要なのでつけました。
	 * C++で使用する分には考慮する必要はありません。
	 */
	void ClearRuleCache( bool isFinal = false );

	/**
	 * @brief <b>注記ルールテーブルを返す</b>\n <- 意味ないので廃止
	 * @return CAnnotationClassTable&
	 */
/*	const annotation_class::CAnnotationClassTable& GetAnnotationClassTable() const {
		return m_cAnnotationClassTable;
	}*/
	/**
	 * @brief <b>注記ルールレコードを取得する</b>\n
	 * @param emTableType	[in]	テーブルタイプ
	 * @param emCode		[in]	注記種別
	 * @return 該当レコードがない場合はNULLを返す
	 */
	const annotation_class::CAnnotationClass* GetAnnoRule( sindyTableType::ECode emTableType, schema::annotation::annotation_code::ECode emCode ) const;

	/**
	 * @brief <b>注記ルールキャッシュテーブルを取得する</b>\n
	 * @retval NULL						エラーまたはキャッシュが作成されていない
	 * @retval CAnnotationClassTable*	注記ルールキャッシュテーブル
	 */
	const annotation_class::CAnnotationClassTable* GetAnnotationClassTable() const;

	/**
	 * @brief <b>背景ルールキャッシュテーブルを取得する</b>\n
	 * @retval NULL						エラーまたはキャッシュが作成されていない
	 * @retval CBackGroundClassTable*	注記ルールキャッシュテーブル
	 */
	const background_class::CBackGroundClassTable* GetBackGroundClassTable() const;

	/**
	 * @brief <b>編集可能メッシュテーブルを取得する</b>\n
	 * @return 編集可能メッシュテーブル
	 */
	const editmesh::CEditMeshTable* GetEditMeshTable() const;

	//@{ @name SiNDY-e編集ルールに関する関数
	/**
	 * @brief <b>現在の作業目的の作業目的レコードを取得する</b>\n
	 * @return 該当レコードがない場合はNULLを返す
	 */
	const workpurpose::CWorkPurpose* GetWorkPurpose() const;

	/**
	 * @brief <b>現在の作業目的IDを取得する</b>\n
	 * @retval 0以上 作業目的ID
	 * @retval -1 ログインしていない
	 */
	long GetWorkPurposeID() const;

	/**
	 * @brief <b>現在のMXDファイル名を取得する</b>\n
	 * ログインしていることを確認した上で使用してください。
	 * @param nLen		[in]	バッファ格納可能文字数（workpurpose::_MXD_MAX_LENを使用すること）
	 * @param lpszMXD	[out]	MXDファイル名格納バッファ
	 * @retval NULL	ログインしていない、又はエラー
	 * @retval LPCTSTR 取得成功
	 */
	LPCTSTR GetMXDFileName( int nLen, LPTSTR lpszMXD ) const;

	/**
	 * @brief <b>現在の自動属性付与の値を取得する</b>\n
	 * ログインしていることを確認した上で使用してください。
	 * @note なるべくGetAutoValueを使用してください。
	 * @param nLen		[in]	バッファ格納可能文字数（workpurpose::_MXD_MAX_LENを使用すること）
	 * @param lpszMXD	[out]	現在の自動属性付与の値
	 * @retval NULL	ログインしていない、又はエラー
	 * @retval LPCTSTR 取得成功
	 */
	LPCTSTR GetAutoAttrGiving( int nLen, LPTSTR lpszAutoAttrGiving ) const;
	/**
	 * @brief <b>現在の自動属性付与の値を取得する</b>\n
	 * ログインしていることを確認した上で使用してください。
	 * GetAutoAttrGivingと役割は同じですが、解析したものを返します。
	 * @note SetLoginInfoが呼ばれた段階で初期化されます。
	 * @return CAutoValue
	 */
	const CAutoValue& GetAutoValue() const { return m_cAutoValue; }

	/**
	 * @brief <b>作業目的テーブルの参照を取得する</b>\n
	 * @warning キャッシュ作成後に呼んでください。
	 * @return 作業目的テーブルの参照
	 */
	const workpurpose::CWorkPurposeTable& GetWorkPurposeTable() const;

	/**
	 * @brief <b>レイヤ名テーブルの参照を取得する</b>\n
	 * @warning キャッシュ作成後に呼んでください。
	 * @return レイヤ名テーブルの参照
	 */
	const layernames::CLayerNamesTable& GetLayerNamesTable() const;

	/**
	 * @brief <b>作成可能レイヤキャッシュテーブルを取得する</b>\n
	 * @retval NULL						エラーまたはキャッシュが作成されていない
	 * @retval CCreatableLayersTable&	作成可能レイヤキャッシュテーブル
	 */
	const creatablelayers::CCreatableLayersTable& GetCreatableLayersTable() const;

	/**
	 * @brief <b>編集可能かどうかをチェックする</b>\n
	 * @param cRow			[in]			チェック対象オブジェクト
	 * @param emEditType	[in]			編集タイプ
	 * @param nLen			[in]			バッファ格納可能文字列
	 * @param lpszMessage	[out]			エラーメッセージ格納用バッファ
	 * @param check_modify	[in,optional]	ModifyCheckをするかどうか（デフォルト：true）
	 * @return sindyeEditableType
	 */
	sindyeEditableType IsEditable( const CRow& cRow, sindyeEditType emEditType, int nLen, LPTSTR lpszMessage, bool check_modify = true ) const;

	/**
	 * @brief <b>編集済みかどうかをチェックする</b>\n
	 * 内部でModifyCheck.dllを使用します。
	 * カスタムポストモードの時には動作しません。
	 * @param cRow			[in]	チェック対象オブジェクト
	 * @param nLen			[in]	バッファ格納可能文字列
	 * @param lpszMessage	[out]	エラーメッセージ格納用バッファ
	 * @retval true 編集済み
	 * @retval false 編集されていない
	 */
	bool IsModified( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;

	/**
	 * @brief <b>移動可能かどうかをチェックする</b>\n
	 * @param cRow			[in]	チェック対象オブジェクト
	 * @param nLen			[in]	lpszMessage収納可能文字数
	 * @param lpszMessage	[out]	チェック結果メッセージ
	 * @return sindyeEditableType
	 */
	sindyeEditableType IsMovable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;

	/**
	 * @brief <b>削除可能かどうかをチェックする</b>\n
	 * @param cRow			[in]	チェック対象オブジェクト
	 * @param nLen			[in]	lpszMessage収納可能文字数
	 * @param lpszMessage	[out]	チェック結果メッセージ
	 * @return sindyeEditableType
	 */
	sindyeEditableType IsDeletable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;

	/**
	 * @brief <b>作成可能かどうかをチェックする</b>\n
	 * 内部ではテーブル名のみで判断するので、cRowは空のまま渡すか、
	 * 又は同じレイヤのオブジェクトを渡してください。
	 * @param cRow			[in]	チェック対象オブジェクト
	 * @param nLen			[in]	lpszMessage収納可能文字数
	 * @param lpszMessage	[out]	チェック結果メッセージ
	 * @return sindyeEditableType
	 */
	sindyeEditableType IsCreatable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;

	/**
	 * @brief <b>属性編集可能かどうかをチェックする</b>\n
	 * @param cRow			[in]	チェック対象オブジェクト
	 * @param nLen			[in]	lpszMessage収納可能文字数
	 * @param lpszMessage	[out]	チェック結果メッセージ
	 * @return sindyeEditableType
	 */
	sindyeEditableType IsAttributeEditable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;

	/**
	 * @brief <b>編集可能領域かどうかをチェックする</b>\n
	 * @param lpcszName		[in]	チェック対象レイヤ名
	 * @param ipGeom		[in]	チェック対象エリア
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode IsEditableArea( LPCTSTR lpcszName, IGeometry* ipGeom ) const;

	/**
	 * @brief <b>ロックされているメッシュかどうかをチェックする</b>\n
	 * @param lpcszConnUser	[in]	チェック対象接続ユーザ名
	 * @param lpcszName		[in]	チェック対象レイヤ名
	 * @param lCode			[in]	チェック対象メッシュコード
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode IsLockMesh( LPCTSTR lpcszConnUser, LPCTSTR lpcszName, long lCode ) const;

	/**
	 * @brief <b>ロックされているエリアかどうかをチェックする</b>\n
	 * @param lpcszConnUser	[in]	チェック対象接続ユーザ名
	 * @param lpcszName		[in]	チェック対象レイヤ名
	 * @param ipGeom		[in]	チェック対象エリア
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode IsLockArea( LPCTSTR lpcszConnUser, LPCTSTR lpcszName, IGeometry* ipGeom ) const;

	/**
	 * @brief <b>ログイン済みであるかどうかをチェックする</b>\n
	 * ログイン情報を取得する前に、必ずこのメソッドでログイン済みかどうか
	 * をチェックしてください。
	 * @retval true ログイン済み
	 */
	bool IsLogined() const;

	/**
	 * @brief <b>ログインする</b>\n
	 * @retval sindyErr_LoginCanceled ログインがキャンセルされた
	 * @retval sindyErr_LoginErr ログインに失敗した
	 * @retval sindyErr_NoErr ログイン成功
	 * @retval 上記以外 エラー
	 */
	errorcode::sindyErrCode Login();

	/**
	 * @brief <b>ログイン情報を書き込む</b>\n
	 * ログイン情報及び自動付与属性を変更します。ログイン時以外に使用すると、編集ルールの整合が取れなくなります。
	 */
	void SetLoginInfo( long lPurposeID, LPCTSTR lpcszUserName );

	/**
	 * @brief <b>保存可能な作業目的かどうかをチェックする</b>\n
	 * ログインしていない場合、又は作業目的レコードが見つからない場合は
	 * 保存可能と判断します。
	 * @retval true 保存可能（デフォルト）
	 * @retval false 保存不可能
	 */
	bool CanSaveEdit() const;

	/**
	 * @brief <b>作業目的IDから作業目的名を取得する</b>\n
	 * @param lID		[in]	作業目的ID
	 * @param nLen		[in]	lpszNameの収納可能文字列数
	 * @param lpszName	[out]	取得文字列を格納するバッファ
	 * @retval LPCTSTR 作業目的名
	 * @retval NULL 対応する作業目的名が存在しない、又はログインしていない
	 */
	LPCTSTR GetOperatingPurposeFromID( long lID, int nLen, LPTSTR lpszName ) const;

	/**
	 * @brief <b>現在の作業者名を取得する</b>\n
	 * @retval LPCTSTR 作業者名
	 * @retval NULL ログインしていない
	 */
	LPCTSTR GetOperatorName() const;

	/**
	 * @brief <b>編集可能レイヤの一覧を取得する</b>\n
	 * @param listLayers	[out]			移動可能レイヤリスト
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode GetMovableLayers( CListString& listLayers ) const;

	/**
	 * @brief <b>削除可能レイヤの一覧を取得する</b>\n
	 * @param listLayers	[out]			削除可能レイヤリスト
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode GetDeletableLayers( CListString& listLayers ) const;

	/**
	 * @brief <b>作成可能レイヤの一覧を取得する</b>\n
	 * @param listLayers	[out]			作成可能レイヤリスト
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode GetCreatableLayers( CListString& listLayers ) const;

	/**
	 * @brief <b>属性編集可能レイヤの一覧を取得する</b>\n
	 * @param lpszLayers	[out]			属性編集可能レイヤリスト
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode GetEditAttrLayers( CListString& listLayers ) const;

	/**
	 * @brief <b>作成可能レイヤかどうかチェックする</b>\n
	 * @param lpctstrLayer	[in]	チェック対象のレイヤ名
	 * @retval true 作成可能
	 * @retval false 作成不可能
	 */
	bool IsCreatableLayer( LPCTSTR lpcszLayer ) const;

	/**
	 * @brief <b>作成可能レイヤかどうかチェックする</b>\n
	 * @warning この関数では、属性による制限はチェック対象外です。
	 * 通常はIsDeletableを使用してください。
	 * @param lpctstrLayer	[in]	チェック対象のレイヤ名
	 * @retval true 削除可能
	 * @retval false 削除不可能
	 */
	bool IsDeletableLayer( LPCTSTR lpcszLayer ) const;

	/**
	 * @brief <b>移動可能レイヤかどうかチェックする</b>\n
	 * @warning この関数では、属性による制限はチェック対象外です。
	 * 通常はIsMovableを使用してください。
	 * @param lpctstrLayer	[in]	チェック対象のレイヤ名
	 * @retval true 移動可能
	 * @retval false 移動不可能
	 */
	bool IsMovableLayer( LPCTSTR lpcszLayer ) const;

	/**
	 * @brief <b>属性編集可能レイヤかどうかチェックする</b>\n
	 * @warning この関数では、属性による制限はチェック対象外です。
	 * 通常はIsAttributeEditableを使用してください。
	 * @param lpctstrLayer	[in]	チェック対象のレイヤ名
	 * @retval true 属性編集可能
	 * @retval false 属性編集不可能
	 */
	bool IsEditAttrLayer( LPCTSTR lpcszLayer ) const;

	/**
	 * @brief <b>渡されたレイヤ名に対応する共有移動対象レイヤ名をリストで返す</b>\n
	 * @param lpcszLayerName	[in]	レイヤ名
	 * @param listNames			[out]	共有移動対象レイヤ名リスト
	 * @return 共有移動レイヤ数
	 */
	long GetSharedMovableLayer( LPCTSTR lpcszLayerName, CListString& listNames ) const;

	/**
	 * @brief <b>渡されたレイヤ名に対応する共有削除対象レイヤ名をリストで返す</b>\n
	 * @param lpcszLayerName	[in]	レイヤ名
	 * @param listNames			[out]	共有削除対象レイヤ名リスト
	 * @return 共有削除レイヤ数
	 */
	long GetSharedDeletableLayer( LPCTSTR lpcszLayerName, CListString& listNames ) const;

	/**
	 * @brief <b>道路リンクの道路種別と経路種別間のルールを取得する</b>\n
	 * @param lRoadClassID	[in]	道路種別
	 * @param lNaviClassID	[in]	経路種別
	 * @return sindyeDispNaviRelation
	 */
	sindyeDispNaviRelation GetDispNaviRule(  long lRoadClassID, long lNaviClassID  ) const;

	/**
	 * @brief <b>道路リンクの道路種別とその他の属性間のルールを取得する</b>\n
	 * @param lRoadClassID		[in]	道路種別
	 * @param lpcszFieldName	[in]	その他の属性名
	 * @return sindyeDispOtherRelation
	 */
	sindyeDispOtherRelation GetDispOtherRule( long lRoadClassID, LPCTSTR lpcszFieldName ) const;
	//@}
	//@{ @name レジストリ・設定ファイル
	/**
	 * @brief <b>編集閾値を取得する</b>\n
	 * @return 編集閾値（ピクセル単位）
	 */
	int GetSelectTolerance() const;

	/**
	 * @brief <b>編集閾値をセットする</b>\n
	 * @param nPixel [in] 編集閾値（ピクセル単位）
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool SetSelectTolerance( int nPixel );

	/// トレースログファイル名を取得する
	LPCTSTR GetTraceLogFileName( int nLen, LPTSTR strBuf ) const;

	/// トレースログファイル名を設定する
	bool SetTraceLogFileName( LPCTSTR lpcszFileName );

	/// エラーログファイル名を取得する
	LPCTSTR GetErrorLogFileName( int nLen, LPTSTR strBuf ) const;

	/// エラーログファイル名を設定する
	bool SetErrorLogFileName( LPCTSTR lpcszFileName );

	/// 再描画バッファ値を取得する
	int GetRefreshBuffer() const;

	/// 再描画バッファ値を設定する
	bool SetRefreshBuffer( int nPixel );
	//@}
	/**
	 * @brief <b>整合性チェック用ルールを作成する</b>\n
	 * オブジェクトごとに適切な整合性チェック用ルールを作成します。
	 * @param cRow		[in]	チェック対象オブジェクト
	 * @param cRule		[out]	整合性チェック用ルール
	 * @return cRuleの参照
	 */
//	CLogicRule& CreateLogicRule( const CRow& cRow, CLogicRule& cRule ) const;
private:
	/**
	 * @brief <b>レイヤ名文字列作成用補助関数</b>\n
	 * この関数は補助関数です。
	 * GetSharedMovableLayer及びGetSharedDeletablelayerから
	 * 呼ばれます。
	 * @param listLayerNameIDs	[in]	レイヤ名IDが格納されているリスト
	 * @param listLayerNames	[out]	レイヤ名格納リスト
	 */
	void CRule::LayerNameID2LayerName( const CListLong& listLayerNameIDs, CListString& listLayerNames ) const;
private:
	HANDLE											m_hMutex;					//!< ルール読み込み時の排他処理用
	// 注記系
	annotation_class::CAnnotationClassTable			m_cAnnotationClassTable;	//!< 注記ルールテーブル
	// 背景系
	background_class::CBackGroundClassTable			m_cBackGroundClassTable;	//!< 背景ルールテーブル
	// 編集ルール系
	workpurpose::CWorkPurposeTable					m_cWorkPurposeTable;		//!< 作業目的管理テーブル
	soperator::COperatorTable						m_cOperatorTable;			//!< 作業者名管理テーブル
	connectuser::CConnectUserTable					m_cConnectUserTable;		//!< 接続ユーザ名管理テーブル
	deletablelayers::CDeletableLayersTable			m_cDeletableLayersTable;	//!< 削除可能レイヤ管理テーブル
	creatablelayers::CCreatableLayersTable			m_cCreatableLayersTable;	//!< 作成可能レイヤ管理テーブル
	movablelayers::CMovableLayersTable				m_cMovableLayersTable;		//!< 移動可能レイヤ管理テーブル
	editattrlayers::CEditAttrLayersTable			m_cEditAttrLayersTable;		//!< 属性編集可能レイヤ管理テーブル
	roadattribute::CRoadAttributeTable				m_cRoadAttributeTable;		//!< 道路属性名称管理テーブル
	roaddispclass::CRoadDispClassTable				m_cRoadDispClassTable;		//!< 道路リンクの道路種別管理テーブル
	roadnaviclass::CRoadNaviClassTable				m_cRoadNaviClassTable;		//!< 道路リンクの経路種別管理テーブル
	shareddeletelayers::CSharedDeleteLayersTable	m_cSharedDeleteLayersTable;	//!< 共有削除レイヤ名管理テーブル
	sharedmovelayers::CSharedMoveLayersTable		m_cSharedMoveLayersTable;	//!< 共有移動レイヤ名管理テーブル
	dispnavirelation::CDispNaviRelationTable		m_cDispNaviRelationTable;	//!< 道路リンクの道路種別と経路種別の関連管理テーブル
	dispotherrelation::CDispOtherRelationTable		m_cDispOtherRelationTable;	//!< 道路リンクの道路種別とその他の属性の関連管理テーブル
	edithistory::CEditHistoryTable					m_cEditHistoryTable;		//!< ルール編集履歴管理テーブル
	editmesh::CEditMeshTable						m_cEditMeshTable;			//!< 編集可能メッシュ管理テーブル
	lockarea::CLockAreaTable						m_cLockAreaTable;			//!< ロックエリア管理テーブル
	lockmesh::CLockMeshTable						m_cLockMeshTable;			//!< ロックメッシュ管理テーブル
	layernames::CLayerNamesTable					m_cLayerNamesTable;			//!< レイヤ名管理テーブル
	// 自動付与属性
	CAutoValue										m_cAutoValue;				//!< 自動付与属性
	// ログイン情報
	long											m_lWorkPurposeID;			//!< 作業目的ID
	CString											m_strOperatorName;			//!< 作業者名
//	bool											m_bCanSaveEdit;				//!< 現在の作業目的が編集可能かどうかのフラグ保持用
	// レジストリ・設定ファイル
	CSiNDYeSettings									m_cSiNDYeSettings;			//!< レジストリ・設定ファイル管理クラス

	bool											m_bAnnoRuleCached;			//!< 注記ルールをキャッシュしたかどうかのフラグ
	bool											m_bEditRuleCached;			//!< 編集ルールをキャッシュしたかどうかのフラグ
	bool											m_bBGClassCached;			//!< 背景種別ルールをキャッシュ下かどうかのフラグ
};
#pragma warning(pop)
} // sindy

#endif // _RULE_H_
