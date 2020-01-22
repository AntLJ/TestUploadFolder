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

#include "stdafx.h"
#include <rns/bitfield.h>

namespace {

enum test_enum
{
	val_1100 = 0xc
};

typedef rns::bitfield_t<0, 0, bool> bitfield1_t;
typedef rns::bitfield_t<1, 1, bool> bitfield2_t;
typedef rns::bitfield_t<2, 5, unsigned short> bitfield3_t;
typedef rns::bitfield_t<6, 11, unsigned int> bitfield4_t;
typedef rns::bitfield_t<12, 15, test_enum> bitfield5_t;

typedef rns::bitfield_t<2, 5, short> sbitfield3_t;
typedef rns::bitfield_t<6, 11, int> sbitfield4_t;

typedef rns::native_bitfield_t<0, 0, bool, unsigned short> native_bitfield1_t;
typedef rns::native_bitfield_t<1, 1, bool, unsigned short> native_bitfield2_t;
typedef rns::native_bitfield_t<2, 5, unsigned short, unsigned short> native_bitfield3_t;
typedef rns::native_bitfield_t<6, 11, unsigned int, unsigned short> native_bitfield4_t;
typedef rns::native_bitfield_t<12, 15, test_enum, unsigned short> native_bitfield5_t;

typedef rns::native_bitfield_t<2, 5, short, unsigned short> native_sbitfield3_t;
typedef rns::native_bitfield_t<6, 11, int, unsigned short> native_sbitfield4_t;

RNS_BITFIELDS_STRUCT(
	BitFields1,
	(1, _(bool, field1))
	(1, _reserved)
	(4, _(unsigned short, field2))
	(6, _(unsigned int, field3))
	(4, _(test_enum, field4))
);

BOOST_STATIC_ASSERT(sizeof(BitFields1) == 2);

void static_test()
{
	unsigned char val[1];

	// byte_width �� sizeof(T) ���傫���Ȃ�s��̏C���m�F
	(rns::bitfield_t<7, 8, unsigned char>::constant(val));

	// �^�ϊ��̃R���p�C�����`�F�b�N
	BOOST_STATIC_ASSERT((
		boost::is_convertible<
			bitfield1_t::variable,
			bitfield1_t::constant
		>::value
	));

	BOOST_STATIC_ASSERT((
		! boost::is_convertible<
			bitfield1_t::constant,
			bitfield1_t::variable
		>::value
	));

	BOOST_STATIC_ASSERT((
		boost::is_convertible<
			native_bitfield1_t::variable,
			native_bitfield1_t::constant
		>::value
	));

	BOOST_STATIC_ASSERT((
		! boost::is_convertible<
			native_bitfield1_t::constant,
			native_bitfield1_t::variable
		>::value
	));
}

} // anonymous namespace

/**
 * @brief rns::bitfield_t �̃e�X�g�P�[�X
 *
 * ���̃e�X�g�ł͌x������؏o�Ȃ��͂��B
 */
class BitfieldTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(BitfieldTest);
	CPPUNIT_TEST(testConstant);
	CPPUNIT_TEST(testConstantSigned);
	CPPUNIT_TEST(testVariable);
	CPPUNIT_TEST(testVariableSigned);
	CPPUNIT_TEST(testConversion);
	CPPUNIT_TEST_SUITE_END();

public:
/// @name �e�X�g�P�[�X
//@{
	/**
	 * @brief �r�b�g�t�B�[���h�l�Q�ƃe�X�g
	 */
	void testConstant()
	{
		const unsigned char val[2] = { 0x4e, 0x1c }; // 01001110 00011100

		// ��0bit�i0) �� bool �ɕϊ�
		CPPUNIT_ASSERT_EQUAL(false, (bitfield1_t::constant(val).get()));

		// ��1bit�i1�j�� bool �ɕϊ�
		CPPUNIT_ASSERT_EQUAL(true, (bitfield2_t::constant(val).get()));

		// ��2�`5bit�i0011�j�� unsignd short �ɕϊ�
		CPPUNIT_ASSERT_EQUAL(static_cast<unsigned short>(0x3),  (bitfield3_t::constant(val).get()));

		// �o�C�g�ׂ��̑�6�`11bit�i10 0001�j�� unsigned int �ɕϊ�
		CPPUNIT_ASSERT_EQUAL(0x21U,  (bitfield4_t::constant(val).get()));

		// ��12�`15bit�i1100�j��񋓌^�ɕϊ�
		CPPUNIT_ASSERT_EQUAL(val_1100,  (bitfield5_t::constant(val).get()));

		// 2�o�C�g�����l�ƂȂ��5�`15bit�i001110 00011100�j�� unsigned int �ɕϊ�
		CPPUNIT_ASSERT_EQUAL(0x61CU,  (rns::bitfield_t<5, 15, unsigned int>::constant(val).get()));
	}

	/**
	 * @brief �����t���r�b�g�t�B�[���h�l�Q�ƃe�X�g
	 */
	void testConstantSigned()
	{
		const unsigned char val[2] = { 0x4e, 0x1c }; // 01001110 00011100

		// ��2�`5bit�i0011�j�� short �ɕϊ�
		CPPUNIT_ASSERT_EQUAL(static_cast<short>(0x3),  (sbitfield3_t::constant(val).get()));

		// �o�C�g�ׂ��̑�6�`11bit�i10 0001�j�� int �ɕϊ�
		CPPUNIT_ASSERT_EQUAL(-31,  (sbitfield4_t::constant(val).get()));
	}

	/**
	 * @brief �r�b�g�t�B�[���h�l�ύX�e�X�g
	 */
	void testVariable()
	{
		unsigned char val[2] = { 0 };

		// �e�r�b�g�t�B�[���h�ɒl����
		(bitfield1_t::variable(val)) = false;    // 0
		(bitfield2_t::variable(val)) = true;     //  1
		(bitfield3_t::variable(val)) = 0x3;      //   0011
		(bitfield4_t::variable(val)) = 0x21;     //       10 0001
		(bitfield5_t::variable(val)) = val_1100; //              1100

		// �l���`�F�b�N
		CPPUNIT_ASSERT_EQUAL(0x4eU, static_cast<unsigned int>(val[0])); // 01001110
		CPPUNIT_ASSERT_EQUAL(0x1cU, static_cast<unsigned int>(val[1])); // 00011100

		// �l������������
		(bitfield1_t::variable(val)) = true;  // 1
		(bitfield2_t::variable(val)) = false; //  0
		(bitfield3_t::variable(val)) = 0x8;   //   1000
		(bitfield4_t::variable(val)) = 0x3c;  //       11 1100

		// �l���`�F�b�N
		CPPUNIT_ASSERT_EQUAL(0xa3U, static_cast<unsigned int>(val[0])); // 10100011 
		CPPUNIT_ASSERT_EQUAL(0xccU, static_cast<unsigned int>(val[1])); // 11001100
	}

	/**
	 * @brief �����t���r�b�g�t�B�[���h�l�ύX�e�X�g
	 */
	void testVariableSigned()
	{
		unsigned char val[2] = { 0 };

		// �e�r�b�g�t�B�[���h�ɒl����
		(bitfield1_t::variable(val)) = false;    // 0
		(bitfield2_t::variable(val)) = true;     //  1
		(sbitfield3_t::variable(val)) = 3;       //   0011
		(sbitfield4_t::variable(val)) = -31;     //       10 0001
		(bitfield5_t::variable(val)) = val_1100; //              1100

		// �l���`�F�b�N
		CPPUNIT_ASSERT_EQUAL(0x4eU, static_cast<unsigned int>(val[0])); // 01001110
		CPPUNIT_ASSERT_EQUAL(0x1cU, static_cast<unsigned int>(val[1])); // 00011100

		// �l������������
		(bitfield1_t::variable(val)) = true;  // 1
		(bitfield2_t::variable(val)) = false; //  0
		(sbitfield3_t::variable(val)) = -8;   //   1000
		(sbitfield4_t::variable(val)) = -4;   //       11 1100 

		// �l���`�F�b�N
		CPPUNIT_ASSERT_EQUAL(0xa3U, static_cast<unsigned int>(val[0])); // 10100011 
		CPPUNIT_ASSERT_EQUAL(0xccU, static_cast<unsigned int>(val[1])); // 11001100
	}

	/**
	 * @brief �r�b�g�t�B�[���h�^�ϊ��e�X�g
	 */
	void testConversion()
	{
		unsigned char val[2] = { 0x4e, 0x1c };

		// �ό^����s�ό^��
		bitfield1_t::variable v1(val);
		bitfield1_t::constant c1(v1);

		// �s�ό^��
		bitfield1_t::constant c2(val);
	}

	void testBitfieldsMacro()
	{

	}
//@}
};

CPPUNIT_TEST_SUITE_REGISTRATION(BitfieldTest);

/**
 * @brief rns::native_bitfield_t �̃e�X�g�P�[�X
 *
 * ���̃e�X�g�ł͌x������؏o�Ȃ��͂��B
 */
class NativeBitfieldTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(NativeBitfieldTest);
	CPPUNIT_TEST(testConstant);
	CPPUNIT_TEST(testConstantSigned);
	CPPUNIT_TEST(testVariable);
	CPPUNIT_TEST(testVariableSigned);
	CPPUNIT_TEST(testConversion);
	CPPUNIT_TEST_SUITE_END();

public:
/// @name �e�X�g�P�[�X
//@{
	/**
	 * @brief �r�b�g�t�B�[���h�l�Q�ƃe�X�g
	 */
	void testConstant()
	{
		const unsigned short val = 0x4e1c; // 01001110 00011100

		// ��0bit�i0) �� bool �ɕϊ�
		CPPUNIT_ASSERT_EQUAL(false, (native_bitfield1_t::constant(&val).get()));

		// ��1bit�i1�j�� bool �ɕϊ�
		CPPUNIT_ASSERT_EQUAL(true, (native_bitfield2_t::constant(&val).get()));

		// ��2�`5bit�i0011�j�� unsignd short �ɕϊ�
		CPPUNIT_ASSERT_EQUAL(static_cast<unsigned short>(0x3),  (native_bitfield3_t::constant(&val).get()));

		// �o�C�g�ׂ��̑�6�`11bit�i10 0001�j�� unsigned int �ɕϊ�
		CPPUNIT_ASSERT_EQUAL(0x21U,  (native_bitfield4_t::constant(&val).get()));

		// ��12�`15bit�i1100�j��񋓌^�ɕϊ�
		CPPUNIT_ASSERT_EQUAL(val_1100,  (native_bitfield5_t::constant(&val).get()));

		// 2�o�C�g�����l�ƂȂ��5�`15bit�i001110 00011100�j�� unsigned int �ɕϊ�
		CPPUNIT_ASSERT_EQUAL(0x61CU,  (rns::native_bitfield_t<5, 15, unsigned int, unsigned short>::constant(&val).get()));
	}

	/**
	 * @brief �����t���r�b�g�t�B�[���h�l�Q�ƃe�X�g
	 */
	void testConstantSigned()
	{
		const unsigned short val = 0x4e1c; // 01001110 00011100

		// ��2�`5bit�i0011�j�� short �ɕϊ�
		CPPUNIT_ASSERT_EQUAL(static_cast<short>(0x3),  (native_sbitfield3_t::constant(&val).get()));

		// �o�C�g�ׂ��̑�6�`11bit�i10 0001�j�� int �ɕϊ�
		CPPUNIT_ASSERT_EQUAL(-31,  (native_sbitfield4_t::constant(&val).get()));
	}

	/**
	 * @brief �r�b�g�t�B�[���h�l�ύX�e�X�g
	 */
	void testVariable()
	{
		unsigned short val = 0;

		// �e�r�b�g�t�B�[���h�ɒl����
		(native_bitfield1_t::variable(val)) = false;    // 0
		(native_bitfield2_t::variable(val)) = true;     //  1
		(native_bitfield3_t::variable(val)) = 0x3;      //   0011
		(native_bitfield4_t::variable(val)) = 0x21;     //       10 0001
		(native_bitfield5_t::variable(val)) = val_1100; //              1100

		// �l���`�F�b�N
		CPPUNIT_ASSERT_EQUAL(0x4e1cU, static_cast<unsigned int>(val)); // 01001110 00011100

		// �l������������
		(native_bitfield1_t::variable(val)) = true;  // 1
		(native_bitfield2_t::variable(val)) = false; //  0
		(native_bitfield3_t::variable(val)) = 0x8;   //   1000
		(native_bitfield4_t::variable(val)) = 0x3c;  //       11 1100

		// �l���`�F�b�N
		CPPUNIT_ASSERT_EQUAL(0xa3ccU, static_cast<unsigned int>(val)); // 10100011 11001100
	}

	/**
	 * @brief �����t���r�b�g�t�B�[���h�l�ύX�e�X�g
	 */
	void testVariableSigned()
	{
		unsigned short val = 0;

		// �e�r�b�g�t�B�[���h�ɒl����
		(native_bitfield1_t::variable(val)) = false;    // 0
		(native_bitfield2_t::variable(val)) = true;     //  1
		(native_sbitfield3_t::variable(val)) = 3;       //   0011
		(native_sbitfield4_t::variable(val)) = -31;     //       10 0001
		(native_bitfield5_t::variable(val)) = val_1100; //              1100

		// �l���`�F�b�N
		CPPUNIT_ASSERT_EQUAL(0x4e1cU, static_cast<unsigned int>(val)); // 01001110 00011100

		// �l������������
		(native_bitfield1_t::variable(val)) = true;  // 1
		(native_bitfield2_t::variable(val)) = false; //  0
		(native_sbitfield3_t::variable(val)) = -8;   //   1000
		(native_sbitfield4_t::variable(val)) = -4;   //       11 1100 

		// �l���`�F�b�N
		CPPUNIT_ASSERT_EQUAL(0xa3ccU, static_cast<unsigned int>(val)); // 10100011 11001100
	}

	/**
	 * @brief �r�b�g�t�B�[���h�^�ϊ��e�X�g
	 */
	void testConversion()
	{
		unsigned short val = 0x4e1c;

		// �ό^����s�ό^��
		native_bitfield1_t::variable v1(val);
		native_bitfield1_t::constant c1(v1);

		// �s�ό^��
		native_bitfield1_t::constant c2(val);
	}
//@}
};

CPPUNIT_TEST_SUITE_REGISTRATION(NativeBitfieldTest);


/**
 * @brief RNS_BITFIELDS_STRUCT �̃e�X�g�P�[�X
 */
class BitfieldsTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(BitfieldsTest);
	CPPUNIT_TEST(testConstant);
	CPPUNIT_TEST(testVariable);
	CPPUNIT_TEST_SUITE_END();

public:
/// @name �e�X�g�P�[�X
//@{
	/**
	 * @brief �r�b�g�t�B�[���h�l�Q�ƃe�X�g
	 */
	void testConstant()
	{
		const unsigned char val[2] = { 0x4e, 0x1c }; // 01001110 00011100
		const BitFields1* bitfields = reinterpret_cast<const BitFields1*>(val);

		// ��0bit�i0) �� bool �ɕϊ�
		CPPUNIT_ASSERT_EQUAL(false, bitfields->field1().get());

		// ��2�`5bit�i0011�j�� unsignd short �ɕϊ�
		CPPUNIT_ASSERT_EQUAL(static_cast<unsigned short>(0x3), bitfields->field2().get());

		// �o�C�g�ׂ��̑�6�`11bit�i10 0001�j�� unsigned int �ɕϊ�
		CPPUNIT_ASSERT_EQUAL(0x21U, bitfields->field3().get());

		// ��12�`15bit�i1100�j��񋓌^�ɕϊ�
		CPPUNIT_ASSERT_EQUAL(val_1100, bitfields->field4().get());
	}

	/**
	 * @brief �r�b�g�t�B�[���h�l�ύX�e�X�g
	 */
	void testVariable()
	{
		unsigned char val[2] = { 0 };
		BitFields1* bitfields = reinterpret_cast<BitFields1*>(val);

		// �e�r�b�g�t�B�[���h�ɒl����
		bitfields->field1() = true;     // 1
		                                //  0
		bitfields->field2() = 0x3;      //   0011
		bitfields->field3() = 0x21;     //       10 0001
		bitfields->field4() = val_1100; //              1100

		// �l���`�F�b�N
		CPPUNIT_ASSERT_EQUAL(0x8eU, static_cast<unsigned int>(val[0])); // 10001110
		CPPUNIT_ASSERT_EQUAL(0x1cU, static_cast<unsigned int>(val[1])); // 00011100

		// �l������������
		bitfields->field1() = false;  // 0
		                              //  0
		bitfields->field2() = 0x8;    //   1000
		bitfields->field3() = 0x3c;   //       11 1100

		// �l���`�F�b�N
		CPPUNIT_ASSERT_EQUAL(0x23U, static_cast<unsigned int>(val[0])); // 10100011 
		CPPUNIT_ASSERT_EQUAL(0xccU, static_cast<unsigned int>(val[1])); // 11001100
	}
//@}
};

CPPUNIT_TEST_SUITE_REGISTRATION(BitfieldsTest);
