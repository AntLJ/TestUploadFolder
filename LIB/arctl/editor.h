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

#ifndef ARCTL_EDITOR_H_
#define ARCTL_EDITOR_H_

#include <atl2/exception.h>

namespace arctl {

/**
 * @brief IEditor �� edit operation �̊J�n���犮���܂ł̖ʓ|������N���X
 */
class editor_operation
{
public:
// �R���X�g���N�^�ƃf�X�g���N�^
	/**
	 * @brief edit operation ���J�n����B
	 *
	 * @param ipEditor [in] �G�f�B�^�B
	 * @param bStartOperation [in] IEditor::StartOperation() ���R���X�g���N�^���Ŏ��s���邩�B
	 * @exception _com_error IEditor::StartOperation() �Ɏ��s�����B
	 */
	explicit editor_operation(IEditor* ipEditor, bool bStartOperation = true) :
	m_ipEditor(ipEditor)
	{
		_ASSERT(m_ipEditor != 0);

		if(bStartOperation) {
			atl2::valid(m_ipEditor->StartOperation());
		}
	}

	/**
	 * @brief edit operation ���܂������Ă���ꍇ�A���f������B
	 *
	 * ���f�Ɏ��s�����ꍇ�A edit operation �� IEditor::AbortOperation() �Ɏ��s�����̂Ɠ�����ԂɂȂ�B
	 */
	~editor_operation()
	{
		if(m_ipEditor == 0)
			return;

		try {
			abort();
		}
		catch(...) {}
	}

// �ҏW����̑���
	/**
	 * @brief edit operation �𒆒f����B
	 *
	 * @exception _com_error edit operation �����Ɋ����i���f�j���Ă������A���f�����Ɏ��s�����B
	 * @par ��O���S��: IEditor::AbortOperation() �Ɉˑ��B
	 */
	void abort()
	{
		atl2::valid(m_ipEditor->AbortOperation());
		m_ipEditor = 0;
	}

	/**
	 * @brief edit operation ����������B
	 *
	 * @param bstrMenuText [in] edit operation �̖��O�B
	 * @exception _com_error edit operation �����Ɋ����i���f�j���Ă������A���������Ɏ��s�����B
	 * @par ��O���S��: IEditor::StopOperation() �Ɉˑ��B
	 */
	void stop(BSTR bstrMenuText)
	{
		_ASSERT(m_ipEditor != 0);
		atl2::valid(m_ipEditor->StopOperation(bstrMenuText));
		m_ipEditor = 0;
	}

private:
	IEditorPtr m_ipEditor;
};

} // namespace arctl

#endif // ARCTL_EDITOR_H_
