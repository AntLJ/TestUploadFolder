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

#define PROGRAM_NAME	_T("CheckGPError")	// �v���O������

/**
 * @enum	ECheckType
 * @brief	�`�F�b�N�^�C�v
 */
enum ECheckType
{
	kCTAddr16	= 0,	//!< 16������
	kCTAddr11	= 1,	//!< 11������
	kCTAddrEX	= 2,	//!< 11�� + �g�����ԍ�=On ����
	kCTAddrCM	= 3,	//!< �ʏ̃��[�h����(Bug8166)
};

/**
 * @enum	EResultStat
 * @brief	�`�F�b�N���ʃX�e�[�^�X
 */
enum EResultStat
{
	kO_NOT_FOUND,			//!< CS or Gou����Y���|�C���g�݂���Ȃ�
	kO_MATCH_PRIOR_GP,		//!< Gou�ƗD��t���O����|�C���g�ň�v(�ꎞ�X�e�[�^�X)
	kO_MATCH_GP,			//!< Gou�ƗD��t���O�Ȃ��|�C���g�ň�v(�ꎞ�X�e�[�^�X)
	kW_DUPLI_CS,			//!< �y�v�m�F�zCS�|�C���g����������
	kW_CS_SAME_ADMIN,		//!< �y�v�m�F�zCS�|�C���g�Ɠ���16���̍s���E�����݂��܂�	[Bug6180]
	kO_CS_ADDED,			//!< �V�K�ǉ�(CS)
	kO_CS_OK,				//!< ���Ȃ�(CS)
	kW_CS_MOVED,			//!< �y�v�m�F�zCS�|�C���g�ړ�
	kE_CS_MOVED_FAIL,		//!<�y�G���[�zCS�|�C���g�ړ����s
	kW_GP_SAME_ADMIN,		//!< �y�v�m�F�z����R�[�h�̕����E������
	kW_GP_DUPLI_PRIOR,		//!< �y�v�m�F�z����20���R�[�h�ŗD��t���O�����Ă�����̂���������
	kW_GP_NOT_PRIOR,		//!< �y�v�m�F�z�D��t���O�������Ă��Ȃ�
	kW_GP_ADDED_PRIOR_TPG,	//!< �V�K�ǉ��^�y�v�m�F�z�D��t���O�̗����Ă���|�C���g��TPG
	kO_GP_ADDED_PRIOR,		//!< �V�K�ǉ�
	kW_GP_PRIOR_TPG,		//!< �y�v�m�F�z�D��t���O�̗����Ă���|�C���g��TPG
	kO_GP_OK,				//!< ���Ȃ�
	kW_GP_MOVED,			//!<�y�v�m�F�z���|�C���g�ړ�
	kW_GP_MOVED_NOT_PRIOR,	//!<�y�v�m�F�z���|�C���g�ړ�(�D��t���OON)
	kW_GP_MOVED_FAIL,		//!<�y�G���[�z���|�C���g�ړ����s
	kW_GP_MOVED_DIFF_ADMIN,	//!<�y�v�m�F�z�ړ��O��ōs���E�̏Z���R�[�h���قȂ�
	kW_CS_INSERT,			//!<�y�v�m�F�zCS�|�C���g�ǉ�
	kE_CS_INSERT_FAIL,		//!<�y�G���[�zCS�|�C���g�ǉ����s
	kW_GP_INSERT,			//!<�y�v�m�F�z���|�C���g�ǉ�
	kE_GP_INSERT_FAIL,		//!<�y�G���[�z���|�C���g�ǉ����s
	kW_GP_INSERT_EX,		//!<�y�v�m�F�z�g�����|�C���g�ǉ�
	kE_GP_INSERT_EX_FAIL,	//!<�y�v�m�F�z�g�����|�C���g�ǉ����s
	kE_INSERT_FAIL,			//!<�y�G���[�z�|�C���g�ǉ����s(�s����MeshXY)
	kE_INSERT_ADMIN_FAIL,	//!<�y�G���[�z�|�C���g�ǉ����s(�s���E�m�F)
	KW_CS_INSERT_CANNOT_CHAR,	//!< �y�v�m�F�z�����t���Z���́ACS�Œǉ��ł��Ȃ�
	kE_PNTNAME_GET_FAIL,	//!< �y�G���[�z�����n�Ԗ��̂̎擾�Ɏ��s
	kE_GOUNAME_GET_FAIL,	//!< �y�G���[�z���������̂̎擾�Ɏ��s
	kE_MOVED_REF_FAIL,	//!< �y�G���[�z�Q��DB�̃|�C���g�ƕs��v�̂��߈ړ��Ɏ��s
	kE_INSERT_REF_FAIL	//!< �y�G���[�z�Q��DB�̃|�C���g�ƕs��v�̂��ߒǉ��Ɏ��s
};

static const _TCHAR* GetResultMsg( EResultStat emStatus )
{
	switch( emStatus )
	{
	case kO_NOT_FOUND:				return _T("�y�v�m�F�z�Y���|�C���g�Ȃ�");
	case kW_DUPLI_CS:				return _T("�y�v�m�F�zCS�|�C���g����������");
	case kW_CS_SAME_ADMIN:			return _T("�y�v�m�F�zCS�|�C���g�Ɠ���16���̍s���E�����݂��܂�");
	case kO_CS_ADDED:				return _T("�V�K�ǉ�(CS)");
	case kO_CS_OK:					return _T("���Ȃ�(CS)");
	case kW_CS_MOVED:				return _T("�y�v�m�F�zCS�|�C���g�ړ�");
	case kE_CS_MOVED_FAIL:			return _T("�yError�zCS�|�C���g�ړ����s");
	case kW_GP_SAME_ADMIN:			return _T("�y�v�m�F�z����R�[�h�̕����E������");
	case kW_GP_DUPLI_PRIOR:			return _T("�y�v�m�F�z����20���R�[�h�ŗD��t���O�����Ă�����̂���������");
	case kW_GP_NOT_PRIOR:			return _T("�y�v�m�F�z�D��t���O�������Ă��Ȃ�");
	case kW_GP_ADDED_PRIOR_TPG:		return _T("�V�K�ǉ��^�y�v�m�F�z�D��t���O�̗����Ă���|�C���g��TPG");
	case kO_GP_ADDED_PRIOR:			return _T("�V�K�ǉ�");
	case kW_GP_PRIOR_TPG:			return _T("�y�v�m�F�z�D��t���O�̗����Ă���|�C���g��TPG");
	case kO_GP_OK:					return _T("���Ȃ�");
	case kW_GP_MOVED:				return _T("�y�v�m�F�z���|�C���g�ړ�");
	case kW_GP_MOVED_NOT_PRIOR:		return _T("�y�v�m�F�z���|�C���g�ړ�(�D��t���OON)");
	case kW_GP_MOVED_FAIL:			return _T("�y�G���[�z���|�C���g�ړ����s");
	case kW_GP_MOVED_DIFF_ADMIN:	return _T("�y�v�m�F�z�ړ��O��ōs���E�̏Z���R�[�h���قȂ�");
	case kW_CS_INSERT:				return _T("�y�v�m�F�zCS�|�C���g�ǉ�");
	case kE_CS_INSERT_FAIL:			return _T("�y�v�m�F�zCS�|�C���g�ǉ����s");
	case kW_GP_INSERT:				return _T("�y�v�m�F�z���|�C���g�ǉ�");
	case kE_GP_INSERT_FAIL:			return _T("�y�G���[�z���|�C���g�ǉ����s");
	case kW_GP_INSERT_EX:			return _T("�y�v�m�F�z�g�����|�C���g�ǉ�");
	case kE_GP_INSERT_EX_FAIL:		return _T("�y�G���[�z�g�����|�C���g�ǉ����s");
	case kE_INSERT_FAIL:			return _T("�y�G���[�z�|�C���g�ǉ����s(�s����MeshXY)");
	case kE_INSERT_ADMIN_FAIL:		return _T("�y�G���[�z�|�C���g�ǉ����s(�s���E�m�F)");
	case KW_CS_INSERT_CANNOT_CHAR:	return _T("�y�v�m�F�z�����t���Z���́ACS�Œǉ��ł��Ȃ�");
	case kE_PNTNAME_GET_FAIL:		return _T("�y�G���[�z�����n�Ԗ��̂̎擾�Ɏ��s");
	case kE_GOUNAME_GET_FAIL:		return _T("�y�G���[�z���������̂̎擾�Ɏ��s");
	case kE_MOVED_REF_FAIL:			return _T("�y�G���[�z�Q��DB�̃|�C���g�ƕs��v�̂��߈ړ��Ɏ��s");
	case kE_INSERT_REF_FAIL:			return _T("�y�G���[�z�Q��DB�̃|�C���g�ƕs��v�̂��ߒǉ��Ɏ��s");
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
	CString			m_strAddrCode;		//!< �Z���R�[�h
	CString			m_strAddrCodeNew;	//!< �V�Z���R�[�h
	CMeshXY			m_cMXY;				//!< ���b�V��XY
	long			m_lPurpose;			//!< ��ƖړI
	long			m_lExFlag;			//!< �g�����ԍ��t���O
	CString			m_strSource;		//!< ���\�[�X
	CMeshXY			m_cMXY_Ref;			//!< �ړ��惁�b�V��XY

	long			m_lEditPointID;		//!< �ҏW�����|�C���g��OID

	CLRec() : m_lExFlag(0), m_lPurpose(0), m_lEditPointID(0){}

	/**
	 * @brief	�����t���n��?(�Z���R�[�h���画�f)
	 * @return	bool
	 */
	bool IsCharPntCode() const{ return m_strAddrCode.GetLength() == 20? !_istdigit(m_strAddrCode.GetAt(11)) : false; }

	/**
	 * @brief	�����t����?(�Z���R�[�h���画�f)
	 * @return	bool
	 */
	bool IsCharGouCode() const{ return m_strAddrCode.GetLength() == 20? !_istdigit(m_strAddrCode.GetAt(16)) : false; }

	/**
	 * @brief	�����t���Z��?(�Z���R�[�h���画�f)
	 * @return	bool
	 */
	bool IsCharAddrCode() const{ return IsCharPntCode() || IsCharGouCode(); }
};

typedef	std::list<CLRec>	CLList;
