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
#include "checker.h"
#include "CheckLog.h"

#define ADD_GEOMETRY_CHECK                                                                  \
  assert(frmt.empty() && attr.empty());                                                     \
  frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));        \
  frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));        \
  frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 8, 3, false)));        \
  frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));        \
  attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));     \
  attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));     \
  attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-DBL_MAX, DBL_MAX)));  \
  attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX))); \
  assert(frmt.size() == attr.size());
#define ADD_NODE_CHECK                                                                      \
  frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));        \
  frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));        \
  attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX))); \
  attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX))); \
  assert(frmt.size() == attr.size());
#define ADD_RELID_CHECK                                                                     \
  frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));        \
  attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX))); \
  assert(frmt.size() == attr.size());
#define ADD_FLAG_CHECK                                                                      \
  frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 1, 0,  true)));        \
  attr.push_back(std::shared_ptr<IChecker>(new CFlagChecker()));                            \
  assert(frmt.size() == attr.size());


class CLogicalChecker {
public:
	virtual ~CLogicalChecker() {}
	CLogicalChecker() {}

	virtual bool run() = 0;
};

class CDataChecker {
public:
	virtual ~CDataChecker() {}
	CDataChecker() : has_pos(false) {}

	virtual void setHeader(const std::vector<std::string>& h) const { header = h; assert(frmt.size() == header.size()); }

	virtual bool isSafe(const std::vector<std::string>& arr, long oid) const {
		assert(arr.size() == 3 || arr.size() == frmt.size());
		assert(arr.size() == 3 || arr.size() == attr.size());
		if (! ((has_pos && arr.size() == 3) || arr.size() == frmt.size()))
		{
			CheckLog::GetInstance()->printError(LOGCODE_ERROR_COLUMNS, LOGMSG_ERROR_COLUMNS, chk_name, oid, "", std::to_string(arr.size()) + "列");
			return false;   // 列が異なる場合 アクセスバイオレーションが発生するため return する
		}

		bool err = false;
		for (auto i=0 ; i<arr.size() ; i++)
		{
			if (! frmt[i]->isSafe(arr[i]))
			{
				CheckLog::GetInstance()->printError(LOGCODE_ERROR_FORMAT, LOGMSG_ERROR_FORMAT, chk_name, oid, header[i], arr[i]);
				err = true;
			}
			if (! arr[i].empty())
			{
				if (! attr[i]->isSafe(arr[i]))
				{
					CheckLog::GetInstance()->printError(LOGCODE_ERROR_INVALID, LOGMSG_ERROR_INVALID, chk_name, oid, header[i], arr[i]);
					err = true;
				}
			}
		}
		return (! err);
	}

	virtual bool hasDeleteCode(const std::vector<std::string>& arr) const {
		assert(arr.size() == frmt.size());

		static const int DELETE_CODE = 2;

		if (arr.size() != frmt.size())
			return true;

		// 末尾が更新コードの前提
		std::string update_code = arr.back();
		return update_code == std::to_string(DELETE_CODE);
	}


public:
	const char* name() const { return chk_name.c_str(); }
	const std::string fielddef() const { return concat(header); }

	bool isFeature() const { return has_pos; }

	//const IChecker* getFormat(unsigned int index) const { if (index < frmt.size()) { return frmt[index].get(); } return nullptr; }
	//const IChecker* getAttrib(unsigned int index) const { if (index < attr.size()) { return attr[index].get(); } return nullptr; }

protected:
	virtual void init() = 0;

protected:
	std::vector<std::shared_ptr<IChecker> > frmt;
	std::vector<std::shared_ptr<IChecker> > attr;
	std::string chk_name;
	bool        has_pos;

	// Note: ログ出力用に登録!!!
	// ※改修を最小にとどめるため mutableを利用
	mutable std::vector<std::string>   header;
};

class CVariableChecker : public CDataChecker {
public:
	virtual ~CVariableChecker() {}
	CVariableChecker() {}

	bool load(const char* fname) {
		std::ifstream ifs(fname);
		if (! ifs) { return false; }

		init();
		return true;
	}

protected:
	void init() {
		if (! is) { assert(false); return; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//                                                                                                //
		// ◆File Format◆                                                                                //
		//   □header                                                                                     //
		//     # から始まる行で {KEY} = {VALUE} で定義                                                    //
		//                                                                                                //
		//     isGeometry   boolean     チェック対象が形状を含むか                                        //
		//     field{N}                                                                                   //
		//                                                                                                //
		//                                                                                                //
		//                                                                                                //
		//                                                                                                //
		//                                                                                                //
		//                                                                                                //
		//                                                                                                //
		//                                                                                                //
		//                                                                                                //
		//                                                                                                //
		//                                                                                                //
		////////////////////////////////////////////////////////////////////////////////////////////////////
	}

private:
	std::ifstream is;
};
