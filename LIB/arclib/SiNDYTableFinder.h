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

// SiNDYTableFinder.h: SiNDYTableFinder �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYTABLEFINDER_H__2A38CACC_E465_4557_9F24_D34CEC1EED3F__INCLUDED_)
#define AFX_SINDYTABLEFINDER_H__2A38CACC_E465_4557_9F24_D34CEC1EED3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @brief �e�[�u���擾�n���N���X
 */
class SiNDYTableFinder  
{
public:
	/**
	 * @brief �w�肵���e�[�u�����ɑ�������e�[�u�����擾����
	 *
	 * @note �e�[�u�����̂�
	 * @note -# �e�[�u�����̂�
	 * @note -# ���[�U���t���e�[�u����
	 * @note -# ���C����
	 * @note �̂�����ɂ��Ή��ł���悤��������̂��]�܂����B
	 * @note �Œ��1��2�͕K�{�ł���B
	 *
	 * @param lpszTableName [in] �e�[�u������
	 * @return �e�[�u���B�擾�ł��Ȃ������ꍇNULL�B
	 */
	virtual ITablePtr getTable(LPCTSTR lpszTableName) = 0;

	virtual IWorkspacePtr getWorkspace(LPCTSTR lpszTableName);
	virtual _IRowPtr getRow(LPCTSTR lpszTableName, long nObjectId);
	virtual IGeometryPtr getBaseMeshGeometry(long nMeshcode);
};

#endif // !defined(AFX_SINDYTABLEFINDER_H__2A38CACC_E465_4557_9F24_D34CEC1EED3F__INCLUDED_)
