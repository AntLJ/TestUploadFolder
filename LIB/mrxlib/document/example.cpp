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

#include "stdafx.h"
#include <exception>
#include "MrxLoad.h"

int main(int argc, char* argv[])
{
 	// TODO: この位置にコードを記述してください。
	try {
		//MRXパラメタの指定
		string	aDir = "E:\\Mapple渋滞抜け道データsample\\MRX\\5339363.mrx";

		//MRXの読み込み
		CMrxLoad	aMRXFile;
		char	aMrxFile[256]; strcpy(aMrxFile,aDir.c_str());
		if( aMRXFile.load(aMrxFile)){

			//MRXデータクラスの取得
			MRX_Alldata	aData = aMRXFile.getMRXdata();

			//MRX情報のディバグ
			aMRXFile.print();

			//MRXファイルのクローズ	::close()なしでも開放可能
//			aMRXFile.close();
		}
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}
	return 0;
}
