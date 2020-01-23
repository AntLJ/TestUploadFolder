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

#include "Editor.h"
#include "Processor.h"
#include "ClippingDefinitionManager.h"

namespace sindy {
	class CTableContainer;
}

/**
 * @brief �N���b�s���O���s�N���X
 */
template <typename T>
class MeshClipProcessor : public Processor<T>
{
public:

	/// �R���X�g���N�^
	MeshClipProcessor(const sindy::CWorkspace& workspace, const ClippingDefinitionManager& m_ClipDef);

	/// �f�X�g���N�^
	virtual ~MeshClipProcessor(){};

	/**
	 * @brief  �������s
	 * @param  src  [in]  ���b�V���R�[�h
	 * @retval true:����
	 */
	virtual bool proc(const uh::tstring& src);

	/// @brief ���[�v�����O�������\�b�h
	bool preprocess() override;

	/// @brief ���[�v��������s���\�b�h
	bool postprocess() override
	{
		return m_workspace.StopEditing(true);
	}

	/**
	 * @brief  �����Ώۂ�Workspace��ԋp
	 * @retval �����Ώۂ�Workspace
	 */
	const sindy::CWorkspace& getWorkspace(){ return m_outputWorkspace; }

private:

	CEditor  m_workspace;			//!< �o�͐��DB
	ClippingDefinitionManager m_mgr;	//!< �}�[�W�Ώے�`�N���X
	uh::tstring m_internalName;		//!< �v���O������(�X�V�p)

	/**
	* @brief �N���b�v���������{����
	* 
	* mesh��`�ŃN���b�v���������{���܂��B\n
	* ���������s���邽�߁Acon�͋�̏�Ԃ�z�肵�Ă��܂��B\n
	* ������́Acon�ɃN���b�v�������ꂽ�t�B�[�`�����i�[����܂��B
	*
	* @param con [in] �N���b�v�����Ώۃt�B�[�`���N���X�i��œn�����Ɓj
	* @param mesh [in] �N���b�v������`
	* @retval true �N���b�v��������
	* @retval false �N���b�v�������s
	*/
	virtual bool doClip( sindy::CTableContainer& con, IEnvelopePtr mesh );
};
