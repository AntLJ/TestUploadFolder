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
 * @file modify_date_stamper.h
 * @brief 更新日時刻印クラスの定義。
 */
#ifndef SINDY_MODIFY_DATE_STAMPER_H_
#define SINDY_MODIFY_DATE_STAMPER_H_

#include <arclib/sindy/stamper/time_stamper.h>
#include <sindy/schema/common.h>

namespace sindy {

/// 作業者による更新日時を刻印するクラス。
typedef time_stamper_t<schema::ipc_table::kModifyDate> operator_modify_date_stamper;

/// プログラムによる更新日時を刻印するクラス。
typedef time_stamper_t<schema::ipc_table::kProgModifyDate> prog_modify_date_stamper;

} // namespace sindy

#endif // SINDY_MODIFY_DATE_STAMPER_H_
