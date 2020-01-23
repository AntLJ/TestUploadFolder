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
	 * @brief ���s�������̃Z�b�g
	 * @param args [in] ���s�������Ǘ��N���X
	 */
	inline void SetArgument(Arguments& args) { m_args = args; }

protected:
	Arguments m_args; //!< ���s������
};

