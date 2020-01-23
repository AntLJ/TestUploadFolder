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
#include "StationGateFinder.h"
#include <utf8/usjis.h>

crd_cnv StationGateFinder::converter;

StationGateFinder::StationGateFinder(LPCTSTR filename) : initialized(false)
{
	initialized = init(filename);
}


StationGateFinder::~StationGateFinder(void)
{
}

bool StationGateFinder::init(LPCTSTR filename)
{
	std::ifstream file(filename);
	if (! file.is_open())
		return false;

	std::string line;
	while (std::getline(file, line)) {
		std::wstring rec = utf8tosjis(line);
		CStringTokenizer tokenizer(CString(rec.c_str()), _T("\t"), TOKEN_RET_EMPTY_ALL);
		int token_count = tokenizer.CountTokens();
		long id;
		WKSPoint p;
		for (int i = 0; i < token_count; i++) {
			CString token = tokenizer.GetNextToken();
			switch (i) {
			case 0: // シリアルID
				id = _ttoi(token);
				break;
			case 8: // 経度
				p.X = _tcstod(token, 0);
				break;
			case 9: // 緯度
				p.Y = _tcstod(token, 0);
				break;
			default:
				break;
			}
		}

		// 経緯度は世界測地系なので変換はしない
		cache.insert(Cache::value_type(id, p));
	}

	return true;
}

long StationGateFinder::find(long id, double x, double y, crd_cnv *crdcnv)
{
	Cache::iterator itr = cache.find(id);
	if (itr == cache.end())
		return -1;

	double d = crdcnv->GetDist_JGD2000(x, y, itr->second.X, itr->second.Y);
	if( d > SGP_RANGE )
		return d;

	return 1;
}
