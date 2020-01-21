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

/**
 * @file SiNDYFieldValue.h
 * @brief �����l���擾�E�ێ��N���X��`
 * @author �n�}DB���암���H����O���[�v �e�n�����Y
 * $Id$
 */
#ifndef _SINDY_FIELDVALUE_H_
#define _SINDY_FIELDVALUE_H_

#include "SiNDYFieldIndex.h"
#include "SiNDYValueGetter.h"
#include "SiNDYAlgorithm.h"

namespace sindy {

template <LPCTSTR TFieldName>
struct FieldValueB {};

template<typename T, LPCTSTR TFieldName>
struct inheres_fieldvalue
{
	typedef char yestype;
	typedef struct { char dummy[2]; } notype;
	
	struct helper { helper(const volatile FieldValueB<TFieldName>&); };

	inline static yestype tester(helper);
	inline static notype tester(...);
	
	inline static T& make_t();
	enum { value = sizeof(tester(make_t()))==sizeof(yestype) };
};

/**
 * @brief �����l�擾�E�ێ��N���X
 */
template <LPCTSTR TFieldName, typename TValueType = _variant_t, bool TNullZero = false>
class FieldValue : public FieldValueB<TFieldName>
{
public:
// ��`
	typedef TValueType ValueType;

	/**
	 * @note typdef�Œ�`����ƌp�����ɕs����N����̂ŁA�����Čp�����g�p�B
	 */
	class FieldIndex : public sindy::FieldIndex<TFieldName>
	{
	public:
		template <typename T>
		FieldIndex(T const& rArg) : sindy::FieldIndex<TFieldName>(rArg)
		{
		}
	};

// �\�z
	FieldValue()
	{
	}

	/// FieldValue�I�u�W�F�N�g���A _IRow* (IFeature*) ��FieldIndex�����o����I�u�W�F�N�g�ɂ��\�z
	template<typename TRow>
	FieldValue(TRow const& rRow) :
	m_Value(Construct<inheres_fieldvalue<TRow, TFieldName>::value>()(rRow))
	{
	}

	/// FieldValue�I�u�W�F�N�g���A _IRow* (IFeature*) �����o����I�u�W�F�N�g�ƁAFieldIndex�I�u�W�F�N�g�ɂ��\�z
	template<typename TRow, typename TFieldIndex>
	FieldValue(TRow const& rRow, TFieldIndex const& rFieldIndex) :
	m_Value(Construct<inheres_fieldvalue<TRow, TFieldName>::value>()(rRow, rFieldIndex))
	{
	}

	/// �����l�̒��ڎw��ɂ��\�z
	FieldValue(const TValueType& rValue) :
	m_Value(rValue)
	{
	}

// �擾
	TValueType& value()
	{
		return m_Value;
	}

	const TValueType& getValue() const
	{
		return m_Value;
	}

// ���/�擾���Z
	template<typename TRow>
	FieldValue& operator = (TRow const& rRow)
	{
		m_Value = Construct<inheres_fieldvalue<TRow, TFieldName>::value>()(rRow);
		return *this;
	}

	FieldValue& operator = (const TValueType& rValue)
	{
		m_Value = rValue;
		return *this;
	}

	/// ��ώQ�ƌ`���ő����l��Ԃ�
	operator const TValueType& () const
	{
		return m_Value;
	}

// �ÓI�ϐ�
	static LPCTSTR fieldName()
	{
		return TFieldName;
	}

private:
	/**
	 * @brief �\�z�⍲�N���X
	 *
	 * @par �e���v���[�g����:
	 * TCopy : �R�s�[�R���X�g���N�g���B 
	 */
	template <bool TCopy>
	class Construct
	{
	public:
	// ��`
		/**
		 * @brief �����擾�⍲�N���X
		 *
		 * @par �e���v���[�g����:
		 * T : VT_NULL ��0�Ɖ��߂��邩�B
		 */
		template <bool T>
		class GetValue
		{
		public:
			/**
			 * @brief TRow����IRow��FieldIndex����ӂɎ��o�����ꍇ�̏���
			 *
			 * @param ipRow [in] �����l�ێ��ҁB
			 * @param rFieldIndex [in] �����t�B�[���h�C���f�b�N�X�B
			 * @return �����l�B
			 */
			TValueType operator()(_IRow* ipRow, const sindy::FieldIndex<TFieldName>& rFieldIndex)
			{
				return sindy::getValue(ipRow, static_cast<long>(rFieldIndex));
			}

			/**
			 * @brief TRow����IFeature��FieldIndex����ӂɎ��o�����ꍇ�̏���
			 *
			 * @param ipFeature [in] �����l�ێ��ҁB
			 * @param rFieldIndex [in] �����t�B�[���h�C���f�b�N�X�B
			 * @return �����l�B
			 */
			TValueType operator()(IFeature* ipFeature, const sindy::FieldIndex<TFieldName>& rFieldIndex)
			{
				return sindy::getValue(ipFeature, static_cast<long>(rFieldIndex));
			}
		};

		template <>
		class GetValue<true>
		{
		public:
			TValueType operator()(_IRow* ipRow, const sindy::FieldIndex<TFieldName>& rFieldIndex)
			{
				_variant_t va;
				sindy::getValue(ipRow, static_cast<long>(rFieldIndex), va);

				if(va.vt == VT_NULL) { va = 0L; }

				return va;
			}

			TValueType operator()(IFeature* ipFeature, const sindy::FieldIndex<TFieldName>& rFieldIndex)
			{
				_variant_t va;
				sindy::getValue(ipFeature, static_cast<long>(rFieldIndex), va);

				if(va.vt == VT_NULL) { va = 0L; }

				return va;
			}
		};

	// ����
		template <typename TRow>
		TValueType operator()(TRow const& rRow)
		{
			return GetValue<TNullZero>()(rRow, rRow);
		}


		template <typename TRow, typename TFieldIndex>
		TValueType operator()(TRow const& rRow, TFieldIndex const& rFieldIndex)
		{
			return GetValue<TNullZero>()(rRow, rFieldIndex);
		}
	};

	template <>
	class Construct<true>
	{
	public:
	// ����
		template <typename TRow>
		TValueType operator()(TRow const& rRow)
		{
			return get<TFieldName>(rRow);
		}

		template <typename TRow, typename TFieldIndex>
		TValueType operator()(TRow const& rRow, TFieldIndex const&)
		{
			return get<TFieldName>(rRow);
		}
	};

// �ϐ�
	TValueType m_Value; ///< �����l
};

//////////////////////////////////////////////////////////////////////////////////////////
//
// fieldvalue_cast
//
//////////////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup fieldvalue_cast FieldValue�L���X�g�֐�
 *
 * FieldValue���d�p���N���X����A������ׂ�FieldValue�𔲂��o���B<BR>
 * �����I�ȏ����͉�����`�����A�Öق̌^�ϊ��Ɉˑ�����B
 */
/*@{*/

/**
 * @brief �����t�B�[���h���ƌ^���w�肵�āA�Y������FieldValue�^�ւ̃L���X�g�����݂�֐��B
 */
template <LPCTSTR TName, typename TType>
inline const FieldValue<TName, TType>& fieldvalue_cast(const FieldValue<TName, TType>& rFieldValues)
{
	return rFieldValues;
}

/**
 * @brief �����t�B�[���h���ƌ^�ANULL��0�Ɖ��߂��邩�ۂ����w�肵�āA�Y������FieldValue�^�ւ̃L���X�g�����݂�֐��B
 */
template <LPCTSTR TName, typename TType, bool TNullZero>
inline const FieldValue<TName, TType, TNullZero>& fieldvalue_cast(const FieldValue<TName, TType, TNullZero>& rFieldValues)
{
	return rFieldValues;
}

/*@}*/

/**
 * @defgroup fieldvalue_mutable_cast FieldValue�σL���X�g�֐�
 *
 * @note FieldValue���d�p���N���X����A������ׂ�FieldValue���ό`���Ŕ����o���B
 */
/*@{*/

template <LPCTSTR TName, typename TType>
inline FieldValue<TName, TType>& fieldvalue_mutable_cast(FieldValue<TName, TType>& rFieldValues)
{
	return rFieldValues;
}

/*@}*/

/**
 * @defgroup fieldvalue_get VC6�p�́A�������o���֐��Q
 *
 * @note VC6�̃e���v���[�g��͋@�\�ł͂��ꂪ����t�B
 * @note �����l�̌^���e���v���[�g�����ɂ���ƁA��Ɍ^�w�肪�K�v�ɂȂ�̂ŋp���B
 */
/*@{*/

template <LPCTSTR TFieldName> long get(const FieldValue<TFieldName, long>& rFieldValue) { return rFieldValue; }
template <LPCTSTR TFieldName> double get(const FieldValue<TFieldName, double>& rFieldValue) { return rFieldValue; }
template <LPCTSTR TFieldName> _variant_t get(const FieldValue<TFieldName, _variant_t>& rFieldValue) { return rFieldValue; }

/*@}*/

} // namespace sindy

//////////////////////////////////////////////////////////////////////////////////////////
//
// SINDY_FIELDVALUE_CLASS
//
//////////////////////////////////////////////////////////////////////////////////////////

#include "SiNDYInheritMacro.h"

/**
 * @brief FieldValue���d�p���N���X�݌v�x���}�N��
 *
 * ���̃}�N���ɂ���Đ������ꂽ�N���X�͈ȉ��̃����o�����B
 * - �N���X
 *   - FieldIndex �p������FieldValue�̃����o�N���X�ł���FieldIndex�����ׂČp�������N���X�B
 * - ���\�b�h
 *   - c(); �f�t�H���g�R���X�g���N�^�B
 *   - c(TRow const& rRow); �p�������S�Ă�FieldValue�ɃR���X�g���N�g�B
 *   - c& operator = (TRow const& rRow); �p�������S�Ă�FieldValue�ɑ�����Z�B
 *
 * - �ÓI�֐�
 * @code
 * template <typename _Functor, typename TRow>
 * static void distribute(TRow const& rRow, _Functor* pFunctor); 
 * @endcode
 * (*pFunctor)(����1, ����2, ... ����n�j��K�p����B
 *
 * @code
 * templat <typename TFunctor, typename TRow>
 * static void for_each(TRow const& rRow, TFunctor f);
 * @endcode
 * f(FieldValue)���J��Ԃ��K�p����B
 * 
 * @param c �N���X���B
 * @param n tuple�ɗ񋓂���Ă����{�N���X���B 
 * @param tuple �J���}��؂�̊�{�N���X�ꗗ�����ʂł����������́B��{�N���X�̕\�L�͈ȉ��ɏ]���B<br>
 *   <table>
 *	   <tr><td>_(�q��{�N���X���r)</td><td>�p������N���X���𒼐ڎw��B</td></tr>
 *	   <tr><td>_1(�q�����t�B�[���h���r)</td><td>FieldValue<�q�����t�B�[���h���r>���p������B</td></tr>
 *	   <tr><td>_2(�q�����t�B�[���h���r,�q�����^���r)</td><td>FieldValue<�q�����t�B�[���h���r,�q�����^���r>���p������B</td></tr>
 *   </table>
 */
#define SINDY_FIELDVALUE_CLASS(c, n, tuple) SINDY_FIELDVALUE_CLASS_I(c, n, _LIST_##n tuple)

#define SINDY_FIELDVALUE_CLASS_I(c, n, list) \
class c : _SINDY_INHERITLIST_UNFOLD_##n(_SINDY_FIELDVALUE_INHERIT, _COMMA, list) \
{ \
public: \
	SINDY_FIELDINDEX_CLASS_D(FieldIndex, n, (_SINDY_INHERITLIST_UNFOLD_##n(_SINDY_FIELDVALUE_FIELDINDEX, _COMMA, list))); \
	\
	c() {} \
\
	template<typename TRow> \
	c(TRow const& rRow) : _SINDY_INHERITLIST_UNFOLD_##n(_SINDY_FIELDVALUE_CONSTRUCT, _COMMA, list) {} \
\
	template<typename TRow, typename TFieldIndex> \
	c(TRow const& rRow, TFieldIndex const& rFieldIndex) : _SINDY_INHERITLIST_UNFOLD_##n(_SINDY_FIELDVALUE_CONSTRUCT2, _COMMA, list) {} \
\
	template<typename TRow> \
	c& operator = (TRow const& rRow) \
	{ \
		_SINDY_INHERITLIST_UNFOLD_##n(_SINDY_FIELDVALUE_COPYOPERATOR, _SEMICOLON, list);\
		return *this; \
	} \
\
	template <typename _Functor, typename TRow> \
	static void distribute(TRow const& rRow, _Functor* pFunctor) \
	{ \
		(*pFunctor)(_SINDY_INHERITLIST_UNFOLD_##n(_SINDY_FIELDVALUE_FUNCTOR, _COMMA, list)); \
	} \
\
	template <typename TFunction, typename TRow> \
	static void for_each(TRow const& rRow, TFunction f) \
	{ \
		_SINDY_INHERITLIST_UNFOLD_##n(_SINDY_FIELDVALUE_FOR_EACH, _SEMICOLON, list); \
	} \
\
	template <typename _Function> \
	class Distributor \
	{ \
	public: \
		Distributor(_Function cFunction) : \
		m_Function(cFunction) \
		{ \
		} \
\
		template <typename TRow> \
		void operator()(TRow const& rRow) \
		{ \
			m_Function(_SINDY_INHERITLIST_UNFOLD_##n(_SINDY_FIELDVALUE_FUNCTOR, _COMMA, list)); \
		} \
	private: \
		_Function m_Function; \
	}; \
\
	template <typename _Function> \
	Distributor<_Function> static make_Distributor(_Function cFunction) \
	{ \
		return Distributor<_Function>(cFunction); \
	} \
};

#define _SINDY_FIELDVALUE_INHERIT(arg) _SINDY_FIELDVALUE_INHERIT_D(arg)
#define _SINDY_FIELDVALUE_INHERIT_D(arg) _SINDY_FIELDVALUE_INHERIT ## arg
#define _SINDY_FIELDVALUE_INHERIT_(arg) public arg
#define _SINDY_FIELDVALUE_INHERIT_1(name) public sindy::FieldValue<name>
#define _SINDY_FIELDVALUE_INHERIT_2(name, type) public sindy::FieldValue<name, type>

#define _SINDY_FIELDVALUE_CONSTRUCT(arg) _SINDY_FIELDVALUE_CONSTRUCT_D(arg)
#define _SINDY_FIELDVALUE_CONSTRUCT_D(arg) _SINDY_FIELDVALUE_CONSTRUCT ## arg
#define _SINDY_FIELDVALUE_CONSTRUCT_(arg) arg(rRow)
#define _SINDY_FIELDVALUE_CONSTRUCT_1(name) sindy::FieldValue<name>(rRow)
#define _SINDY_FIELDVALUE_CONSTRUCT_2(name, type) sindy::FieldValue<name, type>(rRow)

#define _SINDY_FIELDVALUE_CONSTRUCT2(arg) _SINDY_FIELDVALUE_CONSTRUCT2_D(arg)
#define _SINDY_FIELDVALUE_CONSTRUCT2_D(arg) _SINDY_FIELDVALUE_CONSTRUCT2 ## arg
#define _SINDY_FIELDVALUE_CONSTRUCT2_(arg) arg(rRow, rFieldIndex)
#define _SINDY_FIELDVALUE_CONSTRUCT2_1(name) sindy::FieldValue<name>(rRow, rFieldIndex)
#define _SINDY_FIELDVALUE_CONSTRUCT2_2(name, type) sindy::FieldValue<name, type>(rRow, rFieldIndex)

#define _SINDY_FIELDVALUE_COPYOPERATOR(arg) _SINDY_FIELDVALUE_COPYOPERATOR_D(arg)
#define _SINDY_FIELDVALUE_COPYOPERATOR_D(arg) _SINDY_FIELDVALUE_COPYOPERATOR ## arg
#define _SINDY_FIELDVALUE_COPYOPERATOR_(arg) static_cast<arg&>(*this) = rRow
#define _SINDY_FIELDVALUE_COPYOPERATOR_1(name) static_cast<sindy::FieldValue<name>&>(*this) = rRow
#define _SINDY_FIELDVALUE_COPYOPERATOR_2(name, type) static_cast<sindy::FieldValue<name, type>&>(*this) = rRow

#define _SINDY_FIELDVALUE_FIELDINDEX(arg) _SINDY_FIELDVALUE_FIELDINDEX_D(arg)
#define _SINDY_FIELDVALUE_FIELDINDEX_D(arg) _SINDY_FIELDVALUE_FIELDINDEX ## arg
#define _SINDY_FIELDVALUE_FIELDINDEX_(c) c::FieldIndex
#define _SINDY_FIELDVALUE_FIELDINDEX_1(name) sindy::FieldIndex<name>
#define _SINDY_FIELDVALUE_FIELDINDEX_2(name, type) sindy::FieldIndex<name>

#define _SINDY_FIELDVALUE_FUNCTOR(arg) _SINDY_FIELDVALUE_FUNCTOR_D(arg)
#define _SINDY_FIELDVALUE_FUNCTOR_D(arg) _SINDY_FIELDVALUE_FUNCTOR ## arg
#define _SINDY_FIELDVALUE_FUNCTOR_(arg) static_cast<const arg&>(rRow).getValue()
#define _SINDY_FIELDVALUE_FUNCTOR_1(name) sindy::fieldvalue_cast<name, _variant_t>(rRow).getValue()
#define _SINDY_FIELDVALUE_FUNCTOR_2(name, type) sindy::fieldvalue_cast<name, type>(rRow).getValue()

#define _SINDY_FIELDVALUE_FOR_EACH(arg) _SINDY_FIELDVALUE_FOR_EACH_D(arg)
#define _SINDY_FIELDVALUE_FOR_EACH_D(arg) _SINDY_FIELDVALUE_FOR_EACH ## arg
#define _SINDY_FIELDVALUE_FOR_EACH_(arg) f(static_cast<const arg&>(rRow))
#define _SINDY_FIELDVALUE_FOR_EACH_1(name) f(sindy::fieldvalue_cast<name, _variant_t>(rRow))
#define _SINDY_FIELDVALUE_FOR_EACH_2(name, type) f(sindy::fieldvalue_cast<name, type>(rRow))

#endif // _SINDY_FIELDVALUE_H_
