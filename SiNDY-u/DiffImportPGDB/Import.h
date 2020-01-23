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
#include "sdebase.h"

/**
 * @brief 編集差分反映する（所謂「上海データインポート」のメイン）クラス
 */
class CImport :
	public CSDEBase
{
public:
	/**
	 * @brief コンストラクタ。
	 */
	CImport(void);

	/**
	 * @brief デストラクタ。
	 */
	~CImport(void);

	/**
	 * @brief 初期化関数。
	 *
	 * @param args [in] 実行時オプション
	 */
	bool init(const CArguments& args, const IWorkspacePtr& ipWorkspace,
		const ISpatialReferencePtr& ipSpRef, const FCArray& ipFeatureClasses, bool isSindyCImport );

private:
	CString m_OldPath;         //!< 編集前PGDBのベースパス
	CString m_NewPath;         //!< 編集後PGDBのベースパス
	ID_SET m_AppendID;         //!< メッシュ境界の属性変更により、新規追加扱いになったものを管理するIDセット
	ID_SET m_TargetID;         //!< 処理済みのオブジェクトの元のIDを管理するセット
	long m_OrgIdIndex;         //!< オリジナルオブジェクトIDのフィールドインデックス
	long m_ChgStatusIndex;     //!< 更新ステータスのフィールドインデックス
	long m_OrgModifyDateIndex; //!< オリジナルの最終更新日付のフィールドインデックス
	COMMAND m_Command;         //!< 反映用コマンド（追加のみ・更新・削除のみ）
	std::map<COMMAND,CString> m_CommandString; //!< メッシュリストのコマンドに対応する日本語表記
	bool m_tdcMode;            //!< TDC優先モード
	bool m_geospaceMode;       //!< Geospaceモード
	list<CString> m_layerList; //!< Geospaceモード対象レイヤリスト
	COleDateTime m_availDate;  //!< 有効日付(poiインポート時使用)
	bool m_emptyDBMode;        //!< 空DBインポートモード

	std::map<CString,vector<long>> m_updateOid; //!< 処理済み(削除以外)ORG_OBJID格納
	std::map<CString,vector<long>> m_deleteOid; //!< 処理済み(削除)ORG_OBJID格納
	std::map<CString,vector<long>> m_crossMeshOldOid; //!< OLD側でメッシュまたいでいるORG_OBJID格納
	std::map<CString,vector<long>> m_crossMeshNewOid; //!< NEW側でメッシュまたいでいるORG_OBJID格納
	std::map<CString,vector<long>> m_onlyNewOid; //!< NEW側にしかいないORG_OBJID格納

	/**
	 * @brief メッシュ単位での差分インポート（反映）関数。
	 *
	 * @param cCommand		[in]	反映用コマンド（追加のみ・更新・削除のみ・削除＋追加）
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnExecMesh(COMMAND cCommand);

	/**
	 * @brief PGDBオープン関数。
	 *
	 * @param cBasePath		[in]	PGDBベースパス
	 *
	 * @retval NULL以外	PGDBのワークスペース
	 * @retval NULL		失敗、エラー
	 */
	IWorkspacePtr fnOpenLocalDatabase(LPCTSTR cBasePath);
	
	/**
	 * @brief PGDBオープン関数。
	 * @note  "～\hoge.mdb(gdb)"まで指定されている場合に、fnOpenLocalDatabase内部で呼ばれます
	 *
	 * @param cFilePath		[in]	PGDBパス
	 *
	 * @retval NULL以外	PGDBのワークスペース
	 * @retval NULL		失敗、エラー
	 */
	IWorkspacePtr fnOpenWorkspaceFromFile(LPCTSTR cFilePath);

	/**
	 * @brief PGDB内の指定インデックスのフィーチャクラスオープン関数。
	 *
	 * @param ipWorkspace	[in]	PGDBのワークスペース
	 * @param cIndex		[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 *
	 * @retval NULL以外	フィーチャクラス
	 * @retval NULL		失敗、エラー
	 */
	IFeatureClassPtr fnOpenLocalFeatureClass(IWorkspacePtr ipWorkspace, long cIndex);

	/**
	 * @brief メッシュ境界を考慮しての比較と変更・削除関数。
	 *
	 * @param ipOldClass	[in]	編集前のフィーチャクラス
	 * @param ipNewClass	[in]	編集後のフィーチャクラス
	 * @param cIndex		[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnCompare(IFeatureClassPtr ipOldClass, IFeatureClassPtr ipNewClass, long cIndex);

	/**
	 * @brief 	メッシュ境界を考慮しなくていいものの比較と変更・削除関数。
	 *	　　　（注記とポイントが該当するが、注記がなくなったので、実質ポイントのみ）
	 *
	 * @param ipOldClass	[in]	編集前のフィーチャクラス
	 * @param ipNewClass	[in]	編集後のフィーチャクラス
	 * @param cIndex		[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnComparePoint(IFeatureClassPtr ipOldClass, IFeatureClassPtr ipNewClass, long cIndex);

	/**
	 * @brief ipSrcFeatureを元にフィーチャを新規追加します。
	 * @note  関数内でm_ipFeatureClasses[cIndex]からInsertCursor作ります。
	          インポート先が空である(オブジェクトが重なる等のケアは考えない)こと前提です。
	 * @param ipSrcFeature [in] 編集後PGDBのフィーチャ
	 * @param cIndex       [in] テーブルの何番目のフィーチャクラスかを示すインデックス
	 * @param updateStatus [in,opt] 新規追加フィーチャにセットするUPDATE_Cの値(指定しなければfnSetCommonAttr()での処理結果そのまま)
	 * @retval true  成功
	 * @retval false 失敗、エラー
	 */
	bool fnInsertIntoEmptyDB( const IFeaturePtr& ipNewFeature, long cIndex, long updateStatus = -1 );


	/**
	 * @brief POI_POINTの比較と変更関数。
	 * @param ipOldClass [in] 編集前のフィーチャクラス
	 * @param ipNewClass [in] 編集後のフィーチャクラス
	 * @param cIndex     [in] テーブルの何番目のフィーチャクラスかを示すインデックス
	 * @retval true  成功
	 * @retval false 失敗、エラー
	 */
	bool fnComparePoi( const IFeatureClassPtr& ipOldClass, const IFeatureClassPtr& ipNewClass, long cIndex );

	/**
	 * @brief POI_POINTの属性変更と更新を行う
	 * @param sindyFeature [in] SDE上のフィーチャ
	 * @param pgdbFeature  [in] 編集後（PGDB）のフィーチャ
	 * @param cIndex       [in] テーブルの何番目のフィーチャクラスかを示すインデックス
	 * @param oid          [in] SDE上のオブジェクトID
	 * @param message      [in] 更新しようとしている内容（ログ出力用）
	 * @retval true  成功
	 * @retval false 失敗、エラー
	 */
	bool fnSiNDYPoiChange( IFeaturePtr& sindyFeature, const IFeaturePtr& pgdbFeature, long cIndex, long oid, CString& message );

	/**
	 * @brief Storeする
	 *
	 * @param sindyFeature [in] Storeするフィーチャ
	 * @param cIndex       [in] テーブルの何番目のフィーチャクラスかを示すインデックス
	 * @param message      [in,out] ログ用メッセージ
	 *
	 * @retval true 成功
	 * @retval false 失敗、エラー
	 */
	bool fnStoreFeature( const IFeaturePtr& sindyFeature, long cIndex, long oid, CString &message);


	/**
	 * @brief 	メッシュポリゴン内削除、メッシュ境界は切り取って更新
	 *	　　　（削除のみ又は削除＋追加の時に使用。最近ではほとんど使われないはず）
	 *
	 * @param cIndex		[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnClipData(long cIndex);

	/**
	 * @brief 	新規追加分を一括追加関数。
	 *
	 * @param ipFeatureClass	[in]	編集後のフィーチャクラス
	 * @param cIndex			[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnAddNew(IFeatureClassPtr ipFeatureClass, long cIndex);

	/**
	 * @brief フィーチャを追加する
	 *
	 * @param ipInsCursor [in,out] フィーチャ追加用フィーチャカーソル
	 * @param ipBuffer    [in]     追加するフィーチャ
	 * @param aID         [out]    追加したフィーチャのオブジェクトID
	 * @param cIndex      [in]     テーブルの何番目のフィーチャクラスかを示すインデックス
	 * @param message     [in,out]     ログ用メッセージ
	 *
	 * @retval true 成功
	 * @retval false 失敗、エラー
	 */
	bool fnInsertFeature(IFeatureCursorPtr& ipInsCursor, const IFeatureBufferPtr& ipBuffer, CComVariant &aID, long cIndex, CString& message);


	/**
	 * @brief 	Export後にSiNDY上に変更か加えられたか、更新日付2種の比較で判断する関数。
	 *          関数名と戻り値が逆っぽい気もするが、更新できない・更新してはいけない時にfalseを返す
	 *
	 * @param cIndex		[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 * @param cObjID		[in]	SDE上のオブジェクトID
	 * @param ipOrgFeature	[in]	SDE上のフィーチャ（NULLの時は内部で検索する）
	 * @param ipPgdbFeature	[in]	編集後（PGDB）のフィーチャ
	 * @param cChgType		[in]	更新しようとしている内容（ログ出力用）
	 *
	 * @retval true		変更なし又は変更ありでも更新は可
	 * @retval false	削除されている又は国内優先モードで変更されている
	 */
	bool fnSiNDYChange(long cIndex, long cObjID, IFeaturePtr ipOrgFeature, IFeaturePtr ipPgdbFeature, LPCTSTR cChgType);

	/**
	 * @brief 	メッシュをまたいだオブジェクトかを判定する関数。
	 *
	 * @param cIndex	[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 * @param cObjID	[in]	SDE上のオブジェクトID
	 * @param ipFeature	[in]	SDE上のフィーチャ（NULLの時は内部で検索する）
	 *
	 * @retval true		変更なし又は変更ありでも更新は可
	 * @retval false	削除されている又は国内優先モードで変更されている
	 */
	bool fnOverMesh(long cIndex, long cObjID, IFeaturePtr ipFeature = NULL);

	/**
	 * @brief 	メッシュをまたいだオブジェクトかを判定する関数。
	 *
	 * @param ipNewClass	[in]	編集後のフィーチャクラス
	 * @param cIndex		[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 * @param cObjID		[in]	SDE上のオブジェクトID
	 * @param ipNewFeature	[in]	編集後のフィーチャ（削除対象の時はNULL）
	 * @param cStatus		[in]	更新ステータス
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnClipAndApend(IFeatureClassPtr ipNewClass, long cIndex, long cObjID, IFeaturePtr ipNewFeature, long cStatus);

	/**
	 * @brief 	メッシュをまたいでいないものの単純更新関数。
	 *
	 * @param ipNewFeature	[in]	編集後のフィーチャ
	 * @param cIndex		[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 * @param cObjID		[in]	SDE上のオブジェクトID
	 * @param cStatus		[in]	更新ステータス
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnReflectToSiNDY(IFeaturePtr ipNewFeature, long cIndex, long cObjID, long cStatus);


	/**
	 * @brief フィーチャからフィーチャへ形状をコピー
	 *
	 * @param ipTgtFeature [out] コピーされるフィーチャ(IFeatureまたはIFeatureBuffer)
	 * @param ipSrcFeature [in]  コピーするフィーチャ
	 * @param cIndex       [in]  テーブルの何番目のフィーチャクラスかを示すインデックス
	 * @param cObjID       [in]  オブジェクトID(ログ出力用)
	 *
	 * @retval true 成功
	 * @retval false 失敗
	 */
	template <typename T>
	bool fnReflectShape( T& ipTgtFeature, const IFeaturePtr& ipSrcFeature, long cIndex, long cObjID );

	/**
	 * @brief 	メッシュポリゴンでクリップして更新または削除する関数。
	 *
	 * @param ipCursor	[in]	更新対象を検索したカーソル
	 * @param cIndex	[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnCutObject(IFeatureCursorPtr ipCursor, long cIndex);

	/**
	 * @brief 	検索されたオブジェクトを無条件で削除する関数（主にポイント用）。
	 *          DeleteSearchedRowsでも事足りるが、ログを出すため（だけ？）にあるようなもの。しかも最近はほとんど使われないはず
	 *
	 * @param ipCursor	[in]	削除対象を検索したカーソル
	 * @param cIndex	[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnDeletePoint(IFeatureCursorPtr ipCursor, long cIndex);

	/**
	 * @brief 	反映対象属性のコピー関数。
	 *
	 * @param ipTgtFeature	[in]	コピー先フィーチャ
	 * @param ipSrcFeature	[in]	コピー元フィーチャ
	 * @param cIndex		[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 * @param cField		[out]	更新したフィールド名（ログ出力用）
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnReflectAttr(IFeaturePtr ipTgtFeature, IFeaturePtr ipSrcFeature, long cIndex, CString& cField);

	/**
	 * @brief 	メッシュポリゴンでクリップした形状を求める関数。
	 *          fnGetRemakeShapeと似ているように見えるが、こちらは、メッシュポリゴンで切り取った残りのジオメトリを求める
	 *
	 * @param ipFeature		[in]	対象フィーチャ
	 * @param cIndex		[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 * @param cObjID		[in]	SDE上のオブジェクトID
	 * @param cForceMode	[in]	true：単純にクリップした形状を求める false:メッシュ境界が関係するかを簡易的に求める（？）名前と内容の関係がわからなくなった
	 *
	 * @retval NULL以外	演算後のジオメトリ
	 * @retval NULL		失敗、エラー
	 */
	IGeometryPtr fnGetClipShape(IFeaturePtr ipFeature, long cIndex, long cObjID, bool cForceMode = false);

	/**
	 * @brief 	メッシュ境界のオブジェクトに対して、当該メッシュ外部分と当該メッシュ内の切り貼りを行う関数。
	 * 　　　　 マルチパートになることもあり、オリジナルIDが同じものを1つにまとめようとするが、属性が変更になっているものがあれば新規追加扱いとする
	 *
	 * @param ipNewClass	[in]	編集後のフィーチャクラス
	 * @param ipFeature		[in]	編集後のフィーチャ
	 * @param cIndex		[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 * @param cObjID		[in]	SDE上のオブジェクトID
	 *
	 * @retval NULL以外	演算後のジオメトリ
	 * @retval NULL		失敗、エラー
	 */
	IGeometryPtr fnGetRemakeShape(IFeatureClassPtr ipNewClass, IFeaturePtr ipFeature, long cIndex, long cObjID);
	
	/**
	 * @brief メッシュ内の家形、階数ポリゴンを削除する。メッシュをまたぐ場合は、はみ出す分の形状を残す。
	 *        GEOSPACEインポート(-Gオプション指定時)の処理。
	 *
	 * @param cIndex [in] テーブルの何番目のフィーチャクラスかを示すインデックス
	 *
	 * @retval true  削除、形状変更成功
	 * @retval false 削除、形状変更失敗、エラー
	 */
	bool fnSiNDYDelete( const int cIndex );
	
	/**
	 * @brief fnSiNDYDeleteの中で本当に削除を行う関数
	 *
	 * @param cIndex    [in] テーブルの何番目のフィーチャクラスかを示すインデックス
	 * @param ipFeature [in] 削除するフィーチャ
	 * @param oid       [in] 削除するフィーチャのOBJECTID (ログ出力用)
	 * @param message   [in] ログ出力用メッセージ(家形の場合GEOSPACEIDを出力する必要がある)
	 * @retval true  削除成功
	 * @retval false 削除失敗、エラー
	 */
	bool fnSiNDYDeleteCore( const int cIndex, const IFeaturePtr& ipFeature, const long oid, const CString& message );
	
	/**
	 * @brief fnSiNDYDeleteの中でメッシュ形状で切り取る処理(形状変更)を行う関数
	 *
	 * @param cIndex     [in] テーブルの何番目のフィーチャクラスかを示すインデックス
	 * @param ipFeature  [in] 切り取るフィーチャ
	 * @param oid        [in] 切り取るフィーチャのOBJECTID (ログ出力用)
	 * @param ipDiffGeom [in] 切り取り後の形状
	 *
	 * @retval true  形状変更成功
	 * @retval false 形状変更失敗、エラー
	 */
	bool fnSiNDYPutDifferenceGeom( const int cIndex, const IFeaturePtr& ipFeature, const long oid, const IGeometryPtr& ipDiffGeom );

};
