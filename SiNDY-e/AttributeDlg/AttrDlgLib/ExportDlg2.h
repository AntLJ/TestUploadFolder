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

//#include "ExportDlg.h"
class CExportDlg;

/**
 * @brief 拡張版の属性ダイアログインターフェース
 * @note  属性ダイアログ側で関連テーブル等の更新をせざる得ない場合は<br>
 *        属性ダイアログクラスでこちらを継承する
 */
class CExportDlg2 : public CExportDlg
{
public:
	virtual bool Update() = 0;
};

