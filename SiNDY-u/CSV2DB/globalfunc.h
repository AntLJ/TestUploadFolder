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

#include <boost/shared_ptr.hpp>

extern const CString NULL_VALUE;    // NULL�l�w��

namespace sindy{
	class CTableContainer;
	typedef boost::shared_ptr<CTableContainer> CSPTableContainer;
}


/**
* @brief �e�[�u�����I�[�v������
* @note ���s�����ꍇ�͗�O�𓊂���
* @param work [in] �Ώۃ��[�N�X�y�[�X
* @param table [in] �Ώۂ̃e�[�u����
* @return �Ώۂ̃e�[�u��
*/
ITablePtr TryOpenTable( const IWorkspacePtr& work, const CString& table );

/**
* @brief ���[�N�X�y�[�X�ɐڑ�����
* @note ���s�����ꍇ�͗�O�𓊂���
* @param work [in] ���[�N�X�y�[�X�ڑ�������
* @return �Ώۂ̃��[�N�X�y�[�X
*/
IWorkspacePtr TryConnect( const CString& conn );

/**
* @brief �q�o�[�W�������쐬����
* @note ���s�����ꍇ�͗�O�𓊂���
* @param table [in] �Ώۂ̃e�[�u��
* @return �쐬�����q�o�[�W�����B
*         �o�[�W������Ή��������ꍇ�͑Ώۃe�[�u���̃o�[�W����
*/
IWorkspacePtr TryCreateVersion( const sindy::CTableContainer& table );
