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
 * @file Features.h
 * @brief <b>CFeaturesクラス定義ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @vertion $Id$
 */
#ifndef _FEATURES_H_
#define _FEATURES_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FeatureContainer.h"

class CListString;

namespace sindy {

//class CFeatureContainer;
//class CLogicRule;
//class CErrorObjects;
//class CRow;
//class CGeometry;

/**
 * @class CFeatures
 * @brief <b>フィーチャ・レコードの統合的な処理を行うクラス</b>\n
 * このクラスは、全てにおける処理を行うことを目的としたクラスです。\n
 * このクラスを使用することで、全ての処理はライブラリに任せることができ、
 * クライアントを記述する手間を大幅に省くことが可能です。\n
 * 使用方法：\n
 * @li クラス初期化（Init）
 * @li フィーチャ選択（Select...）
 * @li 編集処理（Move、Delete、...）
 * @li 論理チェック（Check）
 */
class SINDYLIB_API CFeatures
{
public:
	explicit CFeatures()
	{
	}
	virtual ~CFeatures()
	{
		clear();
	}

	/**
	 * @brief <b>整合性をチェックする</b>\n
	 * フィーチャを編集した後に必ずチェックを行ってください。
	 * @param cRule			[in]	論理チェック用ルール
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const;

	/**
	 * @brief <b>既に編集されていないかどうか確認する</b>\n
	 * 変更があったものに対してのみチェックを行います。\n
	 * キャッシュを作成していないと変更のチェックに引っかからないので注意してください。\n
	 * @param cErrObjects	[in,out]	エラーオブジェクトが入る又は、中身をチェックする
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckModify( CErrorObjects& cErrObjects ) const;

	/**
	 * @brief <b>フィーチャを選択する</b>\n
	 * @param ipGeom		[in]			選択形状
	 * @param listNames		[in]			選択するレイヤ・テーブル名
	 * @param bRecycling	[in,optional]	trueにするとキャッシュを作成して切り離し
	 * @param bCreateCache	[in,optional]	キャッシュを作成する（bRecyclingがtrueの場合はこのフラグに関係なく必ず作成される）
	 * @param cErrs			[out,optional]	エラー格納コンテナ（オブジェクト同士の関連付けに失敗したもののみ入る）
	 *
	 * @return sindyErrCode	エラーがある場合はcErrsのGetDefaultErrorCodeの結果が返る
	 */
	errorcode::sindyErrCode SelectByShape( IWorkspace* ipWorkspace, IGeometry* ipGeom, const CListString& listNames, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* cErrs = NULL, bool bInit = true );

	/**
	 * @brief <b>フィーチャを選択する</b>\n
	 * @param ipGeom		[in]			選択形状
	 * @param strName		[in]			選択するレイヤ・テーブル名
	 * @param bRecycling	[in,optional]	trueにするとキャッシュを作成して切り離し
	 * @param bCreateCache	[in,optional]	キャッシュを作成する（bRecyclingがtrueの場合はこのフラグに関係なく必ず作成される）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SelectByShape( IWorkspace* ipWorkspace, IGeometry* ipGeom, LPCTSTR strName, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* cErrs = NULL, bool bInit = true );

	/**
	 * @brief <b>フィーチャを選択する</b>\n
	 * 渡されたフィーチャの変更前・変更後の形状で検索します。
	 * 変更前の形状取得にはIFeatureChanges::get_OriginalShapeを、変更後の形状取得にはIFeature::get_Shapeを使用します。
	 * @param ipGeom		[in]			選択形状
	 * @param bRecycling	[in,optional]	trueにするとキャッシュを作成して切り離し
	 * @param bCreateCache	[in,optional]	キャッシュを作成する（bRecyclingがtrueの場合はこのフラグに関係なく必ず作成される）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SelectByObject( IWorkspace* ipWorkspace, _IRow* ipRow, bool bRecycling = false, bool bCreateCache = false );

	/**
	 * @brief <b>与えられた形状に関連するものをコンテナに格納する</b>\n
	 * @param ipGeom		[in]	クエリ形状（NULLの場合は全て取得）
	 * @param listFeatures	[out]	格納コンテナ
	 */
	void QueryFeatures( IGeometry* ipGeom, CContainer& cContainer ) const;

	//@{ @name 編集系関数
	/**
	 * @brief <b>与えられた形状の範囲で移動処理を行う</b>\n
	 * 移動モードは次の通り：
	 * @li <b>sindyeMove_Segment</b>\n
	 *		構成点を追加せずにクエリ形状内を平行移動するモードです。\n
	 *		クエリ形状の境界にあるセグメントは伸縮対象となります。\n
	 *		クエリ形状の境界にある構成点は、移動対象とはなりません。
	 * @li sindyeMove_Point
	 *		クエリ形状内の中心に一番近い構成点のみを移動するモードです。\n
	 *		標準の移動処理に一番近いですが、クエリ形状内に構成点が一つも
	 *		存在しない場合、構成点を作成し移動します。
	 * @li sindyeMove_SegmentAddPointBorderIntersect
	 *		クエリ形状の境界に構成点を一つ作成して平行移動するモードです。\n
	 *		クエリ形状の境界にあるセグメントに構成点が一つ追加され、その構成点
	 *		及びクエリ形状内の構成点を移動します。\n
	 *		クエリ形状の境界にある構成点も移動対象となります。
	 * @li sindyeMove_SegmentAddPointBorderWithin
	 *		クエリ形状の境界に構成点を一つ作成して平行移動するモードです。\n
	 *		クエリ形状の境界にあるセグメントに構成点が一つ追加された後、sindyeMove_Segment
	 *		と同様の処理を行います。クエリ形状内のみが移動対象となる点が、sindyeMove_SegmentAddPointBorderIntersect
	 *		と異なります。
	 * @li sindyeMove_SegmentAddPointsBorder
	 *		クエリ形状の境界に構成点を二つ作成して平行移動するモードです。\n
	 *		クエリ形状の境界に構成点を二つ作成し、片方はそのまま、もう片方を
	 *		移動します。クエリ形状の境界にあるほかの構成点は移動対象とはなりません。
	 * @param cGeom			[in]	クエリ形状
	 * @param pairMove		[in]	移動差分
	 * @param emMode		[in]	移動モード
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Move( const CGeometry& cGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMode, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>点->点の移動処理を行う</b>\n
	 * Moveの点->点移動限定版です。
	 * 移動ルーチンは sindyeMove_Point を使用します。
	 * @param ipFrom		[in]	移動前の点
	 * @param ipTo			[in]	移動後の点
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode MovePoint( IPoint* ipFrom, IPoint* ipTo, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>与えられた形状との交差部分で分割処理を行う</b>\n
	 * @param pSource		[in]	分割対象
	 * @param cGeom			[in]	分割形状
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Split( CSPRow pSource, const CGeometry& cGeom, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>与えられたフィーチャのマージを行う</b>\n
	 * 指定フィーチャが格納されていること。
	 * @param pSource		[in]	マージ元
	 * @param pDest			[in]	マージ先
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Merge( CSPRow pSource, CSPRow pDest, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>点の削除を行う</b>\n
	 * @param pRow			[in]	削除対象
	 * @param ipGeom		[in]	削除形状
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @param bEraseOnly	[in/optional]	CRoadNetworkのみ使用。
	 *                                      自分自身のみにするかどうか（pRowが内包関係の時にしか使わない）
	 *                                      削除ツールのセグメント削除で始点・終点どちらかが含まれている時に、
	 *                                      OIDを保持するために構成点構成点削除をするようにしている関係上
	 *                                      pRowで指定している以外の内包関係は処理しないようにするためだけに使用
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Erase( CSPRow pRow, const CGeometry& cGeom, CErrorObjects& cErrObjects, bool bEraseOnly=false );

	/**
	 * @brief <b>1削除を行う</b>\n
	 * @param pRow			[in]	削除対象
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Delete( CSPRow pRow, CErrorObjects& cErrObjects );

	/**
	 * @brief クローンを作成する
	 *
	 * 既存のオブジェクト属性を保持しつつ、新規にオブジェクトを作成して
	 * 属性をコピーします。その際、既存のオブジェクトは開放されます。
	 * @param ipWorkspace [in,optional] クローンを作成するワークスペース（デフォルト：NULL）
	 * @note ipWorkspaceを指定した場合は同じテーブル名を検索してクローンを作成します。
	 * @warning ipWorkspaceを指定する場合、指定ワークスペース内には既存オブジェクトが所属する
	 * テーブル名と同一のテーブルが存在する必要があります。また、SDEの場合はipWorkspaceの接続
	 * ユーザ名をオーナー名としてテーブルを検索します。
	 */
	errorcode::sindyErrCode Clone( IWorkspace* ipWorkspace = NULL );

	/**
	 * @brief <b>編集内容を保存する</b>\n
	 * この関数では以下の処理を自動的に行います：
	 * @li ModifyCheckで編集されたオブジェクトを対象にチェック
	 * @li キャッシュがあれば、キャッシュへの変更結果を反映
	 * @li Store
	 * @param cErrObjects	[out]			エラーオブジェクト格納用コンテナ
	 * @retval sindyErr_NoErr				エラーなし
	 * @retval 上記以外						エラー
	 */
	errorcode::sindyErrCode Store( CErrorObjects& cErrObjects );
	//@}

	/**
	 * @brief <b>キャッシュされているかどうかをチェックする</b>\n
	 * @retval true		全てキャッシュされている
	 * @retval false	一つでもキャッシュされていないものがある
	 */
	bool IsCached() const;

	//@{ @name コンテナ操作系関数
	/**
	 * @brief <b>メンバをクリアする</b>\n
	 */
	void clear();

	/**
	 * @brief <b>コンテナが空かどうかをチェックする</b>\n
	 * @retval false	空ではない
	 * @retval true		空である
	 */
	bool empty() const;

	/**
	 * @brief <b>登録されているフィーチャがあればそのポインタを返す</b>\n
	 * 今のところ属性ダイアログで使用中
	 * @return CRow*	登録されているフィーチャのポインタ
	 */
	CRow* FindObject( _IRow* ipRow );

	//@}

	//@{ @name コンテナ操作系関数
	typedef std::list<CFeatureContainer*>::iterator iterator;
	typedef std::list<CFeatureContainer*>::const_iterator const_iterator;
	iterator begin(){ return m_listFeatureContainer.begin(); }
	iterator end(){ return m_listFeatureContainer.end(); }
	const_iterator begin() const{ return m_listFeatureContainer.begin(); }
	const_iterator end() const{ return m_listFeatureContainer.end(); }
	//@}
	/**
	 * @brief <b>現在格納されているフィーチャの存在矩形を返す<b>\n
	 * @param bOriginal              [in,optional] 変更前の形状矩形が欲しい場合はtrue（デフォルト：true）
	 * @param bChanged               [in,optional] 変更後の形状矩形が欲しい場合はtrue（デフォルト：true）
	 * @param bOriginalIsArcOriginal [in,optional] IFeatureChanges::get_OriginalShapeの形状矩形が欲しい場合にはtrue（デフォルト：true）
	 * @note この関数は、例えば編集後の再描画をする場合に変更前・変更後の形状を含んだ矩形が欲しい場合などに使用します。
	 * @warning Store()してしまうとキャッシュは全て破棄されますので、変更前の形状が取得できなくなります。
	 * 変更前の形状を取得する必要がある場合はStore()前にこの関数を呼ぶ必要があります。
	 */
	IEnvelopePtr GetEnvelope( bool bOriginal = true, bool bChanged = true, bool bFeatureChangedOriginal = true ) const;

	/**
	 * @brief <b>現在格納されているフィーチャの形状をマージしたものを返す</b>\n
	 * @param bLatest                [in,optional] 最新の形状が欲しい場合はtrue（デフォルト：true）
	 * @param bOriginal              [in,optional] 変更前の形状が欲しい場合はtrue（デフォルト：false）
	 * @param bOriginalIsArcOriginal [in,optional] IFeatureChanges::get_OriginalShapeの形状が欲しい場合にはtrue（デフォルト：false）
	 * @param lParam                 [in,optional] 現状はスケール値を想定していますが、変更される可能性があります。（デフォルト：-1）
	 * @note この関数は、変更前・変更後を含む全ての形状をUnionしたものを取得することができます。
	 * また、Point、Polyline、Polygonが混ざっている場合はGeometryBagで返します。
	 * @note この関数は、例えば編集後の論理チェックをする場合に変更後の場所に存在するフィーチャを検索しなければならない場合などに使用します。
	 */
	IGeometryPtr GetShape( bool bLatest = true, bool bOriginal = false, bool bFeatureChangedOriginal = false, long lParam = -1 ) const;

	/**
	 * @brief キャッシュを作成する
	 * @param bDetatch [in,optional] レコード・テーブルを切り離す場合はtrue（デフォルト：false）
	 * 
	 * 属性・形状のキャッシュを作成を行います。
	 * キャッシュを削除したい場合はDeleteCacheを呼びます。
	 */
	errorcode::sindyErrCode CreateCache( bool bDetatch = false ) {
		for( iterator it = begin(); it != end(); ++it )
		{
			errorcode::sindyErrCode emErr = (*it)->CreateCache( bDetatch );
			if( errorcode::sindyErr_NoErr != emErr )
				return emErr;
		}
		return errorcode::sindyErr_NoErr;
	}

	/**
	 * @brief <b>キャッシュを削除する</b>
	 * @param bRecursive [in,optional] CRowが抱えているコンテナのキャッシュも削除する場合はtrue（デフォルト：false）
	 */
	void DeleteCache( bool bRecursive = false )	{
		for( iterator it = begin(); it != end(); ++it )
			(*it)->DeleteCache( bRecursive );
	}

	/**
	 * @brief キャッシュ以外のパラメータを更新する
	 * @param bForce [in,optional] 強制的に作り直す場合はtrue
	 * 
	 * 属性・形状に付随する各種パラメータを更新します。
	 * デフォルトでは何も動作しません。注記の記号・文字列
	 * 矩形や、リンク列形状などを想定しています。
	 * @see CContainer::UpdateParameter
	 */
	errorcode::sindyErrCode UpdateParameter( bool bForce = false ) {
		for( iterator it = begin(); it != end(); ++it )
		{
			errorcode::sindyErrCode emErr = (*it)->UpdateParameter( bForce );
			if( errorcode::sindyErr_NoErr != emErr )
				return emErr;
		}
		return errorcode::sindyErr_NoErr;
	}
	void DeleteParameter() {
		for( iterator it = begin(); it != end(); ++it )
			(*it)->DeleteParameter();
	}

	void Trace( bool bAttribute = true, bool bGeometry = true ) const;
private:
	/**
	 * @brief <b>検索するためのコンテナをセットする</b>\n
	 * 共有編集対象は強制的に登録されます。\n
	 * SelectByShape関数から使用されます。\n
	 * 内部でリスト版の同名関数を使用しています。
	 * @param ipWorkspace		[in]	検索対象ワークスペース
	 * @param lpcszName			[in]	テーブル名（オーナー名付きであることが望ましい）
	 * @param listContainer		[out]	listNamesに対応するコンテナリスト
	 * @retval sindyErrCode
	 */
	errorcode::sindyErrCode SetFeatureContainer( IWorkspace* ipWorkspace, LPCTSTR lpcszName, std::list<CFeatureContainer*>& listContainer );

	/**
	 * @brief <b>検索するためのコンテナをセットする</b>\n
	 * 共有編集対象は強制的に登録されます。\n
	 * SelectByShape関数から使用されます。
	 * @param ipWorkspace		[in]	検索対象ワークスペース
	 * @param listNames			[in]	テーブル名（オーナー名付きであることが望ましい）
	 * @param listContainer		[out]	listNamesに対応するコンテナリスト
	 * @retval sindyErrCode
	 */
	errorcode::sindyErrCode SetFeatureContainer( IWorkspace* ipWorkspace, const CListString& listNames, std::list<CFeatureContainer*>& listContainer );

	/**
	 * @brief <b>誘導リンク整合性をチェックする</b>\n
	 * フィーチャを編集した後に必ずチェックを行ってください。
	 * [Bug 5846] [歩行者NW]誘導リンクの端点選択で短縮しないようにして欲しい
	 * @param pContainer	[in]	誘導リンクコンテナ
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckLogic_GuideLink( CFeatureContainer* pContainer, CErrorObjects& cErrObjects ) const;

private:
#pragma warning(push)
#pragma warning(disable: 4251)
	std::list<CFeatureContainer*> m_listFeatureContainer;	//!< 各フィーチャクラスやグループ（道路系とか注記系とか）別のコンテナのリスト
#pragma warning(pop)
};

} // namespace sindy

#endif // ifndef _FEATURES_H_
