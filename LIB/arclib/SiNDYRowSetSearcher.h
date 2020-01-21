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

#ifndef _ROW_SET_SEARCHER_H_
#define _ROW_SET_SEARCHER_H_

#include "SiNDYTraits.h"

namespace sindy {

template <typename TRow>
class RowSetSearcher_
{
public:
// 定義
	typedef traits<TRow>::RowInterface RowInterface;
	typedef traits<TRow>::TableInterface TableInterface;
	typedef traits<TRow>::CursorInterface CursorInterface;
	typedef traits<TRow>::CursorInterfacePtr CursorInterfacePtr;

// 構築/消滅消滅
	RowSetSearcher_(traits<TRow>::TableInterface* ipTable) :
	m_ipTable(ipTable)
	{
	}

	virtual ~RowSetSearcher_()
	{
	}

// 設定/取得
	traits<TRow>::TableInterface* table() const
	{
		return m_ipTable;
	}

// 処理
	/**
	 * @brief 次のSearchを行う
	 *
	 * @param ppCursor [out] Cursor
	 * @retval S_OK Search成功
	 * @retval S_FALSE Search終了
	 * @retval その他 Search失敗
	 */
	virtual HRESULT __stdcall SearchNext(traits<TRow>::CursorInterface** ppCursor) = 0;

protected:
// 変数
	traits<TRow>::TableInterfacePtr m_ipTable;
};

typedef RowSetSearcher_<_IRow> RowSetSearcher;
typedef RowSetSearcher_<IFeature> FeatureSetSearcher;

typedef StreamContainerT<RowSetSearcher, RowSetSearcher::CursorInterface*, &RowSetSearcher::SearchNext> RowSetSearcher_Container;

template <typename T>
struct _row_set_searcher_traits : public traits_base<T>
{
public:
	template<>
	struct traits_base<T>::accessor<T::CursorInterface*>
	{
		typedef RowSetSearcher_Container Container;
		typedef StreamIteratorT<Container> iterator;
		static iterator make_first(T* pContainer) { return iterator(pContainer); } \
	};

	template<>
	struct traits_base<T>::accessor<T::RowInterface*>
	{
		struct iterator
		{
		public:
			iterator() :
			m_pContainer(0)
			{
			}

			iterator(T* pContainer) :
			m_pContainer(pContainer)
			{

				operator++();
			}

		// 反復子の挙動
			bool operator==(last_iterator it) { return m_pContainer == 0; }
			bool operator!=(last_iterator it) { return ! operator==(it); }

			iterator& operator++()
			{
				if(m_ipCursor == 0 || ++m_it == last_iterator()) {
					if(m_pContainer->SearchNext(&m_ipCursor) != S_OK) {
						m_pContainer = 0;
					}
					else {
						m_it = traits<T::CursorInterface>::make_first(m_ipCursor);
						if(m_it == last_iterator()) {
							m_pContainer = 0;
							m_ipCursor = 0;
						}
					}
				}
				return *this;
			}

			T::RowInterface* const operator*()
			{
				return *m_it;
			}

		private:
		// 変数
			T* m_pContainer;
			T::CursorInterfacePtr m_ipCursor;
			traits<T::CursorInterface>::iterator m_it;
		};

		static iterator make_first(T* pContainer) { return iterator(pContainer); } \
	};

	typedef accessor<T::CursorInterface*>::iterator iterator; \
	static iterator make_first(T* ipContainer) { return accessor<T::CursorInterface*>::make_first(ipContainer); }
};

template <>
struct pointer_traits<RowSetSearcher*> : public _row_set_searcher_traits<RowSetSearcher> {};

template <>
struct pointer_traits<FeatureSetSearcher*> : public _row_set_searcher_traits<FeatureSetSearcher> {};

} // namespace sindy

#endif // _ROW_SET_SEARCHER_H_
