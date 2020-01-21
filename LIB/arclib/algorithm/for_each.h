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

#ifndef __SINDY_ALGORITHM_FOR_EACH_H__
#define __SINDY_ALGORITHM_FOR_EACH_H__

namespace sindy {

// �ėpfor_each

template <typename TFunction, typename TContainer>
TFunction for_each(TContainer pContainer, TFunction f)
{
	typedef container_traits<TContainer> __tr;

	for(__tr::iterator first = __tr::make_first(pContainer); first != last_iterator(); ++first) {
		f(*first);
	}

	return f;
}

template <typename TFunction, typename TPredicate, typename TContainer>
TFunction for_each_if(TContainer pContainer, TPredicate pred, TFunction f)
{
	typedef container_traits<TContainer> __tr;

	for(__tr::iterator first = __tr::make_first(pContainer); first != last_iterator(); ++first) {
		if(pred(*first)) {
			f(*first);
		}
	}

	return f;
}

template <typename TElement, typename TFunction, typename TContainer>
TFunction for_each_element(TContainer pContainer, TFunction f)
{
	typedef traits<TContainer>::accessor<TElement> __ac;

	for(__ac::iterator first = __ac::make_first(pContainer); first != last_iterator(); ++first) {
		f(*first);
	}

	return f;
}

template <typename TElement, typename TFunction, typename TContainer>
TFunction for_each_sole_element(TContainer pContainer, TFunction f)
{
	typedef traits<TContainer>::accessor<TElement> __ac;

	for(__ac::sole_iterator first = __ac::make_sole_first(pContainer); first != last_iterator(); ++first) {
		f(*first);
	}

	return f;
}

/**
 * @brief Subset�Ƃ��đ��݂�����̂��܂ށi������Subset�������̎��g�͏����j�S�Ă�Dataset�ɑ΂���f(IDataset*)��K�p����
 *
 * @note boost::call_traits���C���N���[�h����Ă����Function�ɎQ�ƌ^���g����悤�ɂȂ�B
 * @note Function��for_each()�����̉����邽�߂̃e���v���[�g�����Ƃ��Ďg���邽�߁A�֐����N���X���g�����Ƃ͏o���Ȃ��B
 */
template<typename Function>
inline Function for_each_sole(IEnumDataset* ipEnumDataset, Function f)
{
	struct Local
	{
		template<typename T>
		class Delegator
		{
		public:
			Delegator(T f) :
			m_rFunction(f)
			{
			}

			void operator()(IDataset* ipDataset)
			{
				IEnumDatasetPtr ipEnumDataset;
				if(ipDataset->get_Subsets(&ipEnumDataset) == S_OK && ipEnumDataset != 0) {
					for_each(static_cast<IEnumDataset* const>(ipEnumDataset), *this);
				}
				else {
					m_rFunction(static_cast<IDataset* const>(ipDataset));
				}
			}

		private:
			T m_rFunction;
		};
	};

	for_each(ipEnumDataset, Local::Delegator<_REFERENCE_TO(Function)>(f));

	return f;
}

/**
 * @brief Subset�Ƃ��đ��݂�����̂��܂ށi������Subset�������̎��g�͏����j�S�Ă�DatasetName�ɑ΂���f(IDatasetName*)��K�p����
 *
 * @note boost::call_traits���C���N���[�h����Ă����Function�ɎQ�ƌ^���g����悤�ɂȂ�B
 * @note Function��for_each()�����̉����邽�߂̃e���v���[�g�����Ƃ��Ďg���邽�߁A�֐����N���X���g�����Ƃ͏o���Ȃ��B
 */
template<typename Function>
inline Function for_each_sole(IEnumDatasetName* ipEnumDatasetName, Function f)
{
// �֐�����`
	struct Local
	{
		template<typename T>
		class Delegator
		{
		public:
			Delegator(T f) :
			m_rFunction(f)
			{
			}

			void operator()(IDatasetName* ipDatasetName)
			{
				IEnumDatasetNamePtr ipEnumDatasetName;
				if(ipDatasetName->get_SubsetNames(&ipEnumDatasetName) == S_OK && ipEnumDatasetName != 0) {
					for_each(static_cast<IEnumDatasetName* const>(ipEnumDatasetName), *this);
				}
				else {
					m_rFunction(static_cast<IDatasetName* const>(ipDatasetName));
				}
			}

		private:
			T m_rFunction;
		};
	};

// �{����
	for_each(ipEnumDatasetName, Local::Delegator<_REFERENCE_TO(Function)>(f));
	return f;
}

/**
 * @brief Subset�Ƃ��đ��݂�����̂��܂ޑS�Ă�DatasetName�ɑ΂���f(IDatasetName*)��K�p����
 *
 * @note boost::call_traits���C���N���[�h����Ă����Function�ɎQ�ƌ^���g����悤�ɂȂ�B
 * @note Function��for_each()�����̉����邽�߂̃e���v���[�g�����Ƃ��Ďg���邽�߁A�֐����N���X���g�����Ƃ͏o���Ȃ��B
 */
template<typename Function>
inline Function for_each_nest(IEnumDatasetName* ipEnumDatasetName, Function f)
{
	/// �֐����N���X��`�p�\����
	struct Local
	{
		template<typename T>
		class Delegator
		{
		public:
			Delegator(T f) :
			m_rFunction(f)
			{
			}

			void operator()(IDatasetName* ipDatasetName)
			{
				m_rFunction(static_cast<IDatasetName* const>(ipDatasetName));
				IEnumDatasetNamePtr ipEnumDatasetName;
				if(ipDatasetName->get_SubsetNames(&ipEnumDatasetName) == S_OK && ipEnumDatasetName != 0) {
					for_each(static_cast<IEnumDatasetName* const>(ipEnumDatasetName), *this);
				}
			}

		private:
			T m_rFunction;
		};
	};

	for_each(ipEnumDatasetName, Local::Delegator<_REFERENCE_TO(Function)>(f));

	return f;
}

} // namespace sindy

#endif // __SINDY_ALGORITHM_FOR_EACH_H__
