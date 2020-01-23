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

// RNSConfig.h: RNSConfig �N���X�̃C���^�[�t�F�C�X
//
// $Log: RNSConfig.h,v $
// Revision 1.7  2003/08/18 08:36:00  f_kiku
// �ݒ�̓ǂݍ��݂Ɏg�p�\�ȃX�g���[�����A�t�@�C���X�g���[��������̓X�g���[���S�ʂɕύX�B
//
// Revision 1.6  2003/07/14 01:22:22  f_kiku
// �w�肵�����ڂɊY������l���A�o�����ɘA�����Ď擾�B
//
// Revision 1.5  2003/05/16 09:39:05  f_kiku
// �A���S���Y���ύX�B
//
// Revision 1.4  2003/05/06 06:45:32  f_kiku
// �m�F�֐��ǉ��B
//
// Revision 1.3  2003/04/16 01:46:22  f_kiku
// init()��virtual�ɁiObjective C�̗��V�j�B
//
// Revision 1.2  2003/01/22 11:00:22  f_kiku
// �R�����g�C���B
//
// Revision 1.1  2003/01/10 06:21:28  f_kiku
// �ݒ�t�@�C���ǂݍ��݃N���X�wRNSConfig�x��ǉ��B
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONFIG_H__3909720C_72F9_406F_B829_C6044BED81D8__INCLUDED_)
#define AFX_CONFIG_H__3909720C_72F9_406F_B829_C6044BED81D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <RNS.h>

#include <map>
#include <vector>
#include <string>

#include <atlconv.h>

/**
 * @brief	�ݒ�t�@�C���ǂݍ��݃N���X
 *
 * @note	<���ږ�> <�l> <�l> ... �Ƃ����`���̃t�@�C������͂���B
 * @note	"#"�Ŏn�܂�s�̓R�����g�Ƃ��ēǂݔ�΂��B
 * @warning	256Byte�ȏ��<���ږ�>�i��������<�l>�j��256Byte�ŕ��������B
 */
class RNSConfig  
{
public:
	typedef	std::string					Item;				///< ���ږ�
	typedef unsigned int				ItemID;				///< ����ID
	typedef std::string					Value;				///< �l

	typedef	std::vector<Value>			ValueList;			///< �l���X�g
	typedef std::map<Item, ValueList>	ItemMap;			///< ���ڕʒl���X�g

	typedef	std::map<Item, ItemID>		ItemIDMap;			///< ���ڕ�ID
	typedef std::map<ItemID, ValueList>	RegisteredItemMap;	///< �o�^���ꂽ���ڕʒl���X�g

	// --- �\�z/���� --- //

	RNSConfig();
	virtual ~RNSConfig();

	// --- �����ݒ� --- //

	virtual void init();

	// --- �ǂݍ��� --- //

	/// �t�@�C�������w�肵�ēǂݍ���
	bool read(const char* cCongfiFileName);

		/// �X�g���[�����w�肵�ēǂݍ���
	bool read(std::istream& cStream);

	// --- �擾 --- //

	/// ���ڕʒl���X�g���擾����
	const ItemMap&				itemMap() const;

	/// �w�肵�����ڂ̒l���X�g���擾����
	const ValueList*			valueList(const char* cItemName) const;

	/// �w�肵�����ڂ̒l���擾����
	Value						value(const char* cItemName, const char* cDelimiter = "\t") const;

	/// �o�^���ꂽ���ڕʒl���X�g���擾����
	const RegisteredItemMap&	registeredItemMap() const;

	/// �w�肵������ID�̒l���X�g���擾����
	const ValueList*			registeredValueList(ItemID cItemID) const;

	/// �w�肵������ID�̒l���擾����
	Value						registeredValue(ItemID cItemID, const char* cDelimiter = "\t") const;

	// --- �m�F --- //

	/// �o�^����Ă��Ȃ����ڃ��X�g���o�͂���
	bool check(std::ostream& cStream) const;

protected:

	/**
	 * @brief	���ږ���ID�t��������
	 *
	 * @note	coItemIDMap[<���ږ�>] = <ID> �Ƃ��āA���ږ���ID��t�^����B
	 * @note	ID��t�������ڂ́AmRegisteredItemMap�ɓo�^�����悤�ɂȂ�B
	 * @warning	ID�͐��̐����ɂ��邱�Ɓi0�����߁j�B
	 *
	 * @param	coItemMap	[out]	���ڕ�ID
	 */
	virtual void getItemID(ItemIDMap& coItemIDMap) const
	{
	}

	/**
	 * @brief	���ړǂݍ��ݎ��g���֐�
	 *
	 * @note	<���ږ�>�ɂ���Ċg���������������I�[�o�[���C�h����B
	 *
	 * @param	cItem	[in]	���ږ�
	 * @retval	true	��`���ꂽ���ږ�
	 * @retval	false	��`����Ă��Ȃ����ږ�
	 */
	virtual bool itemFunc(const char* cItem)
	{
		return false;
	}

	/**
	 * @brief	���ړǂݍ��ݎ��g���֐�
	 *
	 * @note	<����ID>�ɂ���Ċg���������������I�[�o�[���C�h����B
	 *
	 * @param	cID		[in]	����ID
	 */
	virtual void itemFunc(ItemID cID)
	{
	}

	/**
	 * @brief	�l�ǂݍ��ݎ��g���֐�
	 *
	 * @note	<�l>�ɂ���Ċg���������������I�[�o�[���C�h����B
	 *
	 * @param	cItem	[in]	�J�����g���ږ�
	 * @param	cValue	[in]	�l������
	 * @retval	true	��`���ꂽ���ږ��ɑ΂���l�ǂݍ���
	 * @retval	false	��`����Ă��Ȃ����ږ��ɑ΂���l�ǂݍ���
	 */
	virtual bool valueFunc(const std::string& cItem, const char* cValue)
	{
		return false;
	}

	/**
	 * @brief	�l�ǂݍ��ݎ��g���֐�
	 *
	 * @note	<�l>�ɂ���Ċg���������������I�[�o�[���C�h����B
	 *
	 * @param	cItemID	[in]	�J�����g����ID
	 * @param	cValue	[in]	�l������
	 * @retval	true	��`���ꂽ����ID�ɑ΂���l�ǂݍ���
	 * @retval	false	��`����Ă��Ȃ�����ID�ɑ΂���l�ǂݍ���
	 */
	virtual bool valueFunc(ItemID cItemID, const char* cValue)
	{
		return false;
	}

	// === �q�v���p�e�B�r === //

	ItemIDMap			mItemIDMap;			///< �o�^���ꂽ���ڕ�ID
	RegisteredItemMap	mRegisteredItemMap;	///< �o�^���ꂽ���ڕʒl���X�g
	ItemMap				mItemMap;			///< ���ڕʒl���X�g
};

#endif // !defined(AFX_CONFIG_H__3909720C_72F9_406F_B829_C6044BED81D8__INCLUDED_)
