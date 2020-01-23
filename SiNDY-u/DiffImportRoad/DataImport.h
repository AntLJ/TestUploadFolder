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

#include "NWData.h"
#include "InputInfo.h"

using namespace std;

extern crd_cnv g_cnv;

class CImport
{
public:
	CImport(){}
	CImport( const CInputInfo& cInputInfo ){ m_cInputInfo = cInputInfo; }
	~CImport(){}

public:
	bool run();

private:

	/**
	 * @brief   テーブル初期化
	 */
	void setTableType();

	/**
	 * @brief   2次メッシュ形状取得関数
	 * @param   lMeshCode    [in]  メッシュコード
	 * @param   ipMeshGeom   [out] メッシュ形状
	 * @return  true  メッシュ形状取得成功
	 *          false メッシュ形状取得失敗
	 */
	bool getMeshGeom( long lMeshCode, IGeometryPtr& ipMeshGeom );

	/**
	 * @brief   編集前後のPGDBから更新情報をセットする
	 * @note    編集前後のPGDBを比較し、「追加、削除、形状更新、属性更新、形状＋属性更新」情報をセットする
	 * @param   cBeforeData  [in]  編集前PGDB
	 * @param   cAfterData   [in]  編集後PGDB
	 * @return  true  更新情報セット成功
	 *          false 更新情報セット失敗
	 */
	bool setUpdateData( const CNWData& cBeforeData, const CNWData& cAfterData );

	/**
	 * @brief   編集前後のデータから更新情報をセットする
	 * @note    setUpdateData()で呼ばれ、テーブル毎に処理させる
	 *          m_mapData2Update, m_mapUpdate2Dataに更新情報をセット
	 * @param   strTableName      [in]  指定テーブル名
	 * @param   beforeTableData   [in]  編集前の指定テーブルデータ
	 * @param   afterTableData    [in]  編集後の指定テーブルデータ
	 * @param   dataType          [in]  テーブルタイプ
	 * @return  true  更新情報セット成功
	 *          false 更新情報セット失敗
	 */
	bool setUpdateTableData( const CString& strTableName, const TableData& beforeTableData, const TableData& afterTableData, DATATYPE dataType );

	/**
	 * @brief   コンフリクト確認
	 * @param   cSDEFieldMap   [in]  SDEのフィールドマップ
	 * @param   cPGDBFieldMap  [in]  PGDBのフィールドマップ
	 * @param   ipSDERow       [in]  SDEのデータ
	 * @param   ipBeforeRow    [in]  編集前PGDBのデータ
	 * @param   ipAfterRow     [in]  編集前PGDBのデータ
	 * @param   exceptFieldSet [in]  インポート除外フィールドset
	 * @return  true   コンフリクトしている
	 *          false  コンフリクトしていない
	 */
	bool IsConflict( const CFieldMap& cSDEFieldMap, const CFieldMap& cPGDBFieldMap, const _IRowPtr& ipSDERow, const _IRowPtr& ipBeforeRow, const _IRowPtr& ipAfterRow, const std::set<CString>& exceptFieldSet);

	/**
	 * @brief 2つのテーブル行の属性が実質的に等価であるか
	 * @note  インポート除外フィールドやインポート対象外のフィールドを除き、属性が全部一致するかを見る
	 * @param srcFieldMap    [in] 比較元のフィールドマップ
	 * @param dstFieldMap    [in] 比較先のフィールドマップ
	 * @param ipSrcRow       [in] 比較元の行
	 * @param ipDstRow       [in] 比較先の行
	 * @param exceptFieldSet [in] インポート除外フィールドset
	 * @param sdeCheck       [in] SDEとの比較であるか(SDEとPGDBでは一部の同一名称フィールドの値が異なるため)
	 * @retval true  実質的に等価
	 * @retval false 実質的に等価ではない
	 */
	bool IsEqualAttributeBySubstance(
		const CFieldMap& srcFieldMap,
		const CFieldMap& dstFieldMap,
		const _IRowPtr& ipSrcRow,
		const _IRowPtr& ipDstRow,
		const std::set<CString>& exceptFieldSet,
		bool  sdeCheck = true );

	/**
	 * @brief   コンフリクトしているデータのステータス変更
	 * @note    この関数内で全テーブルのコンフリクト状態をチェックし
	 *          それに紐付くデータの更新ステータスを変更する
	 * @param   cBeforeData   [in]  編集前PGDBデータ
	 * @param   cAfterData    [in]  編集後PGDBデータ
	 * @param   cSDEData      [in]  インポート先のSDEデータ
	 * @return  true   コンフリクトしている
	 *          false  コンフリクトしていない
	 */
	bool setConflictData( const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData );

	/**
	 * @brief   コンフリクトしているデータのステータス変更
	 * @note    各テーブルのインポート対象データがコンフリクトしている場合、更新ステータスを変更する
	 *          またそのレコードに紐付くインポート対象レコードの更新ステータスも変更する
	 * @param   dataStatus    [in]   インポート対象データの更新タイプ（新規、削除、属性更新、形状更新、属性＋形状更新）
	 * @param   cBeforeData   [in]  編集前PGDBデータ
	 * @param   strTableName  [in]  テーブル名
	 * @param   cAfterData    [in]  編集後PGDGBデータ
	 * @param   cSDEData      [in]  インポート先のSDEデータ
	 * @return  true   判別成功
	 *          false  判別失敗
	 */
	bool setConflictTableData( DATASTATUS dataStatus, const CString& strTableName, const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData );

	/**
	 * @brief   インポート対象データが2次メッシュ境界に関連するインポート対象データの更新ステータスを変更
	 * @param   dataStatus    [in]   インポート対象データの更新タイプ（新規、削除、属性更新、形状更新、属性＋形状更新）
	 * @param   cBeforeData   [in]  編集前PGDBデータ
	 * @param   cAfterData    [in]  編集後PGDGBデータ
	 * @param   cSDEData      [in]  インポート先のSDEデータ
	 * @return  true   判別成功
	 *          false  判別失敗
	 */
	bool setNGDataForEdgeNode( DATASTATUS dataStatus, const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData );

	/**
	 * @brief   道路リンクのFromToノードが「0」のケースがあるため、それに付随するデータの更新ステータスを変更
	 * @param   dataStatus    [in]  更新予定データのステータス情報
	 * @param   cBeforeData   [in]  編集前PGDBデータ
	 * @param   cAfterData    [in]  編集後PGDGBデータ
	 * @param   cSDEData      [in]  インポート先のSDEデータ
	 * @return  true   判別成功
	 *          false  判別失敗
	 */
	bool setNGDataForIDZeroNodeLink( DATASTATUS dataStatus, const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData );

	/**
	 * @brief   コンフリクトしているデータの更新ステータス変更
	 * @note    指定テーブルのコンフリクトしているデータの更新ステータスを変更する
	 *          コンフリクトしたデータに紐付くリンクも更新ステータス変更の対象となる
	 * @param   strTableName    [in]     コンフリクトを検知したいテーブル名
	 * @param   updateType      [in]     インポート対象データの更新タイプ（新規、削除、属性更新、形状更新、属性＋形状更新）
	 * @param   cBeforeData     [in]     編集前PGDBデータ([bug 10363]でコンフリクトの判定で必要なため、編集前後を別々に設定)
	 * @param   cAfterData      [in]     編集後PGDBデータ(同上)
	 * @param   cSDEData        [in]     インポート先のSDEデータ
	 * @param   setSearchedLink [in,out] 探索済みリンク
	 * @param   setNGLink       [in,out] NGリンク
	 * @return  true   判別成功
	 *          false  判別失敗
	 */
	bool setConflictTableData2( const CString& strTableName, UPDATETYPE updateType, const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData, set<long>& setSearchedLink, set<long>& setNGLink );

	/**
	 * @brief   更新対象の行であるかチェック(bug10363対応で、リンクのみからテーブル全般に修正)
	 * @note    この関数内で更新対象リンクを指定のdataStatusに更新する
	 * @param   tableName    [in]  テーブル名称(bug10363)
	 * @param   dataStatus   [in]  更新対象リンクだった場合に設定するステータス情報
	 * @param   lPGDBLinkID  [in]  PGDBのリンクID
	 * @return  true   更新対象リンクである
	 *          false  更新対象リンクではない
	 */
	bool IsUpdateRow( LPCTSTR tableName, DATASTATUS dataStatus, long lPGDBLinkID );

	/**
	 * @brief   更新データのステータスを設定する
	 * @note    この関数は道路ノード専用
	 * @param   dataStatus    [in]  道路ノードに設定するステータス情報
	 * @param   setNGLink     [in]  NGリンク
	 * @param   cPGDBData     [in]  編集前or編集後PGDBデータ
	 * @return  true   ステータス設定成功
	 *          false  ステータス設定失敗
	 */
	bool setUpdateNGNode( DATASTATUS dataStatus, const set<long>& setNGLink, const CNWData& cPGDBData );

	/**
	 * @brief   更新データのステータスを設定する
	 * @note    この関数はリンク内属性、傾斜、一方通行、通行規制、簡易傾斜、一通/通禁車種別規制専用
	 * @param   dataStatus    [in]  設定するステータス情報
	 * @param   strTableName  [in]  設定したいデータのテーブル名
	 * @param   setNGLink     [in]  NGリンク
	 * @param   cPGDBData     [in]  編集前or編集後PGDBデータ
	 * @return  true   ステータス設定成功
	 *          false  ステータス設定失敗
	 */
	bool setUpdateNGAGOPS( DATASTATUS dataStatus, const CString& strTableName, const set<long>& setNGLink, const CNWData& cPGDBData );

	/**
	 * @brief   車種タイプのNGなものを設定する
	 * @param   dataStatus    [in]  設定するステータス情報
	 * @param   cPGDBData     [in]  編集前or編集後PGDBデータ
	 * @return  true   ステータス設定成功
	 *          false  ステータス設定失敗
	 */
	bool setUpdateNGVehicleType( DATASTATUS dataStatus, const CNWData& cPGDBData );

	/**
	 * @brief   NGリンクに対応する更新データのステータスを変更する
	 * @param   dataStatus  [in]  設定するステータス情報
	 * @param   setNGLink   [in]  NGリンク
	 * @param   cPGDBData   [in]  編集前or編集後PGDB
	 * @return  true   ステータス設定成功
	 *          false  ステータス設定失敗
	 */
	bool setUpdateNGData( DATASTATUS dataStatus, const set<long>& setNGLink, const CNWData& cPGDBData );

	/**
	 * @brief   更新データのステータスを設定する
	 * @note    このリンク列（INF）専用→LQは今関数内で対応
	 * @param   dataStatus    [in]  設定するステータス情報
	 * @param   strTableName  [in]  設定したいデータのテーブル名
	 * @param   setNGLink     [in]  NGリンク
	 * @param   cPGDBData     [in]  編集前or編集後PGDBデータ
	 * @return  true   ステータス設定成功
	 *          false  ステータス設定失敗
	 */
	bool setUpdateNGInf( DATASTATUS dataStatus, const CString& strTableName, const set<long>& setNGLink, const CNWData& cPGDBData );

	/**
	 * @brief   更新対象のリンクを辿り、ステータスを変更する関数
	 * @note    更新不可にするのはこの関数の実装次第
	 * @param   dataStatus             [in]      設定するステータス情報
	 * @param   lSearchNodeID          [in]      設定したいデータのテーブル名
	 * @param   mapPGDBLink2FromToNode [in]      PGDBのリンクに紐付くFromToノードIDのマップ
	 * @param   mapPGDBNode2Link       [in]      PGDBのノードに紐付くリンクのマップ
	 * @param   setSearchedLink        [in,out]  探索済みリンク
	 * @param   setNGLink              [in,out]  NGリンク
	 * @return  true   ステータス設定成功
	 *          false  ステータス設定失敗
	 */
	bool searchNoUpdateLink( DATASTATUS dataStatus, long lSearchNodeID, const map<long,pair<long,long>>& mapPGDBLink2FromToNode, const map<long,set<long>>& mapPGDBNode2Link, set<long>& setSearchedLink, set<long>& setNGLink );


	/**
	 * @brief   SDEに更新情報を反映
	 * @param   cBeforeData  [in]  編集前PGDBデータ
	 * @param   cAfterData   [in]  編集後PGDBデータ
	 * @param   cSDEData     [in]  SDEデータ
	 * @return  true   反映成功
	 *          false  反映失敗
	 */
	bool reflectData( const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData );

	/**
	 * @brief   SDEの指定テーブルに更新情報を反映
	 * @param   strTabelName [in]  テーブル名
	 * @param   cBeforeData  [in]  編集前PGDBデータ
	 * @param   cAfterData   [in]  編集後PGDBデータ
	 * @param   cSDEData     [in]  SDEデータ
	 * @return  true   反映成功
	 *          false  反映失敗
	 */
	bool reflectTableData( const CString& strTabelName, const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData );

	/**
	 * @brief   更新情報をSDEに反映（新規作成）
	 * @param   strTableName    [in]  テーブル名
	 * @param   lPGDBOID        [in]  PGDBのOID
	 * @param   cSDEData        [in]  SDEデータ
	 * @param   cAfterData      [in]  編集後PGDBデータ
	 * @param   sdeTableData    [in]  SDEのテーブルデータ
	 * @param   afterTableData  [in]  編集後のPGDBデータ
	 * @return  true   反映成功
	 *          false  反映失敗
	 */
	bool reflectNewData( const CString& strTableName, long lPGDBOID, const CNWData& cSDEData, const CNWData& cAfterData, const TableData& sdeTableData, const TableData& afterTableData );

	/**
	 * @brief   更新情報をSDEに反映（削除）
	 * @param   strTableName    [in]  テーブル名
	 * @param   lPGDBOID        [in]  PGDBのOID
	 * @param   cSDEData        [in]  SDEデータ
	 * @param   cBeforeData     [in]  編集前PGDBデータ
	 * @param   sdeTableData    [in]  SDEのテーブルデータ
	 * @param   beforeTableData [in]  編集前のPGDBデータ
	 * @return  true   反映成功
	 *          false  反映失敗
	 */
	bool reflectDeleteData( const CString& strTableName, long lPGDBOID, const CNWData& cSDEData, const CNWData& cBeforeData, const TableData& sdeTableData, const TableData& beforeTableData );

	/**
	 * @brief   更新情報をSDEに反映（属性、形状更新）
	 * @param   strTableName    [in]  テーブル名
	 * @param   lPGDBOID        [in]  PGDBのOID
	 * @param   updateType      [in]  データ更新情報
	 * @param   cSDEData        [in]  SDEデータ
	 * @param   cAfterData      [in]  編集後PGDBデータ
	 * @param   sdeTableData    [in]  SDEのテーブルデータ
	 * @param   afterTableData  [in]  編集後のPGDBデータ
	 * @param   beforeTableData [in]  編集前のPGDBデータ([bug 10363]でインポートされなかった編集分の除外フィールドを出力するため使う)
	 * @return  true   反映成功
	 *          false  反映失敗
	 */
	bool reflectUpdateData( const CString& strTableName, long lPGDBOID, UPDATETYPE updateType, const CNWData& cSDEData, const CNWData& cAfterData, const TableData& sdeTableData, const TableData& beforeTableData, const TableData& afterTableData );

	/**
	 * 
	 */
	CString getExceptFieldString(
		const TableData& beforeTableData,
		const TableData& afterTableData,
		const _IRowPtr& ipBeforeRow,
		const _IRowPtr& ipAfterRow,
		const std::set<long>& exceptFieldIndexSet
		);

	std::set<CString> getExceptFieldSet(LPCTSTR fieldName);

	/**
	 * @brief   FromToノードID付け替え
	 * @param   strTableName    [in]  テーブル名
	 * @param   cAfterData      [in]  編集後PGDBデータ
	 * @param   ipSDERow        [in]  SDEのレコードデータ
	 * @param   lPGDBNodeID     [in]  PGDBのFrom,ToノードID
	 * @param   lNodeIndex      [in]  From,Toノードのフィールドインデックス
	 * @return  true   反映成功
	 *          false  反映失敗
	 */
	bool replaceFromToNodeID( const CString& strTableName, const CNWData& cAfterData, _IRowPtr& ipSDERow, long lPGDBNodeID, long lNodeIndex );


	/** 
	 * @brief   作業用子バージョンの作成又は使いまわし後、ワークスペースに接続
	 * @param   ipWorkspace        [in]  作業元ワークスペース
	 * @param   ipSDEWork          [out] 作業用ワークスペース
	 * @return  true   子バージョン取得＋ワークスペース接続成功
	 *          false  子バージョン取得＋ワークスペース接続失敗
	 */ 
	bool createWorkVersion( IWorkspacePtr& ipWorkspace, IWorkspacePtr& ipSDEWork );

	/**
	 * @brief   SDEのレコードにPGDBのデータを反映する（形状＋属性）
	 * @param   ipSDERow    [in]  SDEのレコード
	 * @param   ipPGDBRow   [in]  PGDBのレコード
	 * @param   cSDEField   [in]  SDEのフィールド情報
	 * @param   cPGDBField  [in]  PGDBのフィールド情報
	 * @param   exceptFieldSet [in] インポート除外フィールドのset(bug 10363)
	 * @return  true   データ設定成功
	 *          false  データ設定失敗
	 */
	bool setRowData( _IRowPtr& ipSDERow, const _IRowPtr& ipPGDBRow, const CFieldMap& cSDEField, const CFieldMap& cPGDBField, const std::set<CString>& exceptFieldSet = std::set<CString>() );

	/**
	 * @brief   INFに紐付くLQを再構築
	 * @note    編集後PGDBのLQ情報を元にSDE側に再構築する
	 * @param   lSDEInfID       [in]  SDEのINFのOID
	 * @param   ipSDELqTable    [in]  SDEのLQテーブル
	 * @param   cSDELqField     [in]  SDEのLQテーブルのフィールド情報
	 * @param   cAfterData      [in]  編集後PGDBのテーブルデータ
	 * @param   setAfterLqData  [in]  編集後PGDBのLQデータ
	 * @return  true   LQ構築成功
	 *          false  LQ構築失敗
	 */
	bool createLqData( long lSDEInfID, ITablePtr& ipSDELqTable, const CFieldMap& cSDELqField, const CNWData& cAfterData, const set<LqData>& setAfterLqData );

	/**
	 * @brief   LQ削除
	 * @param   lSDEInfID     [in]  SDEの削除するINFのOID
	 * @param   ipSDELqTable  [in]  SDEのLQテーブル
	 * @return  true   LQ削除成功
	 *          false  LQ削除失敗
	 */
	bool deleteLqData( long lSDEInfID, ITablePtr& ipSDELqTable );

	/**
	 * @brief   対応オブジェクトID取得用関数
	 * @note    bug10363に伴い、リンクのみの対応から汎化
	 * @param   tableName [in]  対象のテーブル名
	 * @param   cNWData   [in]  編集前後のPGDBデータ
	 * @param   lPgdbID   [in]  PGDBのオブジェクトID
	 * @param   lOrgID    [out] SDEのオブジェクトID
	 * @return  true   対応オブジェクトID取得成功
	 *          false  対応オブジェクトID取得失敗
	 */
	bool getOrgIDFromPgdbID( LPCTSTR tableName, const CNWData& cNWData, long lPgdbID, long& lOrgID );

	/**
	 * @brief   ログハンドルツール確認用のログ取得関数
	 * @param   dbType        [in]  編集前PGDB、編集後PGDB、SDEか
	 * @param   strTableName  [in]  テーブル名
	 * @param   lOID          [in]  オブジェクトID
	 * @param   bOK           [in]  SDEに反映したかどうか
	 * @param   strStatus     [in]  データの状況
	 * @return  出力するログ
	 */
	CString getImportLog( DBTYPE dbType, const CString& strTableName, long lOID, bool bOK, const CString& strStatus );

	/**
	 * @brief   エラーログ出力関数
	 * @param   errType        [in]  エラータイプ
	 * @param   strFuncName    [in]  関数名
	 * @param   strComment     [in]  エラーの状況
	 * @param   strTableName   [in]  テーブル名
	 * @param   lOID           [in]  オブジェクトID
	 * @param   bOutTime       [in]  現在の時間を出力するかどうか
	 */
	void writeErrorLog( ERRTYPE errType, const CString& strFuncName, const CString& strComment, const CString& strTableName, long lOID, bool bOutTime = false );

	/**
	 * @brief   編集開始
	 * @param   ipWorkspace  [in]  ipWorkspace
	 * @return  true    編集開始成功
	 *          false   編集開始失敗
	 */
	bool StartEdit( IWorkspacePtr& ipWorkspace );

	/**
	 * @brief   編集終了開始
	 * @param   bSave        [in]  保存するかどうか
	 * @param   ipWorkspace  [in]  ipWorkspace
	 * @return  true    編集開始成功
	 *          false   編集開始失敗
	 */
	bool StopEdit( bool bSave, IWorkspacePtr& ipWorkspace );

	/**
	 * @brief   リコンサイル処理
	 * @param   ipWorkspaceEdit  [in]  ワークスペース
	 * @return  true    処理成功
	 *          false   処理失敗
	 */
	bool DoReconcile( IWorkspaceEditPtr& ipWorkspaceEdit );

	/**
	 * @brief   途中中断処理
	 * @return  true    処理終了
	 *          false   処理続行
	 */
	bool IsSuspension();

	/**
	 * @brief インポート除外フィールドリストを読み込む(bug 10363)
	 * @retval 読み込みの成否
	 */
	bool readExceptFieldList();

	/**
	 * @brief インポート除外フィールドとして妥当であるかを確認する(bug 10363)
	 * @note  元々インポート対象外のフィールドや、他テーブルのIDを示すようなものは除外フィールドとして不適と判断する
	 * @note  ArcHelperExのIsEditableFieldと違い、UPDATETYPE_Cは妥当な名称として取り扱う
	 * @note  属性変更のツールによってはUPDATETYPE_Cを更新することがあるかもしれないから
	 * @param fieldName [in] フィールド名称
	 * @retval true  インポート除外フィールドとして妥当である
	 * @retval false インポート除外フィールドとして妥当ではない
	 */
	bool checkExceptField(const CString& fieldName);

private:
	CString m_strProgModifyDate;           //!< ツール更新日
	CString m_strWorkVersionName;          //!< 編集用子バージョン名（実行時に名前を設定する）
	CInputInfo m_cInputInfo;               //!< コマンドライン引数情報
	CString m_strToolName;                 //!< ツール名
	ISpatialReferencePtr m_ipSpRef;        //!< 空間参照（SDE）

	long m_lMeshCode;      //!< 処理中のメッシュコード
	ofstream m_ofs;        //!< ログ用ストリーム
	ofstream m_ofsErr;     //!< エラーログ用ストリーム

	map<CString,DATATYPE> m_mapTableMap;   //!< テーブル名リスト
	vector<pair<CString,DATASTATUS>> m_vecConflictTable;    //!< コンフリクト処理するテーブルの順番
	vector<CString> m_vecReflectTable;     //!< SDEにデータ反映する処理のテーブルの順番

	map<CString,map<long,UpdateData>> m_mapData2Update;         // データと[新規、削除、形状更新、属性更新、形状＋属性更新]のマップ
	map<CString,map<UPDATETYPE,set<long>>> m_mapUpdate2Data;    //!< [新規、削除、形状更新、属性更新、形状＋属性更新]とデータマップ

	map<CString,map<long,long>> m_mapPGDBOldID2SDENewID;  //!< 新規作成したデータのSDE上のOIDと編集後データのOIDとのマップ

	map<CString,set<CString>> m_exceptFieldMap; //!< インポート除外フィールドmap(bug 10363)

	vector<CString> m_vecImportLog;  //!< インポートログ
};
