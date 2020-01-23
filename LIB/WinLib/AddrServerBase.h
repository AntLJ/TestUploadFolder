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

/**
 * @brief �Z���T�[�o�ڑ��Ǘ��N���X
 *
 * �w�ǂ�proteus�̃v���V�[�W���̈ڐA�����ł��B
 * �������A5�����x���܂ł̓L���b�V�����邽�߁A�����f�[�^��
 * ��������Α����قǃv���V�[�W�����������ɓ��삵�܂��B
 *
 **/
class CAddrServerBase
{
public:
	CAddrServerBase(void);
	virtual ~CAddrServerBase(void);

	/// �ڑ�����
	bool Connect( LPCTSTR lpcszConn );
	/// �I�[�i�[�Z�b�g
	void SetOwner( LPCTSTR lpcszOwner ){ m_strOwner = lpcszOwner; }
	LPCTSTR GetOwner() const { return m_strOwner; }
	/// �v���V�[�W�����g�p���邩�ǂ���
	void SetUseProcedure( bool bUse ){ m_bUseProcedure = bUse; }
	bool GetUseProcedure() const { return m_bUseProcedure; }

	CADOBase& ADO() { return m_cConn; }
	/**
	* @brief �Z�������񂩂���W���擾����t�@���N�V����(SS-PJ�Ŏg�p)
	* @param lpcszAddr  �Z��������
	* @retval ���ʕ�����(�e�������J���}��؂�ŕԂ����)
	*         �e�����͉��L�̒ʂ�
	*          (1) ��͑ΏۏZ��������
	*          (2) ��͐��x(�ǂ̃��x���܂ŉ�͂ł�����)
	*          (3)�`(8)  �s���{���R�[�h�`�Z���ԍ��R�[�h
	*          (9)�`(14) �s���{�����́`�Z���ԍ�����
	*          (15) ���b�V���R�[�h
	*          (16) ���b�V��X
	*          (17) ���b�V��Y
	*          (18) ���W���x
	*/
	CString addrtocode( LPCTSTR lpcszAddr ) const;

	/// �Z���R�[�h�擾�֐�
	CString addr2code( const CString& strAddr ) const;
	CString _addr2code( const CString& strAddr ) const;

	/// �Z���R�[�h�擾�֐��Q(�s�撬���R�[�h�w���)
	CString addr2code( const CString& strCityCode, const CString& strAddr ) const;

private:
	/**
	 * @brief �Z���R�[�h����Z�����擾
	 * @param lpcszAddrCode	[in]		�Z���R�[�h
	 * @param listName		[in out]	�Z�����i�[�p
	 */
	bool GetAddrName( LPCTSTR lpcszAddrCode, std::vector<CString>& listName ) const;

	/**
	 * @brief �Z���R�[�h������W�擾
	 * @param lpcszAddrCode	[in]	�Z���R�[�h
	 * @param seido			[in]	�����񐸓x
	 * @param mesh			[out]	���b�V���R�[�h
	 * @param x				[out]	100�����K��X���W
	 * @param y				[out]	100�����K��Y���W
	 * @param seido_ap		[out]	���W���x
	 */
	bool GetAddrPos( LPCTSTR lpcszAddrCode, long seido, int& mesh, int& x, int& y, long& seido_ap ) const;

	/// �Z��������𐳋K������i�h�炬�̋z���j
	static CString CommonExpr( CString& strAdrs );

	/// ���������񂪐擪���瑶�݂����ꍇ�ɂ��̕�������J�b�g����
	static CString ReplaceHead( const CString& src, const CString& rep );

	/// �u�厚�v�u���v�u�����v�̏���
	static CString RemoveAzaHead( const CString& src );

	/// ���������A���r�A�����ϊ��֐��i�u��v�`�u��\��v�� �u�P�v�`�u�X�X�v�j
	static CString Kanji2Num( const CString& src );

	/**
	 * @brief	���������A���r�A�����ϊ��֐��i�u��v�`�u��\��v�� �u�P�v�`�u�X�X�v�j
	 * @note	���ł�����ł��ϊ������A�ȉ��̏����ɍ��v����������ϊ�
	 *			(1) ���� ���n�� ���� ���� ���|
	 * @param	SrcStr	[in]	�Ώۃf�[�^
	 * @return	�ϊ��f�[�^
	 */
	static CString Kanji2NumEx( const CString& src );

	/**
	 * @brief	�s���{���R�[�h�擾�֐�
	 * 
	 * strAddr�Ɋ܂܂��s���{������O����v�Ō������A���̂ƃR�[�h���Z�b�g�ŕԂ��܂��B
	 * strAddr�͓s���{�����ȊO���܂�ł��Ă������ł��B
	 *
	 * @param	strAddr		[in,out]		��͕�����i��͂ł������͍폜�����j
	 * @param	doCache		[in,optional]	�L���b�V�������邩�ǂ����i�f�t�H���g�Ftrue�j
	 * @retval bool
	 */
	bool GetKenCode( CString& strAddr, CString& strRetKenCode, bool doCache = true ) const;
	/**
	 * @brief	�s�撬���R�[�h�擾�֐�
	 * 
	 * strAddr�Ɋ܂܂��s�撬������O����v�Ō������A���̂ƃR�[�h���Z�b�g�ŕԂ��܂��B
	 * strAddr�͎s�撬�����ȊO���܂�ł��Ă������ł��B
	 * �Ȃ��A�߂�l�̎s�撬�����͐��K�����ꂽ������ł��邱�Ƃɒ��ӂ��Ă��������B
	 *
	 * @param	strAddr		[in,out]		��͕�����i��͂ł������͍폜�����j
	 * @param	strKenCode	[in]			�s���{���R�[�h
	 * @param	doCache		[in,optional]	�L���b�V�������邩�ǂ����i�f�t�H���g�Ftrue�j
	 * @retval bool
	 */
	bool GetShiCode( CString& strAddr, const CString& strKenCode, CString& strRetShiCode, bool doCache = true ) const;

	/**
	* @brief	�厚�E���R�[�h�擾�֐�
	* @param	strAddr		[in out]	��͕�����
	* @param	strKenCode	[in]		�s���{���R�[�h
	* @param	strShiCode	[in]		�s�撬���R�[�h
	* @param	strOazaCode	[out]		��͌��ʑ厚�E�ʏ̃R�[�h
	* @param	strAzaCode	[out]		��͌��ʎ��E���ڃR�[�h
	*/
	bool GetOazaAzaCode( CString& strAddr, const CString& strKenCode, const CString& strShiCode, CString& strRetOazaCode, CString& strRetAzaCode ) const;

	/**
	 * @brief	�n�ԃR�[�h�擾�֐�
	 * @param	AdrsStr		[in out]	��͕�����
	 * @param	RetF		[in]		�厚���? �����?
	 * @param	KenCode		[in]		�s���{���R�[�h
	 * @param	ShiCode		[in]		�s�撬���R�[�h
	 * @param	OazaCode	[in]		�厚�E�ʏ̃R�[�h
	 * @param	AzaCode		[in out]	���E���ڃR�[�h
	 * @param	r_ChibanCode[out]		��͌��ʒn�ԃR�[�h
	 */
	bool GetChibanCode( CString& strAddr, int nRetF, const CString& strKenCode, const CString& strShiCode, const CString& strOazaCode, CString& strAzaCode, CString& strRetChibanCode ) const;

	/**
	 * @brief	�Z���ԍ��R�[�h�擾�֐�
	 * @param	AdrsStr		[in out]	��͕�����
	 * @param	KenCode		[in]		�s���{���R�[�h
	 * @param	ShiCode		[in]		�s�撬���R�[�h
	 * @param	OazaCode	[in]		�厚�E�ʏ̃R�[�h
	 * @param	AzaCode		[in]		���E���ڃR�[�h
	 * @param	ChibanCode	[in]		�n�ԃR�[�h
	 * @param	r_BangoCode	[in]		��͌��ʏZ���ԍ��R�[�h
	 */
	bool GetBangoCode( CString& strAddr, const CString& strKenCode, const CString& strShiCode, const CString& strOazaCode, const CString& strAzaCode, const CString& strChibanCode, CString& strRetBangoCode ) const;

protected:
	CADOBase m_cConn;
	CString m_strOwner;
	bool m_bUseProcedure; //!< �v���V�[�W�����g�p���邩�ǂ���
	bool m_bIsOracle; //!< Oracle���ǂ����ifalse�Ȃ�PostgreSQL�Ƃ������f�j
};
