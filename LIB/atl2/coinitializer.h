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

#ifndef ATL2_COINITIALIZER_H_
#define ATL2_COINITIALIZER_H_

namespace atl2 {

/**
 * @brief CoInitializeラッピングクラス
 */
class coinitializer
{
public:
	coinitializer() :
	m_hResult(::CoInitialize(NULL))
	{
	}

	virtual ~coinitializer()
	{
		if(m_hResult == S_OK) {
			::CoUninitialize();
		}
	}

private:
	HRESULT m_hResult;
};

} // namespace atl2

#endif // ATL2_COINITIALIZER_H_
