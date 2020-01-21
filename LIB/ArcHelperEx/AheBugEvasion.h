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

#ifndef ARCHELPEREX_AHEBUGEVASION_H__
#define ARCHELPEREX_AHEBUGEVASION_H__

/**
 * @brief IWkb::ExportToWkbの回避関数
 * 
 * 内部的におかしくなっている形状でExportToWkbを呼ぶと
 * 落ちるため、構造化例外を使用して回避を行っています。
 * IWkb::ExportToWkbは直接使用せずにこの関数を
 * 通して使用することでtry...catchをすることなく安全に
 * 呼ぶことができます。
 */
HRESULT IWkb_ExportToWkb( IWkb* ipWkb, long* pBytes, BYTE* pBuf );

/**
 * @brief IDataset::get_Workspaceの回避関数
 *
 * ソースが不明なオブジェクト（通称：赤びっくり）に対してget_Workspaceを呼ぶと
 * 落ちるため、構造化例外を使用して回避しています。
 * IDataset::get_Workspaceを直接使用せず、この関数を
 * 通して使用することでtry...catchをすることなく安全に
 * 呼ぶことができます。
 */
HRESULT IDataset_getWorkspace( IDataset* ipDataset, IWorkspace** ppWork );

/**
 * @brief Arc10.1でのITopologicalOperator::Intersectのバグを回避する関数
 *
 * @note 水平・垂直のラインの差を取ると、ITopologicalOperator::IntersectでE_FAILが
 *       返ってしまうため、S_OKでない時に形状が同じかどうかを調べて、同じならBaseと同じ
 *       形状を返すことにする。もし、同じでない場合にはとりあえずNULLを返す。
 *       使う側でNULLをハンドリングすること。もし、同じでないケースがでてきた場合には
 *       ちゃんとIntersect形状を作成することをしなければならない。
 *       SiNDY-Qでのみ使用
 *
 * @param ipBase  [in] 比較元形状
 * @param ipOther [in] Intersectを取る形状
 *
 * @return IGeometryPtr  ipBaseとipOtherの形状の重なり部分
 */
IGeometryPtr ITopologicalOperator_Intersect( IGeometryPtr ipBase, IGeometryPtr ipOther );

/**
 * @brief Arc10.1でのITopologicalOperator::Differenceのバグを回避する関数
 *
 * @note 水平・垂直のラインの差を取ると、ITopologicalOperator::DifferenceでE_FAILが
 *       返ってしまうため、S_OKでない時に形状が同じかどうかを調べて、同じなら空形状を
 *       返すことにする。もし、同じでない場合にはとりあえずNULLを返す。
 *       使う側でNULLをハンドリングすること。もし、同じでないケースがでてきた場合には
 *       ちゃんと差分形状を作成することをしなければならない。
 *       SiNDY-Qでのみ使用
 *
 * @param ipBase  [in] 比較元形状
 * @param ipOther [in] 差分を取る形状
 *
 * @return IGeometryPtr  ipBaseとipOtherの形状の差
 */
IGeometryPtr ITopologicalOperator_Difference( IGeometryPtr ipBase, IGeometryPtr ipOther );

#endif // ifndef ARCHELPEREX_AHEBUGEVASION_H__
