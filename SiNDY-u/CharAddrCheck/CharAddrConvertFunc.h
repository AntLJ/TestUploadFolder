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


#pragma once

#include "ADOBase.h"

#include "../../LIB/jis/x0208/spirit/shift_jis.h"
#include <boost/assign/std/vector.hpp>

using namespace boost::spirit;
using namespace jis::x0208::spirit;

enum GouOrCityAddrLayer
{
	None_Layer			= 0,
	CityAdmin_Layer		= 1,
	GouPoint_Layer		= 2,
};

enum OutputType
{
	SplitCharAddr		= 0,
	GetYomiFromCharAddr	= 1,
};

enum YomiType
{
	NoneType			= 0,
	NumberType			= 1,
	AlphabetType		= 2,
	HiraKataType		= 3,
	KanjiYomiTableType	= 4,
};

enum CharAddrType
{
	NoneCharType	= 0,
	ObjectNameType	= 1,
	ObjectYomiType	= 2,
	KanjiNameType	= 3,
	KanjiYomiType	= 4,
};

typedef std::pair<GouOrCityAddrLayer,long>		ObjectLayerPair;
typedef std::pair<std::string,std::string>		KanjiYomiPair;

/**
* @brief	�T�u�e�[�u���N���X
* @note		�A���t�@�x�b�g�Ȃǂ̓ǂݗp�e�[�u��
*/
class CSubConvertTable
{
public:
	CSubConvertTable(){};

	~CSubConvertTable(){};

public:
	std::map<std::wstring,std::wstring> GetSubtable() { return m_mapSubTable; }
	
	sjis_ranges getAlphabet() { return m_sjis_alphabet_p; }
	sjis_ranges getNumber() { return m_sjis_num_p; }
	sjis_ranges getKanjiKana() { return m_sjis_KanjiKana_p; } 
	sjis_ranges getKana() { return m_sjis_kana_p2; } 
	sjis_ranges getObjectName() { return m_sjis_ObjectName_p; }
	sjis_ranges getObjectYomi() { return m_sjis_ObjectYomi_p; }
	sjis_ranges getKanjiName() { return m_sjis_KanjiName_p; }
	sjis_ranges getKanjiYomi() { return m_sjis_KanjiYomi_p; }
	sjis_ranges getHyphen() { return m_sjis_hyphen_p; }
	void Init()
	{

		// ���S�p�p��
		m_sjis_alphabet_p = sjis_ranges().add(sjis_range(0x41,0x5A)).add(sjis_range(0x61,0x7A)).add(sjis_alpha_p);
		// ���S�p����
		m_sjis_num_p = sjis_ranges().add(sjis_range(0x30,0x39)).add(sjis_digit_p);
		// �S�p�J�i�i�����������j
		m_sjis_katakana_p2 = sjis_ranges().add(sjis_range(0x8340,0x838F)).add(sjis_range(0x8392,0x8396));
		// �S�p���ȃJ�i�{[�[]
		m_sjis_kana_p2 = sjis_ranges().add(sjis_kana_p).add(sjis_range(0x815B,0x815B));
		// sjis���A��񐅏������{�S�p���ȃJ�i�{[�[�X]
		m_sjis_KanjiKana_p = sjis_ranges().add(sjis_kanji_p).add(sjis_kana_p).add(sjis_range(0x815B,0x815B)).add(sjis_range(0x8158,0x8158));
		// [-]
		m_sjis_hyphen_p = sjis_ranges().add(sjis_range(0x2d,0x2d));

		// sjis���A��񐅏������{�S�p���ȃJ�i�{�S�p�p���{���p�����{[�[-�X]
		m_sjis_ObjectName_p = sjis_ranges().add(sjis_kanji_p).add(sjis_kana_p).add(sjis_alpha_p).add(sjis_range(0x30,0x39)).add(sjis_range(0x815B,0x815B)).add(sjis_range(0x2d,0x2d)).add(sjis_range(0x8158,0x8158));
		// �S�p�����J�i�i�����ȊO�j�{[�[�|]
		m_sjis_ObjectYomi_p = sjis_ranges().add(m_sjis_katakana_p2).add(sjis_digit_p).add(sjis_range(0x815B,0x815B)).add(sjis_range(0x817C,0x817C));
		// sjis���A��񐅏������{�S�p�p���{�S�p���ȃJ�i�{[�[�X]
		m_sjis_KanjiName_p = sjis_ranges().add(sjis_kanji_p).add(sjis_alpha_p).add(sjis_kana_p).add(sjis_range(0x815B,0x815B)).add(sjis_range(0x8158,0x8158));
		// �S�p�J�i�i�����ȊO�j�{[�[]
		m_sjis_KanjiYomi_p = sjis_ranges().add(m_sjis_katakana_p2).add(sjis_range(0x815B,0x815B));

		// insert�ŋL�q�����ق��������������񂾂���100�����x�����炢�����c
		m_mapSubTable[L"a"] = L"�G�[";			m_mapSubTable[L"A"] = L"�G�[";			m_mapSubTable[L"��"] = L"�G�[";			m_mapSubTable[L"�`"] = L"�G�[";
		m_mapSubTable[L"b"] = L"�r�[";			m_mapSubTable[L"B"] = L"�r�[";			m_mapSubTable[L"��"] = L"�r�[";			m_mapSubTable[L"�a"] = L"�r�[";
		m_mapSubTable[L"c"] = L"�V�[";			m_mapSubTable[L"C"] = L"�V�[";			m_mapSubTable[L"��"] = L"�V�[";			m_mapSubTable[L"�b"] = L"�V�[";
		m_mapSubTable[L"d"] = L"�f�B�[";		m_mapSubTable[L"D"] = L"�f�B�[";		m_mapSubTable[L"��"] = L"�f�B�[";			m_mapSubTable[L"�c"] = L"�f�B�[";
		m_mapSubTable[L"e"] = L"�C�[";			m_mapSubTable[L"E"] = L"�C�[";			m_mapSubTable[L"��"] = L"�C�[";			m_mapSubTable[L"�d"] = L"�C�[";
		m_mapSubTable[L"f"] = L"�G�t";			m_mapSubTable[L"F"] = L"�G�t";			m_mapSubTable[L"��"] = L"�G�t";			m_mapSubTable[L"�e"] = L"�G�t";
		m_mapSubTable[L"g"] = L"�W�[";			m_mapSubTable[L"G"] = L"�W�[";			m_mapSubTable[L"��"] = L"�W�[";			m_mapSubTable[L"�f"] = L"�W�[";
		m_mapSubTable[L"h"] = L"�G�C�`";		m_mapSubTable[L"H"] = L"�G�C�`";		m_mapSubTable[L"��"] = L"�G�C�`";			m_mapSubTable[L"�g"] = L"�G�C�`";
		m_mapSubTable[L"i"] = L"�A�C";			m_mapSubTable[L"I"] = L"�A�C";			m_mapSubTable[L"��"] = L"�A�C";			m_mapSubTable[L"�h"] = L"�A�C";
		m_mapSubTable[L"j"] = L"�W�F�C";		m_mapSubTable[L"J"] = L"�W�F�C";		m_mapSubTable[L"��"] = L"�W�F�C";			m_mapSubTable[L"�i"] = L"�W�F�C";
		m_mapSubTable[L"k"] = L"�P�[";			m_mapSubTable[L"K"] = L"�P�[";			m_mapSubTable[L"��"] = L"�P�[";			m_mapSubTable[L"�j"] = L"�P�[";
		m_mapSubTable[L"l"] = L"�G��";			m_mapSubTable[L"L"] = L"�G��";			m_mapSubTable[L"��"] = L"�G��";			m_mapSubTable[L"�k"] = L"�G��";
		m_mapSubTable[L"m"] = L"�G��";			m_mapSubTable[L"M"] = L"�G��";			m_mapSubTable[L"��"] = L"�G��";			m_mapSubTable[L"�l"] = L"�G��";
		m_mapSubTable[L"n"] = L"�G�k";			m_mapSubTable[L"N"] = L"�G�k";			m_mapSubTable[L"��"] = L"�G�k";			m_mapSubTable[L"�m"] = L"�G�k";
		m_mapSubTable[L"o"] = L"�I�[";			m_mapSubTable[L"O"] = L"�I�[";			m_mapSubTable[L"��"] = L"�I�[";			m_mapSubTable[L"�n"] = L"�I�[";
		m_mapSubTable[L"p"] = L"�s�[";			m_mapSubTable[L"P"] = L"�s�[";			m_mapSubTable[L"��"] = L"�s�[";			m_mapSubTable[L"�o"] = L"�s�[";
		m_mapSubTable[L"q"] = L"�L���[";		m_mapSubTable[L"Q"] = L"�L���[";		m_mapSubTable[L"��"] = L"�L���[";			m_mapSubTable[L"�p"] = L"�L���[";
		m_mapSubTable[L"r"] = L"�A�[��";		m_mapSubTable[L"R"] = L"�A�[��";		m_mapSubTable[L"��"] = L"�A�[��";			m_mapSubTable[L"�q"] = L"�A�[��";
		m_mapSubTable[L"s"] = L"�G�X";			m_mapSubTable[L"S"] = L"�G�X";			m_mapSubTable[L"��"] = L"�G�X";			m_mapSubTable[L"�r"] = L"�G�X";
		m_mapSubTable[L"t"] = L"�e�B�[";		m_mapSubTable[L"T"] = L"�e�B�[";		m_mapSubTable[L"��"] = L"�e�B�[";			m_mapSubTable[L"�s"] = L"�e�B�[";
		m_mapSubTable[L"u"] = L"���[";			m_mapSubTable[L"U"] = L"���[";			m_mapSubTable[L"��"] = L"���[";			m_mapSubTable[L"�t"] = L"���[";
		m_mapSubTable[L"v"] = L"�u�C";			m_mapSubTable[L"V"] = L"�u�C";			m_mapSubTable[L"��"] = L"�u�C";			m_mapSubTable[L"�u"] = L"�u�C";
		m_mapSubTable[L"w"] = L"�_�u�����[";	m_mapSubTable[L"W"] = L"�_�u�����[";	m_mapSubTable[L"��"] = L"�_�u�����[";		m_mapSubTable[L"�v"] = L"�_�u�����[";
		m_mapSubTable[L"x"] = L"�G�b�N�X";		m_mapSubTable[L"X"] = L"�G�b�N�X";		m_mapSubTable[L"��"] = L"�G�b�N�X";		m_mapSubTable[L"�w"] = L"�G�b�N�X";
		m_mapSubTable[L"y"] = L"���C";			m_mapSubTable[L"Y"] = L"���C";			m_mapSubTable[L"��"] = L"���C";			m_mapSubTable[L"�x"] = L"���C";
		m_mapSubTable[L"z"] = L"�[�b�g";		m_mapSubTable[L"Z"] = L"�[�b�g";		m_mapSubTable[L"��"] = L"�[�b�g";			m_mapSubTable[L"�y"] = L"�[�b�g";
	}

private:
	std::map<std::wstring,std::wstring> m_mapSubTable;

	sjis_ranges m_sjis_ObjectName_p;	//!< sjis���A��񊿎��{�S�p�p�������ȃJ�i�{[�[-�X]�{�S�p�p���{���p����
	sjis_ranges m_sjis_ObjectYomi_p;	//!< �S�p�����J�i�i�����������j�{[�[�|]
	sjis_ranges m_sjis_KanjiName_p;		//!< sjis���A��񊿎��{�S�p�A���t�@�x�b�g�{�S�p���ȃJ�i�{[�[�X]
	sjis_ranges m_sjis_KanjiYomi_p;		//!< �S�p�J�i�i�����������j�{[�[]

	sjis_ranges m_sjis_alphabet_p;	//!< ���S�p�p��
	sjis_ranges m_sjis_num_p;		//!< ���S�p����
	sjis_ranges m_sjis_katakana_p2;	//!< �S�p�J�i�i�����������j
	sjis_ranges m_sjis_kana_p2;		//!< �S�p���ȃJ�i�{[�[]
	sjis_ranges m_sjis_KanjiKana_p;	//!< sjis���A��񊿎��{�S�p���ȃJ�i�{[�[�X]
	sjis_ranges m_sjis_hyphen_p;	//!< [-]
};


/**
* @brief	�����t���Z���̓ǂݕϊ��N���X
*/
class CCharAddrConvert
{
public:
	CCharAddrConvert(){};
	~CCharAddrConvert(){};

public:
	/**
	* @brief	�����ǂ݃e�[�u���A�T�u�ǂ݃e�[�u��������
	* @param	lpcszCovnertTablePath[in]	�ǂݕϊ��e�[�u����PGDB�p�X
	* @return	�����ǂ݃e�[�u���ϊ��G���[�̏o�́i�o�̓T�C�Y��0�Ȃ�OK�j
	*/
	std::multimap<long,CString> Init( LPCTSTR lpcszCovnertTablePath );

	/**
	* @brief	�����t���Z�����I�u�W�F�N�gID�ǂݕϊ��e�[�u���ŕϊ����ďo��
	* @param	LayerType[in]				���C���^�C�v�iGouOrCityAddrLayer���Q�Ɓ@CityAdmin_Layer(1):�s�s�n�}�s���E,GouPoint_Layer(2):���|�C���g�j
	* @param	lOID[in]					�I�u�W�F�N�gID
	* @param	lpcszCharAddr[in]			�����t���Z������
	* @param	bRet[in,out]				�ǂݕϊ� true:����,false:���s
	* @return	�ϊ�������̕�������o�́i���s������NULL�j
	*/
	std::string GetYomiFromCharAddr_Object( GouOrCityAddrLayer LayerType, long lOID, LPCTSTR lpcszCharAddr, bool& bRet );

	/**
	* @brief	�����t���Z���������ǂݕϊ��e�[�u���ŕϊ����ďo��
	*
	* @note		��j�@12-A1����C21-45�R�c��89-�͂Ђ�34
	*			������j�P�Q�|�`�P�^�J�n�V�b�Q�P�|�S�T���}�_�A�W�X�|�n�q�t�R�S
	*			���s��j�P�Q�|�`�P�^�J�n�V�b�Q�P�|�S�T�R�c���W�X�|�n�q�t�R�S
	*
	* @param	lpcszCharAddr[in]			�����t���Z������
	* @param	bRet[in,out]				�ǂݕϊ� true:����,false:���s
	* @return	�ϊ�������̕�������o�́i���s���Ă��ǂ݂̏o�͕͂Ԃ��j
	*/
	std::string GetYomiFromCharAddr_Kanji( LPCTSTR lpcszCharAddr, bool& bRet );

	/**
	* @brief	�����t���Z���̕������P�ꖈ�ɕ����������X�g���擾
	* @note		1�͊������ȃJ�i�A�p���A�����A�u-�v�ŃZ�p���[�g
	* @note		2�͊������ȃJ�i�p���A�����A�u-�v�ŃZ�p���[�g�i��������g�p����̂�[bug7333]�Ή��̂��߁j
	*
	* @param	lpszCharAddr[in]			�����t���Z��
	* @param	sjisSeparate[in]			�u-�v���u�|�v���w��
	* @return	�����t���Z�����X�g
	*/
	std::list<std::pair<YomiType,std::string>> GetCharAddrList( LPCTSTR lpcszCharAddr, const sjis_ranges& sjisSeparate = sjis_ranges().add(sjis_range(0x2d,0x2d)) );
	std::list<std::pair<YomiType,std::string>> GetCharAddrList2( LPCTSTR lpcszCharAddr, const sjis_ranges& sjisSeparate = sjis_ranges().add(sjis_range(0x2d,0x2d)));

	/**
	* @brief	�I�u�W�F�N�gID�ǂ݃e�[�u���̃��R�[�h���X�g���擾����
	*/
	std::map<ObjectLayerPair,KanjiYomiPair> getObjectYomiItems(){ return m_mapObjectYomiTable; }

	/**
	* @brief	�����ǂ݃e�[�u���̃��R�[�h���X�g���擾����
	*/
	std::map<std::string,std::string> getKanjiYomiItems(){ return m_mapKanjiYomiTable; }

private:
	/**
	* @brief	�I�u�W�F�N�g�ǂݕϊ��e�[�u���̃f�[�^���X�g���擾
	* @param	ipTable[in]					�I�u�W�F�N�gID�ǂ݃e�[�u��
	* @param	mapError[in,out]			�e�[�u���̃��R�[�h�ɃG���[���������ꍇ�o�͂���
	* @return	�I�u�W�F�N�gID�ǂ݃e�[�u���}�b�v
	*/
	std::map<ObjectLayerPair,KanjiYomiPair> GetObjectYomiTableItems( ITable* ipTable, std::multimap<long,CString>& mapError );

	/**
	* @brief	�����ǂݕϊ��e�[�u���̃f�[�^���X�g���擾
	* @param	cKanjiYomiCursor[in]		�����ǂ݃e�[�u��
	* @param	mapError[in,out]			�e�[�u���̃��R�[�h�ɃG���[���������ꍇ�o�͂���
	* @return	�����ǂ݃e�[�u���}�b�v
	*/
	std::map<std::string,std::string> GetKanjiYomiTableItems( CADOCursor& cKanjiYomiCursor, std::multimap<long,CString>& mapError );

	/**
	* @brief	�����ǂ݃e�[�u������ǂ݂��擾
	* @param	strCharAddr[in]				�����ǂ݃e�[�u���Ɠ˂����킹�邽�߂̕�����
	* @param	bRet[in,out]				�e�[�u���ɂ��镶���񂾂������ǂ����itrue:������,false:�Ȃ��j
	* @return	�ǂ�
	*/
	std::string GetYomiFromKanjiYomiTable( const std::string& strCharAddr, bool& bRet );

	/**
	* @brief	�T�u�e�[�u������ǂ݂��擾
	* @param	strCharAddr[in]				�T�u�e�[�u���Ɠ˂����킹�邽�߂̕�����
	* @param	bRet[in,out]				�e�[�u���ɂ��镶���񂾂������ǂ����itrue:������,false:�Ȃ��j
	* @return	�ǂ�
	*/
	std::string GetYomiFromSubTable( const std::wstring& strCharAddr, bool& bRet );

	/**
	* @brief	�����t���Z���������ǂݕϊ��e�[�u���ŕϊ����ďo��
	* @note		GetYomiFromCharAddr()�ŌĂ΂��֐�
	*			GetCharAddrList()�Ŏ擾�������X�g���g�p����
	* @param	listCharAddr[in]			�����t���Z���̕������P�ꖈ�ɕ����������X�g
	* @param	bRet[in,out]				�e�[�u���ɂ��镶���񂾂������ǂ����itrue:������,false:�Ȃ��j
	* @return	�ǂ�
	*/
	std::string GetYomi( const std::list<std::pair<YomiType,std::string>>& listCharAddr, bool& bRet );

	/**
	* @brief	�ϊ��e�[�u���̎g�p�����`�F�b�N
	* @param	strYomi[in]					�`�F�b�N������
	* @param	nType[in]					�ϊ��e�[�u���^�C�v
	* @retval	true	[�g�p����OK]
	* @retval	false	[�g�p����NG]
	*/
	bool IsCheckYomi( const std::string& strYomi, CharAddrType nType );

	/**
	* @brief	�����Ɠǂ݂̐����������Ă��邩�`�F�b�N
	* @param	lpcszKanji[in]				����
	* @param	lpcszYomi[in]				�ǂ�
	* @param	lOID[in]					�I�u�W�F�N�gID
	* @param	mapError[in,out]			�G���[���i�������ɃG���[������΂���ɒǋL����j
	* @retval	true	�����������Ă���
	* @retval	false	�����������Ă��Ȃ�
	*/
	bool IsCheckKanjiYomi(LPCTSTR lpcszKanji, LPCTSTR lpcszYomi, long lOID, std::multimap<long,CString>& mapError);

	/**
	* @brief	�����ϊ�
	* @note		�S�p������->�S�p�J�^�J�i
	* @note		���p����->�S�p����
	*/
	std::string ConvertHiraToKata(std::string strHira);
	std::string ConvertHanNumToZenNum(std::string strHanNum);

private:
	CSubConvertTable m_cSubConvert;									//!< �T�u�ǂݕϊ��e�[�u��
	std::map<std::string,std::string> m_mapKanjiYomiTable;			//!< �����ǂݕϊ��e�[�u���̃}�b�v
	std::map<ObjectLayerPair,KanjiYomiPair> m_mapObjectYomiTable;	//!< �I�u�W�F�N�gID�ǂݕϊ��e�[�u��
	std::map<std::wstring,std::wstring> m_mapSubTable;				//!< �T�u�ǂݕϊ��e�[�u���̃}�b�v
};
