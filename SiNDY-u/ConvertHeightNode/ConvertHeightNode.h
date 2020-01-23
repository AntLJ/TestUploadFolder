#pragma once

#include "Arguments.h"
#include "LogSys.h"


/**
 * @brief   ���C���N���X
 */
class CConvertHeightNode
{

public:

	/**
	 * @brief   �R���X�g���N�^
	 * @param   args [in]  ����
	 */
	CConvertHeightNode( const Arguments& args ) : m_args( args ){}

	/**
	 * @brief   �f�X�g���N�^
	 */
	~CConvertHeightNode(){};

	/**
	 * @brief   ���s
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool execute();

private:

	Arguments m_args;      //!< ����
};

