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

#ifndef __SINDY_ITERATOR_H__
#define __SINDY_ITERATOR_H__

#include <functional>
#include <vector>
#include "ref.h"
#include "traits.h"

namespace sindy {

#pragma warning( push )
#pragma warning( disable : 4284 )
/**
 * @brief コンテナ要素特性テンプレート
 */
template <typename TElement>
struct element_traits
{
	/**
	 * @brief インタフェースポインタ以外の要素に関する詳細。
	 */
	template <int TisCP>
	struct detail_t
	{
		typedef TElement element;
		typedef TElement holder;
		static void construct(element&) {}
		static void copy(element&, const element&) {}
		static void reset(element&) {}
	};

	/**
	 * @brief インタフェースポインタ型要素に関する詳細。
	 */
	template <>
	struct detail_t<2>
	{
		typedef TElement element;
		class holder
		{
		public:
			holder() :
			m_Element(0)
			{
			}

			holder(TElement cElement) :
			m_Element(cElement)
			{
				if(m_Element) {
					m_Element->AddRef();
				}
			}

			~holder()
			{
				if(m_Element) {
					m_Element->Release();
				}
			}

			HRESULT QueryInterface(const IID& iid, void** p)
			{
				if(m_Element) {
					m_Element->QueryInterface(iid, reinterpret_cast<void**>(p));
				}
			}

			const TElement& operator->() const { return m_Element; }

			operator bool() const throw() { return m_Element != 0; }

		private:
			TElement m_Element;
		};

		static void construct(element& rpInterface) 
		{
			rpInterface = 0;
		}

		static void copy(element& rpDstInterface, element pSrcInterface)
		{
			if(rpDstInterface) {
				rpDstInterface->Release();
			}

			rpDstInterface = pSrcInterface;

			if(rpDstInterface) {
				rpDstInterface->AddRef();
			}
		}

		static void reset(element& rpInterface)
		{
			if(rpInterface) {
				rpInterface->Release();
				rpInterface = 0;
			}
		}
	};

	typedef detail_t<type_of<TElement>::value> detail_; ///< 要素に関する詳細。

// 定義
	typedef typename detail_::element Element; ///< 要素型。
	typedef typename detail_::holder ElementHolder; ///< 要素型オブジェクト。

// 処理
	/**
	 * @brief 要素の初期状態を構築する
	 *
	 * @param rElement [in] 対象要素。
	 */
	static void construct(Element& rElement)
	{
		detail_::construct(rElement);
	}

	/**
	 * @brief 要素の複製を行う
	 *
	 * ディープでもシャローでも、好みの方法で。<br>
	 * @a rDstElement は construct() 済みだが、 reset() されているかは不明。
	 *
	 * @param rDstElement [out] 複製先要素。
	 * @param rSrcElement [in] 複製元要素。
	 */
	static void copy(Element& rDstElement, const Element& rSrcElement)
	{
		detail_::copy(rDstElement, rSrcElement);
	}

	/**
	 * @brief 要素を初期状態に戻す
	 *
	 * @param rElement [in] 対象要素。
	 */
	static void reset(Element& rElement)
	{
		detail_::reset(rElement);
	}
};
#pragma warning( pop )

/**
 * @brief VARIATN型要素の特性
 */
template <>
struct element_traits<VARIANT>
{
// 定義
	typedef VARIANT Element; ///< 要素型。
	typedef _variant_t ElementHolder; ///< 要素型オブジェクト。

// 処理
	static void construct(VARIANT& rVariant)
	{
		::VariantInit(&rVariant);
	}

	static void copy(VARIANT& rDstVariant, const VARIANT& rSrcElement)
	{
		reset(rDstVariant);
		::VariantCopy(&rDstVariant, const_cast<VARIANT*>(&rSrcElement));
	}

	static void reset(VARIANT& rVariant)
	{
		::VariantClear(&rVariant);
	}
};

/**
 * @brief SiNDY系反復子用、絶対終端反復子
 */
class last_iterator {};

/**
 * @brief 流出一方向反復子
 *
 * ICursor など、巻き戻し不可なストリームの反復子。
 *
 * - テンプレート引数
 *   - TContainerType : StreamContainerT か、その派生クラス。
 *   - TElementTraits : コンテナに積まれた要素の特性。 element_traits か、その互換クラス。
 */
template <typename TContainerType>
class StreamIteratorT : public std::iterator<std::input_iterator_tag, typename container_traits<TContainerType>::Element>
{
public:
// 定義
	typedef typename container_traits<TContainerType>::ElementTraits ElementTraits;
	typedef typename container_traits<TContainerType>::ElementHolder ElementHolder;

// 構築/消滅
	/**
	 * @brief 終端反復子を構築する
	 */
	StreamIteratorT()
	{
		ElementTraits::construct(m_Element);
	}

	/**
	 * @brief 始端反復子を構築する
	 */
	StreamIteratorT(const TContainerType& rContainer) :
	m_Container(rContainer)
	{
		ElementTraits::construct(m_Element);
		if(operator!=(last_iterator())) {
			operator++();
		}
	}

	/**
	 * @brief 始端反復子を構築する
	 */
	StreamIteratorT(typename TContainerType::ContainerInterface* pContainer) :
	m_Container(pContainer)
	{
		ElementTraits::construct(m_Element);
		if(operator!=(last_iterator())) {
			operator++();
		}
	}

	/**
	 * @brief 反復子を複製構築する
	 */
	StreamIteratorT(const StreamIteratorT& rIterator)
	{
		ElementTraits::construct(m_Element);
		ElementTraits::copy(m_Element);
	}

	/**
	 * @brief 反復子を破棄する
	 */
	~StreamIteratorT()
	{
		ElementTraits::reset(m_Element);
	}

// 反復子の挙動
	/**
	 * @brief 反復子の終端判定
	 *
	 * @param it [in] 反復子終端オブジェクト。
	 * @retval true 終端。
	 * @retval false 終端ではない。
	 */
	bool operator==(last_iterator)
	{
		return ! m_Container;
	}

	/**
	 * @brief 反復子の非終端判定
	 *
	 * @param it [in] 反復子終端オブジェクト。
	 * @retval true 終端ではない。
	 * @retval false 終端。
	 */
	bool operator!=(last_iterator it)
	{
		return ! operator==(it);
	}

	/**
	 * @brief 終端反復子との同一性判定
	 *
	 * @param rIt [in] 終端反復子。
	 * @retval true 終端を示している。
	 * @retval false 終端を示してはいない。
	 */
	bool operator==(const StreamIteratorT& rIt)
	{
		return ! m_Container;
	}

	/**
	 * @brief 終端反復子との非同一性判定
	 *
	 * @param rIt [in] 終端反復子。
	 * @retval true 終端を示してはいない。
	 * @retval false 終端を示している。
	 */
	bool operator!=(const StreamIteratorT& rIt)
	{
		return ! operator==(rIt);
	}

	/**
	 * @brief 反復子を終端側へ一歩進める
	 */
	StreamIteratorT& operator++()
	{
		// 要素解放後、ストリームを１つ進め、要素を取得する
		ElementTraits::reset(m_Element);
		static_cast<ContainerType&>(m_Container).getNextElement(&m_Element);
		return *this;
	}

	/**
	 * @brief 反復子が指し示している要素を取得する。
	 */
	value_type const& operator*()
	{
		return m_Element;
	}

	/**
	 * @brief 反復子を複製する
	 */
	StreamIteratorT& operator=(const StreamIteratorT& rIterator)
	{
		m_Container = rIterator.m_Container;
		ElementTraits::copy(m_Element, rIterator.m_Element);
		return *this;
	}

protected:
	typedef typename unwrap_reference<TContainerType>::type ContainerType;

private:
// 変数
	TContainerType m_Container; ///< コンテナへのポインタ。
	value_type m_Element; ///< 現在指している要素。
};

//////////////////////////////////////////////////////////////////////
// ランダムアクセス反復子
//////////////////////////////////////////////////////////////////////

/**
 * @brief ランダムアクセス反復子インタフェース
 *
 * IPointCollection など、0～n個の要素にランダムアクセス可能なインタフェース用反復子のインタフェース。<br>
 * 実装される挙動は以下のとおり。
 * - 要素数の保持
 * - 反復子の順行・逆行
 * - 反復子の同一性比較
 * - 反復子の終端判定
 * - 反復子が指し示す値の取得
 * 要素の保持は派生クラス側で行う。
 * 
 * - テンプレート引数
 *   - TContainerType : IndexContainerT か、その派生クラス。あるいはそのref。
 *   - TFore : 順方向？
 */
template <typename TContainerType, bool TFore>
class IndexIteratorIT : public std::iterator<std::input_iterator_tag, typename container_traits<TContainerType>::Element>
{
public:
// 定義
	typedef typename container_traits<TContainerType>::ElementTraits ElementTraits;
	typedef typename container_traits<TContainerType>::ElementHolder ElementHolder;

// 構築
	/**
	 * @brief 終端反復子を構築する
	 *
	 * @param cContainer [in] コンテナ。
	 */
	IndexIteratorIT(const TContainerType& rContainer) :
	m_Container(rContainer)
	{
		m_nIndex = static_cast<ContainerType const&>(m_Container).getLast();
	}

	/**
	 * @brief 始端反復子を構築する
	 *
	 * @param cContainer [in] コンテナ。
	 * @param nFirst [in] 始端インデックス。通常は（逆行反復子であっても）0を指定する。
	 */
	IndexIteratorIT(const TContainerType& rContainer, long nFirst) :
	m_Container(rContainer),
	m_nIndex(nFirst)
	{
	}

	/**
	 * @brief 反復子を複製構築する
	 */
	IndexIteratorIT(const IndexIteratorIT&  rIt) :
	m_Container(rIt.m_Container),
	m_nIndex(rIt.m_nIndex)
	{
	}

// 取得
	/**
	 * @brief 反復子が指し示している要素を取得する。
	 *
	 * @param pElement [out] 要素出力先。
	 */
	void getElement(value_type* pElement)
	{
		static_cast<ContainerType const&>(m_Container).getElement(getIndex(), pElement);
	}

// 反復子の挙動
	/**
	 * @brief 反復子を終端側へ一歩進める
	 */
	IndexIteratorIT& operator++()
	{
		++m_nIndex;
		return *this;
	}

	/**
	 * @brief 反復子を始端側へ一歩戻す
	 */
	IndexIteratorIT& operator--()
	{
		--m_nIndex;
		return *this;
	}

	/**
	 * @brief 反復子の終端判定
	 *
	 * @param it [in] 反復子終端オブジェクト。
	 * @retval true 終端。
	 * @retval false 終端ではない。
	 */
	bool operator==(const last_iterator)
	{
		return m_nIndex == static_cast<ContainerType const&>(m_Container).getLast();
	}

	/**
	 * @brief 反復子の非終端判定
	 *
	 * @param it [in] 反復子終端オブジェクト。
	 * @retval true 終端ではない。
	 * @retval false 終端。
	 */
	bool operator!=(const last_iterator it) { return ! operator==(it); }

	/**
	 * @brief 反復子同一性判定
	 *
	 * @param rIt [in] 比較対象反復子。
	 * @retval true 両反復子は同一である。
	 * @retval false 両反復子は同一ではない。
	 */
	bool operator==(const IndexIteratorIT& rIt)
	{
		return static_cast<ContainerType const&>(m_Container) == static_cast<ContainerType const&>(rIt.m_Container) && m_nIndex == rIt.m_nIndex;
	}

	/**
	 * @brief 反復子非同一性判定
	 *
	 * @param rIt [in] 比較対象反復子。
	 * @retval true 両反復子は同一ではない。
	 * @retval false 両反復子は同一である。
	 */
	bool operator!=(const IndexIteratorIT& rIt)
	{
		return ! operator==(rIt);
	}

protected:
	typedef typename unwrap_reference<TContainerType>::type ContainerType;

private:
// 定義
	/**
	 * @brief 反復子の向きに応じてインデックスを計算するファンクタテンプレート
	 */
	template <bool T> struct GetIndex {};

	/**
	 * @brief インデックス計算ファンクタを順方向用に特殊化
	 */
	template <> struct GetIndex<true> { long operator()(long nIndex, long) { return nIndex; } };

	/**
	 * @brief インデックス計算ファンクタを逆方向用に特殊化
	 */
	template <> struct GetIndex<false> { long operator()(long nIndex, long nLast) { return nLast - nIndex - 1; } };

// 取得
	/**
	 * @brief 反復子が現在指し示している要素のインデックスを取得する
	 *
	 * @return インデックス。
	 */
	long getIndex()
	{
		return GetIndex<TFore>()(m_nIndex, static_cast<ContainerType const&>(m_Container).getLast());
	}

// 変数
	const TContainerType m_Container; ///< コンテナへのポインタ。
	long m_nIndex; ///< 現在指しているインデックス（順方向の場合）。
};

/**
 * @brief ランダムアクセス反復子
 *
 *   - TContainerType : IndexContainerT か、その派生クラス。あるいはそのref。
 * - TFore : 順方向？
 * - TElementTraits : コンテナに積まれた要素の特性。 element_traits か、その互換クラス。
 */
template <typename TContainerType, bool TFore = true>
class IndexIteratorT : public IndexIteratorIT<TContainerType, TFore>
{
public:
// 定義
	typedef IndexIteratorIT<TContainerType, TFore> Base;

// 構築/消滅
	/**
	 * @brief 終端反復子を構築する
	 *
	 * @param cContainer [in] コンテナ。
	 */
	IndexIteratorT(const TContainerType cContainer) : Base(cContainer)
	{
		ElementTraits::construct(m_Element);
	}

	/**
	 * @brief 始端反復子を構築する
	 *
	 * 始端インデックスが終端を指し示していないようなら、要素の保持も行う。
	 *
	 * @param cContainer [in] コンテナ。
	 * @param nFirst [in] 始端インデックス。通常は（逆行反復子であっても）0を指定する。
	 */
	IndexIteratorT(const TContainerType cContainer, long nFirst) : Base(cContainer, nFirst)
	{
		ElementTraits::construct(m_Element);
		if(Base::operator!=(last_iterator())) {
			getElement(&m_Element);
		}
	}

	/**
	 * @brief 反復子を複製する
	 */
	IndexIteratorT(const IndexIteratorT& rIterator) : Base(rIterator)
	{
		ElementTraits::construct(m_Element);
		ElementTraits::copy(m_Element, rIterator.m_Element);
	}

	/**
	 * @brief 反復子を破棄する
	 */
	~IndexIteratorT()
	{
		ElementTraits::reset(m_Element);
	}

// 反復子の挙動
	/**
	 * @brief 反復子を終端側へ一歩進める
	 *
	 * 同時に、要素の保持も行う。
	 */
	IndexIteratorT& operator++()
	{
		ElementTraits::reset(m_Element);
		if(Base::operator++() != last_iterator()) {
			getElement(&m_Element);
		}
		return *this;
	}

	/**
	 * @brief 反復子が指し示している要素を取得する。
	 */
	value_type const& operator*() const
	{
		return m_Element;
	}

// 演算子
	/**
	 * @brief 反復子を複製する
	 */
	IndexIteratorT& operator=(const IndexIteratorT& rIterator)
	{
		IndexIteratorIT<TContainerType, TFore>::operator=(rIterator);
		ElementTraits::reset(m_Element);
		ElementTraits::copy(m_Element, rIterator.m_Element);
		return *this;
	}

protected:
// 処理
	value_type m_Element; ///< 現在指している要素。
};

/**
 * @brief ストリーム反復子用コンテナテンプレート
 *
 * StreamIteratorT の第一テンプレート引数となるクラスのテンプレート。
 *
 * - TIContainer : IFields など、ランダムアクセス可能な要素を格納するインタフェース。
 * - TElement : 要素型。
 * - TGetNextMethod : 次の要素を取り出すメソッド。
 */
template <typename TIContainer, typename TElement, HRESULT (__stdcall TIContainer::*TGetNextMethod)(TElement*)>
struct StreamContainerT
{
public:
	typedef TIContainer ContainerInterface; ///< コンテナインタフェース。
	typedef TElement Element; ///< 要素型。
	typedef element_traits<TElement> ElementTraits; ///< 要素の特性。
	typedef typename ElementTraits::ElementHolder ElementHolder; ///< 要素保持クラス。

	typedef StreamIteratorT<reference_wrapper<StreamContainerT> > iterator; ///< 順方向反復子。

	typedef _COM_SMARTPTR<_COM_SMARTPTR_LEVEL2<TIContainer, &__uuidof(TIContainer)> > ContainerInterfacePtr; ///< スマートポインタ。

// 構築/消滅
	StreamContainerT() :
	m_ipContainer(0)
	{
	}

	/**
	 * @param [in] コンテナインタフェース。
	 */
	StreamContainerT(TIContainer* ipContainer) :
	m_ipContainer(ipContainer)
	{
	}

	/**
	 * @param [in] コンテナインタフェース（スマートポインタ）。
	 */
	StreamContainerT(const ContainerInterfacePtr& ipContainer) :
	m_ipContainer(ipContainer)
	{
	}

// 取得
	/**
	 * @brief 次の要素を取得する
	 *
	 * @param pElement [out] 要素取得先。
	 */
	void getNextElement(Element* pElement)
	{
		if((m_ipContainer->*TGetNextMethod)(pElement) != S_OK) {
			m_ipContainer = 0;
		}
	}

	/**
	 * @brief 順行反復子の始点を取得する
	 */
	iterator begin() const
	{
		return iterator(*this);
	}

	/**
	 * @brief 順行反復子の終点を取得する
	 */
	iterator end() const
	{
		return iterator();
	}

// 比較
	/**
	 * @brief コンテナ同一性判定
	 */
	bool operator == (const StreamContainerT& rContainer) const
	{
		return static_cast<ContainerInterface*>(m_ipContainer) == static_cast<ContainerInterface*>(rContainer.m_ipContainer);
	}

	/**
	 * @brief 無効コンテナ判定
	 */
	bool operator ! () const
	{
		return m_ipContainer == 0;
	}

// 静的関数
	static iterator make_first(StreamContainerT& rContainer)
	{
		return iterator(ref(rContainer));
	}

private:
// 変数
	ContainerInterfacePtr m_ipContainer; ///< コンテナインタフェース。
};

/**
 * @brief ランダムアクセス反復子用コンテナテンプレート
 *
 * IndexIteratorIT の第一テンプレート引数となるクラスのテンプレート。
 *
 * @note begin() end() rbegin() rend() によって生成された反復子は、それを生み出したオブジェクトが破棄された時点で無効となる。
 *
 * - TIContainer : IFields など、ランダムアクセス可能な要素を格納するインタフェース。
 * - TElement : 要素型。
 * - TGetMethod : インデックスを指示して要素を取り出すメソッド。
 * - TCountMethod : 要素数を数えるメソッド。
 */
template <
	typename TIContainer,
	typename TElement,
	HRESULT (__stdcall TIContainer::*TGetMethod)(long, TElement*),
	HRESULT (__stdcall TIContainer::*TCountMethod)(long*)>
class IndexContainerT
{
public:
	typedef TIContainer ContainerInterface; ///< コンテナインタフェース。
	typedef TElement Element; ///< 要素型。
	typedef element_traits<TElement> ElementTraits; ///< 要素の特性。
	typedef typename ElementTraits::ElementHolder ElementHolder; ///< 要素保持クラス。
	typedef IndexIteratorT<reference_wrapper<IndexContainerT const>, true> iterator; ///< 順方向反復子。
	typedef IndexIteratorT<reference_wrapper<IndexContainerT const>, false> reverse_iterator; ///< 逆方向反復子。

	typedef _COM_SMARTPTR<_COM_SMARTPTR_LEVEL2<TIContainer, &__uuidof(TIContainer)> > ContainerInterfacePtr; ///< スマートポインタ。

// 構築/消滅
	/**
	 * @brief ランダムアクセスコンテナ構築
	 *
	 * @param [in] コンテナインタフェース。
	 */
	IndexContainerT(TIContainer* ipContainer) :
	m_ipContainer(ipContainer),
	m_nLast(0)
	{
		if(m_ipContainer) {
			(m_ipContainer->*TCountMethod)(&m_nLast);
		}
	}

	/**
	 * @brief ランダムアクセスコンテナ構築
	 *
	 * @param [in] コンテナインタフェース（スマートポインタ）。
	 */
	IndexContainerT(const ContainerInterfacePtr& ipContainer) :
	m_ipContainer(ipContainer),
	m_nLast(0)
	{
		if(m_ipContainer) {
			(m_ipContainer->*TCountMethod)(&m_nLast);
		}
	}

// 取得
	/**
	 * @brief 終端インデックス（=要素数）を取得する
	 * @return 終端インデックス。
	 */
	long getLast() const
	{
		return m_nLast;
	}

	/**
	 * @brief 要素数（=終端インデックス）を取得する
	 * @return 要素数。
	 */
	long size() const
	{
		return m_nLast;
	}

	/**
	 * @brief 要素が空か判定する。
	 *
	 * @retval true 空。
	 * @retval false 空ではない。
	 */
	bool empty() const
	{
		return size() == 0;
	}

	/**
	 * @brief 要素を取得する
	 *
	 * @param nIndex [in] インデックス。
	 * @param pElement [out] 要素取得先。
	 */
	void getElement(long nIndex, Element* pElement) const
	{
		(m_ipContainer->*TGetMethod)(nIndex, pElement);
	}

	/**
	 * @brief 順行反復子の始点を取得する
	 */
	iterator begin() const
	{
		return iterator(ref(*this), 0);
	}

	/**
	 * @brief 順行反復子の終点を取得する
	 */
	iterator end() const
	{
		return iterator(ref(*this));
	}

	/**
	 * @brief 逆行反復子の始点を取得する
	 */
	reverse_iterator rbegin() const
	{
		return reverse_iterator(ref(*this), 0);
	}

	/**
	 * @brief 逆行反復子の終点を取得する
	 */
	reverse_iterator rend() const
	{
		return reverse_iterator(ref(*this));
	}

// 比較
	/**
	 * @brief コンテナ同一性判定
	 */
	bool operator == (const IndexContainerT& rIndexContainer) const
	{
		return static_cast<ContainerInterface*>(m_ipContainer) == static_cast<ContainerInterface*>(rIndexContainer.m_ipContainer);
	}

// 静的関数
	static iterator make_first(const IndexContainerT& rContainer)
	{
		return iterator(ref(rContainer));
	}

private:
// 変数
	ContainerInterfacePtr m_ipContainer; ///< コンテナインタフェース。
	long m_nLast; ///< 終端インデックス。
};

#if(0)
template<typename T> struct is_container
{
	typedef char yestype;
	typedef struct { char dummy[2]; } notype;
	
	struct helper {
		template <
			typename TIContainer,
			typename TElement,
			HRESULT (__stdcall TIContainer::*TGetNextMethod)(TElement*)>
		helper(const volatile StreamContainerT<TIContainer, TElement, TGetNextMethod>&);

		template <
			typename TIContainer,
			typename TElement,
			HRESULT (__stdcall TIContainer::*TGetMethod)(long, TElement*),
			HRESULT (__stdcall TIContainer::*TCountMethod)(long*)>
		helper(const volatile IndexContainerT<TIContainer, TElement, TGetMethod, TCountMethod>&);
	};

	inline static yestype tester(helper);
	inline static notype tester(...);
	
	inline static T& make_t();
	enum { value = sizeof(tester(make_t()))==sizeof(yestype) };
};


template <typename T>
struct container_type_of
{
	typedef char not_container;
	typedef struct { char dummy[2]; } stream_container;
	typedef struct { char dummy[3]; } index_container;

	template <typename TIContainer, typename TElement, HRESULT (__stdcall TIContainer::*TGetNextMethod)(TElement*)>
	inline static stream_container tester(const StreamContainerT<TIContainer, TElement, TGetNextMethod>&);

	template <
		typename TIContainer,
		typename TElement,
		HRESULT (__stdcall TIContainer::*TGetMethod)(long, TElement*),
		HRESULT (__stdcall TIContainer::*TCountMethod)(long*)>
	inline static index_container tester(const IndexContainerT<TIContainer, TElement, TGetMethod, TCountMethod>&);

	inline static not_container tester(...);
	
	inline static T& make_t();

	enum {
		value = 
			sizeof(tester(make_t())) == sizeof(index_container) ? 2 :
			sizeof(tester(make_t())) == sizeof(stream_container) ? 1 : 0
	};
};
#endif

} // namespace sindy

#if(0)
namespace std {

template <typename TContainerType, bool TFore>
inline input_iterator_tag iterator_category(const sindy::IndexIteratorIT<TContainerType, TFore>&)
{
	return input_iterator_tag();
}

} // namespace std
#endif // namespace std

#endif // __SINDY_ITERATOR_H__	
