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
 * @file mainpage.h
 * @brief for Doxygen
 */
/**
@mainpage SiNDY�e���v���[�g���C�u����

<center>
@image html ipc_logo.png
<br>
�C���N�������gP�������<br>
�n�}DB���암
</center>

@version 1.1.0.0
@date 2005/04/28
@author �e�n �����Y

@section intro_prelibs �g�p���郉�C�u����
- <a href="http://www.boost.org/"><b>Boost</b></a>
- @b rns - ����̃e���v���[�g���C�u����
- @b sqltl - SQL�\���e���v���[�g���C�u����
- @b atl2 - �����ATL�g�����C�u����
- @b arctl - ArcObjects�e���v���[�g���C�u����

@section intro_preimports �g�p����^�C�v���C�u����
- @b msxml3.dll - esricore.olb �� high_method ���K�v�Ƃ��� IXMLDOMElementPtr ���`���邽�߁B
  - named_guids
  - raw_interfaces_only
- @b esricore.olb
  - high_method_prefix("_")
  - raw_method_prefix("")
  - exclude("OLE_HANDLE", "OLE_COLOR")

@section intro_libs �A���t�@�x�b�g�����C�u�������X�g

- stamper.h - �I�u�W�F�N�g�ɗl�X�ȏ������ރN���X���C�u�����B
- @ref stamp_modifier.h "modifier/stamp_modifier.h" - iPCFeature�p���I�u�W�F�N�g�̍X�V�E�������ɁA@ref term_sindy_history�����ݍ��ރN���X���C�u�����B
- transform_to_nq.h - �����N�̕��т��m�[�h�̕��тɕϊ�����A���S���Y���B
- lq_relation.h - �����N���_�ɂ�郊���N����Ǘ����C�u�����B

@section intro_lastupdate �X�V����

- <b>2005/04/28 VERSION 1.1.0.0</b>
  - lq_relation.h ��ǉ��B
  - @b sindy::modifier �� sindy::stamp_modifier �ɉ����B
- <b>2005/04/12 VERSION 1.0.0.1</b>
  - �����̑S�Ẵ��C�u�������C�����C�����B
- <b>2005/03/31 VERSION 1.0.0.0</b>
  - ���J�o�[�W�����B
*/

/**
 * @defgroup unit_test ���j�b�g�e�X�g
 */
