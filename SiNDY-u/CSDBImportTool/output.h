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
#include <fstream>
#include <atlfile.h>
#include <map>

class COutput
{
public:
	COutput(void);
	~COutput(void);
	void InitFile( LPCTSTR lpszFileName );
	
	void ResetValue();
	void SetPOI( IFeature* ipPOIFeature, long Meshcode );
	void SetNewAnnoPoint( long lOID, LPCTSTR lpcszName, int nPlace, int nCrossingLinks );
	void SetOrgAnnoPoint( long lOID, int type );
	void SetOverlapAnno( LPCTSTR lpszNewName, long lOID, long lAnnoCode, LPCTSTR lpszName ){ m_strAnnoName = lpszNewName, m_lOverlapOID = lOID; m_lOverlapAnnoCode = lAnnoCode; m_strOverlapAnnoName = lpszName;}
	void SetExitAnno( long lOID, LPCTSTR strAnnoName ){ m_lExistOID = lOID, m_strExistAnnoName = strAnnoName;}
	void SetResult( int n );	// �~�F�|�P, �����F�O, �V�K�F�P, PEC�F�Q
	void SetWornString( int n );	// 1:�����ԍ�	2:�r�`�{��

	void WriteLog();
	void VariantLog( CComVariant& cval);

private:
	std::map<CString, long> GetPOIFieldMap( IFeature* ipFeature );

private:
	std::ofstream m_fout;								// �t�@�C���o�̓X�g���[��

	long m_lOverlapOID;									// �d�Ȃ������L��OID
	long m_lOverlapAnnoCode;							// �d�Ȃ������L�̒��L��ʃR�[�h
	CString m_strOverlapAnnoName;						// �d�Ȃ������L�̕\��������
	long m_lPOIObjectID;								// ����POI�̃I�u�W�F�N�gID
	long m_lMeshCode;									// ����POI�̂��郁�b�V���R�[�h
	CString m_strAccCode;								// POI���x
	CString m_strPOIName;								// POI�X�ܖ�
	long m_lNewAnnoOID;									// �V�K�^�ʒu�̃I�u�W�F�N�gID
	int m_nNewDispType;									// �V�K�^�ʒu�̕\����� 2:�L���{������ 4:�L���̂�
	int m_nPlaceType;									// �ǂ̏ꏊ�ɒu������
	std::list< std::pair<long, int> > m_lstOrgAnno;     // �V�K�^�ʒu�ɑΉ����錳�^�ʒu�̌�⃊�X�g�i1�ł͖�����������Ȃ��j
	std::map<int, CString> mapDispType;                 // �\����ʂ��`����}�b�v
	std::map<long, CString> m_mapAnnoCodeDomain;		// ���L��ʃh���C���}�b�v
	std::map<CString, long> m_mapPOIField;				// POI�t�B�[���h�}�b�v

	long m_lExistOID;									// ���X���������꒍�L��OID
	CString m_strExistAnnoName;
	CString m_strResult;								// ���� or �V�K or �s��

	int m_nCrossingLinks;								// �d�Ȃ������H�����N�̐�

	CString m_strAnnoName;
	CString m_strWorn;									// �x�����O�p
};
