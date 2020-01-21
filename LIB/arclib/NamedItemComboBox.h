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

#ifndef __SINDY_NAMED_ITEM_COMBO_BOX_H__
#define __SINDY_NAMED_ITEM_COMBO_BOX_H__

namespace sindy {

/**
 * @brief ���̕t���I�u�W�F�N�g�I���R���{�{�b�N�X���^
 *
 * get_Name() �\�ȃC���^�t�F�[�X�̖��́A����т��̃|�C���^���i�[����R���{�{�b�N�X�B<br>
 * �Q�ƌ���ێ��iAddRef�j���Ȃ��̂ŁA�����|�C���^�ɃA�N�Z�X���邱�Ƃ̂Ȃ��悤���ӂ��邱�ƁB
 * 
 * @pre �v ATLControls.h �B
 *
 * - �e���v���[�g����
 *   - T : get_Name() �\�ȃC���^�t�F�[�X�B
 *   - TBase : �K��N���X�B
 */
template <typename T, typename TBase = ATLControls::CComboBoxT<CWindow> >
class CNamedItemComboBoxT : public TBase
{
public:
// ��`
	typedef T Interface; ///< �C���^�t�F�[�X�B

	/**
	 * @brief �R���{�{�b�N�X�̗v�f������������N���X
	 */
	class Initializer
	{
	public:
	// �\�z/����
		Initializer(CNamedItemComboBoxT& rComboBox) :
		m_rComboBox(rComboBox),
		m_pInterface(m_rComboBox.GetCurSelData())
		{
			// �o�^����Ă��閼�̂�S�č폜����B
			m_rComboBox.ResetContent();
		}

	// ����
		/**
		 * @brief �R���{�{�b�N�X�̗L���E������ݒ肷��B
		 *
		 * �v�f����2�ȏ�Ȃ�΃R���{�{�b�N�X�̑�����L���ɁA����ȊO�̏ꍇ�͖����ɂ���B
		 */
		void EnableWindow()
		{
			m_rComboBox.EnableWindow(m_rComboBox.GetCount() >= 2);
		}

	// ���Z�q
		/**
		 * @brief ���̂�ǉ�����
		 *
		 * pInterface ���A���̂��폜�����O�ɑI������Ă����f�[�^�������ꍇ�A�ēx�I������B
		 *
		 * @param pInterface [in] @a T �B
		 */
		void operator()(Interface* pInterface)
		{
			// ���̂�ǉ�����B
			int nItem = m_rComboBox.AddNamedItem(pInterface);
			if(nItem == CB_ERR)
				return;

			if(m_pInterface && pInterface == m_pInterface) {
				// �I������Ă����f�[�^�Ɠ������̂��ǉ����ꂽ�炻���I������B
				m_rComboBox.SetCurSel(nItem);
				m_pInterface = 0;
			}
			else if(nItem == 0) {
				// �Ƃ肠�����ŏ��̖��̂�I������B
				m_rComboBox.SetCurSel(0);
			}
		}

	private:
	// �ϐ�
		CNamedItemComboBoxT& m_rComboBox; ///< ���O�t���I�u�W�F�N�g�I���R���{�{�b�N�X�B
		Interface* m_pInterface; ///< �I������Ă����f�[�^�B
	};

// �\�z
	CNamedItemComboBoxT(HWND hWnd = NULL) :
	TBase(hWnd)
	{
	}

// �ݒ�/�擾
	/**
	 * @brief ���O�t���I�u�W�F�N�g��ǉ�����
	 *
	 * @param pInterface [in] @a T �B
	 * @retval CB_ERR �ǉ��Ɏ��s�����B
	 * @retval CB_ERRSPACE ������������Ȃ������B
	 * @retval >=0 �ǉ����ꂽ�ʒu�������A0����n�܂鑊�΃C���f�b�N�X�B
	 */
	int AddNamedItem(Interface* pInterface)
	{
		if(! pInterface)
			return CB_ERR;

		// ���̂��擾����B
		CComBSTR bstrName;
		pInterface->get_Name(&bstrName);

		// ���̂�ǉ�����B
		USES_CONVERSION;
		int nItem = AddString(OLE2CT(bstrName));
		if(nItem < 0)
			return nItem;

		// �I�u�W�F�N�g���f�[�^�Ƃ��Đݒ肷��B
		SetItemData(nItem, reinterpret_cast<DWORD>(pInterface));

		return nItem;
	}

	/**
	 * @brief �I�u�W�F�N�g���擾����
	 *
	 * @param nIndex [in] 0����n�܂鑊�΃C���f�b�N�X�B
	 * @return �I�u�W�F�N�g�B���s�����ꍇnull�B
	 */
	Interface* GetItemData(int nIndex) const
	{
		DWORD dw = TBase::GetItemData(nIndex);
		return dw != CB_ERR ? reinterpret_cast<Interface*>(dw) : 0;
	}

	/**
	 * @brief �I������Ă���I�u�W�F�N�g���擾����
	 *
	 * @reutrn �I������Ă���I�u�W�F�N�g�B�I�u�W�F�N�g���I������Ă��Ȃ��ꍇnull�B
	 */
	Interface* GetCurSelData()
	{
		int nCurSel = GetCurSel();
		return nCurSel != CB_ERR ? GetItemData(nCurSel) : 0;
	}
};

} // namespace sindy

#endif __SINDY_NAMED_ITEM_COMBO_BOX_H__
