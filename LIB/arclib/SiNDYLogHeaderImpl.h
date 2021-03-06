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

#ifndef __SINDYLOG_HEADER_IMPL_H__
#define __SINDYLOG_HEADER_IMPL_H__
#ifndef _LIB
	#ifdef __SINDYLOG_FUNCTION_EXECUTOR_H__
		// EXECUTOR_FUNCTION_IMPLEMENTの有効的実装
		#define EXECUTOR_FUNCTION_IMPLEMENT(cn) \
		SiNDYLogHeader::Function::ExecutorPtr SiNDYLogHeader:: ## cn ## Function::executor( \
			SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, std::set<int>* pUpdatedColSet) const \
		{ \
			return ExecutorPtr(new cn ## Executor(*this, rTableFinder, rLogRow, pUpdatedColSet)); \
		}
	#else
		// EXECUTOR_FUNCTION_IMPLEMENTの無効的実装
		#define EXECUTOR_FUNCTION_IMPLEMENT(cn) \
		SiNDYLogHeader::Function::ExecutorPtr SiNDYLogHeader:: ## cn ## Function::executor( \
			SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, std::set<int>* pUpdatedColSet) const \
		{ \
			throw std::logic_error("Fuctionのexecutorは無効です。"); \
		}
	#endif // __SINDYLOG_FUNCTION_EXECUTOR_H__

	EXECUTOR_FUNCTION_IMPLEMENT(Field)
	EXECUTOR_FUNCTION_IMPLEMENT(SubField)
	EXECUTOR_FUNCTION_IMPLEMENT(Point)
	EXECUTOR_FUNCTION_IMPLEMENT(Buffer)
	EXECUTOR_FUNCTION_IMPLEMENT(Length)
	EXECUTOR_FUNCTION_IMPLEMENT(Longitude)
	EXECUTOR_FUNCTION_IMPLEMENT(Latitude)
	EXECUTOR_FUNCTION_IMPLEMENT(BaseMesh)
	EXECUTOR_FUNCTION_IMPLEMENT(Meshcode)
	EXECUTOR_FUNCTION_IMPLEMENT(MeshX)
	EXECUTOR_FUNCTION_IMPLEMENT(MeshY)
	EXECUTOR_FUNCTION_IMPLEMENT(Distinct)
	EXECUTOR_FUNCTION_IMPLEMENT(Avg)
	EXECUTOR_FUNCTION_IMPLEMENT(Min)
	EXECUTOR_FUNCTION_IMPLEMENT(Max)
	EXECUTOR_FUNCTION_IMPLEMENT(Sum)
	EXECUTOR_FUNCTION_IMPLEMENT(Count)
	EXECUTOR_FUNCTION_IMPLEMENT(Belong)
	EXECUTOR_FUNCTION_IMPLEMENT(Root)
#endif // _LIB
#endif // __SINDYLOG_HEADER_IMPL_H__
