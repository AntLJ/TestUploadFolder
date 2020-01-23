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

#include "StdAfx.h"
#include "RowSet.h"

namespace adlib
{
	namespace row_set
	{
		// 読み込み
		void RowSet::load( _ICursor& crCursor )	// [Bug7560]のついでに、エラー処理をしなくて済むように引数をポインタから参照に変更
		{
			_IRowPtr ipRow;
			while (crCursor.NextRow(&ipRow) == S_OK)
			{
				m_OIDTable.insert(
					std::make_pair(ipRow->OID, ipRow)
				);
			}
		}

		_IRowPtr RowSet::searchByOID( long cOID ) const
		{
			_IRowPtr ipRow;
			OIDTable::const_iterator it = m_OIDTable.find(cOID);
			if (it != m_OIDTable.end())
			{
				ipRow = it->second;
			}
			return ipRow;
		}

		void RowSet::clear(void)
		{
			m_OIDTable.clear();
		}

	} // namespace row_set
} // namespace adlib
