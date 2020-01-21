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

#ifndef LINK_SNAP_TRACE_H_
#define LINK_SNAP_TRACE_H_

#include <boost/scoped_ptr.hpp>

/**
 * @brief �X�i�b�v���������N���q���Ă����N���X�B
 */
class LinkSnapTrace
{
	class impl;
public:
// �R���X�g���N�^�ƃf�X�g���N�^
	LinkSnapTrace(IFeatureClass* ipLinkClass);
	~LinkSnapTrace();

// �ݒ�
	void setTolerance(double dTolerance, double dCacheRatio);
	void setAhead(bool bAhead);
	/**
	 * @brief ���������ݒ�
	 * @bug   [Bug 10374]�g���[�X�\�ȃ��C���f�[�^���A���s�ړ���i[MANAGEMENT_ID]is not Null)�̃I�u�W�F�N�g�Ɍ��肷��
	 * @param lpcszWhere [in] ��������
	*/
	void setWhere(LPCTSTR lpcszWhere);

// �擾
	IPolylinePtr getTrace() const;
	IArrayPtr getLinkArray() const;
	long getLastMarkingIndex() const;

// ����
	bool start(IPoint* ipPoint);
	bool trace(IPoint* ipPoint);
	void mark();
	void demark();
	void undo();
	void stop();
	bool back_public();
private:
// ����
	boost::scoped_ptr<impl> pimpl;
};

#endif // LINK_SNAP_TRACE_H_
