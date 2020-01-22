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
 * @file ErrorCode.h
 * @brief エラーコード定義ファイル
 * ErrorLog.[cpp|h]でマクロ展開される。
 * code   : enumで展開するエラーコード名
 * value  : エラーコード
 * level  : ErrorDef.h にある emErrLevel
 * msg_jp : エラーメッセージ(日本語)
 */

ERROR_CODE(
kAC_AddrPolygonCheckOverLap,
0x01004001,
kError,
住所ポリゴンが市区町村ポリゴンと重なっていない.)

ERROR_CODE(
kAC_AddrPolygonCheckMissing,
0x01004002,
kError,
住所ポリゴンに抜けがある.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError1,
0x01002001,
kError,
町丁目界未整備で11桁住所コードが「0」ではない.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError2,
0x01002002,
kError,
町丁目界未整備以外で11桁住所コードが「0」.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError3,
0x01002003,
kError,
町丁目界未整備なのに街区符号が付いている.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError4,
0x01002004,
kError,
街区符号・拡張名称（公称）の両方が入っている.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError5,
0x01002005,
kError,
街区符号が０.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError6,
0x01002006,
kError,
街区符号が３桁以上.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError7,
0x01002007,
kError,
住居表示・地番未整備なのに街区符号が入っている.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError8,
0x01002008,
kError,
住居表示完了済/地番整備完了済なのに街区符号がない.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError9,
0x01002009,
kError,
地番整備完了済なのに拡張名称がある.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError10,
0x01002010,
kError,
拡張名称１、2の名称が異なる.)

ERROR_CODE(
kAC_AddrPolygonSameAddrCode,
0x01002011,
kError,
住所コード(公称)と住所コード(通称)が同じ.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError13,
0x01002012,
kError,
住居表示整備済/地番整備済なのに通称関連フィールドに値が格納されている.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError11,
0x01001002,
kError,
住所整備種別コードに不正な値が入っている.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError12,
0x01003001,
kError,
住所整備種別コードが異なるポリゴンが存在する.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeGeoError1,
0x01003002,
kError,
飛び地がある.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeGeoError2,
0x01003003,
kError,
同一属性のポリゴンが隣接している.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrCodeError1,
0x01004004,
kError,
住所マスタに存在しない住所コード（公称）.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrCodeError2,
0x01004005,
kError,
住所マスタに存在しない住所コード（通称）.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrCodeError3,
0x01004006,
kError,
廃止になった住所コード(公称).)

ERROR_CODE(
kAC_AddrPolygonCheckAddrCodeError4,
0x01004007,
kError,
廃止になった住所コード(通称).)

ERROR_CODE(
kAC_AddrPolygonNoPoint,
0x01004009,
kWarn,
同一住所コードをもつ地名代表ポイントが存在しない.)

ERROR_CODE(
kAC_AddrPolygonExtNameExistMaster,
0x01004011,
kError,
拡張街区符号文字が11桁レベルで存在.)

ERROR_CODE(
kAC_AddrPolygonNoPolygonInMesh,
0x01004003,
kError,
メッシュ内に行政界ポリゴンがない.)

ERROR_CODE(
kAC_AddrPolygonExistProhibitChar,
0x01001001,
kError,
拡張名称に禁則文字.)

ERROR_CODE(
kAC_AddrPolygonAddrCode2Villa,
0x01001003,
kError,
別荘地の住所コードが通称に設定されている.)

// city_polygon error message
ERROR_CODE(
kAC_CityPolygonSameCityAdjoin,
0x02003001,
kError,
同一属性のポリゴンが隣接している.)

ERROR_CODE(
kAC_CityPolygonAbolishedCode,
0x02004001,
kError,
廃止になったコード.)

ERROR_CODE(
kAC_CityPolygonCityNotInMaster,
0x02004002,
kError,
市区町村コードが住所マスタにない.)

ERROR_CODE(
kAC_CityPolygonCityMissing,
0x02004003,
kError,
市区町村ポリゴンに抜けがある.)

ERROR_CODE(
kAC_CityPolygonNoPolygonInMesh,
0x02004004,
kError,
メッシュ内に市区町村ポリゴンがない.)
