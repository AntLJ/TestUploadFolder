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

#include "Processor.h"
#include <atltime.h>

/**
 * @brief ループ処理実施クラス
 * @note  将来的には本クラスを継承したクラスを実装し
 *        マルチスレッドで動かす事を想定している。
 */
template <typename T>
class ListProcessor
{
public:
    ListProcessor<T>(const std::vector<T>& cProcessList, std::shared_ptr<Processor<T>> cProcessor);
	~ListProcessor<T>();

	/// ループ処理実施
	bool doLoopProcessing();

	/// ループ前処理実施
	bool doPreprocessing();

	///ループ後処理実施
	bool doPostprocessing();

	/// 一括処理実施
	bool doBulkProcessing();

protected:
	/// 処理クラス
	/// ループの元となるリスト
	std::vector<T> m_processList;
	std::shared_ptr<Processor<T>> m_processor;
};

template <typename T>
ListProcessor<T>::ListProcessor(const std::vector<T>& cProcessList,std::shared_ptr<Processor<T>> cProcessor)
{
	m_processList = cProcessList;
	m_processor = cProcessor;
}

template <typename T>
ListProcessor<T>::~ListProcessor()
{
}

template <typename T>
bool ListProcessor<T>::doLoopProcessing()
{

#ifdef _DEBUG
	CFileTime cTimeStart, cTimeEnd;
    CFileTimeSpan cTimeSpan;
    cTimeStart = CFileTime::GetCurrentTime(); // 現在時刻
#endif
	for(const T& src : m_processList)
	{
		if (! m_processor->proc(src) )
		{
			return false;
		}
	}
#ifdef _DEBUG
	cTimeEnd = CFileTime::GetCurrentTime(); // 現在時刻
    cTimeSpan = cTimeEnd - cTimeStart;
    std::cout<< "処理時間:" << cTimeSpan.GetTimeSpan()/10000 << "[ms]" << std::endl;
#endif
	return true;

}

template <typename T>
bool ListProcessor<T>::doPreprocessing()
{
	return m_processor->preprocess();
}

template <typename T>
bool ListProcessor<T>::doPostprocessing()
{
	return m_processor->postprocess();
}

template <typename T>
bool ListProcessor<T>::doBulkProcessing()
{
	return m_processor->bulkproc(m_processList);
}
