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

#include "SINDY2INHWYINFO.h"
#include "Customer.h"

/** 
 * @brief I/Nの施設種別コードの取得
 * @note	与えられたオブジェクトが属する施設のFacil_Info_PointからSiNDYの施設種別コードを取得し、対応するI/Nの施設種別コードを返す
 * @param[in]	cpRow	対象オブジェクト（Road_CodeとRoad_Seqを持っていることが前提）
 * @param[in]	cpHwyInfo	Facil_Info_Point取得のためのSINDY2INHWYINFO
 * @return	I/Nの施設種別コード
 */
long GetFacilClass(_IRow* cpRow, SINDY2INHWYINFO* cpHwyInfo);

/**
 * @brief 施設名称変換が必要な仕向け先か否かを判定する
 * @param[in]	cCustomer	対象仕向け先
 * @return	判定結果
 */
bool needFacilNameChange( Customer cCustomer );
