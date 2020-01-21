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
 * @brief IEditor の edit operation の開始から完了までの面倒を見るクラス
 */
class editor_operation
{
public:
// コンストラクタとデストラクタ
	/**
	 * @brief edit operation を開始する。
	 *
	 * @param ipEditor [in] エディタ。
	 * @param bStartOperation [in] IEditor::StartOperation() をコンストラクタ内で実行するか。
	 * @exception _com_error IEditor::StartOperation() に失敗した。
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
	 * @brief edit operation がまだ生きている場合、中断させる。
	 *
	 * 中断に失敗した場合、 edit operation は IEditor::AbortOperation() に失敗したのと同じ状態になる。
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

// 編集操作の操作
	/**
	 * @brief edit operation を中断する。
	 *
	 * @exception _com_error edit operation が既に完了（中断）していたか、中断処理に失敗した。
	 * @par 例外安全性: IEditor::AbortOperation() に依存。
	 */
	void abort()
	{
		atl2::valid(m_ipEditor->AbortOperation());
		m_ipEditor = 0;
	}

	/**
	 * @brief edit operation を完了する。
	 *
	 * @param bstrMenuText [in] edit operation の名前。
	 * @exception _com_error edit operation が既に完了（中断）していたか、完了処理に失敗した。
	 * @par 例外安全性: IEditor::StopOperation() に依存。
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
