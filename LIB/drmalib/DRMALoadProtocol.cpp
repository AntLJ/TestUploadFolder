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

// DRMALoadProtocol.cpp: DRMALoadProtocol クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include <fstream>
#include <sstream>

#include "DRMADevPlant.h"
#include "DRMAOriginalValues.h"
#include "DRMARecord.h"
#include "DRMALoadProtocol.h"

using namespace drma;

struct DRMARecordFormat {
	_DRMADec eRecordId[2];
};

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DRMALoadProtocol::DRMALoadProtocol() :
mLoadComplete(false)
{
}

//////////////////////////////////////////////////////////////////////
// 開く
//////////////////////////////////////////////////////////////////////

/**
 * @brief 指定したディレクトリ内に存在する、指定したメッシュコードのDRMAファイルを開く
 *
 * @note 開くのに失敗した場合、標準エラー出力にエラーを出力する。
 *
 * @param cDirectoryName [in] ディレクトリ名
 * @param cMeshCode [in] メッシュコード
 * @param cSuffix [in] 拡張子
 * @retval true 成功
 * @retval false 失敗
 */
bool DRMALoadProtocol::load(const char* cDirectoryName, int cMeshCode, const char* cSuffix)
{
	std::stringstream aStream;

	aStream << cDirectoryName;

	// ディレクトリ区切り文字が付いていなかったら付ける
	int aLen = strlen(cDirectoryName);
	if(aLen && cDirectoryName[aLen-1] != dev_plant::_pathChr) {
		aStream << dev_plant::_pathChr;
	}

	// メッシュコード解析
	aStream
		<< int(cMeshCode / 100) //< １次メッシュコード
		<< dev_plant::_pathChr //< パス区切り文字
		<< cMeshCode; //< ２次メッシュコード

	if(cSuffix) {
		aStream << cSuffix; //< 拡張子
	}

	return load(aStream.str().c_str());
}

/**
 * @brief 指定した名称のDRMAファイルを開く
 *
 * @note 開くのに失敗した場合、標準エラー出力にエラーを出力する。
 *
 * @param cFileName [in] ファイル名
 * @retval true 成功
 * @retval false 失敗
 */
bool DRMALoadProtocol::load(const char* cFileName)
{
	std::ifstream aFileStream(cFileName, std::ios::binary);
	if(! aFileStream) {
		std::cerr << "ファイル\"" << cFileName << "\"を開けない" << std::endl;
		return false;
	}

	return load(aFileStream);
}

/**
 * @brief 指定したストリームからDRMAデータを読み込む
 *
 * @param cStream [in] DRMAデータストリーム
 * @retval true 成功
 * @retval false 失敗
 */
bool DRMALoadProtocol::load(std::istream& cStream)
{
	// 開いていたら閉じる
	close();

	char aRecord[256];

	for(int i = 1; ! (mLoadComplete || cStream.read(aRecord, 256).eof()); i++) {
		// レコードID取得用構造体に型変換
		const DRMARecordFormat* pFormat = reinterpret_cast<DRMARecordFormat*>(aRecord);

		// レコードID取得
		int aRecordId;
		VALID(DRMAItemConverter::setValueTo(pFormat->eRecordId, sizeof(pFormat->eRecordId), aRecordId));

		// レコードハンドリング
		if(! append(aRecordId, aRecord)) {
			std::cerr << "レコード #" << i << " ID:" << aRecordId << " のハンドリングに失敗" << std::endl;
			return false;
		}
	}

	setComplete();

	return true;
}

//////////////////////////////////////////////////////////////////////
// 閉じる
//////////////////////////////////////////////////////////////////////

/**
 * @brief 読み込んだDRMAデータを解放する
 */
void DRMALoadProtocol::close()
{
	mLoadComplete = false;
	clear();
}
