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

#ifndef ZOOMOVE_CONTROLLER_H_
#define ZOOMOVE_CONTROLLER_H_

#include <boost/utility.hpp>
#include <boost/scoped_ptr.hpp>

/**
 * @briev �g��k���X�N���[������𐧌䂷��N���X
 *
 * �R���X�g���N�^��
 * - �}�E�X�L���v�`���̕ێ�
 * - �}�E�X�J�[�\���̕ێ�
 * ���s���A�f�X�g���N�^�Ō��ɖ߂��B
 */
class ZooMoveController : boost::noncopyable
{
	class Impl;
public:
	/// ���샂�[�h�B
	enum EMode {
		kMoveMode, ///< �X�N���[��
		kZoomMode, ///< �g��k��
	};

// �R���X�g���N�^�ƃf�X�g���N�^
	ZooMoveController(IActiveView* ipActiveView, bool bPage=false);
	ZooMoveController(IActiveView* ipActiveView, const POINT& rScreenPoint, bool bPage=false);
	~ZooMoveController();

	void changeMode(EMode eMode);
	void changeMode(EMode eMode, const POINT& rScreenPoint);

	void onMouseMove();
	void onMouseMove(const POINT& rScreenPoint);

	void apply();
	void cancel();

private:
// �ϐ�
	boost::scoped_ptr<Impl> pimpl;
};

#endif // ZOOMOVE_CONTROLLER_H_
