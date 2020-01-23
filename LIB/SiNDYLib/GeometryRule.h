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
 * @file GeometryRule.h
 * @brief �`�󃋁[����`�t�@�C��
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @version $Id$
 */
#ifndef _GEOMETRYRULE_H_
#define _GEOMETRYRULE_H_

#include "exportimport.h"

namespace sindy {

class CBasicLogicRule;
class SINDYLIB_API CLogicRule
{
public:
	explicit CLogicRule();
	virtual ~CLogicRule();

	double GetMinLen( LPCTSTR lpcszTableName ) const;
	double GetMinDeg( LPCTSTR lpcszTableName ) const;
	double GetMaxDeg( LPCTSTR lpcszTableName ) const;
	const std::pair<double,double>& GetRelThresholdPx( LPCTSTR lpcszTableName1, LPCTSTR lpcszTableName2 ) const;
	void SetRelThresholdPx( LPCTSTR lpcszTableName1, LPCTSTR lpcszTableName2, const std::pair<double,double>& pairThreshold );
	void SetUseEditableRule( bool bUse ){ m_bUseEditableRule = bUse; }
	bool GetUseEditableRule() const { return m_bUseEditableRule; }
	void SetDisableCheckToNoSubstance( bool bDisable ){ m_bDisableCheckToNoSubstance = bDisable; }
	bool GetDisableCheckToNoSubstance() const { return m_bDisableCheckToNoSubstance; }
	void SetDisableCheckNoChange( bool bDisable ) { m_bDisableCheckNoChange = bDisable; }
	bool GetDisableCheckNoChange() const { return m_bDisableCheckNoChange; }
	void SetDoCheckModify( bool check_modify ){ m_bDoCheckModify = check_modify; }
	bool GetDoCheckModify() const { return m_bDoCheckModify; }
	const CBasicLogicRule& GetBasicRule( LPCTSTR lpcszTableName, LPCTSTR lpcszConnectUserName, CBasicLogicRule& cRule ) const;
private:
	void init();
private:
	double m_dDefaultMinLen;
	double m_dDefaultMinDeg, m_dDefaultMaxDeg;
	bool m_bUseEditableRule; //!< �ҏW�s���[�����`�F�b�N���邩�ǂ����i�f�t�H���g�FTRUE�j
	bool m_bDisableCheckToNoSubstance; //!< ���̂��Ȃ����̂��`�F�b�N�Ώۂ���O���i�f�t�H���g�FFALSE�j
	bool m_bDisableCheckNoChange; //!< �ύX���������̂��`�F�b�N�Ώۂ���O���i�f�t�H���g�FTRUE�j
	bool m_bDoCheckModify; //!< ModifyCheck�����邩�ǂ����i�f�t�H���g�Ftrue�j
#pragma warning(push)
#pragma warning(disable:4251)
	std::pair<double,double> m_pairDefaultRelThresholdPx;
	std::map<CString,double> m_mapMinLen;
	std::map<CString,double> m_mapMinDeg;
	std::map<CString,double> m_mapMaxDeg;
	std::map<CString,std::map<CString,std::pair<double,double> > > m_mapRelThresholdPx;
#pragma warning(pop)
};

class SINDYLIB_API CBasicLogicRule
{
	friend class CLogicRule;
public:
	explicit CBasicLogicRule() : m_dMinLen(0.0), m_dMinDeg(0.0), m_dMaxDeg(0.0), m_bDoCheckRoop(false), m_bDoCheckOneselfTouches(false)
	{
	}
	virtual ~CBasicLogicRule(){}

	double GetMinLen() const { return m_dMinLen; }
	double GetMinDeg() const { return m_dMinDeg; }
	double GetMaxDeg() const { return m_dMaxDeg; }
	LPCTSTR GetTableName() const { return m_strTableName; }
	LPCTSTR GetConnectUserName() const { return m_strConnUserName; }
	ISpatialReferencePtr GetMeterProjection() const { 
		if( m_ipSpRef == NULL )
		{
			IProjectedCoordinateSystemPtr ipProj;
			ISpatialReferenceFactory2Ptr ipSpRefFct( CLSID_SpatialReferenceEnvironment );
			ipSpRefFct->CreateProjectedCoordinateSystem( esriSRProjCS_TokyoJapan10, &ipProj );
			m_ipSpRef = ipProj;
			ATLTRACE(_T("CBasicLogicRule::GetMeterProjection() : �v���W�F�N�V�������쐬���܂���\n") );
		}
		return m_ipSpRef;
	}
	bool DoCheckRoop() const { return m_bDoCheckRoop; }
	bool DoCheckOneselfTouches() const { return m_bDoCheckOneselfTouches; }
private:
#pragma warning(push)
#pragma warning(disable:4251)
	double m_dMinLen;
	double m_dMinDeg, m_dMaxDeg;
	mutable ISpatialReferencePtr m_ipSpRef;
	CString m_strTableName, m_strConnUserName;
	bool m_bDoCheckRoop; //!< ���[�v�`�F�b�N�����邩�ǂ���
	bool m_bDoCheckOneselfTouches; //!< ���ȐڐG���`�F�b�N���邩�ǂ���
#pragma warning(pop)
};

} // sindy

#endif // ifndef _GEOMETRYRULE_H_
