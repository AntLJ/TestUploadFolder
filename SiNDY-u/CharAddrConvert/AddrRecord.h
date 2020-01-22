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

/**
 * @struct	CMeshXY
 * @brief	���b�V���w�x
 */
struct CMeshXY
{
	int	m_nMeshCode;	//!< ���b�V���R�[�h
	int	m_nX;			//!< X
	int	m_nY;			//!< Y

	CMeshXY() : m_nMeshCode(0), m_nX(0), m_nY(0){}

	/**
	 * @brief	�ʒu���Z�b�g
	 * @param	ipPoint	[in]	�|�C���g
	 */
	void SetPos( IPointPtr ipPoint );
};

/**
 * @struct	CAddrBaseRec
 * @brief	�Z�����R�[�h���N���X
 */
struct CAddrBaseRec
{
	CString		m_strAddrCode;		//!< �Z���R�[�h
	double		m_dLon;				//!< �o�x
	double		m_dLat;				//!< �ܓx
	CMeshXY		m_cMXY;				//!< ���b�V���w�x

	CString		m_strPntName;		//!< �n�Ԗ���
	CString		m_strPntYomi;		//!< �n�ԃ��~

	CString		m_strZipCode;		//!< �X�֔ԍ�

	long		m_lGouType;			//!< ���^�C�v
	bool		m_bTsushoConv;		//!< �ʏ̍s���E�ɂ�萶���������R�[�h

	CAddrBaseRec() : m_dLon(0.0), m_dLat(0.0), m_lGouType(-1), m_bTsushoConv(false){}

	/**
	 * @brief	�ʒu���Z�b�g
	 * @brief	ipPoint	[in]	�|�C���g
	 */
	void SetCoord( IPointPtr ipPoint )
	{
		if( ipPoint )
		{
			ipPoint->QueryCoords( &m_dLon, &m_dLat );
			m_cMXY.SetPos( ipPoint );
		}
	}
};

struct CPntRec;
struct CGouRec;

/**
 * @struct	CGouRec
 * @brief	Gou�f�[�^���R�[�h���
 */
struct CGouRec : public CAddrBaseRec
{
	long		m_lOID;				//!< ���ƂȂ��Ă��鍆�|�C���gID
	CString		m_strGouName;		//!< �Z���ԍ�����
	CString		m_strGouYomi;		//!< �Z���ԍ����~

	bool		m_bPriorF;			//!< �D��t���O
	bool		m_bInArea;			//!< �G���A���H

	double		m_dKakeiArea;		//!< ���|�C���g���ƌ`���ɂ���Ƃ��A���̉ƌ`�̖ʐ�

	CGouRec() : m_lOID(0), m_bPriorF(false), m_bInArea(false){}
};

/**
 * @struct	CPntRec
 * @brief	Pnt�f�[�^���R�[�h���
 */
struct CPntRec : public CAddrBaseRec
{
	long	m_lRepDomariF;	//!< ��\�_�E�n�Ԏ~�܂�t���O

	CPntRec() : m_lRepDomariF(0){}

	/**
	 * @brief	GOU���R�[�h����̃R���X�g���N�^
	 * @param	cGouRec		[in]	�����R�[�h
	 * @param	lRepDomariF	[in]	��\�_�E�n�Ԏ~�܂�t���O
	 */
	CPntRec( const CGouRec& cGouRec, long lRepDomariF )
	{
		m_strAddrCode	= cGouRec.m_strAddrCode.Left(16);
		m_strPntName	= cGouRec.m_strPntName;
		m_strPntYomi	= cGouRec.m_strPntYomi;
		m_strZipCode	= cGouRec.m_strZipCode;
		m_lGouType		= cGouRec.m_lGouType;
		m_bTsushoConv	= cGouRec.m_bTsushoConv;
		m_dLon			= cGouRec.m_dLon;
		m_dLat			= cGouRec.m_dLat;
		m_cMXY			= cGouRec.m_cMXY;
		m_lRepDomariF	= lRepDomariF;
	}
};

typedef std::map<CString, CPntRec> PNTMAP;		// �L�[:16���Z���R�[�h �l:���R�[�h
typedef std::vector<CGouRec> GOULIST;			// �����R�[�h���X�g
typedef std::multimap<CString, CGouRec> GOUMAP;	// �L�[:20���Z���R�[�h �l:���R�[�h
