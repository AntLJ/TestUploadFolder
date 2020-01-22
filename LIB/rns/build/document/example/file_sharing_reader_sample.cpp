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

void check_mesh(long nMeshCode);

/*
 * @brief 全てのメッシュをチェックする。
 *
 * @param lpszMeshListFilePath [in] メッシュリストファイルパス。
 * @param lpszSharingInfoPath [in,optional] 共用情報ファイルパス。省略した場合は共用しない。
 */
void check_all(const char* lpszMeshListFilePath, const char* lpszSharingInfoPath = 0)
{
	using namespace rns;

	// 実際のコーディングでは boost::scoped_ptr を使う。
	file_reader* pFileReader = 0;

	try {
		if(lpszSharingInfoPath) {
			// 共用情報ファイルが指定されたときは共用読み込み。
			pFileReader = new file_sharing_reader(lpszMeshListFilePath, lpszSharingInfoPath);
		}
		else {
			// 共用情報ファイルが指定されなかったときは通常読み込み。
			pFileReader = new simple_file_reader(lpszMeshListFilePath);
		}

		// ファイルからメッシュコードを取り出してチェック。
		for(long nMeshCode; *pFileReader->lock() >> nMeshCode; ) {
			check_mesh(nMeshCode);
		}

		delete pFileReader;
	}
	catch(...) {
		delete pFileReader;
		throw;
	}
}
