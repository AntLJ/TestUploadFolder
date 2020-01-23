#pragma once

#include "Arguments.h"
#include "Logger.h"

class CheckBase
{
public:
	CheckBase();
	virtual ~CheckBase();
	virtual bool Init() = 0;
	virtual bool Check() = 0;

	/**
	 * @brief 実行時引数のセット
	 * @param args [in] 実行時引数管理クラス
	 */
	inline void SetArgument(Arguments& args) { m_args = args; }

protected:
	Arguments m_args; //!< 実行時引数
};

