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

#include "stdafx.h"
#include <map>
#include <vector>

namespace gf {

	// �T�[�o�ɐڑ����ă��[�N�X�y�[�X���擾����
	IWorkspacePtr OpenWorkSpace( LPCTSTR strServername, LPCTSTR strUsername, LPCTSTR strPassword, LPCTSTR strVersionname );


	/**
	* @brief	����o�[�W�����Ɏq�o�[�W�������쐬���A���̃��[�N�X�y�[�X��Ԃ�
	*
	* @note		���ɓ����̃o�[�W�������������ꍇ�͏����Ă���V���ɍ�蒼��
	*
	* @param	ipWorkspace			[in]	�e�o�[�W�����̃��[�N�X�y�[�X
	* @param	strVersionName		[in]	���o�[�W�����̃o�[�W������
	* @return	�V����������q�o�[�W�����̃��[�N�X�y�[�X
	*/
	IWorkspacePtr CreateNewVersion( IWorkspacePtr ipWorkspace, LPCTSTR strVersionName );

	// �e�[�u���̃o�[�W�����Ԃɂ����鑮�������}�b�v���擾����
	bool DiffMapBetweenVersion( IWorkspacePtr ipCurrentWorkspace, LPCTSTR strTableName, MESHCODEMAP& DiffMap );

	// �e�[�u����S�s�ǂݍ���Ń}�b�v�Ɋi�[����
	MESHCODEMAP Table2Map( IWorkspace* ipWorkspace, LPCTSTR strTableName, LPCTSTR strKey );

	/**
	* @brief	2�}�b�v���r���ďd������L�[�͍폜����
	*
	* @note		TargetMap��CompareMap���r���ē����L�[������΁ATargetMap���ł͍폜���܂��B
	*
	* @param	TargetMap			[in]	���ۂɍ폜�����
	* @param	CompareMap			[in]	��r�Ɏg����
	* @return	�폜�����s��
	*/
	int DeleteDuplicateKeys( MESHCODEMAP& TargetMap, const MESHCODEVECMAP& CompareMap );

	MESHCODEMAP CreateMapFromQuery( IFeatureClass* ipFeatClass, const MESHCODEMAP& mapTmpCityMesh );
	
	int UpdateTable( const MESHCODEMAP& SrcMap, MESHCODEMAP& DstMap );

	int UpdateTable( const MESHCODEVECMAP& SrcMap, const std::vector<CString>& vecFdield, MESHCODEMAP& DstMap );
	
	std::vector<CString> Tokenize2Vector( const CString & strSrc, LPCTSTR lpcszTorkens, LPCTSTR lpcszTrim );

}
