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
 * @file	define.h
 * @brief	�e���`
 */

using namespace std;

#define PROGRAM_NAME	_T("CheckTelTPError")	// �v���O������

/**
 * @enum	ECheckType
 * @brief	�`�F�b�N�^�C�v
 */
enum ECheckType
{
	kCTAddr16	= 0,	//!< 16������
	kCTAddr11	= 1,	//!< 11������
	kCTAddrEX	= 2,	//!< 11�� + �g�����ԍ�=On ����
	kCTAddrCM	= 3,	//!< 8���{�u000�v���m�Z���ϊ����[�h�F�厚�������n����
};

/**
 * @enum	EResultStat
 * @brief	�`�F�b�N���ʃX�e�[�^�X
 */
enum EResultStat
{
	kO_TELTP_ADDED,			//!< �V�K�ǉ�
	kO_TELTP_OK,			//!< ���Ȃ�
	kW_TELTP_NOT_FOUND,		//!< �y�v�m�F�z�Ώۃ|�C���g�����݂��Ȃ�
	kW_TELTP_FAIL_PSTAT,	//!< �y�v�m�F�z�u�Y���{�݂���v�łȂ�
	kW_TELTP_FAIL_ADDXY,	//!< �y�v�m�F�z���W�t�^��ʂ��Ⴄ
	kW_TELTP_MOVED,			//!< �y�v�m�F�z�Ώۃ|�C���g�ړ�
	kW_DUPLI_TELTP,			//!< �y�v�m�F�zTelTP�|�C���g����������
	kE_FAIL_GET_INFO,		//!< �y�G���[�zTelTP�|�C���g�̏��擾���s
	kE_FAIL_GET_ADDXY,		//!< �y�G���[�zADDXY_C�擾���s
};

static const _TCHAR* GetResultMsg( EResultStat emStatus )
{
	switch( emStatus )
	{
	case kO_TELTP_ADDED:			return _T("�V�K�ǉ�");
	case kO_TELTP_OK:				return _T("���Ȃ�");
	case kW_TELTP_NOT_FOUND:		return _T("�y�v�m�F�z�Ώۃ|�C���g�����݂��Ȃ�");
	case kW_TELTP_FAIL_PSTAT:		return _T("�y�v�m�F�z�u�Y���{�݂���v�łȂ�");
	case kW_TELTP_FAIL_ADDXY:		return _T("�y�v�m�F�z���W�t�^��ʂ��Ⴄ");
	case kW_TELTP_MOVED:			return _T("�y�v�m�F�z�Ώۃ|�C���g�ړ�");
	case kW_DUPLI_TELTP:			return _T("�y�v�m�F�z����V�[�P���X�����W�t�^��ʂ�TelTP�|�C���g����������");
	case kE_FAIL_GET_INFO:			return _T("�y�G���[�zTelTP�|�C���g�̏��擾���s");
	case kE_FAIL_GET_ADDXY:			return _T("�y�G���[�zADDXY_C�擾���s");
	default:
		return _T("�s��");
	}
}

/**
 * @struct	CMeshXY
 * @brief	���b�V��XY
 */
struct CMeshXY
{
	int			m_nMesh;	//!< ���b�V���R�[�h
	int			m_nX;		//!< ���b�V��X
	int			m_nY;		//!< ���b�V��Y

	CMeshXY() : m_nMesh(0), m_nX(0), m_nY(0){}

	/**
	 * @brief	== �̃I�[�o�[���[�h
	 * @note	�����ł́A���b�V���R�[�h���������@���� ���K��XY��100�����̍��ł���Γ������Ƃ���
	 * @return	bool
	 */
	bool operator==( const CMeshXY& ob )
	{
		return (m_nMesh == ob.m_nMesh) && (abs(m_nX - ob.m_nX) < 100) && (abs(m_nY - ob.m_nY) < 100);
	}

};

/**
 * @struct	CLRec
 * @brief	���X�g���R�[�h
 */
struct CLRec
{
	CString			m_strClaimNo;		//!< �N���[��No
	CString			m_strObjectID;		//!< ObjectID
	CString			m_strSequence;		//!< �V�[�P���X
	CMeshXY			m_cMXY;				//!< ���b�V��XY
	long			m_lAddXY;			//!< ���W�t�^���(AddXY_C)
	long			m_lUserClaimF;		//!< ���[�U�N���[���t���O
	CString			m_strTelNum;		//!< �d�b�ԍ�
	CString			m_strAddrName;		//!< �f�ڏZ������
	CString			m_strAddrCode;		//!< �Z���R�[�h(11��)
	CString			m_strKatagaki;		//!< ��������
	CString			m_strKeisaiName;	//!< �f�ږ���
	long			m_lPointStat;		//!< �������ʃR�[�h
	CString			m_strSource;		//!< ���\�[�X
	CMeshXY			m_cMXY_Ref;			//!< �ړ��惁�b�V��XY
	CString			m_strAddComment;	//!< �ǉ��R�����g

	CLRec() : m_lAddXY(1), m_lPointStat(0), m_lUserClaimF(0){}

};

typedef	std::list<CLRec>	CLList;
