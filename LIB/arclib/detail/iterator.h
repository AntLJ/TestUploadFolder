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
 * @brief �R���e�i�v�f�����e���v���[�g
 */
template <typename TElement>
struct element_traits
{
	/**
	 * @brief �C���^�t�F�[�X�|�C���^�ȊO�̗v�f�Ɋւ���ڍׁB
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
	 * @brief �C���^�t�F�[�X�|�C���^�^�v�f�Ɋւ���ڍׁB
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

	typedef detail_t<type_of<TElement>::value> detail_; ///< �v�f�Ɋւ���ڍׁB

// ��`
	typedef typename detail_::element Element; ///< �v�f�^�B
	typedef typename detail_::holder ElementHolder; ///< �v�f�^�I�u�W�F�N�g�B

// ����
	/**
	 * @brief �v�f�̏�����Ԃ��\�z����
	 *
	 * @param rElement [in] �Ώۗv�f�B
	 */
	static void construct(Element& rElement)
	{
		detail_::construct(rElement);
	}

	/**
	 * @brief �v�f�̕������s��
	 *
	 * �f�B�[�v�ł��V�����[�ł��A�D�݂̕��@�ŁB<br>
	 * @a rDstElement �� construct() �ς݂����A reset() ����Ă��邩�͕s���B
	 *
	 * @param rDstElement [out] ������v�f�B
	 * @param rSrcElement [in] �������v�f�B
	 */
	static void copy(Element& rDstElement, const Element& rSrcElement)
	{
		detail_::copy(rDstElement, rSrcElement);
	}

	/**
	 * @brief �v�f��������Ԃɖ߂�
	 *
	 * @param rElement [in] �Ώۗv�f�B
	 */
	static void reset(Element& rElement)
	{
		detail_::reset(rElement);
	}
};
#pragma warning( pop )

/**
 * @brief VARIATN�^�v�f�̓���
 */
template <>
struct element_traits<VARIANT>
{
// ��`
	typedef VARIANT Element; ///< �v�f�^�B
	typedef _variant_t ElementHolder; ///< �v�f�^�I�u�W�F�N�g�B

// ����
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
 * @brief SiNDY�n�����q�p�A��ΏI�[�����q
 */
class last_iterator {};

/**
 * @brief ���o����������q
 *
 * ICursor �ȂǁA�����߂��s�ȃX�g���[���̔����q�B
 *
 * - �e���v���[�g����
 *   - TContainerType : StreamContainerT ���A���̔h���N���X�B
 *   - TElementTraits : �R���e�i�ɐς܂ꂽ�v�f�̓����B element_traits ���A���̌݊��N���X�B
 */
template <typename TContainerType>
class StreamIteratorT : public std::iterator<std::input_iterator_tag, typename container_traits<TContainerType>::Element>
{
public:
// ��`
	typedef typename container_traits<TContainerType>::ElementTraits ElementTraits;
	typedef typename container_traits<TContainerType>::ElementHolder ElementHolder;

// �\�z/����
	/**
	 * @brief �I�[�����q���\�z����
	 */
	StreamIteratorT()
	{
		ElementTraits::construct(m_Element);
	}

	/**
	 * @brief �n�[�����q���\�z����
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
	 * @brief �n�[�����q���\�z����
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
	 * @brief �����q�𕡐��\�z����
	 */
	StreamIteratorT(const StreamIteratorT& rIterator)
	{
		ElementTraits::construct(m_Element);
		ElementTraits::copy(m_Element);
	}

	/**
	 * @brief �����q��j������
	 */
	~StreamIteratorT()
	{
		ElementTraits::reset(m_Element);
	}

// �����q�̋���
	/**
	 * @brief �����q�̏I�[����
	 *
	 * @param it [in] �����q�I�[�I�u�W�F�N�g�B
	 * @retval true �I�[�B
	 * @retval false �I�[�ł͂Ȃ��B
	 */
	bool operator==(last_iterator)
	{
		return ! m_Container;
	}

	/**
	 * @brief �����q�̔�I�[����
	 *
	 * @param it [in] �����q�I�[�I�u�W�F�N�g�B
	 * @retval true �I�[�ł͂Ȃ��B
	 * @retval false �I�[�B
	 */
	bool operator!=(last_iterator it)
	{
		return ! operator==(it);
	}

	/**
	 * @brief �I�[�����q�Ƃ̓��ꐫ����
	 *
	 * @param rIt [in] �I�[�����q�B
	 * @retval true �I�[�������Ă���B
	 * @retval false �I�[�������Ă͂��Ȃ��B
	 */
	bool operator==(const StreamIteratorT& rIt)
	{
		return ! m_Container;
	}

	/**
	 * @brief �I�[�����q�Ƃ̔񓯈ꐫ����
	 *
	 * @param rIt [in] �I�[�����q�B
	 * @retval true �I�[�������Ă͂��Ȃ��B
	 * @retval false �I�[�������Ă���B
	 */
	bool operator!=(const StreamIteratorT& rIt)
	{
		return ! operator==(rIt);
	}

	/**
	 * @brief �����q���I�[���ֈ���i�߂�
	 */
	StreamIteratorT& operator++()
	{
		// �v�f�����A�X�g���[�����P�i�߁A�v�f���擾����
		ElementTraits::reset(m_Element);
		static_cast<ContainerType&>(m_Container).getNextElement(&m_Element);
		return *this;
	}

	/**
	 * @brief �����q���w�������Ă���v�f���擾����B
	 */
	value_type const& operator*()
	{
		return m_Element;
	}

	/**
	 * @brief �����q�𕡐�����
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
// �ϐ�
	TContainerType m_Container; ///< �R���e�i�ւ̃|�C���^�B
	value_type m_Element; ///< ���ݎw���Ă���v�f�B
};

//////////////////////////////////////////////////////////////////////
// �����_���A�N�Z�X�����q
//////////////////////////////////////////////////////////////////////

/**
 * @brief �����_���A�N�Z�X�����q�C���^�t�F�[�X
 *
 * IPointCollection �ȂǁA0�`n�̗v�f�Ƀ����_���A�N�Z�X�\�ȃC���^�t�F�[�X�p�����q�̃C���^�t�F�[�X�B<br>
 * ��������鋓���͈ȉ��̂Ƃ���B
 * - �v�f���̕ێ�
 * - �����q�̏��s�E�t�s
 * - �����q�̓��ꐫ��r
 * - �����q�̏I�[����
 * - �����q���w�������l�̎擾
 * �v�f�̕ێ��͔h���N���X���ōs���B
 * 
 * - �e���v���[�g����
 *   - TContainerType : IndexContainerT ���A���̔h���N���X�B���邢�͂���ref�B
 *   - TFore : �������H
 */
template <typename TContainerType, bool TFore>
class IndexIteratorIT : public std::iterator<std::input_iterator_tag, typename container_traits<TContainerType>::Element>
{
public:
// ��`
	typedef typename container_traits<TContainerType>::ElementTraits ElementTraits;
	typedef typename container_traits<TContainerType>::ElementHolder ElementHolder;

// �\�z
	/**
	 * @brief �I�[�����q���\�z����
	 *
	 * @param cContainer [in] �R���e�i�B
	 */
	IndexIteratorIT(const TContainerType& rContainer) :
	m_Container(rContainer)
	{
		m_nIndex = static_cast<ContainerType const&>(m_Container).getLast();
	}

	/**
	 * @brief �n�[�����q���\�z����
	 *
	 * @param cContainer [in] �R���e�i�B
	 * @param nFirst [in] �n�[�C���f�b�N�X�B�ʏ�́i�t�s�����q�ł����Ă��j0���w�肷��B
	 */
	IndexIteratorIT(const TContainerType& rContainer, long nFirst) :
	m_Container(rContainer),
	m_nIndex(nFirst)
	{
	}

	/**
	 * @brief �����q�𕡐��\�z����
	 */
	IndexIteratorIT(const IndexIteratorIT&  rIt) :
	m_Container(rIt.m_Container),
	m_nIndex(rIt.m_nIndex)
	{
	}

// �擾
	/**
	 * @brief �����q���w�������Ă���v�f���擾����B
	 *
	 * @param pElement [out] �v�f�o�͐�B
	 */
	void getElement(value_type* pElement)
	{
		static_cast<ContainerType const&>(m_Container).getElement(getIndex(), pElement);
	}

// �����q�̋���
	/**
	 * @brief �����q���I�[���ֈ���i�߂�
	 */
	IndexIteratorIT& operator++()
	{
		++m_nIndex;
		return *this;
	}

	/**
	 * @brief �����q���n�[���ֈ���߂�
	 */
	IndexIteratorIT& operator--()
	{
		--m_nIndex;
		return *this;
	}

	/**
	 * @brief �����q�̏I�[����
	 *
	 * @param it [in] �����q�I�[�I�u�W�F�N�g�B
	 * @retval true �I�[�B
	 * @retval false �I�[�ł͂Ȃ��B
	 */
	bool operator==(const last_iterator)
	{
		return m_nIndex == static_cast<ContainerType const&>(m_Container).getLast();
	}

	/**
	 * @brief �����q�̔�I�[����
	 *
	 * @param it [in] �����q�I�[�I�u�W�F�N�g�B
	 * @retval true �I�[�ł͂Ȃ��B
	 * @retval false �I�[�B
	 */
	bool operator!=(const last_iterator it) { return ! operator==(it); }

	/**
	 * @brief �����q���ꐫ����
	 *
	 * @param rIt [in] ��r�Ώ۔����q�B
	 * @retval true �������q�͓���ł���B
	 * @retval false �������q�͓���ł͂Ȃ��B
	 */
	bool operator==(const IndexIteratorIT& rIt)
	{
		return static_cast<ContainerType const&>(m_Container) == static_cast<ContainerType const&>(rIt.m_Container) && m_nIndex == rIt.m_nIndex;
	}

	/**
	 * @brief �����q�񓯈ꐫ����
	 *
	 * @param rIt [in] ��r�Ώ۔����q�B
	 * @retval true �������q�͓���ł͂Ȃ��B
	 * @retval false �������q�͓���ł���B
	 */
	bool operator!=(const IndexIteratorIT& rIt)
	{
		return ! operator==(rIt);
	}

protected:
	typedef typename unwrap_reference<TContainerType>::type ContainerType;

private:
// ��`
	/**
	 * @brief �����q�̌����ɉ����ăC���f�b�N�X���v�Z����t�@���N�^�e���v���[�g
	 */
	template <bool T> struct GetIndex {};

	/**
	 * @brief �C���f�b�N�X�v�Z�t�@���N�^���������p�ɓ��ꉻ
	 */
	template <> struct GetIndex<true> { long operator()(long nIndex, long) { return nIndex; } };

	/**
	 * @brief �C���f�b�N�X�v�Z�t�@���N�^���t�����p�ɓ��ꉻ
	 */
	template <> struct GetIndex<false> { long operator()(long nIndex, long nLast) { return nLast - nIndex - 1; } };

// �擾
	/**
	 * @brief �����q�����ݎw�������Ă���v�f�̃C���f�b�N�X���擾����
	 *
	 * @return �C���f�b�N�X�B
	 */
	long getIndex()
	{
		return GetIndex<TFore>()(m_nIndex, static_cast<ContainerType const&>(m_Container).getLast());
	}

// �ϐ�
	const TContainerType m_Container; ///< �R���e�i�ւ̃|�C���^�B
	long m_nIndex; ///< ���ݎw���Ă���C���f�b�N�X�i�������̏ꍇ�j�B
};

/**
 * @brief �����_���A�N�Z�X�����q
 *
 *   - TContainerType : IndexContainerT ���A���̔h���N���X�B���邢�͂���ref�B
 * - TFore : �������H
 * - TElementTraits : �R���e�i�ɐς܂ꂽ�v�f�̓����B element_traits ���A���̌݊��N���X�B
 */
template <typename TContainerType, bool TFore = true>
class IndexIteratorT : public IndexIteratorIT<TContainerType, TFore>
{
public:
// ��`
	typedef IndexIteratorIT<TContainerType, TFore> Base;

// �\�z/����
	/**
	 * @brief �I�[�����q���\�z����
	 *
	 * @param cContainer [in] �R���e�i�B
	 */
	IndexIteratorT(const TContainerType cContainer) : Base(cContainer)
	{
		ElementTraits::construct(m_Element);
	}

	/**
	 * @brief �n�[�����q���\�z����
	 *
	 * �n�[�C���f�b�N�X���I�[���w�������Ă��Ȃ��悤�Ȃ�A�v�f�̕ێ����s���B
	 *
	 * @param cContainer [in] �R���e�i�B
	 * @param nFirst [in] �n�[�C���f�b�N�X�B�ʏ�́i�t�s�����q�ł����Ă��j0���w�肷��B
	 */
	IndexIteratorT(const TContainerType cContainer, long nFirst) : Base(cContainer, nFirst)
	{
		ElementTraits::construct(m_Element);
		if(Base::operator!=(last_iterator())) {
			getElement(&m_Element);
		}
	}

	/**
	 * @brief �����q�𕡐�����
	 */
	IndexIteratorT(const IndexIteratorT& rIterator) : Base(rIterator)
	{
		ElementTraits::construct(m_Element);
		ElementTraits::copy(m_Element, rIterator.m_Element);
	}

	/**
	 * @brief �����q��j������
	 */
	~IndexIteratorT()
	{
		ElementTraits::reset(m_Element);
	}

// �����q�̋���
	/**
	 * @brief �����q���I�[���ֈ���i�߂�
	 *
	 * �����ɁA�v�f�̕ێ����s���B
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
	 * @brief �����q���w�������Ă���v�f���擾����B
	 */
	value_type const& operator*() const
	{
		return m_Element;
	}

// ���Z�q
	/**
	 * @brief �����q�𕡐�����
	 */
	IndexIteratorT& operator=(const IndexIteratorT& rIterator)
	{
		IndexIteratorIT<TContainerType, TFore>::operator=(rIterator);
		ElementTraits::reset(m_Element);
		ElementTraits::copy(m_Element, rIterator.m_Element);
		return *this;
	}

protected:
// ����
	value_type m_Element; ///< ���ݎw���Ă���v�f�B
};

/**
 * @brief �X�g���[�������q�p�R���e�i�e���v���[�g
 *
 * StreamIteratorT �̑��e���v���[�g�����ƂȂ�N���X�̃e���v���[�g�B
 *
 * - TIContainer : IFields �ȂǁA�����_���A�N�Z�X�\�ȗv�f���i�[����C���^�t�F�[�X�B
 * - TElement : �v�f�^�B
 * - TGetNextMethod : ���̗v�f�����o�����\�b�h�B
 */
template <typename TIContainer, typename TElement, HRESULT (__stdcall TIContainer::*TGetNextMethod)(TElement*)>
struct StreamContainerT
{
public:
	typedef TIContainer ContainerInterface; ///< �R���e�i�C���^�t�F�[�X�B
	typedef TElement Element; ///< �v�f�^�B
	typedef element_traits<TElement> ElementTraits; ///< �v�f�̓����B
	typedef typename ElementTraits::ElementHolder ElementHolder; ///< �v�f�ێ��N���X�B

	typedef StreamIteratorT<reference_wrapper<StreamContainerT> > iterator; ///< �����������q�B

	typedef _COM_SMARTPTR<_COM_SMARTPTR_LEVEL2<TIContainer, &__uuidof(TIContainer)> > ContainerInterfacePtr; ///< �X�}�[�g�|�C���^�B

// �\�z/����
	StreamContainerT() :
	m_ipContainer(0)
	{
	}

	/**
	 * @param [in] �R���e�i�C���^�t�F�[�X�B
	 */
	StreamContainerT(TIContainer* ipContainer) :
	m_ipContainer(ipContainer)
	{
	}

	/**
	 * @param [in] �R���e�i�C���^�t�F�[�X�i�X�}�[�g�|�C���^�j�B
	 */
	StreamContainerT(const ContainerInterfacePtr& ipContainer) :
	m_ipContainer(ipContainer)
	{
	}

// �擾
	/**
	 * @brief ���̗v�f���擾����
	 *
	 * @param pElement [out] �v�f�擾��B
	 */
	void getNextElement(Element* pElement)
	{
		if((m_ipContainer->*TGetNextMethod)(pElement) != S_OK) {
			m_ipContainer = 0;
		}
	}

	/**
	 * @brief ���s�����q�̎n�_���擾����
	 */
	iterator begin() const
	{
		return iterator(*this);
	}

	/**
	 * @brief ���s�����q�̏I�_���擾����
	 */
	iterator end() const
	{
		return iterator();
	}

// ��r
	/**
	 * @brief �R���e�i���ꐫ����
	 */
	bool operator == (const StreamContainerT& rContainer) const
	{
		return static_cast<ContainerInterface*>(m_ipContainer) == static_cast<ContainerInterface*>(rContainer.m_ipContainer);
	}

	/**
	 * @brief �����R���e�i����
	 */
	bool operator ! () const
	{
		return m_ipContainer == 0;
	}

// �ÓI�֐�
	static iterator make_first(StreamContainerT& rContainer)
	{
		return iterator(ref(rContainer));
	}

private:
// �ϐ�
	ContainerInterfacePtr m_ipContainer; ///< �R���e�i�C���^�t�F�[�X�B
};

/**
 * @brief �����_���A�N�Z�X�����q�p�R���e�i�e���v���[�g
 *
 * IndexIteratorIT �̑��e���v���[�g�����ƂȂ�N���X�̃e���v���[�g�B
 *
 * @note begin() end() rbegin() rend() �ɂ���Đ������ꂽ�����q�́A����𐶂ݏo�����I�u�W�F�N�g���j�����ꂽ���_�Ŗ����ƂȂ�B
 *
 * - TIContainer : IFields �ȂǁA�����_���A�N�Z�X�\�ȗv�f���i�[����C���^�t�F�[�X�B
 * - TElement : �v�f�^�B
 * - TGetMethod : �C���f�b�N�X���w�����ėv�f�����o�����\�b�h�B
 * - TCountMethod : �v�f���𐔂��郁�\�b�h�B
 */
template <
	typename TIContainer,
	typename TElement,
	HRESULT (__stdcall TIContainer::*TGetMethod)(long, TElement*),
	HRESULT (__stdcall TIContainer::*TCountMethod)(long*)>
class IndexContainerT
{
public:
	typedef TIContainer ContainerInterface; ///< �R���e�i�C���^�t�F�[�X�B
	typedef TElement Element; ///< �v�f�^�B
	typedef element_traits<TElement> ElementTraits; ///< �v�f�̓����B
	typedef typename ElementTraits::ElementHolder ElementHolder; ///< �v�f�ێ��N���X�B
	typedef IndexIteratorT<reference_wrapper<IndexContainerT const>, true> iterator; ///< �����������q�B
	typedef IndexIteratorT<reference_wrapper<IndexContainerT const>, false> reverse_iterator; ///< �t���������q�B

	typedef _COM_SMARTPTR<_COM_SMARTPTR_LEVEL2<TIContainer, &__uuidof(TIContainer)> > ContainerInterfacePtr; ///< �X�}�[�g�|�C���^�B

// �\�z/����
	/**
	 * @brief �����_���A�N�Z�X�R���e�i�\�z
	 *
	 * @param [in] �R���e�i�C���^�t�F�[�X�B
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
	 * @brief �����_���A�N�Z�X�R���e�i�\�z
	 *
	 * @param [in] �R���e�i�C���^�t�F�[�X�i�X�}�[�g�|�C���^�j�B
	 */
	IndexContainerT(const ContainerInterfacePtr& ipContainer) :
	m_ipContainer(ipContainer),
	m_nLast(0)
	{
		if(m_ipContainer) {
			(m_ipContainer->*TCountMethod)(&m_nLast);
		}
	}

// �擾
	/**
	 * @brief �I�[�C���f�b�N�X�i=�v�f���j���擾����
	 * @return �I�[�C���f�b�N�X�B
	 */
	long getLast() const
	{
		return m_nLast;
	}

	/**
	 * @brief �v�f���i=�I�[�C���f�b�N�X�j���擾����
	 * @return �v�f���B
	 */
	long size() const
	{
		return m_nLast;
	}

	/**
	 * @brief �v�f���󂩔��肷��B
	 *
	 * @retval true ��B
	 * @retval false ��ł͂Ȃ��B
	 */
	bool empty() const
	{
		return size() == 0;
	}

	/**
	 * @brief �v�f���擾����
	 *
	 * @param nIndex [in] �C���f�b�N�X�B
	 * @param pElement [out] �v�f�擾��B
	 */
	void getElement(long nIndex, Element* pElement) const
	{
		(m_ipContainer->*TGetMethod)(nIndex, pElement);
	}

	/**
	 * @brief ���s�����q�̎n�_���擾����
	 */
	iterator begin() const
	{
		return iterator(ref(*this), 0);
	}

	/**
	 * @brief ���s�����q�̏I�_���擾����
	 */
	iterator end() const
	{
		return iterator(ref(*this));
	}

	/**
	 * @brief �t�s�����q�̎n�_���擾����
	 */
	reverse_iterator rbegin() const
	{
		return reverse_iterator(ref(*this), 0);
	}

	/**
	 * @brief �t�s�����q�̏I�_���擾����
	 */
	reverse_iterator rend() const
	{
		return reverse_iterator(ref(*this));
	}

// ��r
	/**
	 * @brief �R���e�i���ꐫ����
	 */
	bool operator == (const IndexContainerT& rIndexContainer) const
	{
		return static_cast<ContainerInterface*>(m_ipContainer) == static_cast<ContainerInterface*>(rIndexContainer.m_ipContainer);
	}

// �ÓI�֐�
	static iterator make_first(const IndexContainerT& rContainer)
	{
		return iterator(ref(rContainer));
	}

private:
// �ϐ�
	ContainerInterfacePtr m_ipContainer; ///< �R���e�i�C���^�t�F�[�X�B
	long m_nLast; ///< �I�[�C���f�b�N�X�B
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
