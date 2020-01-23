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
 * @file Annotations.h
 * @brief <b>CAnnotationsクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _ANNOTATIONS_H_
#define _ANNOTATIONS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RelAnnotationParameter.h"
#include "FeatureContainer.h"
#include "SpatialIndex.h"
#include "SiNDYLogFunctions.h"
#include "meshutil.h"

namespace sindy {

/**
 * @class CAnnotations
 * @brief <b>都市地図注記に関する総合処理を行うクラス</b>\n
 * このクラスでは、真位置と表示位置の関連付け、検索、検索結果の
 * 関連付けと管理を行います。
 */
class SINDYLIB_API CAnnotations :
	public CFeatureContainer
{
public:
	explicit CAnnotations(){}
	virtual ~CAnnotations(){ clear(); }

	explicit CAnnotations( sindyTableType::ECode emTableType, IFeatureClass* lp )
	{
		SetObject( emTableType, lp );
	}

	explicit CAnnotations( const CFeatureClass& obj )
	{
		CTable::SetObject( obj.GetTableType(), (ITable*)obj );
	}

	void SetObject( sindyTableType::ECode emTableType, IFeatureClass* lp )
	{
		CTable::SetObject( emTableType, lp );
		clear();
	}
	sindyTableType::ECode GetCurrentTableType() const {return m_emTableType;}

	/**
	 * @brief <b>メッシュコードで検索する</b>\n
	 * 真位置、表示位置両方を検索します。検索結果は自動的に
	 * 関連付けられリスト内に保持されます。\n
	 * 関連付けることができなかったものは捨てられます（エラーオブジェクトは作成します）。
	 * 関連付けをする事ができなかったフィーチャ
	 * があった場合にはsindyErr_RelFeatureNotFoundが返ります。
	 * @param lMeshCode		[in]			メッシュコード
	 * @param emMeshType	[in]			メッシュタイプ（ミドルメッシュ対応のため [bug 4054]）
	 * @param bRecycling	[in,optional]	trueにすると、表示位置のみリサイクル検索。その後真位置、表示位置のキャッシュを作成して切り離すので、bCreateCacheはtrueでなければならない（デフォルト：false）
	 * @param bCreateCache	[in,optional]	trueにするとキャッシュを作成する。bRecyclingがtrueの時は必ずtrueにしなければならない（デフォルト：false）
	 * @param pErrs			[out,optional]	エラー格納コンテナ（オブジェクト同士の関連付けに失敗したもののみ入る）
	 *
	 * @return sindyErrCode	エラーがある場合はcErrsのGetDefaultErrorCodeの結果が返る
	 */
	errorcode::sindyErrCode SelectByMesh( long lMeshCode, mesh_type::ECode emMeshType, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL );
	/**
	 * @brief <b>形状で検索する</b>\n
	 * 真位置、表示位置両方を検索します。検索結果は自動的に
	 * 関連付けられリスト内に保持されます。\n
	 * 内部でバッファリングなどはしないので、ポイントで検索する場合は
	 * バッファリングした形状を渡してください。\n
	 * 検索形状内に真位置、表示位置のど
	 * ちらかしかない場合に関連フィーチャをテーブルから検索します。
	 * 関連付けることができなかったものは捨てられます。
	 * があった場合にはsindyErr_RelFeatureNotFoundが返ります。
	 * @param ipGeom		[in]			検索形状
	 * @param bRecycling	[in,optional]	trueにすると、表示位置のみリサイクル検索。その後真位置、表示位置のキャッシュを作成して切り離し
	 * @param bCreateCache	[in,optional]	キャッシュを作成する（bRecyclingがtrueの場合はこのフラグに関係なく必ず作成される）
	 * @param pErrs			[out,optional]	エラー格納コンテナ（オブジェクト同士の関連付けに失敗したもののみ入る）
	 *
	 * @return sindyErrCode	エラーがある場合はcErrsのGetDefaultErrorCodeの結果が返る
	 */
	errorcode::sindyErrCode Select( IGeometry* ipGeom, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true );

	/**
	 * @brief <b>形状で検索する（レンダラ対応）</b>\n
	 * 基本的にはSelectByShapeと同じですが、レンダラ用に引数を追加しています。
	 * ルーチンを共通化するため、SelectByShapeもこの関数を呼んでいます。
	 * @param ipDispCursor		[in,optional]	表示位置テーブルでのカーソル（デフォルト：NULL）
	 * @param pDispTableType	[in,optional]	カーソルのテーブルタイプポインタ（デフォルト：NULL）
	 * @param bCreateIndex		[in,optional]	空間インデックスを作成するかどうか（デフォルト：true）
	 **/
	errorcode::sindyErrCode _SelectByShape( IGeometry* ipGeom, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true, IFeatureCursor* ipDispCursor = NULL, sindyTableType::ECode* pDispTableType = NULL,bool bCreateIndex = true  );

	/**
	 * @brief <b>形状で検索する</b>\n
	 * 真位置、表示位置両方を検索します。検索結果は自動的に
	 * 関連付けられリスト内に保持されます。\n
	 * 内部でバッファリングなどはしないので、ポイントで検索する場合は
	 * バッファリングした形状を渡してください。\n
	 * 検索形状内に真位置、表示位置のど
	 * ちらかしかない場合に関連フィーチャをテーブルから検索します。
	 * 関連付けることができなかったものは捨てられます。
	 * があった場合にはsindyErr_RelFeatureNotFoundが返ります。
	 * @param ipGeom		[in]			検索形状
	 * @param bRecycling	[in,optional]	trueにすると、表示位置のみリサイクル検索。その後真位置、表示位置のキャッシュを作成して切り離し
	 * @param bCreateCache	[in,optional]	キャッシュを作成する（bRecyclingがtrueの場合はこのフラグに関係なく必ず作成される）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SelectByObject( _IRow* ipRow, bool bRecycling = false, bool bCreateCache = false, bool bInit = true );

	/**
	 * @brief <b>渡されたオブジェクトセットに関連するものを検索する</b>\n
	 * 真位置、表示位置両方検索します。検索結果は自動的に
	 * 関連付けられリスト内に保持されます。\n
	 * 渡されたオブジェクトセット内で表示位置、真位置が完
	 * 全にセットになっていなくても関連オブジェクトを全て
	 * 検索し格納します。\n
	 * 渡されたオブジェクトと重複したオブジェクトが検索さ
	 * れた場合、渡されたオブジェクトが常に優先されます。
	 * @param ipRowSet		[in]			オブジェクトセット
	 * @param bCreateCache	[in,optional]	キャッシュを作成する（デフォルト：false）
	 * @param pErrs			[in,optional]	エラー格納コンテナ（オブジェクト同士の関連付けに失敗したもののみ入る デフォルト：NULL）
	 * @param bInit			[in,optional]	既存のオブジェクトをクリアする場合はtrue（デフォルト：true）
	 * @param bCreateIndex	[in,optional]	注記専用の擬似インデックスを作成する場合はtrue（デフォルト：true）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CAnnotations::Select( _ISet* ipRowSet, bool bCreateCache/* = false*/, CErrorObjects* pErrs/* = NULL*/, bool bInit /*= true*/, bool bCreateIndex/* = true*/ );


	//@{ @name CFeaturesContainer インターフェース
	/**
	 * @brief <b>渡されたジオメトリに関連するフィーチャを検索済みフィーチャの中から選択する</b>\n
	 * この関数は、実際の処理のための前処理（ラバーバンド作成など）に使用することを前提に作成してあるため、
	 * テーブルオブジェクトは対象に入りません。\n
	 * また、この関数は渡されたコンテナに結果を格納するため、インラインになっています。\n
	 * @param ipGeom		[in]	フィルタジオメトリ（NULLの場合は全て取得）
	 * @param emSpRel		[in]	関連フィルタ（今の所使用されません…）
	 * @param cContainer	[out]	関連するフィーチャが格納される
	 * @return sindyErrCode
	 */
	void QueryFeatures( IGeometry* ipGeom, esriSpatialRelEnum/* emSpRel*/, CContainer& cContainer ) const
	{
		bool bAll = ( ipGeom ) ? false : true;
		CGeometry cGeom( ipGeom );

		// 実際の形状ではなく、記号なら記号の大きさ、文字列なら文字列の表示領域で検索する必要がある。
		// なのに、この関数ではスケールの概念が無いため全てのスケールにおいてチェックせざるを得ない。
		// そんなことなら、無理にバウンディングボックスで計算しようとせずにそのままの形状で引っ掛けても
		// 変わらないよね？？
		// ということで、そのままの形状でチェックすることにする。written by furukawa 2005/11/17
		for( const_iterator it = begin(); it != end(); ++it )
		{
			for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			{
				// 真位置
				if( bAll || ( ! cGeom.Disjoint( (IGeometry*)((*it2)->GetShape()) ) ) )
					cContainer.push_back( *it2 );

				for( CRelAnnotation::const_iterator itDisp = (*it2)->begin(); itDisp != (*it2)->end(); ++itDisp )
				{
					for( CRelAnnotation::const_rows_iterator itDisp2 = itDisp->second.begin(); itDisp2 != itDisp->second.end(); ++itDisp2 )
					{
						// 表示位置
						if( bAll || ( ! cGeom.Disjoint( (IGeometry*)((*itDisp2)->GetShape()) ) ) )
							cContainer.push_back( *itDisp2 );
					}
				}
			}
		}
	}

	/**
	 * @brief <b>与えられた形状の範囲で平行移動処理を行う</b>\n
	 * @param cGeom			[in]	クエリ形状
	 * @param pairMove		[in]	移動差分
	 * @param emMode		[in]	移動モード（現在のところ完全平行移動のみ）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Move( const CGeometry&/* cGeom*/, const std::pair<double,double>& pairMove, sindyeMoveMode/* emMode*/, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>与えられた形状との交差部分で分割処理を行う</b>\n
	 * @param cGeom			[in]	クエリ形状
	 * @param cSplitTarget	[in]	分割対象
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
//	errorcode::sindyErrCode Split( const CGeometry& cGeom, const CSplitTarget& cSplitTarget, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>整合性をチェックする</b>\n
	 * 選択済みの注記に関してチェックを行います。\n
	 * フィーチャを編集した後に必ずチェックを行ってください。
	 * このメソッドでチェックされる項目は以下のとおりです。
	 * @li 注記文字列チェック
	 * @li 注記種別チェック
	 * @li 表示タイプチェック
	 * @li 注記文字列と真位置の位置関係チェック
	 * @li 表示位置同士の重なりチェック
	 * @li 真位置同士の重なりチェック
	 * @li 真位置と表示位置同士の重なりチェック
	 * @param cRule			[in]	論理チェック用ルール
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const;
	//@}

	/**
	 * @brief <b>空間インデックスを作成する</b>\n
	 * 作成された属性・形状キャッシュ、及びパラメータキャッシュを元に、空間インデックス
	 * を作成します。キャッシュが存在する必要があるので、必ずキャッシュを作成してから
	 * 実行してください。\n\n
	 * 空間インデックスを作成する前に、適切なインデックス作成矩形を更新します。\n
	 * cIndexRectには、適切な矩形を与えてください。通常は、SelectByShpeに渡した形状の
	 * 矩形が適切です。
	 * @param cIndexRect	[in]	空間インデックス作成用初期矩形
	 * @param nGrid			[in]	グリッドの単位距離
	 */
	void CAnnotations::CreateSpatialIndex( const WKSEnvelope& cIndexRect, double dGrid );

	/**
	 * @brief <b>空間インデックスを作成する</b>\n
	 * 作成された属性・形状キャッシュ、及びパラメータキャッシュを元に、空間インデックス
	 * を作成します。キャッシュが存在する必要があるので、必ずキャッシュを作成してから
	 * 実行してください。\n\n
	 * 空間インデックスを作成する前に、適切なインデックス作成矩形を更新します。\n
	 * cIndexRectには、適切な矩形を与えてください。通常は、SelectByShpeに渡した形状の
	 * 矩形が適切です。
	 * @param cIndexRect	[in]	空間インデックス作成用初期矩形
	 * @param nDivideX		[in]	横分割数
	 * @param nDivideY		[in]	縦分割数
	 */
	void CreateSpatialIndex( const WKSEnvelope& cIndexRect, int nDivideX, int nDivideY );

	//@{ @name 注記スケールイテレータ
	typedef DispIterator scale_iterator;
	typedef DispIterator scale_reverse_iterator;
	scale_iterator scale_begin() const { scale_iterator sc = DispIterator(GetTableType()); return sc; }
	scale_iterator scale_end() const { scale_iterator sc = DispIterator(sindyTableType::unknown); return sc; }
	scale_reverse_iterator scale_rbegin() const { scale_reverse_iterator sc = DispIterator(GetTableType(),true); return sc; }
	scale_reverse_iterator scale_rend() const { scale_reverse_iterator sc = DispIterator(sindyTableType::unknown,true); return sc; }
	//@}
private:
	CSpatialIndex m_cAnnoSpatialIndex[4];	//!< 注記真位置用擬似空間インデックス
	CSpatialIndex m_cDispSpatialIndex[4];	//!< 注記表示位置用擬似空間インデックス
};

} // sindy

#endif // _ANNOTATIONS_H_
