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

#ifndef __SINDY_DETAIL_REF_H__
#define __SINDY_DETAIL_REF_H__

namespace sindy {

/**
 * @brief ���t�@�����X���b�p�[�N���X�B
 */
template<class T>
class reference_wrapper
{ 
public:
// ��`
    typedef T type;

// �\�z
    explicit reference_wrapper(T& t): m_T(&t) {}

// �擾
    T& get() const { return *m_T; }
    T* get_pointer() const { return m_T; }

// ���Z�q�̃I�[�o�[���C�h
    operator T& () const { return *m_T; }

private:
// �ϐ�
    T* m_T;
};

/**
 * @brief ���t�@�����X���b�p�[�̃w���p�֐�
 */
template<class T> inline reference_wrapper<T> ref(T& t)
{ 
    return reference_wrapper<T>(t);
}

/**
 * @brief �|�C���^���ʃN���X�B
 *
 * @note ���T�� http://www.tietew.jp/cppll/archive/6360 �B
 */
template<typename T> struct is_pointer
{
	typedef char yestype;
	typedef struct { char dummy[2]; } notype;
	
	struct helper { helper(const volatile void*); };
	inline static yestype tester(helper);
	inline static notype tester(...);
	
	inline static T& make_t();
	enum { value = sizeof(tester(make_t()))==sizeof(yestype) };
};
template <> struct is_pointer<void> { enum { value = 0 }; };

/**
 * @brief �C���^�t�F�[�X���ʃN���X�B
 */
template<typename T> struct is_interface
{
	typedef char yestype;
	typedef struct { char dummy[2]; } notype;
	
	struct helper { helper(const volatile IUnknown&); };
	inline static yestype tester(helper);
	inline static notype tester(...);
	
	inline static T& make_t();
	enum { value = sizeof(tester(make_t()))==sizeof(yestype) };
};

/**
 * @brief �C���^�t�F�[�X�|�C���^���ʃN���X�B
 */
template<typename T> struct is_interface_pointer
{
	typedef char yestype;
	typedef struct { char dummy[2]; } notype;
	
	struct helper { helper(const volatile IUnknown*); };
	inline static yestype tester(helper);
	inline static notype tester(...);
	
	inline static T& make_t();
	enum { value = sizeof(tester(make_t()))==sizeof(yestype) };
};
	
/**
 * @brief �X�}�[�g�|�C���^���ʃN���X�B
 */
template<typename T> struct is_com_ptr_t
{
	typedef char yestype;
	typedef struct { char dummy[2]; } notype;
	
	template <typename TIIID> inline static yestype tester(const volatile _com_ptr_t<TIIID>&);
	inline static notype tester(...);
	
	inline static T& make_t();
	enum { value = sizeof(tester(make_t()))==sizeof(yestype) };
};
	
/**
 * @brief ���t�@�����X���b�p���ʃN���X�B
 */
template<typename T> struct is_reference_wrapper
{
	typedef char yestype;
	typedef struct { char dummy[2]; } notype;
	
	template <typename T> inline static yestype tester(const volatile reference_wrapper<T>&);
	inline static notype tester(...);
	
	inline static T& make_t();
	enum { value = sizeof(tester(make_t()))==sizeof(yestype) };
};

/**
 * @brief @a T ���u�|�C���^�v�u�C���^�t�F�[�X�|�C���^�v�u�X�}�[�g�|�C���^�v�u���t�@�����X���b�p�v�̂�����ɑ����邩�̔�����s���N���X
 *
 * - value
 *   - 0 : �ȉ��̂�����ł��Ȃ��^�B
 *   - 1 : �|�C���^�^�B
 *   - 2 : �C���^�t�F�[�X�iIUnknon�h���j�|�C���^�^�B
 *   - 3 : �X�}�[�g�|�C���^�i_com_ptr_t�j�^�B
 *   - 4 : ���t�@�����X���b�p�isindy::ref�j�^�B
 */
template <typename T>
struct type_of
{
	typedef char not_ptr;
	typedef struct { char dummy[2]; } ptr;
	typedef struct { char dummy[3]; } interface_ptr;
	typedef struct { char dummy[4]; } com_ptr;
	typedef struct { char dummy[5]; } sindy_ref;
	
	template <typename T> inline static sindy_ref tester(reference_wrapper<T>);
	template <typename TIIID> inline static com_ptr tester(_com_ptr_t<TIIID>);
	inline static interface_ptr tester(const volatile IUnknown*);
	inline static ptr tester(const volatile void*);
	inline static not_ptr tester(...);
	
	inline static T& make_t();

	enum {
		value = 
			sizeof(tester(make_t())) == sizeof(sindy_ref) ? 4 :
			sizeof(tester(make_t())) == sizeof(com_ptr) ? 3 :
			sizeof(tester(make_t())) == sizeof(interface_ptr) ? 2 :
			sizeof(tester(make_t())) == sizeof(ptr) ? 1 : 0
	};
};

namespace detail
{

template<bool wrapped> struct complement_interface_pointer { template <typename T> struct apply { typedef T type; }; };
template<> struct complement_interface_pointer<true> { template <typename T> struct apply { typedef T* type; }; };

template<bool wrapped> struct reference_unwrapper { template <typename T> struct apply { typedef T type; }; };
template<> struct reference_unwrapper<true> { template <typename T> struct apply { typedef typename T::type type; }; };

} // namespace detail

/**
 * @brief IUnknown��IUnknown*�AIUnknown*��IUnknown*�Aother��other
 */
template <typename T>
struct interface_to_pointer : public detail::complement_interface_pointer<is_interface<T>::value>::apply<T> {};

/**
 * @brief ref<T>��T�Aother��other
 */
template <typename T>
struct unwrap_reference : public detail::reference_unwrapper<is_reference_wrapper<T>::value>::apply<T> {};

} // namespace sindy

#endif // __SINDY_DETAIL_REF_H__
