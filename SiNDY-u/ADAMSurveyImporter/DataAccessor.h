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

#include <crd_cnv/coord_converter.h>


namespace adam_v2
{
/**
 * データアクセス基底クラス
 */
class DataAccessor
{
public:
	DataAccessor(void) :
		_path(_T(""))
	{}
	virtual ~DataAccessor(void){}

	/**
	 * データクリア
	 */
	virtual void Clear() = 0;
	/**
	 * データ読み込み
	 * @param[in]	fileName	CSVファイル名
	 * @retval	true:	成功
	 * @retval	false:	失敗
	 */
	virtual bool Read(const uh::tstring& fileName) = 0;
	/**
	 * データ書き込み
	 * @param[in]	tableName	ADAMテーブル名
	 * @retval	true:	成功
	 * @retval	false:	失敗
	 */
	virtual bool Write(const uh::tstring& tableName) = 0;

	/**
	 * パラメータ設定
	 *
	 * @param param	[in] 入力DB名
	 */
	void SetParam(const uh::tstring& param)
	{
		_path = param;
	}

	/**
	 * @brief 座標変換用パラメータファイルの読み込み
	 *
	 * @param fileName [in] ファイル名
	 * @retval true 読み込み成功
	 * @retval false 読み込み失敗
	 */
	bool ReadCrdParam(const uh::tstring& fileName);

	/**
	 * バージョン利用有無設定
	 *
	 * @param[in]	flg	true:利用しない
	 */
	void SetUnuseVersion(bool flg)
	{
		_unuse_version = flg;
	}

	/**
	 * バージョン利用有無取得
	 *
	 * @return	true:利用しない
	 */
	bool GetUnuseVersion()
	{
		return _unuse_version;
	}


protected:
	uh::tstring _path;				///< パス
	crd_cnv		_cnv;				//!< 座標変換パラメータ
	bool		_unuse_version;		//!< バージョン利用しない
};

typedef std::shared_ptr<DataAccessor> DataAccessorPtr;

};	//namespace adam_v2