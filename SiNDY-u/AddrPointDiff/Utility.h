#pragma once

#include "stdafx.h"

/**
 * @brief �Z�p���[�^�i���p�n�C�t���j
 */
const CString SEPARATOR_HALF_HYPHEN = _T("-");

/**
 * @brief �Z�p���[�^�i�S�p�n�C�t���j
 */
const CString SEPARATOR_FULL_HYPHEN = _T("�]");

/**
 * @brief ����Z�����ݎ��
 */
enum AddrExistAttr
{
	ExistSameAddrInArea,    // �ΏۃG���A���ɓ���Z�������݂���
	ExistSameAddrOutArea,   // �ΏۃG���A�O�ɓ���Z�������݂���
	ExistOtherAddrInArea,   // �ΏۃG���A���ɈقȂ�Z�������݂���
	NoExist                 // ���݂��Ȃ�
};

/**
 * @brief �S�p��
 * @param[in]  str �ϊ��Ώە�����
 * @return �S�p������������
 */
inline CString ToFull(LPCTSTR str)
{
	INT nTextLen = lstrlen( str ) + 1;
	CString strTextBuf;
	LCMapString( GetUserDefaultLCID(), LCMAP_FULLWIDTH, str, nTextLen, strTextBuf.GetBuffer(nTextLen), nTextLen );

	CString strResult(strTextBuf);
	strTextBuf.ReleaseBuffer();
	return strResult;
}

/**
 * @brief ���p�X�y�[�X�݂̂̏ꍇ�A���p�X�y�[�X���폜����iShape�����j
 * @param[in]  value �Ώە�����
 * @return ������̕�����
 */
inline CString replaceStr(const CString& value)
{
	return ( _T(" ") == value ) ? _T("") : value;
}

/**
 * @brief ���l�݂̂ō\������Ă��邩
 * @param[in]  target   �m�F�Ώە�����
 * @retval  true  ���l�̂�
 * @retval  false ���l�݂̂ł͂Ȃ�
 */
inline bool isNumber(const CString& target)
{
	return ( target.SpanIncluding(_T("0123456789�O�P�Q�R�S�T�U�V�W�X")) == target );
}

namespace mid_addr_schema
{
	namespace addr_point
	{
		extern const _TCHAR kTableName[];     //!< �e�[�u����

		extern const _TCHAR kGeoBFid[];       //!< GEOSPACE����ID
		extern const _TCHAR kIpcObjId[];      //!< ���ԃt�@�C���I�u�W�F�N�gID
		extern const _TCHAR kPlaceCd[];       //!< �Z��11���R�[�h
		extern const _TCHAR kStreetCd[];      //!< �ʂ薼�t���O
		extern const _TCHAR kAddress1[];      //!< �Z��������1
		extern const _TCHAR kAddress2[];      //!< �Z��������2
		extern const _TCHAR kAddress3[];      //!< �Z��������3
		extern const _TCHAR kSourceCd[];      //!< �o�T�t���O
		extern const _TCHAR kPtnF[];          //!< �����p�^�[���t���O
		extern const _TCHAR kOrderCd[];       //!< �̗p���t���O
		extern const _TCHAR kJudgCd[];        //!< �̗p���藝�R�t���O
		extern const _TCHAR kGeoSf[];         //!< GEOSPACE�Z���̐M���x�t���O
		extern const _TCHAR kIpcSf[];         //!< �C���N�������gP�Z���̐M���x�t���O
		extern const _TCHAR kShinraiF[];      //!< �M���x�t���O�i���Ɠ��͎��j
		extern const _TCHAR kHyoki[];         //!< �}�ʕ\�L��̒n��

	} // addr_point

} // mid_addr_schema


