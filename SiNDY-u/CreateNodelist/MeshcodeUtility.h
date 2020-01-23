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


/**
 * @brief メッシュ関連のユーティリティ関数群
 * @note  旧ツール(ReleaseDataCreate)のソースを流用(ほぼコピペ)
 */
class CMeshcodeUtility
{
public:
	CMeshcodeUtility(void){}
	~CMeshcodeUtility(void){}

	/**
	 * @brief   実数緯度経度から2次メッシュコードを取得する(境界線上)
	 * @param[in]  latlon    実数緯度経度
	 * @return     2次メッシュコード群
	 */
	static std::vector<int> GetBelong2ndMeshcode(const WKSPoint &latlon);

	/**
	 * @brief   2次メッシュコードから実数緯度経度を取得する
	 * @param[in]  meshcode メッシュコード
	 * @return     実数緯度経度
	 */
	static WKSPoint MeshToLL(const int &meshcode);

	/**
	 * @brief   数緯度経度から2次メッシュコードを取得する
	 * @param[in]  meshcode メッシュコード
	 * @return     実数緯度経度
	 */
	static int Get2ndMeshcode(const WKSPoint &latlon);

};

