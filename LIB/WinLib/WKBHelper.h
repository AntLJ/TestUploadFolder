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
 * @brief ArcGIS 10.1で作成したWKBをArcGIS 9.3.1と同じWKBに変換する
 *
 * @param bytes [in,out] WKB（IWkb::ExportToWkbで出力されたもの）
 *
 * @note Below is the C++ code that will reverse the new binary data and make it consistent with 9.3.1.  
 * It should be called right after the calling of ExportToWkb. 
 */
inline void reverse_wkb_bytes_for_polygons(char* bytes)
{
	if (!bytes)
		return;

	int type = *((int*)(bytes + 1));

	if (type != 6 && type != 3)
		return; // not a Polygon type

	int offset;
	int polygonCount;

	if (type == 6)
	{
		polygonCount = *((int*)(bytes + 5));
		offset = 9;
	}
	else
	{
		polygonCount = 1;
		offset = 0;
	}

	int ipolygonend = 0;

	double coord[2];

	// read Coordinates
	for (int ipolygon = 0; ipolygon < polygonCount; ipolygon++)
	{
		offset += 5; // skip redundant byte order and type fields 
		int ipartcount = *((int*)(bytes + offset)); offset += 4;
		int ipolygonstart = ipolygonend;
		ipolygonend = ipolygonstart + ipartcount;

		for (int ipart = ipolygonstart; ipart < ipolygonend; ipart++)
		{
			int ipointcount = *((int*)(bytes + offset)); offset += 4;

			if (ipointcount == 0)
				continue;

			int tempOffset = offset;
			tempOffset += ipointcount * 2 * 8;

			char* left = bytes + offset;
			char* right = bytes + tempOffset - 16;

			while (left < right)
			{
				memcpy(coord, left, 16);
				memcpy(left, right, 16);
				memcpy(right, coord, 16);

				left += 16;
				right -= 16;
			}

			offset = tempOffset;
		}
	}
}

/**
 * @brief access violationを別の例外に置き換えるためのフィルタ
 * @param code	例外コード
 * @param ep	例外ポインタ
 * @retval 1	EXCEPTION_EXECUTE_HANDLER
 * @retval 0	EXCEPTION_CONTINUE_SEARCH
 */
inline int exception_filter(unsigned int code, struct _EXCEPTION_POINTERS *ep){

   if (code == EXCEPTION_ACCESS_VIOLATION)
      return EXCEPTION_EXECUTE_HANDLER;

   else
      return EXCEPTION_CONTINUE_SEARCH;
}
