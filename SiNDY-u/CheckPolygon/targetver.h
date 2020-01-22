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

// 以下のマクロは、最低限必要なプラットフォームを定義します。最低限必要なプラットフォームとは、
// アプリケーションを実行するために必要な機能を備えた最も古いバージョンの Windows や Internet Explorer など
// をいいます。これらのマクロは、指定したバージョンと、それ以前のバージョンのプラットフォーム上で利用できるすべての機能を有効にすることによって 
// 動作します。

// 下で指定された定義の前に対象プラットフォームを指定しなければならない場合、以下の定義を変更してください。
// 異なるプラットフォームに対応する値に関する最新情報については、MSDN を参照してください。
#ifndef _WIN32_WINNT            // 最低限必要なプラットフォームが Windows Vista であることを指定します。
#define _WIN32_WINNT 0x0600     // これを Windows の他のバージョン向けに適切な値に変更してください。
#endif

