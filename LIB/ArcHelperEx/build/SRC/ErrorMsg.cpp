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

#include "StdAfx.h"
#include "ErrorMsg.h"

namespace libErrMsg
{

#ifndef SINDY_FOR_ENGLISH
LPCTSTR ERROR_ROAD_LINK_IS_FOLDED_MESH_BOUNDARY     = _T("%d - %d 同じメッシュ内で道路リンクが折り返しています。");
LPCTSTR GETUNION_LAYER_OID                          = _T("CTargetItems::GetUnion() : レイヤ=%s OID=%d");
LPCTSTR ERROR_FROM_OR_TO_MISSING                    = _T("FROM、TOどちらかが入っていません。");
LPCTSTR CONFIRM_CHECK_POLYGON_WITH_OVER_10000_POINT = _T("レイヤ：%s OID=%d : 構成点数が10000以上（%d）のポリゴンに対してチェックを行うと時間がかかります。チェックしますか？");
LPCTSTR GEOMETRY_CHECK                              = _T("ジオメトリチェック");
LPCTSTR ERROR_DUPLICATE_OF_OID                      = _T("OID=%dのポリゴンと重複しています。");
LPCTSTR ERROR_LINK_MUST_HAVE_FROM_OR_TO_ID          = _T("ネットワークリンクにFROM又はTOのIDがついていません。");
LPCTSTR ERROR_LINK_LOOPES                           = _T("%d - %d 道路リンクがループしています。");
LPCTSTR ERROR_SEGMENT_OVERLAP                       = _T("%d - %d セグメントがオーバーラップしています。");
LPCTSTR ERROR_SAME_TYPE_SEGMENT_OVERLAP             = _T("%d - %d 同種別でセグメントがオーバーラップしています。");
LPCTSTR ERROR_ANGLE_IS_135_DEGREE_OR_LESS           = _T("%d - %d メッシュ線をはさんだ接続角が135°以下です（%d°）。");
LPCTSTR ERROR_POINTS_ARE_TOUCHING                   = _T("%d - %d 構成点どうしが接触しています");
LPCTSTR ERROR_ANGLE_IS_20_DEGREE_OR_LESS            = _T("%d - %d 2叉路の接続角が20°以下です（%d°）。");
LPCTSTR ERROR_SEGMENT_CONNECT_SAME_NODE_IS_CLOSE    = _T("%ld - %ld 同一ノードに接続する線分が近接しています（正規化座標距離 %.1lf）");
#else
LPCTSTR ERROR_ROAD_LINK_IS_FOLDED_MESH_BOUNDARY     = _T("%d - %d　Road link is folded at the mesh boundary within a same mesh.");
LPCTSTR GETUNION_LAYER_OID                          = _T("CTargetItems::GetUnion() : Layer=%s OID=%d");
LPCTSTR ERROR_FROM_OR_TO_MISSING                    = _T("Either FROM or TO is missing");
LPCTSTR CONFIRM_CHECK_POLYGON_WITH_OVER_10000_POINT = _T("Layer: %s OID=%d: A long time is required to check a polygon with 10,000 or more (%d) composition points. Do you want to perform the check?");
LPCTSTR GEOMETRY_CHECK                              = _T("Geometry check");
LPCTSTR ERROR_DUPLICATE_OF_OID                      = _T("Duplicate of OID=%d polygon.");
LPCTSTR ERROR_LINK_MUST_HAVE_FROM_OR_TO_ID          = _T("Link must has FROM or TO ID.");
LPCTSTR ERROR_LINK_LOOPES                           = _T("%d - %d Link Loopes.");
LPCTSTR ERROR_SEGMENT_OVERLAP                       = _T("%d - %d  Segments overlap.");
LPCTSTR ERROR_SAME_TYPE_SEGMENT_OVERLAP             = _T("%d - %d  Same type and segments overlap.");
LPCTSTR ERROR_ANGLE_IS_135_DEGREE_OR_LESS           = _T("%d - %d Connection angle across the mesh boundary is 135 degree or less.");
LPCTSTR ERROR_POINTS_ARE_TOUCHING                   = _T("%d - %d  A pair of composition points are touching.");
LPCTSTR ERROR_ANGLE_IS_20_DEGREE_OR_LESS            = _T("%d - %d  The angle of the fork in a road is 20 degrees or less (%d degrees).");
LPCTSTR ERROR_SEGMENT_CONNECT_SAME_NODE_IS_CLOSE    = _T("%d - %d Line segments connecting to a same node is close to each other  (Normarized coordinates distance:  %.1lf)");
#endif // SINDY_FOR_ENGLISH

} // namespace libErrMsg
