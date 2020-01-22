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

#include <memory>
#include <cassert>
#include "data_aux.h"
#include "data_checker.h"
#include "ProgressCallback.h"
#include "adam2_format_checker.h"
#include "adam2_filelist.h"


namespace sindy  {
namespace adam2  {
namespace format {

/**
 * @brief  論理チェック向けのベースクラス
 */
class CAdam2LoglcalChecker : public CLogicalChecker 
{
public:
	static std::vector<std::vector<Feature> >* sALL_FEATURE;
	static std::vector<std::vector<Row> >    * sALL_ROW;

	static IProgressCallback*                  sCALLBACK;

	virtual bool finalize  () { return true; }
	virtual bool initialize(const std::initializer_list<long>& target_indexes) { indexes = target_indexes; return true; }

protected:
	std::vector<long> indexes;
};


/**
 * @brief  単一フィーチャクラス論理チェック向けベースクラス
 */
class CSimpleLogicalChecker : public CAdam2LoglcalChecker
{
public:
	virtual ~CSimpleLogicalChecker() {}
	CSimpleLogicalChecker() {}

	virtual bool run() {
		assert(sCALLBACK && sALL_FEATURE && indexes.size() == 1);
		if (! (sCALLBACK && sALL_FEATURE && indexes.size() == 1)) { return false; }

		const auto& kInfo = kFeatureFileList[indexes[0]];
		const auto& kData = (*sALL_FEATURE) [indexes[0]];

		sCALLBACK->callback(_T("[LOGICAL][TRACE][%s]論理チェック"), kInfo.alias_name);

		// sub()側では 変数の有効判断は行わない!!
		bool f = sub(kInfo, kData);

		return true;
	}

protected:
	virtual bool sub(const ImportFileList& kInfo, const std::vector<Feature>& kData) = 0;

	virtual bool nomentenance(const Feature& feature, const std::initializer_list<long>& indexes) {
		for (const auto& index : indexes)
		{
			if (! feature.attr[index].empty()) { return false; }
		}
		return true;
	};
};
typedef std::shared_ptr<CAdam2LoglcalChecker> SPCAdam2LoglcalChecker;
}
}
}
