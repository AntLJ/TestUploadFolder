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
 * @brief ���[�v�������{�N���X
 * @note  �����I�ɂ͖{�N���X���p�������N���X��������
 *        �}���`�X���b�h�œ���������z�肵�Ă���B
 */
template <typename T>
class ListProcessor
{
public:
    ListProcessor<T>(const std::vector<T>& cProcessList, std::shared_ptr<Processor<T>> cProcessor);
	~ListProcessor<T>();

	/// ���[�v�������{
	bool doLoopProcessing();

	/// ���[�v�O�������{
	bool doPreprocessing();

	///���[�v�㏈�����{
	bool doPostprocessing();

	/// �ꊇ�������{
	bool doBulkProcessing();

protected:
	/// �����N���X
	/// ���[�v�̌��ƂȂ郊�X�g
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
    cTimeStart = CFileTime::GetCurrentTime(); // ���ݎ���
#endif
	for(const T& src : m_processList)
	{
		if (! m_processor->proc(src) )
		{
			return false;
		}
	}
#ifdef _DEBUG
	cTimeEnd = CFileTime::GetCurrentTime(); // ���ݎ���
    cTimeSpan = cTimeEnd - cTimeStart;
    std::cout<< "��������:" << cTimeSpan.GetTimeSpan()/10000 << "[ms]" << std::endl;
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
