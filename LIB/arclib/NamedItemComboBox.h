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
 * @brief 名称付きオブジェクト選択コンボボックス雛型
 *
 * get_Name() 可能なインタフェースの名称、およびそのポインタを格納するコンボボックス。<br>
 * 参照権を保持（AddRef）しないので、無効ポインタにアクセスすることのないよう注意すること。
 * 
 * @pre 要 ATLControls.h 。
 *
 * - テンプレート引数
 *   - T : get_Name() 可能なインタフェース。
 *   - TBase : 規定クラス。
 */
template <typename T, typename TBase = ATLControls::CComboBoxT<CWindow> >
class CNamedItemComboBoxT : public TBase
{
public:
// 定義
	typedef T Interface; ///< インタフェース。

	/**
	 * @brief コンボボックスの要素を初期化するクラス
	 */
	class Initializer
	{
	public:
	// 構築/消滅
		Initializer(CNamedItemComboBoxT& rComboBox) :
		m_rComboBox(rComboBox),
		m_pInterface(m_rComboBox.GetCurSelData())
		{
			// 登録されている名称を全て削除する。
			m_rComboBox.ResetContent();
		}

	// 処理
		/**
		 * @brief コンボボックスの有効・無効を設定する。
		 *
		 * 要素数が2以上ならばコンボボックスの走査を有効に、それ以外の場合は無効にする。
		 */
		void EnableWindow()
		{
			m_rComboBox.EnableWindow(m_rComboBox.GetCount() >= 2);
		}

	// 演算子
		/**
		 * @brief 名称を追加する
		 *
		 * pInterface が、名称が削除される前に選択されていたデータだった場合、再度選択する。
		 *
		 * @param pInterface [in] @a T 。
		 */
		void operator()(Interface* pInterface)
		{
			// 名称を追加する。
			int nItem = m_rComboBox.AddNamedItem(pInterface);
			if(nItem == CB_ERR)
				return;

			if(m_pInterface && pInterface == m_pInterface) {
				// 選択されていたデータと同じものが追加されたらそれを選択する。
				m_rComboBox.SetCurSel(nItem);
				m_pInterface = 0;
			}
			else if(nItem == 0) {
				// とりあえず最初の名称を選択する。
				m_rComboBox.SetCurSel(0);
			}
		}

	private:
	// 変数
		CNamedItemComboBoxT& m_rComboBox; ///< 名前付きオブジェクト選択コンボボックス。
		Interface* m_pInterface; ///< 選択されていたデータ。
	};

// 構築
	CNamedItemComboBoxT(HWND hWnd = NULL) :
	TBase(hWnd)
	{
	}

// 設定/取得
	/**
	 * @brief 名前付きオブジェクトを追加する
	 *
	 * @param pInterface [in] @a T 。
	 * @retval CB_ERR 追加に失敗した。
	 * @retval CB_ERRSPACE メモリが足りなかった。
	 * @retval >=0 追加された位置を示す、0から始まる相対インデックス。
	 */
	int AddNamedItem(Interface* pInterface)
	{
		if(! pInterface)
			return CB_ERR;

		// 名称を取得する。
		CComBSTR bstrName;
		pInterface->get_Name(&bstrName);

		// 名称を追加する。
		USES_CONVERSION;
		int nItem = AddString(OLE2CT(bstrName));
		if(nItem < 0)
			return nItem;

		// オブジェクトをデータとして設定する。
		SetItemData(nItem, reinterpret_cast<DWORD>(pInterface));

		return nItem;
	}

	/**
	 * @brief オブジェクトを取得する
	 *
	 * @param nIndex [in] 0から始まる相対インデックス。
	 * @return オブジェクト。失敗した場合null。
	 */
	Interface* GetItemData(int nIndex) const
	{
		DWORD dw = TBase::GetItemData(nIndex);
		return dw != CB_ERR ? reinterpret_cast<Interface*>(dw) : 0;
	}

	/**
	 * @brief 選択されているオブジェクトを取得する
	 *
	 * @reutrn 選択されているオブジェクト。オブジェクトが選択されていない場合null。
	 */
	Interface* GetCurSelData()
	{
		int nCurSel = GetCurSel();
		return nCurSel != CB_ERR ? GetItemData(nCurSel) : 0;
	}
};

} // namespace sindy

#endif __SINDY_NAMED_ITEM_COMBO_BOX_H__
