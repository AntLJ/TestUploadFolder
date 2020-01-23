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

#include "WorkTxtImpl.h"

#include <AddrLib/AreaCodeHelper.h>
#include <AddrLib/KLinkPoint.h>
#include <AddrLib/CityAdmin.h>

using namespace addr;

/**
 * @class	CKP2WorkTxt
 * @brief	�ƌ`�����N�|�C���g���ԃf�[�^�쐬�N���X
 */
class CKP2WorkTxt : public CWorkTxtImpl
{
public:
	CKP2WorkTxt( const CParameter& rParam, std::ofstream& ofs ) : CWorkTxtImpl(rParam, ofs)
	{
	}
	~CKP2WorkTxt(void)
	{
	}

	/**
	 * @brief	������
	 * @return	bool
	 */
	bool Init();

	/**
	 * @brief	�f�[�^�o��
	 * @return	bool
	 */
	bool WriteData();

private:

	/**
	 * @brief	�s�撬���R�[�h���X�g�ǂݍ���
	 * @param	lpcszFile	[in]	�R�[�h���X�g
	 * @return	bool
	 */
	bool LoadCityCodeList( const _TCHAR* lpcszFile );

	/**
	 * @brief	�ŐV11���Z���R�[�h�擾
	 * @param	lpcszAddrCode	[in]	11���Z���R�[�h
	 * @return	�ŐV11���Z���R�[�h
	 */
	CString GetNewestCode( const _TCHAR* lpcszAddrCode );

	/**
	 * @brief	���񋟌��R�[�h�ϊ�
	 * @param	eInfoSrc	[in]	���񋟌��R�[�h
	 * @return	�ϊ��������񋟌��R�[�h
	 */
	const _TCHAR* ConvInfoSrc( klink_point::info_src::ECode eInfoSrc );

	/**
	 * @brief	���͗p���W���x�R�[�h�ϊ�
	 * @param	eInputSeido	[in]	���͗p���W���x�R�[�h
	 * @return	�ϊ��������͗p���W���x�R�[�h
	 */
	const _TCHAR ConvInputSeido( klink_point::input_seido::ECode eInputSeido );

	/**
	 * @brief	�����[�X�p���W���x�R�[�h�ϊ�
	 * @param	eReleaseSeido	[in]	�����[�X�p���W���x�R�[�h
	 * @return	�ϊ����������[�X�p���W���x�R�[�h
	 */
	const _TCHAR ConvReleaseSeido( klink_point::release_seido::ECode eReleaseSeido );

	/**
	 * @brief	�s���E�Ƃ̐������擾
	 * @param	strKPAddrCode	[in]	�ƌ`�����N�|�C���g�Z���R�[�h
	 * @param	ipGeo			[in]	�ƌ`�����N�|�C���g�W�I���g��
	 * @param	pAdmin			[in]	���ɕR�t���s���E�������Ă���Ƃ��w��(�Ȃ����NULL�w��)
	 * @param	pEqualF			[out]	�������i�[
	 *                                  0 : �Z���R�[�h��v
 	 *                                  1 : �s��v or �Ή��s���E�Ȃ�
 	 *                                  2 : �ƌ`�����N�̏Z���R�[�h���s��
 	 *                                  3 : �����ȏZ���R�[�h(�p�~ or �R�[�h���̂��Z���}�X�^�ɂȂ�)
	 * @retval	true  : ����
	 * @retval	false : �s���E�擾�ł���
	 */
	bool GetEqualF( const CString& strKPAddrCode, IGeometry* ipGeo, const CCityAdmin* pAdmin, long* pEqualF );

	/**
	 * @brief	�f�[�^�����o��
	 * @param	stream		[in]	�X�g���[��
	 * @param	cKPPoint	[in]	KP�|�C���g
 	 * @param	strAddrCode	[in]	�Z���R�[�h
	 * @param	lEqualF		[in]	�s���E������
	 */
	void WriteData( std::ofstream& stream, const CKLinkPoint& cKPPoint, const CString& strAddrCode, long lEqualF );

private:

	std::set<CString>			m_setCode;				//!< �s�撬���R�[�h���X�g
	std::map<CString, CString>	m_mapNewestAddrCode;	//!< �ŐV11���R�[�h(�L���b�V��)
	std::set<long>				m_setWritedOID;			//!< �����ωƌ`�����N�|�C���gID���X�g

	CAreaCodeHelper				m_cACHelper;			//!< �Z��DB�ڑ��Ǘ��N���X
};
