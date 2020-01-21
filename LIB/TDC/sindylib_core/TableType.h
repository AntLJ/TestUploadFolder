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
 * @file TableType.h
 * @brief <b>テーブルタイプ定義ファイル</b>\n
 * このファイルでは、テーブルタイプをenumで定義しています。
 * 定義の際に、FeatureTableType.hを読み込んでマクロ経由で定義します。
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _TABLETYPE_H_
#define _TABLETYPE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace sindy {
	namespace sindyTableType {
		enum ECode {
#define FEATURETABLE_TYPE(name_space,table_name,class_name) name_space,
#include "FeatureTableType.h"
#undef FEATURETABLE_TYPE
		};
	} // sindyTableType
} // sindy

#endif // _TABLETYPE_H_
