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

#ifndef __RNS_STRING__
#define	__RNS_STRING__

/* ************************************************

 	���e :
 	���� :  
 	�ǋL : 
 
 	���� :	Fukuta.K
 	
************************************************* */

/* ///// �C���N���[�h�t�@�C�� ///// */

#include <atlbase.h>

#include <string>
#include <vector>

#include "RNS.h"
#include "RNSPlant.h"
#include "RNSObject.h"


/* ////// �N���X��`  ////// */


/**
 * ������N���X
 *
 * STL��string��
 *
 * - ���̓X�g���[��
 * - ���l�̕�����ϊ�
 *
 * �Ȃǂ̋@�\��ǉ���������
 *
 */
class RNSString : public std::string, public RNSObject {
public:
	/* ----- �����ݒ� ----- */
	/// �f�t�H���g�R���X�g���N�^
	RNSString(){}

	/**
	* �R���X�g���N�^�i���������j
	*
	* @param	iStr	������镶����
	*/
	RNSString(const char *iStr);

	/**
	* �R���X�g���N�^�i���������j
	*
	* @param	iStr	������镶����
	*/
	RNSString(const std::string &iStr);

	/**
	 * �R���X�g���N�^�i���l��������ϊ�����j
	 *
	 * @param	iValue	������鐔�l
	 */
	RNSString(int iValue);

	/**
	 * �R���X�g���N�^�i�����Ȃ����l��������ϊ�����j
	 *
	 * @param	iValue	������镄���Ȃ����l
	 */
	RNSString(unsigned int iValue);

	/// �f�X�g���N�^
	virtual ~RNSString(){}

	/* ----- �ǉ� ----- */
	/**
	* �������ǉ�����
	*
	* @param	iStr	�ǉ����镶����
	*
	* @return	���Ȃւ̎Q��
	*/
	virtual RNSString& append(const RNSString &iStr);

	/**
	* �w�肵��������������ǉ�����
	*
	* @param	n		�ǉ����镶����
	* @param	iChar	�ǉ����镶��
	*
	* @return	���Ȃւ̎Q��
	*/
	virtual RNSString& append(size_type n, char &iChar);

	/**
	* �������������
	*
	* @param	iStr	�ǉ����镶����
	*
	* @return	���Ȃւ̎Q��
	*/
	virtual RNSString& operator = (const std::string &iStr);

	/**
	* ������萔��������
	*
	* @param	iStr	�ǉ����镶����
	*
	* @return	���Ȃւ̎Q��
	*/
	virtual RNSString& operator = (const char *iStr);

	/* ----- �폜 ----- */
	/**
	 * �R�����g�����폜����istring �^�j
	 *
	 * �f�t�H���g�ł� # �ڍs�̕����񂪃R�����g�Ɣ��f�����B
	 *
	 * @param	iStr	�R�����g������
	 */
	void eraseComment(const std::string& iStr);

	/**
	 * �R�����g�����폜����iconst char �^�j
	 *
	 * �f�t�H���g�ł� # �ڍs�̕����񂪃R�����g�Ɣ��f�����B
	 *
	 * @param	iStr	�R�����g������
	 */
	void eraseComment(const char* iStr = "#");

	/**
	 * �����񗼒[�̗]���Ȃ��̂�����
	 *
	 * �f�t�H���g�ł̓X�y�[�X�ƃ^�u���������
	 *
	 * @param	iTrimChars
	 */
	RNSString trimedString(const char* iTrimChars);

	/* ----- �擾 ----- */
	/**
	 * ������𕪊�����istring �Z�p���[�^�j
	 *
	 * �w�肵���Z�p���[�^�ŁA���̕�����𕪊�����B
	 * �Ⴆ�΁A�Z�p���[�^�� ":" �� ������ "CODE : VALUE" �̏ꍇ�A"CODE " �� " VALUE" �Ƃ����Q�̕�����ɕ��������B
	 *
	 * @param	oDivideStrList	�������ꂽ������ꗗ
	 * @param	iSeparator		�Z�p���[�^
	 * @param	iTrimChars		���������ہA���[����g�������镶��
	 * @return	�������ꂽ������
	 */
	int divide(std::vector< RNSRef<RNSString> >* oDivideStrList, const std::string& iDelim, const char* iTrimChars = NULL) const;

	/* ----- �ǉ��I�y���[�^ ----- */
	/// �������ǉ�����
	RNSString& operator += (const char* iStr);
	/// �������ǉ�����
	RNSString& operator += (const RNSString &iStr);
	/// ������ǉ�����
	RNSString& operator += (char iChar );
	/// �����𕶎��Ƃ��Ēǉ�����
	RNSString& operator += (int iValue);
	/// �����Ȃ������𕶎��Ƃ��Ēǉ�����
	RNSString& operator += (unsigned int iValue);

	/// �������ǉ�����
	RNSString& operator << (const RNSString &iStr);
	/// ������ǉ�����
	RNSString& operator << (char iChar);
	/// �����𕶎��Ƃ��Ēǉ�����
	RNSString& operator << (int iValue);

private:
	/// �ǉ��A������s���O�ɍs������
 	virtual void appendPreEffect(){};

	/// �ǉ��A������s������ɍs������
	virtual void appendEpiEffect(){};

};

_RNSOBJECT_TYPEDEF(RNSString);

const std::string narrow(const std::wstring& cInput);
const std::wstring widen(const std::string& cInput);

#endif /* __RNS_STRING__ */
