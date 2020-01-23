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
 * @brief 反映可・不可となったオブジェクトのID管理用構造体
 */
struct CHG_ID{
	ID_SET m_OK;	///< 反映可能だったもののPGDBのオブジェクトIDのセット
	ID_SET m_NG;	///< 反映不可だったもののPGDBのオブジェクトIDのセット
};

typedef std::map<long, long> ID_MAP;	///< SDE上のOIDとPGDBのOIDの対応用マップ
typedef std::map<CString, ID_MAP> ID_MAP_MAP;	///< レイヤ名と対応するIDの対応用マップのマップ
typedef std::map<CString, CHG_ID> ID_SET_MAP;	///< レイヤ名とIDセットのマップ
typedef std::map<long, long> LQ_MAP;	///< LQの並びとリンクID管理用マップ（並び順にソートしたいだけ）

/**
 * @brief 編集差分反映する（所謂「上海データインポート」のメイン）クラス
 *        TODO: 下記ロジックが致命的のため、不正オブジェクトができることを後で確認して修正や元に戻している
 *        TODO: リンク変更・削除→リンク追加→ノード変更・削除→ノード追加→・・・
 *        TODO: ⇒ ex:リンク分断時、リンクの変更・削除ができても、追加ができず、さらにノードが作れないため、縮小して端点ノードなしリンクができる
 *        TODO:    ex:リンク1本削除＆1本付け替え時、削除OK、付け替えNGのため、浮きリンクができる
 *        TODO: 【変更案】変更・削除・追加を分けない、レイヤごとの処理に分けず、関連オブジェクトは一セットで処理できるよう修正する
 *        TODO: （後対応でやるにも限界のため、次の修正の際には、劇的Before/Afterが必要）
 *        TODO: 【制作要望】Bug9997の判定の正常化、後対応に伴い、速度が遅くなり、運用上厳しいと言われているため、速度改善も合わせて必要
 *        TODO: （まっとうな処理にある程度戻したため、遅くなるのはしょうがないとは伝えてます）
 *        TODO: 【再作成の際のソース流用】色々レビュー指摘を放置しているので、流用する場合は、規約や指摘に沿って修正すること
 *        TODO: （r2331参照のこと）
 */
class CImport :
	public CSDEBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CImport(CLayerManage* cLayerManage);

	/**
	 * @brief デストラクタ
	 */
	virtual ~CImport(void);

	/**
	 * @brief 初期化関数
	 *
	 * @param cOldPath		[in]	編集前PGDBのベースパス
	 * @param cNewPath		[in]	編集後PGDBのベースパス
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool init(LPCTSTR cOldPath, LPCTSTR cNewPath);

private:
	CString m_OldPath;			///< 編集前PGDBのベースパス
	CString m_NewPath;			///< 編集後PGDBのベースパス
	ID_SET m_AppendID;			///< メッシュ境界の属性変更により、新規追加扱いになったものを管理するIDセット（ポリゴン用）
	ID_SET_MAP m_IDSets;		///< 処理済みのオブジェクトの元のIDをレイヤ単位で管理するセット
	ID_MAP_MAP m_IDMaps;		///< SDEとPGDBのIDの対応マップをレイヤごとに管理するマップ（メッシュ毎にクリアすること）
	TABLE_INFO m_OldTableInfo;	///< 編集前PGDB用レイヤ名とユーザ名付きテーブル名（ログ出力用）・テーブルのマップ

	/**
	 * @brief メッシュ単位での差分インポート（反映）関数
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnExecMesh(void);

	/**
	 * @brief PGDBオープン関数
	 *
	 * @param cBasePath		[in]	PGDBベースパス
	 *
	 * @retval NULL以外	PGDBのワークスペース
	 * @retval NULL		失敗、エラー
	 */
	IWorkspacePtr fnOpenLocalDatabase(LPCTSTR cBasePath) const;

	/**
	 * @brief PGDB内の指定レイヤ名のテーブルオープン関数
	 *
	 * @param ipWorkspace	[in]	PGDBのワークスペース
	 * @param cLayerName	[in]	レイヤ名
	 *
	 * @retval NULL以外	テーブル
	 * @retval NULL		失敗、エラー
	 */
	ITablePtr fnOpenLocalTable(const IWorkspacePtr& ipWorkspace, LPCTSTR cLayerName) const;

	/**
	 * @brief 	比較と変更・削除関数
	 *          メッシュ境界を意識するものと処理を分けていたが、特殊なのは背景（ポリゴン）の時だけなので、中で分けた
	 *
	 * @param ipOldCTable	[in]	編集前のフィーチャクラス
	 * @param ipNewTable	[in]	編集後のフィーチャクラス
	 * @param cEnv			[in]	各種情報を保持した構造体
	 * @param cGetIDOnly	[in]	OIDの対応だけを取得するかのフラグ
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnCompare(const ITablePtr& ipOldCTable, const ITablePtr& ipNewTable, EnvTable& cEnv, bool cGetIDOnly);

	/**
	 * @brief 	新規追加分を一括追加関数
	 *
	 * @param ipTable	[in]	編集後のテーブル
	 * @param cEnv		[in]	各種情報を保持した構造体
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnAddNew(const ITablePtr& ipTable, EnvTable& cEnv);

	/**
	 * @brief 	Export後にSiNDY上に変更か加えられたか、更新日付2種の比較で判断する関数
	 *          関数名と戻り値が逆っぽい気もするが、更新できない・更新してはいけない時にfalseを返す
	 *
	 * @param cEnv          [in]  各種情報を保持した構造体
	 * @param cObjID        [in]  SDE上のオブジェクトID
	 * @param ipOrgRow      [in]  SDE上のフィーチャ（NULLの時は内部で検索する）
	 * @param ipPgdbRow     [in]  編集後（PGDB）のフィーチャ
	 * @param cChgType      [in]  更新しようとしている内容（ログ出力用）
	 * @param ipTable[2]    [in]  更新対象のテーブル(リンク列は[0]:INF、[1]:LQ。それ以外は[0]だけ使用
	 * @param cTblType		[in]  INFかLQか
	 * @param cPgdbIndexMap	[in]  PGDBのフィールドインデックスマップ
	 * @param cSdeIndexMap	[in]  SDEのフィールドインデックスマップ
	 * @param cTableName	[in]  識別用のテーブル名称
	 *
	 * @retval true		変更なし又は変更ありでも更新は可
	 * @retval false	削除されている又は国内優先モードで変更されている
	 */
	bool fnSiNDYChange(EnvTable& cEnv, long cObjID, _IRowPtr& ipOrgRow, const _IRowPtr& ipPgdbRow, LPCTSTR cChgType, const ITablePtr ipTable[2] );
	bool fnSiNDYChange(LqEnvTable& cEnv, long cTblType, long cObjID, _IRowPtr& ipOrgRow, const _IRowPtr& ipPgdbRow, LPCTSTR cChgType, const ITablePtr ipTable[2] );
	bool fnSiNDYChange(INDEX_MAP& cPgdbIndexMap, INDEX_MAP& cSdeIndexMap, const CString& cTableName, long cObjID, _IRowPtr& ipOrgRow, const _IRowPtr& ipPgdbRow, LPCTSTR cChgType, const ITablePtr& ipTable );

	/**
	 * @brief 	メッシュをまたいだオブジェクトかを判定する関数
	 *
	 * @param cEnv			[in]	各種情報を保持した構造体
	 * @param cObjID	[in]	SDE上のオブジェクトID
	 * @param ipFeature	[in]	SDE上のフィーチャ（NULLの時は内部で検索する）
	 *
	 * @retval true		変更なし又は変更ありでも更新は可
	 * @retval false	削除されている又は国内優先モードで変更されている
	 */
	bool fnOverMesh(EnvTable& cEnv, long cObjID, IFeaturePtr ipFeature = NULL) const;

	/**
	 * @brief 	メッシュをまたいだオブジェクトの更新関数
	 *
	 * @param ipNewClass	[in]	編集後のフィーチャクラス
	 * @param cEnv			[in]	各種情報を保持した構造体
	 * @param cObjID		[in]	SDE上のオブジェクトID
	 * @param ipNewFeature	[in]	編集後のフィーチャ（削除対象の時はNULL）
	 * @param cStatus		[in]	更新ステータス
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnClipAndApend(const IFeatureClassPtr& ipNewClass, EnvTable& cEnv, long cObjID, const IFeaturePtr& ipNewFeature, long cStatus);

	/**
	 * @brief 	メッシュをまたいでいないものの単純更新関数
	 *
	 * @param ipNewRow	[in]	編集後のロウ
	 * @param cEnv		[in]	各種情報を保持した構造体
	 * @param cObjID	[in]	SDE上のオブジェクトID
	 * @param cStatus	[in]	更新ステータス
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnReflectToSiNDY(const _IRowPtr& ipNewRow, EnvTable& cEnv, long cObjID, long cStatus);

	/**
	 * @brief 	INF,LQ用更新関数
	 *
	 * @param ipNewRow	[in]	編集後のロウ
	 * @param cEnv		[in]	各種情報を保持した構造体
	 * @param cObjID	[in]	SDE上のオブジェクトID
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnInfReflectToSiNDY(const _IRowPtr& ipNewRow, LqEnvTable& cEnv, long cObjID);

	/**
	 * @brief 	反映対象属性のコピー関数
	 *
	 * @param ipTgtRow		[in/out]	コピー先ロウ
	 * @param ipSrcRow		[in]		コピー元ロウ
	 * @param cSdeIndex		[in]		各種情報を保持した構造体
	 * @param cPgdbIndex	[in]		各種情報を保持した構造体
	 * @param cField		[out]		更新したフィールド名（ログ出力用）
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnReflectAttr(_IRowPtr& ipTgtRow, const _IRowPtr& ipSrcRow, INDEX_MAP& cSDEIndex, INDEX_MAP& cPGDBIndex, CString& cField);

	/**
	 * @brief 	メッシュポリゴンでクリップした形状を求める関数
	 *          fnGetRemakeShapeと似ているように見えるが、こちらは、メッシュポリゴンで切り取った残りのジオメトリを求める
	 *
	 * @param ipFeature		[in]	対象フィーチャ
	 * @param cEnv			[in]	各種情報を保持した構造体
	 * @param cObjID		[in]	SDE上のオブジェクトID
	 * @param cForceMode	[in]	true：単純にクリップした形状を求める false:メッシュ境界が関係するかを簡易的に求める（？）名前と内容の関係がわからなくなった
	 *
	 * @retval NULL以外	演算後のジオメトリ
	 * @retval NULL		失敗、エラー
	 */
	IGeometryPtr fnGetClipShape(const IFeaturePtr& ipFeature, EnvTable& cEnv, long cObjID, bool cForceMode = false) const;

	/**
	 * @brief 	メッシュ境界のオブジェクトに対して、当該メッシュ外部分と当該メッシュ内の切り貼りを行う関数
	 * 　　　　 マルチパートになることもあり、オリジナルIDが同じものを1つにまとめようとするが、属性が変更になっているものがあれば新規追加扱いとする
	 *
	 * @param ipNewClass	[in]	編集後のフィーチャクラス
	 * @param ipFeature		[in]	編集後のフィーチャ
	 * @param cIndex		[in]	テーブルの何番目のフィーチャクラスかを示すインデックス（0～2）
	 * @param cObjID		[in]	SDE上のオブジェクトID
	 *
	 * @retval NULL以外	演算後のジオメトリ
	 * @retval NULL		失敗、エラー
	 */
	IGeometryPtr fnGetRemakeShape(const IFeatureClassPtr& ipNewClass, const IFeaturePtr& ipFeature, EnvTable& cEnv, long cObjID);

	/**
	 * @brief 	レイヤ単位で変更を反映する関数
	 * 　　　　 「cGetIDOnly」が「true」の時はIDの対応を取得するだけで反映はしない
	 *
	 * @param ipOldWorkspace	[in]	編集前のPGDBのワークスペース
	 * @param ipNewWorkspace	[in]	編集後のPGDBのワークスペース
	 * @param cEnv				[in]	各種情報を保持した構造体
	 * @param cGetIDOnly		[in]	IDの対応だけ取得するかのフラグ
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnReflectGeneral(const IWorkspacePtr& ipOldWorkspace, const IWorkspacePtr& ipNewWorkspace, EnvTable& cEnv, bool cGetIDOnly);

	/**
	 * @brief 	INF・LQのペア単位で変更を反映する関数
	 *
	 * @param ipOldWorkspace	[in]	編集前のPGDBのワークスペース
	 * @param ipNewWorkspace	[in]	編集後のPGDBのワークスペース
	 * @param cEnv				[in]	各種情報を保持した構造体
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnReflectLQ(const IWorkspacePtr& ipOldWorkspace, const IWorkspacePtr& ipNewWorkspace, LqEnvTable& cEnv);

	/**
	 * @brief 	同じもの（同じ位置又は同じノードID・リンクIDを持つ一時停止ポイント）が既に存在するかを調べる関数
	 *
	 * @param ipFeature	[in]	調べる元になるフィーチャ
	 * @param cEnv		[in]	各種情報を保持した構造体
	 *
	 * @retval true		存在する
	 * @retval false	存在しない
	 */
	bool fnExistSamePoint(const IFeaturePtr& ipFeature, EnvTable& cEnv);

	/**
	 * @brief 	他レイヤのIDを示すフィールドについて、その示しているオブジェクトの変化を調べる関数
	 *　　　　　戻り値が「反映してよいか」を示しているので、そのフィールドを持っていない場合や関連オブジェクトに変更がなければtrueが返り、
	 *　　　　　変更があった場合やエラーか削除されている場合にfalseが返る。
	 *　　　　　変更があった場合でも、それが本プログラム実行中に変更されたものであれば、trueが返る
	 *
	 * @param ipRow			[in]	調べる元になるロウ
	 * @param cPgdbIndexMap	[in]	PGDBのフィールドインデックスマップ
	 * @param cTableName	[in]	識別用のテーブル名称
	 * @param cFieldName	[in]	他のレイヤのIDを示すフィールド名
	 * @param ipGeom		[in]	ログ出力用のジオメトリ
	 * @param cRelLayer		[out]	関連オブジェクトのレイヤ名
	 *
	 * @retval true		更新可能
	 * @retval false	変更された、エラー、削除された等の理由で更新不可
	 */
	bool fnRelObjChange(const _IRowPtr& ipRow, INDEX_MAP& cPgdbIndexMap, const CString& cTableName, const CString& cFieldName, const IGeometryPtr& ipGeom, CString& cRelLayer);

	/**
	 * @brief 	他レイヤのIDを示すフィールドについて、その示しているオブジェクトの変化を調べる関数
	 *　　　　　関係するフィールド分「fnRelObjChange」を呼ぶ。詳細は「fnRelObjChange」参照
	 *
	 * @param ipRow			[in]	調べる元になるロウ
	 * @param cPgdbIndexMap	[in]	PGDBのフィールドインデックスマップ
	 * @param cTableName	[in]	識別用のテーブル名称
	 * @param ipGeom		[in]	ログ出力用のジオメトリ
	 * @param cRelLayer		[out]	関連オブジェクトのレイヤ名
	 *
	 * @retval true		更新可能
	 * @retval false	変更された、エラー、削除された等の理由で更新不可
	 */
	bool fnRelObjsChange(const _IRowPtr& ipRow, INDEX_MAP& cPgdbIndexMap, const CString& cTableName, const IGeometryPtr& ipGeom, CString& cRelLayer);

	/**
	 * @brief 	２つのフィーチャについて、形状の一致を調べる関数
	 *
	 * @param ipFeature1		[in]	形状を比較するフィーチャ１
	 * @param ipFeature2		[in]	形状を比較するフィーチャ２
	 *
	 * @retval true		形状一致
	 * @retval false	形状が違う（一致しない）
	 */
	bool fnShapeComp(const IFeaturePtr& ipFeature1, const IFeaturePtr& ipFeature2) const;

	/**
	 * @brief 	リンク列の変更と削除関数
	 *
	 * @param ipOldCTable	[in]	編集前のテーブル（INF,LQ）
	 * @param ipNewTable	[in]	編集後のテーブル（INF,LQ）
	 * @param cEnv			[in]	各種情報を保持した構造体
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnInfModAndDel(const ITablePtr ipOldTable[2], const ITablePtr ipNewTable[2], LqEnvTable& cEnv);

	/**
	 * @brief 	リンク列の新規追加関数
	 *
	 * @param ipOldCTable	[in]	編集前のテーブル（INF,LQ）
	 * @param ipNewTable	[in]	編集後のテーブル（INF,LQ）
	 * @param cEnv			[in]	各種情報を保持した構造体
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnInfAdd(const ITablePtr ipOldTable[2], const ITablePtr ipNewTable[2], LqEnvTable& cEnv);

	/**
	 * @brief 	LQの変更状態を見ながら変更可能かを調べる関数
	 *
	 * @param cInfID        [in]  INFのオブジェクトID（PGDB）
	 * @param ipOldTable    [in]  編集前のテーブル（LQ）
	 * @param ipNewTable    [in]  編集後のテーブル（LQ）
	 * @param ipSdeTable[2] [in]  SDE上のテーブル（[0]:INF, [1]:LQ）
	 * @param cEnv          [in]  各種情報を保持した構造体
	 * @param cAdd          [out] 追加されたLQのIDセット	
	 * @param cMod          [out] 変更されたLQのIDセット
	 * @param aDel          [out] 削除されたLQのIDセット
	 *
	 * @retval true		変更可能
	 * @retval false	変更不可
	 */
	bool fnCanRefrectLq(long cInfID, const ITablePtr& ipOldTable, const ITablePtr& ipNewTable, const ITablePtr ipSdeTable[2], LqEnvTable& cEnv, ID_SET& cAdd, ID_SET& cMod, ID_SET& aDel);

	/**
	 * @brief 	属性の変更の有無を調べる関数
	 *　　　　　単純な比較ではなく、見なくて良い（見てはいけない）所を除くなど、処理に都合の良い比較をする
	 *
	 * @param ipSrcRow		[in]	比較元のロウ
	 * @param cSrcIndex		[in]	比較元用のフィールドインデックスマップ
	 * @param ipTgtRow		[in]	比較先のロウ
	 * @param cTgtIndex		[in]	比較先用のフィールドインデックスマップ
	 *
	 * @retval true		変更あり
	 * @retval false	変更なし
	 */
	bool fnIsAttrChange(const _IRowPtr& ipSrcRow, const INDEX_MAP& cSrcIndex, const _IRowPtr& ipTgtRow, const INDEX_MAP& cTgtIndex);

	/**
	 * @brief 	対象IDのLQを削除する関数
	 *　　　　　削除可能であることは事前にチェックされていること
	 *
	 * @param cDelIDs		[in]	削除対象のLQのオブジェクトIDセット
	 * @param cLayerName	[in]	LQのレイヤ名（ログ用）
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnDeleteLQs(const ID_SET& cDelIDs, LPCTSTR cLayerName);

	/**
	 * @brief 	対象IDのLQを追加する関数
	 *　　　　　追加可能であることは事前にチェックされていること
	 *
	 * @param cAddIDs		[in]	削除対象のLQのオブジェクトIDセット
	 * @param cInfID		[in]	LQを所有する（？）INFのID
	 * @param ipNewTable	[in]	編集後のテーブル（LQ）
	 * @param cEnv			[in]	各種情報を保持した構造体
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnAddLQs(const ID_SET& cAddIDs, long cInfID, const ITablePtr& ipNewTable, LqEnvTable& cEnv);

	/**
	 * @brief 	対象IDのLQを変更する関数
	 *　　　　　変更可能であることは事前にチェックされていること
	 *
	 * @param cModIDs		[in]	変更対象のLQのオブジェクトIDセット
	 * @param ipNewTable	[in]	編集後のテーブル（LQ）
	 * @param cEnv			[in]	各種情報を保持した構造体
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnModLQs(const ID_SET& cModIDs, const ITablePtr& ipNewTable, LqEnvTable& cEnv);

	/**
	 * @brief 	メッシュ境界に関わる又はメッシュ範囲外かを判定する関数
	 *
	 * @param ipGeom	[in]	判定するジオメトリ
	 *
	 * @retval true		メッシュ境界に関わるか範囲外（処理対象外）
	 * @retval false	完全に内包されている（処理対象）
	 */
	bool fnMeshRelObject(const _IRowPtr& ipGeom) const;

	/**
	 * @brief 	参照している他のレイヤを変更禁止リストに追加する関数（リンクID,ノードID等全部）
	 *
	 * @param ipRow			[in]	処理対象のロウ
	 * @param cPgdbIndexMap	[in]	対象のフィールドインデックスマップ
	 *
	 * @retval true		（固定）
	 */
	bool fnProhibitRelIDAll(const _IRowPtr& ipRow, INDEX_MAP& cPgdbIndexMap);

	/**
	 * @brief 	参照している他のレイヤを変更禁止リストに追加する関数（フィールド名指定）
	 *
	 * @param ipRow			[in]	処理対象のロウ
	 * @param cPgdbIndexMap	[in]	対象のフィールドインデックスマップ
	 * @param cFieldName	[in]	他のレイヤのIDを保持しているフィールド名
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnProhibitRelID(const _IRowPtr& ipRow, INDEX_MAP& cPgdbIndexMap, const CString& cFieldName);

	/**
	 * @brief 	リンク列と関連フラグの整合を取る関数
	 *　　　　　変更できなかったリンク列について、フラグの不整合を起こすことがあるため、その後始末
	 *
	 * @param cEnv			[in]	各種情報を保持した構造体
	 *
	 * @retval true		（固定）
	 */
	bool fnLinkFlagCare(LqEnvTable& cEnv);

	/**
	 * @brief 	ID列指定でリンク列と関連フラグの整合を取る関数
	 *
	 * @param ipClass		[in]	道路リンクのフィーチャクラス
	 * @param cFlagIndex	[in]	関連フラグのフィールドインデックス
	 * @param cIDSet		[in]	処理対象のリンクID列
	 *
	 * @retval true		（固定）
	 */
	bool fnLinkFlagCareByID(const IFeatureClassPtr& ipClass, long cFlagIndex, const ID_SET& cIDSet);

	/**
	 * @brief 	リンクのIDを元にリンク列で参照されているかを判定する関数
	 *　　　　　戻り値をそのまま関連フラグの値とするために数値で0/1を返す
	 *
	 * @param cObjID	[in]	リンクのオブジェクトID
	 *
	 * @retval 0	参照されていない
	 * @retval 1	参照されている
	 */
	int fnIsExistLQ(long cObjID) const;

	/**
	 * @brief 	同じとみなせるものが既に存在するかを調べる関数
	 *
	 * @param ipRow	[in]	調べる元になるフィーチャ
	 * @param cEnv	[in]	各種情報を保持した構造体
	 *
	 * @retval true		存在する
	 * @retval false	存在しない
	 */
	bool fnExistSameObj(const _IRowPtr& ipRow, EnvTable& cEnv);

	/**
	 * @brief 	固有属性（共通属性以外で）の差異を調べる関数
	 *　　　　　「同一とみなせるものが存在するか」で使用するので、比較対象は必要に応じて調整のこと
	 *
	 * @param ipPgdbRow	[in]	PGDBのロウ
	 * @param ipSdeRow	[in]	SDEのロウ
	 * @param cEnv		[in]	各種情報を保持した構造体
	 *
	 * @retval true		同一とみなす
	 * @retval false	別物とみなす
	 */
	bool fnSameAttr(const _IRowPtr& ipPgdbRow, const _IRowPtr& ipSdeRow, EnvTable& cEnv);

	/**
	 * @brief 	２つのフィーチャについて、形状の部分一致（とりあえず2点以上同じ）を調べる関数
	 *
	 * @param ipFeature1		[in]	形状を比較するフィーチャ１
	 * @param ipFeature2		[in]	形状を比較するフィーチャ２
	 *
	 * @retval true		形状部分一致とみなす
	 * @retval false	別物とみなす
	 */
	bool fnShapePartComp(const IFeaturePtr& ipFeature1, const IFeaturePtr& ipFeature2) const;

	/**
	 * @brief 	同じとみなせるリンク列が既に存在するかを調べる関数
	 *
	 * @param ipLqTable    [in]  LQテーブル（追加するものが入っているPGDBの）
	 * @param ipNewRow     [in]  調べる元になるフィーチャ
	 * @param cEnv         [in]  各種情報を保持した構造体
	 * @param ipSdeLqTable [in]  SDE上のLQテーブル
	 *
	 * @retval true		存在する
	 * @retval false	存在しない
	 */
	bool fnExistSameLQ(const ITablePtr& ipLqTable, const _IRowPtr& ipNewRow, LqEnvTable& cEnv, const ITablePtr& ipSdeLqTable );

	/**
	 * @brief 	INFのIDを元にLQが示すリンクIDを順番になるよう取得する関数
	 *
	 * @param ipLqTable		[in]	LQテーブル
	 * @param cInfID		[in]	INFのオブジェクトID
	 * @param cLqIndexMap	[in]	LQのフィールドインデックスマップ
	 * @param cIsPGDB		[in]	PGDBかのフラグ（IDの変換をするか否かで使用）
	 * @param cLqMap		[out]	LQが示すリンクの順番（シーケンス-リンクIDのマップ）
	 *
	 * @retval true		存在する
	 * @retval false	存在しない
	 */
	bool fnGetLqInfo(const ITablePtr& ipLqTable, long cInfID, INDEX_MAP& cLqIndexMap, bool cIsPGDB, LQ_MAP& cLqMap);

	/**
	 * @brief  リンクのIDから、自身の関連Inf/Lqが更新可能か確認
	 * @note   ここでの関連Inf/Lqとは、自身のInfとそのInfIDを持つ全てのLqが対象
	 * @param  ipOldTable  [in]  更新前PGDBのInf/Lqテーブル
	 * @param  ipNewTable  [in]  更新後PGDBのInf/Lqテーブル
	 * @param  ipSdeTable  [in]  SDEのInf/Lqテーブル
	 * @param  env         [in]  各種情報を保持した構造体
	 * @param  linkPgdbOID [in]  リンクのObjectID(PGDBのID)
	 * @retval true        該当リンクが更新OK
	 * @retval false       該当リンクが更新NG
	 */
	bool fnCanRelInfLqUpdate( const ITablePtr ipOldTable[2], const ITablePtr ipNewTable[2], const ITablePtr ipSdeTable[2], LqEnvTable& env, long linkPgdbOID );

};
