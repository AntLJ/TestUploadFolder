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

#include "StdAfx.h"
#include "SetRoadAttrList.h"
#include "INAttrInfo.h"
#include "ErrorInfo.h"

CSetRoadAttrList::CSetRoadAttrList(void)
{
}

CSetRoadAttrList::~CSetRoadAttrList(void)
{
}

//////////////////////////////////////////////////////////////////////
// �S���C���f�[�^�̊�l���Z�b�g
//////////////////////////////////////////////////////////////////////
bool CSetRoadAttrList::setList( CString& rPolDir, AttrList& rAttr )
{
	setINHead( rAttr.eINHead );
	if( !setLinkLayer( rAttr.eLinkLayer, rPolDir ) )
		return false;
	setNodeLayer( rAttr.eNodeLayer );
	setRegLayer( rAttr.eRegLayer );
	setNInfoLayer( rAttr.eNInfoLayer );
	setLaneLayer( rAttr.eLaneLayer );
	setLInfoLayer( rAttr.eLInfoLayer, rAttr.eLinkLayer );
	setLinkExLayer( rAttr.eLinkExLayer, rAttr.eLInfoLayer );

	return true;
}

//////////////////////////////////////////////////////////////////////
// IN�w�b�_����l���Z�b�g
//////////////////////////////////////////////////////////////////////
void CSetRoadAttrList::setINHead(HeadAttrList& rINHead)
{
	// .mnt ���C��ID�i�[
	rINHead.eMnt.insert( 0x00300000 );			///< �����N���C��
	rINHead.eMnt.insert( 0x00300800 );			///< �m�[�h���C��(6.01)
	rINHead.eMnt.insert( 0x00300004 );			///< �V��ʋK�����C��

	// ,rdg���C��ID�i�[
	rINHead.eRdg.insert( 0x00300030 );			///< �����_�U����񃌃C��
	rINHead.eRdg.insert( 0x00300040 );			///< �m�[�h��񃌃C��
	rINHead.eRdg.insert( 0x00300080 );			///< ���[����񃌃C��
	rINHead.eRdg.insert( 0x00300200 );			///< �����N��񃌃C��
	rINHead.eRdg.insert( 0x00300400 );			///< �����N�g�����C��

	// �G���[�R�[�h�i�[
	for( int iCnt = 0; iCnt < INHeadEnd + 1; ++iCnt )
		rINHead.eError[HeadError[iCnt].iCode] = HeadError[iCnt].strMessage;
}

//////////////////////////////////////////////////////////////////////
// �����N���C���f�[�^��l���Z�b�g
//////////////////////////////////////////////////////////////////////
bool CSetRoadAttrList::setLinkLayer( LinkAttrList& rLinkAttr, CString& rPolDir )
{
	// �Ǘ��҃R�[�h
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::jh);				///< �������H���
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::other_public);	///< ���H����
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::country);		///< ��
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::pref);			///< �s���{��
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::city);			///< �w��s
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::other_city);		///< ���̎s���� �i�� �����Q�R��j
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::others);			///< ���̑��̊Ǘ���
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::unknown);		///< �������E��ƒ�

	// �����N��ʃR�[�h
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::hibunri_honsen);		///< �{���i�㉺���񕪗��j
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::bunri_honsen);		///< �{���i�㉺�������j
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::junction);			///< �A���H�i�{���Ԃ̓n����j
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::crossing);			///< �����_�������N
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::ramp);				///< �A���H�i�����v�j
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::side);				///< �{���Ɠ���H���̑��������N
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::sa);					///< SA�����������N
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::bicycle);			///< ���]�ԓ��������N�i�����Ԓʍs�͕s�j
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::connection);			///< �{�������ڑ������N
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::roundabout);			///< ������_�����N
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::unknown);			///< �������E��ƒ�

	//�o�C�p�X�t���O
	rLinkAttr.eLinkInfo.eBypass.insert(in::road::bypass::valid);			///< �o�C�p�X�L��
	rLinkAttr.eLinkInfo.eBypass.insert(in::road::bypass::invalid);			///< �o�C�p�X����

	// �����Ԑ�p�t���O
	rLinkAttr.eLinkInfo.eAuto.insert(1);									///< �����Ԑ�p���H
	rLinkAttr.eLinkInfo.eAuto.insert(0);									///< �����Ԑ�p���H�ȊO

	// ���H����
	rLinkAttr.eLinkInfo.eWidth.insert(in::road::width::over_13m);			///< ����13m�ȏ�
	rLinkAttr.eLinkInfo.eWidth.insert(in::road::width::under_13m);			///< ����5.5m�`13m����
	rLinkAttr.eLinkInfo.eWidth.insert(in::road::width::over_3m);			///< ����3.0m�`5.5m����
	rLinkAttr.eLinkInfo.eWidth.insert(in::road::width::under_3m);			///< ����3.0m����
	rLinkAttr.eLinkInfo.eWidth.insert(in::road::width::unknown);			///< �������E��ƒ�

	// �Ԑ���
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::one);					///< 1�Ԑ�
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::two);					///< 2�Ԑ�
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::three);				///< 3�Ԑ�
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::four);					///< 4�Ԑ�
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::five);					///< 5�Ԑ�
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::six);					///< 6�Ԑ�
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::seven);				///< 7�Ԑ�
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::eight);				///< 8�Ԑ�
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::nine);					///< 9�Ԑ�
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::ten);					///< 10�Ԑ�
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::eleven);				///< 11�Ԑ�
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::twelve);				///< 12�Ԑ�
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::thirteen);				///< 13�Ԑ�
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::forteen);				///< 14�Ԑ�
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::fifteen);				///< 15�Ԑ�
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::unknown);				///< �������E��ƒ�

	// ��ʋK����ʃR�[�h
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::no_rest);		///< �K������
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::no_passage);		///< �ʍs�֎~(�����Ȃ�)
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::no_passage2);	///< �ʍs�֎~(��������)
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::oneway_order);	///< ����ʍs(�������E�����Ȃ�)
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::oneway_reverse);	///< ����ʍs(�t�����E�����Ȃ�)
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::oneway_order2);	///< ����ʍs(�������E��������)
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::oneway_reverse2);///< ����ʍs(�t�����E��������)
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::oneway_both);	///< ����ʍs(���t�؂�ւ�����)
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::unknown);		///< �������E��ƒ�

	// ��ʋK�������R�[�h
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::car);				///< �Ԏ�̂�
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::time);			///< �����̂�
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::day);				///< �j���̂�
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::car_and_time);	///< �Ԏ�y�ю���
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::car_and_day);		///< �Ԏ�y�їj��
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::time_and_day);	///< �����y�їj��
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::all);				///< �Ԏ�A�����y�їj��
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::others);			///< ���̑��̏���
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::unknown);			///< �������E��ƒ�

	// ���H�\����ʃR�[�h
	rLinkAttr.eDispClass.insert(in::road::road_class::highway2);			///< �s�s�ԍ������H
	rLinkAttr.eDispClass.insert(in::road::road_class::highway);				///< �s�s�ԍ������H(�L��)
	rLinkAttr.eDispClass.insert(in::road::road_class::city_highway);		///< �s�s�������H(�L��)
	rLinkAttr.eDispClass.insert(in::road::road_class::kokudo);				///< ��ʍ���
	rLinkAttr.eDispClass.insert(in::road::road_class::kokudo2);				///< ��ʍ���(�L��)
	rLinkAttr.eDispClass.insert(in::road::road_class::pref_mlocal);			///< ��v�n����(�s�{��)
	rLinkAttr.eDispClass.insert(in::road::road_class::pref_mlocal2);		///< ��v�n����(�s�{��)(�L��)
	rLinkAttr.eDispClass.insert(in::road::road_class::city_mlocal);			///< ��v�n����(�w��s)
	rLinkAttr.eDispClass.insert(in::road::road_class::city_mlocal2);		///< ��v�n����(�w��s)(�L��)
	rLinkAttr.eDispClass.insert(in::road::road_class::pref_road);			///< ��ʓs���{����
	rLinkAttr.eDispClass.insert(in::road::road_class::pref_road2);			///< ��ʓs���{����(�L��)
	rLinkAttr.eDispClass.insert(in::road::road_class::city_road);			///< �w��s�̈�ʎs��
	rLinkAttr.eDispClass.insert(in::road::road_class::city_road2);			///< �w��s�̈�ʎs��(�L��)
	rLinkAttr.eDispClass.insert(in::road::road_class::other_road);			///< ���̑��̓��H
	rLinkAttr.eDispClass.insert(in::road::road_class::other_road2);			///< ���̑��̓��H(�L��)
	rLinkAttr.eDispClass.insert(in::road::road_class::other_road3);			///< �i�������̑���
	rLinkAttr.eDispClass.insert(in::road::road_class::detail_road);			///< �׊X�H1
	rLinkAttr.eDispClass.insert(in::road::road_class::detail_road2);		///< �׊X�H2
	rLinkAttr.eDispClass.insert(in::road::road_class::detail_road3);		///< �׊X�H3
	rLinkAttr.eDispClass.insert(in::road::road_class::dummy_link);			///< �_�~�[�����N
	rLinkAttr.eDispClass.insert(in::road::road_class::dummy_link2);			///< �뉀�H
	rLinkAttr.eDispClass.insert(in::road::road_class::dummy_link3);			///< �}�b�`���O�p�����N
	rLinkAttr.eDispClass.insert(in::road::road_class::dummy_link4);			///< PEC�p�}�b�`���O�����N
	rLinkAttr.eDispClass.insert(in::road::road_class::dummy_link5);			///< BT�p�}�b�`���O�����N
	rLinkAttr.eDispClass.insert(in::road::road_class::ferry);				///< �t�F���[�q�H
	rLinkAttr.eDispClass.insert(in::road::road_class::parking);				///< ���ԏ�����H

	// ���H�o�H��ʃR�[�h
	rLinkAttr.eNaviClass.insert(in::road::navi_class::highway);				///< �s�s�ԍ������H
	rLinkAttr.eNaviClass.insert(in::road::navi_class::city_highway);		///< �s�s�������H
	rLinkAttr.eNaviClass.insert(in::road::navi_class::kokudo);				///< ��ʍ���
	rLinkAttr.eNaviClass.insert(in::road::navi_class::kokudo2);				///< ��ʍ���(�L��)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::pref_mlocal);			///< ��v�n����(�s�{��)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::pref_mlocal2);		///< ��v�n����(�s�{��)(�L��)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::city_mlocal);			///< ��v�n����(�w��s)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::city_mlocal2);		///< ��v�n����(�w��s)(�L��)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::pref_road);			///< ��ʓs���{����
	rLinkAttr.eNaviClass.insert(in::road::navi_class::pref_road2);			///< ��ʓs���{����(�L��)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::city_road);			///< �w��s�̈�ʎs��
	rLinkAttr.eNaviClass.insert(in::road::navi_class::city_road2);			///< �w��s�̈�ʎs��(�L��)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::other_road);			///< ���̑��̓��H
	rLinkAttr.eNaviClass.insert(in::road::navi_class::other_road2);			///< ���̑��̓��H(�L��)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::other_road3);			///< �i�������̑���
	rLinkAttr.eNaviClass.insert(in::road::navi_class::detail_road);			///< �׊X�H1
	rLinkAttr.eNaviClass.insert(in::road::navi_class::detail_road2);		///< �׊X�H2
	rLinkAttr.eNaviClass.insert(in::road::navi_class::detail_road3);		///< �׊X�H3
	rLinkAttr.eNaviClass.insert(in::road::navi_class::connectway);			///< �ڑ����H
	rLinkAttr.eNaviClass.insert(in::road::navi_class::kkd_route);			///< KD�o�H�U���s�K���
	rLinkAttr.eNaviClass.insert(in::road::navi_class::dummy_link3);			///< �}�b�`���O�p�����N
	rLinkAttr.eNaviClass.insert(in::road::navi_class::dummy_link4);			///< PEC�p�}�b�`���O�����N
	rLinkAttr.eNaviClass.insert(in::road::navi_class::ferry);				///< �t�F���[�q�H
	rLinkAttr.eNaviClass.insert(in::road::navi_class::parking);				///< ���ԏ�����H
	rLinkAttr.eNaviClass.insert(in::road::navi_class::dummy_link5);			///< BT�p�}�b�`���O�����N

	// �L��t���O
#if IN_VERSION >= 590
	rLinkAttr.eAreaFlag.insert(in::road::wide_area::wide_area);				///< �L��ɑ��݂���
	rLinkAttr.eAreaFlag.insert(in::road::wide_area::not_wide_area);			///< �L��ɑ��݂��Ȃ�
#else
	rLinkAttr.eAreaFlag.insert(in::road::wide_area::wide_area1);			///< �L��1
	rLinkAttr.eAreaFlag.insert(in::road::wide_area::wide_area2);			///< �L��2
#endif

	// ���ԏ���
	rLinkAttr.eParkClass.insert(in::road::parking_class::solid);			///< ���̒��ԏ�
	rLinkAttr.eParkClass.insert(in::road::parking_class::under);			///< �n�����ԏ�
	rLinkAttr.eParkClass.insert(in::road::parking_class::both);				///< ���̂��n�����ԏ�
	rLinkAttr.eParkClass.insert(in::road::parking_class::tower);			///< �^���[�p�[�L���O
	rLinkAttr.eParkClass.insert(in::road::parking_class::auto_tower);		///< ������]�^���[�p�[�L���O
	rLinkAttr.eParkClass.insert(in::road::parking_class::flat);				///< ���ʒ��ԏ�
	rLinkAttr.eParkClass.insert(in::road::parking_class::unknown);			///< �s��

	// ���ԏꃊ���N���
	rLinkAttr.eParkLinkClass.insert(in::road::parkinglink_class::enter);	///< ���������N
	rLinkAttr.eParkLinkClass.insert(in::road::parkinglink_class::exit);		///< �o�������N
	rLinkAttr.eParkLinkClass.insert(in::road::parkinglink_class::entrance);	///< �o���������N
	rLinkAttr.eParkLinkClass.insert(in::road::parkinglink_class::premises);	///< �\�����H�����N
	rLinkAttr.eParkLinkClass.insert(in::road::parkinglink_class::junction);	///< ���x���Ԃ̓n�胊���N
	rLinkAttr.eParkLinkClass.insert(in::road::parkinglink_class::unknown);	///< �������E��ƒ�

	// �㉺�֌W
	rLinkAttr.eUpDown.insert(in::road::updown::up1);						///< ��E���s
	rLinkAttr.eUpDown.insert(in::road::updown::up2);						///< ��E�c���ł���
	rLinkAttr.eUpDown.insert(in::road::updown::down1);						///< ���E���s
	rLinkAttr.eUpDown.insert(in::road::updown::down2);						///< ���E�ւ���ł���
	rLinkAttr.eUpDown.insert(in::road::updown::unknown);					///< ������

	// ������ʃR�[�h(�����N������)
	rLinkAttr.eAttrClass.insert(in::road::linkpoint_attr::bridge);
	rLinkAttr.eAttrClass.insert(in::road::linkpoint_attr::tunnel);
	rLinkAttr.eAttrClass.insert(in::road::linkpoint_attr::doumon);
	rLinkAttr.eAttrClass.insert(in::road::linkpoint_attr::crossing);
	rLinkAttr.eAttrClass.insert(in::road::linkpoint_attr::underpass);
#if IN_VERSION >= 550
	rLinkAttr.eAttrClass.insert(in::road::linkpoint_attr::dirtroad);
#else
	rLinkAttr.eAttrClass.insert(in::road::linkpoint_attr::tollgate);
	rLinkAttr.eAttrClass.insert(in::road::linkpoint_attr::walkbridge);
#endif

	// ������ʃR�[�h(�X�΃t���O)
	rLinkAttr.eGradFlag.insert(in::road::grad::unknown);
	rLinkAttr.eGradFlag.insert(in::road::grad::up);
	rLinkAttr.eGradFlag.insert(in::road::grad::down);
	rLinkAttr.eGradFlag.insert(in::road::grad::flat);

	// �G���[�R�[�h�i�[
	for( int iCnt = 0; iCnt < INLinkEnd + 1; ++iCnt )
		rLinkAttr.eError[LinkError[iCnt].iCode] = LinkError[iCnt].strMessage;

	// �s�����R�[�h
	CString cstrPol = rPolDir + "\\pol_code.txt";
	ifstream ifpol( cstrPol );
	if( ifpol == NULL )
	{
		cout << "# ERROR\tpol_code.txt���J���܂���B\t" << static_cast<LPCTSTR>(cstrPol) << endl;
		return false;
	}

	char rStrLine[1024] = {0};
	while(ifpol.getline( rStrLine, sizeof(rStrLine)))
	{
		CString csLine = rStrLine;
		CString csAdmin = csLine.Left( csLine.Find('\t') );
		int hoge = _ttoi(csAdmin);
		rLinkAttr.eAdminCode.insert( _ttoi(csAdmin) );
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// �m�[�h���C���f�[�^��l���Z�b�g
//////////////////////////////////////////////////////////////////////
void CSetRoadAttrList::setNodeLayer(NodeAttrList &rNodeAttr)
{
	// �M���t���O
	rNodeAttr.eSignal.insert(in::road::signal::valid);						///< �M���@�L��
	rNodeAttr.eSignal.insert(in::road::signal::invalid);					///< �M���@����

	// �m�[�h��ʃR�[�h
	rNodeAttr.eNodeClass.insert(in::road::node_class::crossing);			///< �����_�m�[�h
	rNodeAttr.eNodeClass.insert(in::road::node_class::terminate);			///< �s���~�܂�_�m�[�h
	rNodeAttr.eNodeClass.insert(in::road::node_class::dummy);				///< �_�~�[�m�[�h
	rNodeAttr.eNodeClass.insert(in::road::node_class::edge);				///< ���ύX�_�m�[�h
	rNodeAttr.eNodeClass.insert(in::road::node_class::attr_change);			///< �����ω��_�m�[�h
	rNodeAttr.eNodeClass.insert(in::road::node_class::unknown);				///< �������E��ƒ�

	// ���H�����L���t���O
	rNodeAttr.eHeight.insert(in::road::height::valid);						///< �����L��
	rNodeAttr.eHeight.insert(in::road::height::invalid);					///< ��������

	// �G���[�R�[�h�i�[
	for( int iCnt = 0; iCnt < INNodeEnd + 1; ++iCnt )
		rNodeAttr.eError[NodeError[iCnt].iCode] = NodeError[iCnt].strMessage;
}

//////////////////////////////////////////////////////////////////////
// �V��ʋK�����C���f�[�^��l���Z�b�g
//////////////////////////////////////////////////////////////////////
void CSetRoadAttrList::setRegLayer(RegAttrList &rRegAttr)
{
	// �j���w����Z�b�g
	// �j���R�[�h
	rRegAttr.eDays.insert(in::road::dayofweek::holiday);					///< ���E�x��
	rRegAttr.eDays.insert(in::road::dayofweek::except_holiday);				///< ���E�x��������
	rRegAttr.eDays.insert(in::road::dayofweek::public_holiday);				///< �x���i���j�͏����j
	rRegAttr.eDays.insert(in::road::dayofweek::except_public_holiday);		///< �x��������
	rRegAttr.eDays.insert(in::road::dayofweek::weekday);					///< ����
	rRegAttr.eDays.insert(in::road::dayofweek::sunday);						///< ���j
	rRegAttr.eDays.insert(in::road::dayofweek::monday);						///< ���j	
	rRegAttr.eDays.insert(in::road::dayofweek::tuesday);					///< �Ηj
	rRegAttr.eDays.insert(in::road::dayofweek::wednesday);					///< ���j
	rRegAttr.eDays.insert(in::road::dayofweek::thursday);					///< �ؗj
	rRegAttr.eDays.insert(in::road::dayofweek::friday);						///< ���j
	rRegAttr.eDays.insert(in::road::dayofweek::saturday);					///< �y�j
	rRegAttr.eDays.insert(in::road::dayofweek::weekend);					///< �y���E�x��
	rRegAttr.eDays.insert(in::road::dayofweek::except_weekend);				///< �y���E�x��������
	rRegAttr.eDays.insert(in::road::dayofweek::allday);						///< �S�j��
	rRegAttr.eDays.insert(in::road::dayofweek::except_sunday);				///< ���j��������
	rRegAttr.eDays.insert(in::road::dayofweek::pre_holiday);				///< ���E�x���̑O��
	rRegAttr.eDays.insert(in::road::dayofweek::unknown);		///< �Y���Ȃ�

	// �K���������Z�b�g
	rRegAttr.eCond.insert(in::road::regulation_cond::no_condition);			///< �ʏ�K��(��������)
	rRegAttr.eCond.insert(in::road::regulation_cond::with_condition);		///< �����t���K��
	rRegAttr.eCond.insert(in::road::regulation_cond::unknown);				///< ������

	// �ʍs�K�����Z�b�g
	rRegAttr.eLink.insert(in::road::passage_regulation::no_regulation);		///< �K������
	rRegAttr.eLink.insert(in::road::passage_regulation::no_passage);		///< �ʍs�֎~
	rRegAttr.eLink.insert(in::road::passage_regulation::oneway_order);		///< ����ʍs(������)
	rRegAttr.eLink.insert(in::road::passage_regulation::oneway_reverse);	///< ����ʍs(�t����)
	rRegAttr.eLink.insert(in::road::passage_regulation::bad_passage);		///< �ʍs�s��
	rRegAttr.eLink.insert(in::road::passage_regulation::relation_passage);	///< �ʍs�֎~(�֌W�҈ȊO)
	rRegAttr.eLink.insert(in::road::passage_regulation::etc_only);			///< ETC��p
	rRegAttr.eLink.insert(0);												///< �������E�ΏۊO(�m�[�h��p)

	// �G���[�R�[�h�i�[
	for( int iCnt = 0; iCnt < INRegEnd + 1; ++iCnt )
		rRegAttr.eError[RegError[iCnt].iCode] = RegError[iCnt].strMessage;
}

//////////////////////////////////////////////////////////////////////
// �m�[�h��񃌃C���f�[�^��l���Z�b�g
//////////////////////////////////////////////////////////////////////
void CSetRoadAttrList::setNInfoLayer(NInfoAttrList &rNInfoAttr)
{
	// �m�[�h��ʃR�[�h���Z�b�g
	rNInfoAttr.eClass.insert(in::road::node_info_class::crossname_nodir);		///< �����_����(�������Ȃ�)
	rNInfoAttr.eClass.insert(in::road::node_info_class::crossname_dir);			///< �����_����(����������)
	rNInfoAttr.eClass.insert(in::road::node_info_class::highway_ic_entrance);	///< �������HIC�����m�[�h(��ʓ�����̓��H�m�[�h)
	rNInfoAttr.eClass.insert(in::road::node_info_class::highway_ic_enter);		///< �������HIC���H
	rNInfoAttr.eClass.insert(in::road::node_info_class::highway_ic_exit);		///< �������HIC�o�H
	rNInfoAttr.eClass.insert(in::road::node_info_class::jct_enter);				///< ���HJCT�m�[�h
	rNInfoAttr.eClass.insert(in::road::node_info_class::jct_exit);				///< �o�HJCT�m�[�h
	rNInfoAttr.eClass.insert(in::road::node_info_class::sapa_enter);			///< SAPA���H
	rNInfoAttr.eClass.insert(in::road::node_info_class::sapa_exit);				///< SAPA�o�H
	rNInfoAttr.eClass.insert(in::road::node_info_class::highway_dirguide);		///< ���ʈē�(�������H��)
	rNInfoAttr.eClass.insert(in::road::node_info_class::general_dirguide);		///< �ē��Ŕ�(��ʓ����ʈē�)
	rNInfoAttr.eClass.insert(in::road::node_info_class::noguide);				///< �m�[�K�C�h
	rNInfoAttr.eClass.insert(in::road::node_info_class::guide_code);			///< �����U��(�R�[�h)
	rNInfoAttr.eClass.insert(in::road::node_info_class::guide_string);			///< �����U��(������)
	rNInfoAttr.eClass.insert(in::road::node_info_class::illust_file);			///< �C���X�g�t�@�C����(�U����p)
	rNInfoAttr.eClass.insert(in::road::node_info_class::illust_file2);			///< �C���X�g�t�@�C����(�n�C�E�F�C���[�h��p)
	rNInfoAttr.eClass.insert(in::road::node_info_class::illust_file3);			///< �C���X�g�t�@�C����(�U���A�n�C�E�F�C���[�h�p)

	// �n���ʂ��Z�b�g
	rNInfoAttr.eGuideClass.insert(in::road::dirguide_class::general);			///< �n��(���)
	rNInfoAttr.eGuideClass.insert(in::road::dirguide_class::highway);			///< �n��(�L��)
	rNInfoAttr.eGuideClass.insert(in::road::dirguide_class::kokudo_no);			///< �����ԍ�
	rNInfoAttr.eGuideClass.insert(in::road::dirguide_class::kendo_no);			///< �����ԍ�
	rNInfoAttr.eGuideClass.insert(in::road::dirguide_class::others);			///< ���̑�

	// �ē��R�[�h
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::noguide);				///< ���Ȃ�(���ʈē��Ȃ�)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::straight);				///< ���i(12������)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::right_front);			///< �E�O����(1������)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::right_ahead);			///< �E��O����(2������)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::right);					///< �E��(3������)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::right_behind);			///< �E�΂ߌ�����(4������)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::right_back);			///< �E�߂����(5������)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::back);					///< �߂����(6������)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::left_back);				///< ���߂����(7������)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::left_behind);			///< ���΂ߌ�����(8������)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::left);					///< ����(9������)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::left_ahead);			///< ����O����(10������)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::left_front);			///< ���O����(11������)
	rNInfoAttr.eGuideCode.insert(0xff);											///< ���ݒ�

	// �g�p�֎~�����R�[�h���Z�b�g
	/* �g�p�֎~����(�B�̑O�͑S�p�X�y�[�X)
	   -------------------------------------------
	   �@�B�C�F�G�H�J�K�K�L�M�K�L�M�N�O�P�Q�R�S
       �T�U�V�W�Z�\�]�^�_�a�b�c�d�e�f�g�h�i�j�k
       �l�m�n�o�p�q�r�s�t�u�v�w�x�y�z�{�}�~������
	   ������������������������������������������
	   ������������������������������������������
	   �����������������ȁɁʁˁ́́΁ځہ܁݁ށ�
	   ������������������������
	   -------------------------------------------
	 */

	// ������Ă��镶���i���ʈē��p�j
	rNInfoAttr.eUseOKCharDirCode.insert(0x8141);	// '�A'
	rNInfoAttr.eUseOKCharDirCode.insert(0x8144);	// '�D'
	rNInfoAttr.eUseOKCharDirCode.insert(0x8145);	// '�E'
	rNInfoAttr.eUseOKCharDirCode.insert(0x8158);	// '�X'
	rNInfoAttr.eUseOKCharDirCode.insert(0x815b);	// '�['
	rNInfoAttr.eUseOKCharDirCode.insert(0x8160);	// '�`'
	rNInfoAttr.eUseOKCharDirCode.insert(0x8169);	// '�i'
	rNInfoAttr.eUseOKCharDirCode.insert(0x816a);	// '�j'
	rNInfoAttr.eUseOKCharDirCode.insert(0x817c);	// '�|'
	rNInfoAttr.eUseOKCharDirCode.insert(0x8159);	// '�Y'	[bug7330]
	rNInfoAttr.eUseOKCharDirCode.insert(0x8396);	// '��'

	// ������Ă��镶���i�����_���̗p�j
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8141);	// '�A'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8144);	// '�D'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8145);	// '�E'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8158);	// '�X'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x815b);	// '�['
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8160);	// '�`'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8169);	// '�i'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x816a);	// '�j'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x817c);	// '�|'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8159);	// '�Y'	[bug7330]
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8396);	// '��'

	// �G���[�R�[�h�i�[
	for( int iCnt = 0; iCnt < INNInfoEnd + 1; ++iCnt )
		rNInfoAttr.eError[NInfoError[iCnt].iCode] = NInfoError[iCnt].strMessage;
}

//////////////////////////////////////////////////////////////////////
// ���[����񃌃C���f�[�^��l���Z�b�g
//////////////////////////////////////////////////////////////////////
void CSetRoadAttrList::setLaneLayer(LaneAttrList& rLaneAttr)
{
	// �G���[�R�[�h�i�[
	for( int iCnt = 0; iCnt < INLaneEnd + 1; ++iCnt )
		rLaneAttr.eError[LaneError[iCnt].iCode] = LaneError[iCnt].strMessage;
}

//////////////////////////////////////////////////////////////////////
// �����N��񃌃C���f�[�^��l���Z�b�g
//////////////////////////////////////////////////////////////////////
void CSetRoadAttrList::setLInfoLayer(LInfoAttrList &rLInfoAttr, LinkAttrList& rLinkAttr)
{
	// �\����ʃR�[�h���Z�b�g
	rLInfoAttr.eDisp = rLinkAttr.eDispClass;

	// �K�����x���Z�b�g
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::limit_under30km);			///< 30km/h����
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::limit_40km);				///< 40km/h
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::limit_50km);				///< 50km/h
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::limit_60km);				///< 60km/h
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::limit_70km);				///< 70km/h
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::limit_80km);				///< 80km/h
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::limit_100km);				///< 100km/h
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::unknown);					///< ������

	// ��]�敪
	rLInfoAttr.eSection.insert(in::road::kubun_code::main_line);				///< ��H��
	rLInfoAttr.eSection.insert(in::road::kubun_code::sub_line);					///< �]�H��
	rLInfoAttr.eSection.insert(in::road::kubun_code::unknown);					///< �������E��ƒ�

	// �G���[�R�[�h�i�[
	for( int iCnt = 0; iCnt < INLInfoEnd + 1; ++iCnt )
		rLInfoAttr.eError[LInfoError[iCnt].iCode] = LInfoError[iCnt].strMessage;
}	

//////////////////////////////////////////////////////////////////////
// �����N�g�����C���f�[�^��l���Z�b�g
//////////////////////////////////////////////////////////////////////
void CSetRoadAttrList::setLinkExLayer(LinkExAttrList &rLinkExAttr, LInfoAttrList& rLInfoAttr)
{
	// �����N���L�[���Z�b�g
	rLinkExAttr.eKey.insert(in::road::lext_key::kubun_code);			///< ��]�敪�R�[�h
	rLinkExAttr.eKey.insert(in::road::lext_key::limit_speed);			///< �K�����x
	rLinkExAttr.eKey.insert(in::road::lext_key::trip_speed);			///< ���s���x
	rLinkExAttr.eKey.insert(in::road::lext_key::island_flag);			///< �������e�t���O
	rLinkExAttr.eKey.insert(in::road::lext_key::area_flag);				///< �׊X�H�G���A�t���O
	rLinkExAttr.eKey.insert(in::road::lext_key::oneside_lane);			///< �Б��Ԑ���

	// �K�����x���Z�b�g
	rLinkExAttr.eSpeed = rLInfoAttr.eSpeed;

	// ��]�敪���Z�b�g
	rLinkExAttr.eSection = rLInfoAttr.eSection;

	// �����t���O���Z�b�g
	rLinkExAttr.eIsland.insert(in::road::island_flag::valid);			///< ���e����
	rLinkExAttr.eIsland.insert(in::road::island_flag::invalid);			///< ���e���Ȃ�

	// �׊X�H�G���A���ʃt���O
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_1998);			///< 1998�N�x
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_1999);			///< 1999�N�x
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2000_spr);		///< 2000�N�x�t
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2000_aut);		///< 2000�N�x�H
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2001);			///< 2001�N�x
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2002);			///< 2002�N�x
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2003);			///< 2003�N�x
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2004);			///< 2004�N�x
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2005);			///< 2005�N�x
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2006);			///< 2006�N�x
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2007);			///< 2007�N�x
	rLinkExAttr.eDetail.insert(in::road::area_flag::unknown);			///< �������E��ƒ�

	// �׊X�HL2(0x80000000)
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_1998 | in::road::area_flag::rest);			///< 1998�N�x
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_1999 | in::road::area_flag::rest);			///< 1999�N�x
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2000_spr | in::road::area_flag::rest);		///< 2000�N�x�t
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2000_aut | in::road::area_flag::rest);		///< 2000�N�x�H
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2001 | in::road::area_flag::rest);			///< 2001�N�x
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2002 | in::road::area_flag::rest);			///< 2002�N�x
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2003 | in::road::area_flag::rest);			///< 2003�N�x
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2004 | in::road::area_flag::rest);			///< 2004�N�x
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2005 | in::road::area_flag::rest);			///< 2005�N�x
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2006 | in::road::area_flag::rest);			///< 2006�N�x
	rLinkExAttr.eDetail.insert(in::road::area_flag::unknown | in::road::area_flag::rest);				///< �������E��ƒ�

	// �G���[�R�[�h�i�[
	for( int iCnt = 0; iCnt < INLinkExEnd + 1; ++iCnt )
		rLinkExAttr.eError[LinkExError[iCnt].iCode] = LinkExError[iCnt].strMessage;
}

//////////////////////////////////////////////////////////////////////
// 10�i����16�i���ւ̕ϊ�
//////////////////////////////////////////////////////////////////////
void SetObjectID(CString &rCstrID, int rID)
{
	CString cstrID, cstrTmp;
	cstrID.Format("%x", rID);

	// �󂢂�����0��}��
	for(int iLength = 0; iLength < (8 - cstrID.GetLength()); ++iLength)
		cstrTmp += "0";

	rCstrID = "0x" + cstrTmp + cstrID;
}
