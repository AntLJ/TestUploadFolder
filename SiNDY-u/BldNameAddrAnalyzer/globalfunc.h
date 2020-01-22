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
#include <TDC/sindylib_core/Workspace.h>

namespace sindy{
	class CTableContainer;
	typedef boost::shared_ptr<CTableContainer> CSPTableContainer;
}

class CADOBase;


/**
* @brief �e�[�u�����I�[�v������
* @note ���s�����ꍇ�͗�O�𓊂���
* @param work [in] �Ώۃ��[�N�X�y�[�X
* @param table [in] �Ώۂ̃e�[�u����
* @return �Ώۂ̃e�[�u��
*/
ITablePtr TryOpenTable( const CString& work, const CString& table );

/**
* @brief ADO��Oracle�ɐڑ�����
* @note ���s�����ꍇ�͗�O�𓊂���
* @param svr [in] �T�[�o���itnsnames.ora�Őݒ肳��Ă�����́j
* @param usr [in] ���[�U���i�p�X���[�h�j
* @param ado [out] ADO�Ǘ��p
*/
void connectAdo( const CString& svr, const CString& usr, CADOBase& ado );

/**
* @brief �|�C���g�`����쐬����
* @param point [in] ���W
* @param spRef [in] �ݒ肷���ԎQ��
* @return [in] �쐬�����|�C���g
*/
IPointPtr createPoint( const WKSPoint point, const ISpatialReferencePtr spRef );

CString getDateWhere( IWorkspacePtr work, const CString& key, const CComVariant& val );

/**
* @brief ���[�N�X�y�[�X�ɐڑ�����
* @note ���s�����ꍇ�͗�O�𓊂���
* @param work [in] ���[�N�X�y�[�X�ڑ�������
* @return �Ώۂ̃��[�N�X�y�[�X
*/
//sindy::CWorkspace TryConnectWorkspace( const CString& conn );

/**
* @brief �q�o�[�W�������쐬����
* @note ���s�����ꍇ�͗�O�𓊂���
* @param table [in] �Ώۂ̃e�[�u��
* @return �쐬�����q�o�[�W�����B
*         �o�[�W������Ή��������ꍇ�͑Ώۃe�[�u���̃o�[�W����
*/
//IWorkspacePtr TryCreateVersion( const sindy::CTableContainer& table );
