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

namespace generic
{

/**
 * @brief �h�b�J�u���E�B���h�E���擾����֐�
 * @param clsid [in] �擾����h�b�J�u���E�B���h�E��CLSID
 * @param ipApp [in] �A�v���P�[�V�����N���X�̃|�C���^
 * @return IDockableWindowPtr �h�b�J�u���E�B���h�E�N���X
*/
IDockableWindowPtr getDockableWindow(const CLSID& clsid, IApplication* ipApp);

/**
 * @brief �w��̃|�C���^������UserData���|�C���^�^�ɃL���X�g���Ď擾
 * @warning target��get_UserData(VARIANT*)�֐��̃C���^�[�t�F�[�X�������Ă��Ȃ���΂Ȃ�Ȃ�
 * @param target [in] UserData�����f�[�^�̃|�C���^
 * @return UserData��IUnknownPtr�Ƃ��Ď擾��������
 */
template <typename HAVE_USERDATA_T>
IUnknownPtr castUserDataToIUnknownPtr( HAVE_USERDATA_T& target )
{
	if(!target) return nullptr;
	CComVariant vari;
	if(FAILED(target->get_UserData( &vari )))
		return nullptr;

	// ���̊֐��őΉ����Ă���f�[�^�^�����ϊ�
	switch( vari.vt )
	{
	case VT_UNKNOWN:
		return static_cast<IUnknownPtr>(vari.punkVal);
		break;
	}
	return nullptr;
}


} // generic
