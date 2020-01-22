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

#include "stdafx.h"
#include "OutputLog.h"
#include <jis/x0208/spirit/shift_jis.h>

using namespace jis::x0208::spirit;

/**
 * @brief   ���̃`�F�b�N�N���X
 */
class cNameChk
{
public:

	
	/**
	 * @brief   �R���X�g���N�^
	 */
	cNameChk()
	{
		SetString();
	};

	/**
	 * @brief   �f�X�g���N�^
	 */
	~cNameChk(){};

	/**
	 * @brief   �T�C�Y��������
	 * @param   str [in]  ������
	 * @retval  true  ����
	 * @retval  false �
	 */
	bool IsEvenSize( const std::string& str ){
		if( (str.size() % 2) == 0 )
			return true;
		
		return false;
	};

	/**
	 * @brief   ��������
	 * @note    2�o�C�g�P�ʂŌ�������
	 * @param   str      [in]  ������
	 * @param   find     [in]  �������镶����
	 * @param   startPos [in]  �J�n�ʒu
	 * @retval  string::size_type �������ʒu
	 */
	std::string::size_type string_find( const std::string& str, const std::string& find, int startPos = 0 ){
		long size = str.size();
		for( int i = startPos ; i < size ; ){
			std::string::size_type pos = str.find( find, i );
			if( pos == std::string::npos )
				return std::string::npos;
			if( ( pos % 2 ) != 0 ){
				i = pos + 1;
				continue;
			}
			return pos;
		}
		return std::string::npos;
	};

	/**
	 * @brief   �󕶎��񂩁H
	 * @param   str [in]  ������
	 * @retval  true  ��
	 * @retval  false �󂶂�Ȃ�
	 */
	bool IsNullString( const std::string& str ){
		if( str.empty() || 0 == str.compare( _T(" ") ) )
			return true;
		return false;
	};

	/**
	 * @brief   �S�Ďd�l�͈͂̕�����
	 * @param   str [in]  ������
	 * @retval  true  OK
	 * @retval  false NG
	 */
	bool IsOkString( const std::string& str ){
		
		if( ! parse( str.c_str(), *m_okStrs ).full )
			return false;

		return true;
	};

	/**
	 * @brief   �u�b�v�����݂��邩
	 * @param    str [in]  ������
	 * @retval   true  ���݂���
	 * @retval   false ���݂��Ȃ�
	 */
	bool IsFindPipe( const std::string& str ){
		if( string_find( str, "�b" ) != std::string::npos )
			return true;
		return false;
	};

	/**
	 * @brief   �X�y�[�X�����킩(�擪�A�Ō�A�A����NG)
	 * @param   str [in]  ������
	 * @retval  true  OK
	 * @retval  false NG
	 */
	bool IsOkSpace( const std::string& str ){
		long size = str.size();
		for( int i = 0 ; i < size - 2; i += 2 ){
			auto subStr1 = str.substr( i, 2 );
			auto subStr2 = str.substr( i + 2, 2 );
			if( i == 0 ){ // �ŏ�
				if( IsSpace( subStr1 ) )
					return false;
			}
			if( i == size - 4 ){ // �Ō�
				if( IsSpace( subStr2 ) )
					return false;
			}
			if( IsSpace( subStr1) && IsSpace( subStr2 ) )
				return false;
		}
		return true;
	};

	/**
	 * @brief   �������S�p�X�y�[�X��
	 * @param   word [in]  ����
	 * @retval  true  �S�p�X�y�[�X
	 * @retval  false ����ȊO
	 */
	bool IsSpace( const std::string& word ){ 
		return ( 0 == word.compare( _T("�@") ) ); 
	};

	/**
	 * @brief   �n�C�t���֘A�̕��������킩
	 * @param   str [in]  ������
	 * @retval  err_def::ECode �G���[�R�[�h(OK���܂�)
	 */
	err_def::ECode IsOkHiphen( const std::string& str ){
		using namespace err_def;
		long size = str.size();
		for( int i = 0 ; i < size ; i += 2 ){
			auto subStr = str.substr( i, 2 );
			if( i == 0 ){
				if( IsHiphen( subStr ) )
					return eTopZenHyphen; // �擪NG
				continue;
			}
			auto preStr = str.substr( i - 2, 2 );
			if( IsHiphenPart1( subStr ) ){ // �u�\�vor�u�|�vor�u�]�v
				// �S�p�p�����ȊO�̌�Ɂu�\�vor�u�|�vor�u�]�v������̂�NG
				if( ! parse( preStr.c_str(), *m_alnum ).full )
					return wNgHyphenWord; // NG
			}
			else if( IsHiphenPart2( subStr ) ){ // �u�[�v
				// �Ђ炪�ȁA�J�i�J�i�ȊO�̌�ɁA�u�[�v������̂�NG�i�u��v�Ɓu���v���_���j
				if( ! parse( preStr.c_str(), *m_hirakata ).full || 0 == preStr.compare( _T("��") ) || 0 == preStr.compare( _T("��") ) )
					return wNgHyphenWord; // NG
			}
		}
		return Ok; // OK
	};

	bool IsHiphen( const std::string& word ){ return ( IsHiphenPart1( word ) || IsHiphenPart2( word ) ); };
	bool IsHiphenPart1( const std::string& word ){ return ( 0 == word.compare( _T("�\") ) || 0 == word.compare( _T("�|") ) || 0 == word.compare( _T("�]") ) ); };
	bool IsHiphenPart2( const std::string& word ){ return ( 0 == word.compare( _T("�[") ) ); };

	// �������֘A�����킩
	err_def::ECode IsOkLowStr( const std::string& str ){
		using namespace err_def;
		long size = str.size();
		for( int i = 0 ; i < size ; i += 2 ){
			auto subStr = str.substr( i, 2 );
			if( i == 0 ){
				if( IsHiraLowYaYuYo( subStr ) )
					return eTopHiraLowY; // �擪�u����vNG
				if( IsKataLowYaYuYo( subStr ) )
					return eTopKataLowY; // �擪�u�������vNG
				if( IsHiraLowTsu( subStr ) )
					return eTopHiraLowTsu; // �擪�u���vNG
				if( IsKataLowTsu( subStr ) )
					return eTopKataLowTsu; // �擪�u�b�vNG
				if (IsHiraLowAiueoN(subStr))
					return eTopHiraLowAN; // �擪�u�����������vNG
				if (IsKataLowAiueoN(subStr))
					return eTopKataLowAN; // �擪�u�@�B�D�F�H�vNG
				continue;
			}
			if( i == (size - 2) ){
				if( IsHiraLowTsu( subStr ) )
					return eNgHiraLowTsu; // �Ō�u���vNG
				if( IsKataLowTsu( subStr ) )
					return eNgKataLowTsu; // �Ō�u�b�vNG
			}

			auto preStr = str.substr( i - 2, 2 );
			if( IsHiraLowYaYuYo( subStr ) ){
				// �u���v�u���v�u���v�u�Ɂv�u�Ёv�u�݁v�u��v�u���v�u���v�u���v�u�сv�u�ҁv	�ȊO�̌�Ɂu����v������
				if( ! HaveHiraLowYaYuYo( preStr ) )
					return err_def::eNgHiraLowY; // NG
			}
			else if( IsKataLowYaYuYo( subStr ) ){
				// �u�L�v�u�V�v�u�`�v�u�j�v�u�q�v�u�~�v�u���v�u�M�v�u�W�v�u�a�v�u�r�v�u�s�v�ȊO�̌�Ɂu�������v������
				if( ! HaveKataLowYaYuYo( preStr ) )
					return err_def::eNgKataLowY; // NG
			}
			else if( IsHiraLowTsu( subStr ) ){
				// �u��v�u���v�u�[�v�̌�Ɂu���v������
				if( NotHaveHiraLowTsu( preStr ) )
					return eNgHiraLowTsu; // NG
				// �Ђ炪�ȁA�J�^�J�i�ȊO�̌�Ɂu���v������
				if( ! parse( preStr.c_str(), *m_hirakata ).full )
					return wNgHiraLowTsu; // NG
			}
			else if( IsKataLowTsu( subStr ) ){
				// �u���v�u�b�v�u�[�v�̌�Ɂu�b�v������
				if( NotHaveKataLowTsu( preStr ) )
					return eNgKataLowTsu; // NG
				// �Ђ炪�ȁA�J�^�J�i�A�������ȊO�̌�Ɂu�b�v������
				if( ! parse( preStr.c_str(), *m_hirakata ).full && ! IsKanNum( preStr ) )
					return wNgKataLowTsu; // NG
			}
		}
		return err_def::Ok; // OK
	};

	bool IsHiraLowYaYuYo( const std::string& word ){ return ( 0 == word.compare( _T("��") ) || 0 == word.compare( _T("��") ) || 0 == word.compare( _T("��") ) ); };
	bool IsHiraLowAiueoN(const std::string& word) { return (0 == word.compare(_T("��")) || 0 == word.compare(_T("��")) || 0 == word.compare(_T("��")) || 0 == word.compare(_T("��")) || 0 == word.compare(_T("��")) || 0 == word.compare(_T("��"))); };
	bool HaveHiraLowYaYuYo( const std::string& word ){
		return ( 0 == word.compare( _T("��") ) || 0 == word.compare( _T("��") ) || 0 == word.compare( _T("��") )
			|| 0 == word.compare( _T("��") ) || 0 == word.compare( _T("��") ) || 0 == word.compare( _T("��") )
			|| 0 == word.compare( _T("��") ) || 0 == word.compare( _T("��") ) || 0 == word.compare( _T("��") )
			|| 0 == word.compare( _T("��") ) || 0 == word.compare( _T("��") ) || 0 == word.compare( _T("��") )
			);
	};
	bool IsKataLowYaYuYo( const std::string& word ){ return ( 0 == word.compare( _T("��") ) || 0 == word.compare( _T("��") ) || 0 == word.compare( _T("��") ) ); };
	bool IsKataLowAiueoN(const std::string& word) { return (0 == word.compare(_T("�@")) || 0 == word.compare(_T("�B")) || 0 == word.compare(_T("�D")) || 0 == word.compare(_T("�F")) || 0 == word.compare(_T("�H")) || 0 == word.compare(_T("��"))); };
	bool HaveKataLowYaYuYo( const std::string& word ){
		return ( 0 == word.compare( _T("�L") ) || 0 == word.compare( _T("�V") ) || 0 == word.compare( _T("�`") )
			|| 0 == word.compare( _T("�j") ) || 0 == word.compare( _T("�q") ) || 0 == word.compare( _T("�~") )
			|| 0 == word.compare( _T("��") ) || 0 == word.compare( _T("�M") ) || 0 == word.compare( _T("�W") )
			|| 0 == word.compare( _T("�a") ) || 0 == word.compare( _T("�r") ) || 0 == word.compare( _T("�s") )
			);
	};
	bool IsHiraLowTsu( const std::string& word ){ return ( 0 == word.compare( _T("��") ) ); };
	bool NotHaveHiraLowTsu( const std::string& word ){ return ( 0 == word.compare( _T("��") ) || 0 == word.compare( _T("��") ) || 0 == word.compare( _T("�[") ) ); };
	bool IsKataLowTsu( const std::string& word ){ return ( 0 == word.compare( _T("�b") ) ); };
	bool NotHaveKataLowTsu( const std::string& word ){ return ( 0 == word.compare( _T("��") ) || 0 == word.compare( _T("�b") ) || 0 == word.compare( _T("�[") ) ); };
	bool IsKanNum( const std::string& word ){
		return ( 0 == word.compare( _T("��") ) || 0 == word.compare( _T("��") ) || 0 == word.compare( _T("�O") )
			|| 0 == word.compare( _T("�l") ) || 0 == word.compare( _T("��") ) || 0 == word.compare( _T("�Z") )
			|| 0 == word.compare( _T("��") ) || 0 == word.compare( _T("��") ) || 0 == word.compare( _T("��") )
			|| 0 == word.compare( _T("�\") ) );
	};

	err_def::ECode IsOkKakko( const std::string& str ){
		using namespace err_def;
		if( parse( str.c_str(), *m_kakko ).full )
			return Ok; // �������OK
		long size = str.size();
		long bKakko[2][11] = { 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
		for( int i = 0 ; i < size ; i += 2 ){
			auto subStr = str.substr( i, 2 );
			if( ! IsKakko( subStr ) )
				continue;
			long leftKakko  = IsLeftKakko( subStr );
			if( leftKakko != -1 )
				bKakko[0][leftKakko] = i;

			long rightKakko = IsRightKakko( subStr );
			if( rightKakko != -1 )
				bKakko[1][rightKakko] = i;
		}
		for( int i = 0 ; i < 11 ; ++i ){
			if( ( bKakko[0][i] != -1 ) ^ ( bKakko[1][i] != -1 ) ){
				if( bKakko[0][i] != -1 )
					return eLeftParentOnly;
				return eRightParentOnly;
			}
			if( bKakko[0][i] > bKakko[1][i] )
				return eNgParentheses;
		}
		//std::string::size_type leftPos = str.find_first_of( _T("") );
		//std::string::size_type rightPos = str.find_first_of( _T("") );
		return Ok;
	};
	bool IsKakko( const std::string& word ) { return ( IsLeftKakko(word) || IsRightKakko(word) ); };
	long IsLeftKakko( const std::string& word ) { 
		if( IsLeftKakko0(word) ) return 0; if( IsLeftKakko1(word) ) return 1; if( IsLeftKakko2(word) ) return 2; if( IsLeftKakko3(word) ) return 3;
		if( IsLeftKakko4(word) ) return 4; if( IsLeftKakko5(word) ) return 5; if( IsLeftKakko6(word) ) return 6; if( IsLeftKakko7(word) ) return 7;
		if( IsLeftKakko8(word) ) return 8; if( IsLeftKakko9(word) ) return 9; if( IsLeftKakko10(word) ) return 10;
		return -1;
	};
	long IsRightKakko( const std::string& word ) { 
		if( IsRightKakko0(word) ) return 0; if( IsRightKakko1(word) ) return 1; if( IsRightKakko2(word) ) return 2; if( IsRightKakko3(word) ) return 3;
		if( IsRightKakko4(word) ) return 4; if( IsRightKakko5(word) ) return 5; if( IsRightKakko6(word) ) return 6; if( IsRightKakko7(word) ) return 7;
		if( IsRightKakko8(word) ) return 8; if( IsRightKakko9(word) ) return 9; if( IsRightKakko10(word) ) return 10;
		return -1;
	};

	bool IsLeftKakko0( const std::string& word ) { return ( 0 == word.compare( _T("�i") ) ); };
	bool IsLeftKakko1( const std::string& word ) { return ( 0 == word.compare( _T("��") ) ); };
	bool IsLeftKakko2( const std::string& word ) { return ( 0 == word.compare( _T("�u") ) ); };
	bool IsLeftKakko3( const std::string& word ) { return ( 0 == word.compare( _T("�w") ) ); };
	bool IsLeftKakko4( const std::string& word ) { return ( 0 == word.compare( _T("�y") ) ); };
	bool IsLeftKakko5( const std::string& word ) { return ( 0 == word.compare( _T("�m") ) ); };
	bool IsLeftKakko6( const std::string& word ) { return ( 0 == word.compare( _T("�o") ) ); };
	bool IsLeftKakko7( const std::string& word ) { return ( 0 == word.compare( _T("�k") ) ); };
	bool IsLeftKakko8( const std::string& word ) { return ( 0 == word.compare( _T("�s") ) ); };
	bool IsLeftKakko9( const std::string& word ) { return ( 0 == word.compare( _T("�q") ) ); };
	bool IsLeftKakko10( const std::string& word ) { return ( 0 == word.compare( _T("��") ) ); };
	bool IsRightKakko0( const std::string& word ) { return ( 0 == word.compare( _T("�j") ) ); };
	bool IsRightKakko1( const std::string& word ) { return ( 0 == word.compare( _T("��") ) ); };
	bool IsRightKakko2( const std::string& word ) { return ( 0 == word.compare( _T("�v") ) ); };
	bool IsRightKakko3( const std::string& word ) { return ( 0 == word.compare( _T("�x") ) ); };
	bool IsRightKakko4( const std::string& word ) { return ( 0 == word.compare( _T("�z") ) ); };
	bool IsRightKakko5( const std::string& word ) { return ( 0 == word.compare( _T("�n") ) ); };
	bool IsRightKakko6( const std::string& word ) { return ( 0 == word.compare( _T("�p") ) ); };
	bool IsRightKakko7( const std::string& word ) { return ( 0 == word.compare( _T("�l") ) ); };
	bool IsRightKakko8( const std::string& word ) { return ( 0 == word.compare( _T("�t") ) ); };
	bool IsRightKakko9( const std::string& word ) { return ( 0 == word.compare( _T("�r") ) ); };
	bool IsRightKakko10( const std::string& word ) { return ( 0 == word.compare( _T("��") ) ); };

private:

	void SetString(){
		
		m_okStrs = sjis_ranges()
			.add(sjis_kanji_p) // ���A��񐅏�����
			.add(sjis_kana_p)  // �S�p�Ђ炪�ȁA�J�^�J�i
			.add(sjis_alnum_p) // �S�p�p����
			.add(sjis_greek_p) // �M���V������
			.add(sjis_range(0x8140, 0x81fc)); // �S�p�L���i�S�p�X�y�[�X�`���j

		m_hirakata = sjis_ranges().add(sjis_kana_p);
		m_alnum    = sjis_ranges().add(sjis_alnum_p);
		m_kakko    = sjis_ranges()
			.add(sjis_range(0x8169,0x817a))  // �i	�j	�k	�l	�m	�n	�o	�p	�q	�r	�s	�t	�u	�v	�w	�x	�y	�z
			.add(sjis_range(0x8183,0x8184))  // ���@��
			.add(sjis_range(0x81e1,0x81e2)); // ��@��
	};


private:

	sjis_ranges m_okStrs;   //!< ���e����
	sjis_ranges m_hirakata; //!< �S�p�Ђ炪�ȁE�J�^�J�i
	sjis_ranges m_alnum;    //!< �S�p�A���t�@�x�b�g
	sjis_ranges m_kakko;    //!< �S�p�J�b�R

};
