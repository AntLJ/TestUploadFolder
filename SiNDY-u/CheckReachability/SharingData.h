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

#include <map>
#include <memory>
#include <arctl/field/value.h>
#include <sindy/schema.h>
#include <sindy/link_queue/lq.h>
#include <sindy/link_queue/lq_list.h>
#include <sindy/link_queue/table.h>

typedef sindy::lq_list<sindy::lq_t> LinkQueue; ///< リンク列規制
typedef std::map<arctl::field::value_t<sindy::schema::link_queue::kInfID, long>, LinkQueue> LQMap; ///< リンク列規制一覧

/**
 * @brief 全てのリージョンで共有される情報。
 */
struct SharingData
{
	std::auto_ptr<LQMap> m_pLQMap; ///< 全国に存在する無条件右左折禁止一覧。
};

std::auto_ptr<SharingData> create_sharing_data(const sindy::link_queue_table& rTurnregTable);
