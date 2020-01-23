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
 * @brief INF,LQのテーブル名やタイプの一覧表
 *
 * テーブル間の変換や、リンク列のタイプとテーブル名に対応表等、ちょっとマクロを書くだけで
 * 簡単に処理に組み込むことができます。
 *
 * 以下の例は、linkqueue_typeからLQのテーブル名に置き換えるswitch文の一部として動作します。
 * @code
 *		switch( emCode )
 *		{
 * #define LINK_QUEUE(inf,lq,type) case type: lpcszName = lq; break;
 * #include "define_linkqueue.h"
 * #define LINK_QUEUE(inf,lq,type)
 * 		default: break;
 * 		}
 * @endcode
 **/
LINK_QUEUE( schema::inf_turnreg::kTableName,      schema::lq_turnreg::kTableName,      linkqueue_type::kLRTurn )
LINK_QUEUE( schema::inf_guide::kTableName,        schema::lq_guide::kTableName,        linkqueue_type::kGuide )
LINK_QUEUE( schema::inf_dirguide::kTableName,     schema::lq_dirguide::kTableName,     linkqueue_type::kDirguide )
LINK_QUEUE( schema::inf_vics::kTableName,         schema::lq_vics::kTableName,         linkqueue_type::kVICS )
LINK_QUEUE( schema::inf_lane::kTableName,         schema::lq_lane::kTableName,         linkqueue_type::kLane )
LINK_QUEUE( schema::inf_byway::kTableName,        schema::lq_byway::kTableName,        linkqueue_type::kByway )
LINK_QUEUE( schema::inf_route::kTableName,        schema::lq_route::kTableName,        linkqueue_type::kRoute )
LINK_QUEUE( schema::inf_uturn::kTableName,        schema::lq_uturn::kTableName,        linkqueue_type::kUTurn )
LINK_QUEUE( schema::inf_internavi::kTableName,    schema::lq_internavi::kTableName,    linkqueue_type::kInternavi )
LINK_QUEUE( schema::inf_ipcvics::kTableName,      schema::lq_ipcvics::kTableName,      linkqueue_type::kIPCVICS )
LINK_QUEUE( schema::inf_intersection::kTableName, schema::lq_intersection::kTableName, linkqueue_type::kIntersection )
