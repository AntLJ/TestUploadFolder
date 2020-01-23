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
#include <ppl.h>
#include <concurrent_vector.h>
#include "ListProcessor.h"
#include "Processor.h"
#include <atltime.h>

using namespace concurrency;

/**
 * @brief ループ処理実施クラス(PPL)
 */
template <typename T>
class PPLProcessor : public ListProcessor<T>
{
public:
	PPLProcessor(std::vector<T> cProcessList, Processor<T>& cProcessor)
	: ListProcessor<T>(cProcessList, cProcessor)
	{
	};

	~PPLProcessor(){};
	virtual bool doLoopProcessing();

private:

};

template <typename T>
bool PPLProcessor<T>::doLoopProcessing()
{
	std::cout << _T("PPLProcessor") << std::endl;

	CFileTime cTimeStart, cTimeEnd;
    CFileTimeSpan cTimeSpan;
    cTimeStart = CFileTime::GetCurrentTime();           // 現在時刻

	parallel_for_each(m_processList.begin(),
					  m_processList.end() ,
					  [&](T src){ m_processor->proc(src); });

	cTimeEnd = CFileTime::GetCurrentTime();           // 現在時刻
    cTimeSpan = cTimeEnd - cTimeStart;
    std::cout<< _T("処理時間:") << cTimeSpan.GetTimeSpan()/10000 << _T("[ms]") << std::endl;

	return true;
}
