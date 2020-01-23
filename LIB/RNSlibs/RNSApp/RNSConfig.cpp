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

// RNSConfig.cpp: RNSConfig �N���X�̃C���v�������e�[�V����
//
// $Log: RNSConfig.cpp,v $
// Revision 1.7  2003/08/18 08:36:00  f_kiku
// �ݒ�̓ǂݍ��݂Ɏg�p�\�ȃX�g���[�����A�t�@�C���X�g���[��������̓X�g���[���S�ʂɕύX�B
//
// Revision 1.6  2003/07/16 04:12:19  f_kiku
// �_�u���N�H�[�e�[�V�����ɂ�镶���񌋍��ɑΉ��B
//
// Revision 1.5  2003/07/14 01:22:22  f_kiku
// �w�肵�����ڂɊY������l���A�o�����ɘA�����Ď擾�B
//
// Revision 1.4  2003/05/16 09:39:05  f_kiku
// �A���S���Y���ύX�B
//
// Revision 1.3  2003/05/06 06:45:32  f_kiku
// �m�F�֐��ǉ��B
//
// Revision 1.2  2003/04/16 01:46:22  f_kiku
// init()��virtual�ɁiObjective C�̗��V�j�B
//
// Revision 1.1  2003/01/10 06:21:28  f_kiku
// �ݒ�t�@�C���ǂݍ��݃N���X�wRNSConfig�x��ǉ��B
//
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

#include <RNSDelimitStream.h>
#include "RNSMessage.h"
#include "RNSConfig.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RNSConfig::RNSConfig()
{

}

RNSConfig::~RNSConfig()
{

}

//////////////////////////////////////////////////////////////////////
// ������
//////////////////////////////////////////////////////////////////////

/**
 * @brief	getItemID()���I�[�o�[���C�h����Ă�����ItemIDMap���擾����B
 *
 * @note	�I�[�o�[���C�h���鑤��RNSConfig::init()��Y�ꂸ�ɁB
 */
void RNSConfig::init()
{
	getItemID(mItemIDMap);
}

//////////////////////////////////////////////////////////////////////
// �ǂݍ���
//////////////////////////////////////////////////////////////////////

/**
 * @brief	�t�@�C�������w�肵�ēǂݍ��݂��J�n
 *
 * @param	cConfigFileName	[in]	�t�@�C����
 * @retval	true	�ǂݍ��ݐ���
 * @retval	false	�ǂݍ��ݎ��s
 */
bool RNSConfig::read(const char *cConfigFileName)
{
#if _MSC_VER > 1200
	USES_CONVERSION;
	std::ifstream	aStream(T2W(cConfigFileName));
#else
	std::ifstream	aStream(cConfigFileName);
#endif
	if(! aStream)
		return false;

	return read(aStream);
}

/**
 * @brief	���̓t�@�C���X�g���[�����w�肵�ēǂݍ��݂��J�n
 *
 * @warning	256Byte�ȏ��<���ږ�>�i��������<�l>�j��256Byte�ŕ��������B
 *
 * @param	cStream	[in]	���̓X�g���[��
 * @retval	true	�ǂݍ��ݐ���
 * @retval	false	�ǂݍ��ݎ��s
 */
bool RNSConfig::read(std::istream &cStream)
{
	if(! cStream)
		return false;

	char	aWord[258];
	bool	aFirst = true;

	int			aID = 0;
	std::string	aString;

	while( cStream >> std::setw(256) >> aWord ) {
		// �ꓪ�� "#" �������P�ꂩ����̓R�����g�Ƃ��Ė�������
		if(aWord[0] == '#') {
			cStream.ignore(65536, '\n');
			aFirst = true;
			continue;
		}
		else if(! strcmp(aWord, ":")) {
			aFirst = false;
			continue;
		}
		else if(aFirst) {
			long aTmpID = 0;

			ItemIDMap::iterator	itr = mItemIDMap.find(aWord);

			if(itr != mItemIDMap.end()) {
				aTmpID = itr->second;
			}

			if(aTmpID) {
				aID = aTmpID;
				aString = aWord;
				itemFunc(aTmpID);
			}
			else {
				aID = 0;
				aString = aWord;
				if(! itemFunc(aWord)) {
					mItemMap[aString];
				}
			}


			aFirst = false;
		}
		else {
			std::string aValue;
			if(aWord[0] == '\"') {
				aValue = &(aWord[1]);
				std::string aAppendString;
				std::getline(cStream, aAppendString, '\"');
				aValue += aAppendString;
			}
			else {
				aValue = aWord;
			}


			if(aID) {
				if(! valueFunc(aID, aValue.c_str())) {
					mRegisteredItemMap[aID].push_back(aValue);
				}
			}
			else {
				if(! valueFunc(aString, aValue.c_str())) {
					mItemMap[aString].push_back(aValue);
				}
			}
		}

		// �P��̋�؂肪���s�}�[�N��������s���t���O�𗧂Ă�
		aFirst = (cStream.peek() == '\n');
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// �擾
//////////////////////////////////////////////////////////////////////

/**
 * @brief	���ڕʒl�ꗗ���擾
 *
 * @return	���ڕʒl�ꗗ
 */
const RNSConfig::ItemMap& RNSConfig::itemMap() const
{
	return mItemMap;
}

/**
 * @brief	�w�肵�����ږ��ɊY������l�ꗗ���擾
 *
 * @param	cItemName	���ږ�
 * @return	�l�ꗗ
 */
const RNSConfig::ValueList* RNSConfig::valueList(const char *cItemName) const
{
	ItemMap::const_iterator itr = itemMap().find(cItemName);

	if(itr != itemMap().end())
		return &(itr->second);

	return 0;
}

/**
 * @brief	�w�肵�����ڂɊY������l���擾
 *
 * @note	�l���������݂���ꍇ�͌������ĕԂ��B
 *
 * @param	cItemName	����
 * @param	cDelimiter	�������Ɏg�p�����؂蕶��
 * @return	�l�B���݂��Ȃ��ꍇ�͋󕶎���B
 */
RNSConfig::Value RNSConfig::value(const char* cItemName, const char* cDelimiter) const
{
	std::stringstream	aStream;
	RNSDelimitStream	aDelimStream(&aStream, cDelimiter);

	const ValueList* pValue = valueList(cItemName);
	if(! pValue || pValue->empty())
		return std::string();

	for(int i = 0; i < (int)pValue->size(); i++) {
		aDelimStream << (*pValue)[i];
		++aDelimStream;
	}

	return aStream.str();
}


/**
 * @brief	�o�^���ꂽ���ڕʒl�ꗗ���擾
 *
 * @return	���ڕʒl�ꗗ
 */
const RNSConfig::RegisteredItemMap& RNSConfig::registeredItemMap() const
{
	return mRegisteredItemMap;
}

/**
 * @brief	�w�肵������ID�ɊY������l�ꗗ���擾
 *
 * @param	cItemID	����ID
 * @return	�l�ꗗ
 */
const RNSConfig::ValueList* RNSConfig::registeredValueList(ItemID cItemID) const
{
	RegisteredItemMap::const_iterator itr = registeredItemMap().find(cItemID);

	if(itr != registeredItemMap().end())
		return &(itr->second);

	return 0;
}

/**
 * @brief	�w�肵������ID�ɊY������l���擾
 *
 * @note	�l���������݂���ꍇ�͌������ĕԂ��B
 *
 * @param	cItemID		����ID
 * @param	cDelimiter	�������Ɏg�p�����؂蕶��
 * @return	�l�B���݂��Ȃ��ꍇ�͋󕶎���B
 */
RNSConfig::Value RNSConfig::registeredValue(ItemID cItemID, const char* cDelimiter) const
{
	std::stringstream	aStream;
	RNSDelimitStream	aDelimStream(&aStream, cDelimiter);

	const ValueList* pValue = registeredValueList(cItemID);
	if(! pValue || pValue->empty())
		return std::string();

	for(int i = 0; i < (int)pValue->size(); i++) {
		aDelimStream << (*pValue)[i];
		++aDelimStream;
	}

	return aStream.str();
}

//////////////////////////////////////////////////////////////////////
// �m�F
//////////////////////////////////////////////////////////////////////

/**
 * @brief �ݒ�t�@�C���̑Ó����m�F
 *
 * @note	�K��O�̐ݒ荀�ڂ����݂��Ȃ����Ƃ��m�F����B
 * @note	�K�{�̐ݒ荀�ڂ̑��݂͊m�F���Ȃ��B
 *
 * @retval	true	�K��O�̐ݒ荀�ڂ͑��݂��Ȃ�
 * @retval	false	�K��O�̐ݒ荀�ڂ����݂���
 */
bool RNSConfig::check(std::ostream& cStream) const
{
	using namespace rns_message;

	bool aResult = true;

	const ItemMap&	rItemMap = itemMap();

	for(ItemMap::const_iterator itr = rItemMap.begin(); itr != rItemMap.end(); ++itr) {
		const std::string&	rItemName = itr->first;
		//�K��O�̐ݒ荀�ږ�:���ږ�
		cStream << getMsg(eUnreglatedSettingItemName) << rItemName.c_str() << std::endl;
		aResult = false;
	}

	return aResult;
}
