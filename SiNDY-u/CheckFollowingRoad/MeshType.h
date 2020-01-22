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

namespace meshtype
{

/**
 * @brief	メッシュリストに格納されているメッシュコードの種類を識別
 */
enum MeshType
{
	kUnknown, //!< 不明(初期状態)
	kBasemesh, //!< 二次メッシュ(6ケタ)
	kCitymesh, //!< 都市地図メッシュ(8ケタ)
	kInvalid //!< メッシュリストに不正文字が混在
};

/**
* @brief 処理メッシュ単位に応じて特定の文字列を返す
* @param type[in] 処理メッシュ単位の識別コード
*/
inline uh::tstring getStrMeshType( const MeshType& type )
{
	uh::tstring strMeshType;

	switch( type )
	{
	case kBasemesh: strMeshType = _T("BASEMESH"); break;
	case kCitymesh:	strMeshType = _T("Citymesh"); break;
	case kInvalid: strMeshType = _T("不正なメッシュ単位"); break;
	case kUnknown:
	default:
		assert(false);
		break;
	}

	return strMeshType;
}

} //meshtype
