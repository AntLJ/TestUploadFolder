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
 * @file FeatureContainer.h
 * @brief <b>CFeatureContainer�N���X��`�t�@�C��
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @vertion $Id$
 */
#ifndef _FEATURECONTAINER_H_
#define _FEATURECONTAINER_H_

#include "Container.h"
#include "FeatureClass.h"

namespace sindy {
/**
 * @class CFeatureContainer
 * @brief <b>�t�B�[�`�����i�[����ėp�R���e�i�N���X</b>\n
 * ���̃N���X�́A�P���ނ̕����̃t�B�[�`�����i�[���邽�߂̃R���e�i�ł��B\n
 * ���̃N���X�ŁA�����̃t�B�[�`�����ꊇ�ɏ������邱�Ƃ��ł��܂��B\n
 * ���̃t�B�[�`���Ɠ��Ɋ֘A���Ȃ����̂Ɋւ��ẮA���̃N���X���x�[�X��
 * �������s���Ă��������B
 */
class SINDYLIB_API CFeatureContainer :
	public CContainer,
	public CFeatureClass
{
public:
	// CModelContainContainer���璼�� m_listFeature �������������A
	// protected�ɂ���ƌp�����Ďg�p�ł��Ă��܂����ꂪ���邽�߂�ނȂ�
	friend class CModelContainContainer;

	CFeatureContainer(){}
	CFeatureContainer( sindyTableType::ECode emType ){ m_emTableType = emType; }
	CFeatureContainer( IFeatureClass* lp ) : CFeatureClass( lp ){}
	CFeatureContainer( ITable* lp ) : CFeatureClass( lp ){}
	virtual ~CFeatureContainer(){}

	sindyTableType::ECode GetCurrentTableType() const {return m_emTableType;}
	//@{ @name CFeatureContainerImpl �C���^�[�t�F�[�X��`

	/**
	 * @brief <b>�`��Ō�������</b>\n
	 * �����Ńo�b�t�@�����O�Ȃǂ͂��Ȃ��̂ŁA�|�C���g�Ō�������ꍇ��
	 * �o�b�t�@�����O�����`���n���Ă��������B\n
	 * @param ipGeom		[in]			�����`��
	 * @param bRecycling	[in,optional]	true�ɂ���ƃL���b�V�����쐬���Đ؂藣��
	 * @param bCreateCache	[in,optional]	�L���b�V�����쐬����ibRecycling��true�̏ꍇ�͂��̃t���O�Ɋ֌W�Ȃ��K���쐬�����j
	 * @param pErrs			[out,optional]	�G���[�i�[�R���e�i�i�I�u�W�F�N�g���m�̊֘A�t���Ɏ��s�������̂̂ݓ���j
	 *
	 * @return sindyErrCode	�G���[������ꍇ��cErrs��GetDefaultErrorCode�̌��ʂ��Ԃ�
	 */
	virtual errorcode::sindyErrCode Select( IGeometry* ipGeom, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true );
	errorcode::sindyErrCode Select( IGeometry* ipGeom, const type_info& info, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true );
	errorcode::sindyErrCode Select( IQueryFilter* ipFileter, const type_info& info, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true );
	errorcode::sindyErrCode Select( LPCTSTR lpcszTargetTableName, const std::list<long> listIDs, const type_info& info, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true, LPCTSTR lpcszWhereFieldName = NULL, LPCTSTR lpcszParentTableName = NULL );
	errorcode::sindyErrCode Add( _ICursor* ipCursor, const type_info& info, sindyTableType::ECode emType, const CSPFieldMap& cFields, const CSPTableNameString& cNames, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true, LPCTSTR lpcszWhereFieldName = NULL, LPCTSTR lpcszParentTableName = NULL );
	/**
	 * @brief <b>�`��Ō�������</b>\n
	 * �����Ńo�b�t�@�����O�Ȃǂ͂��Ȃ��̂ŁA�|�C���g�Ō�������ꍇ��
	 * �o�b�t�@�����O�����`���n���Ă��������B\n
	 * @param ipGeom		[in]			�����`��
	 * @param bRecycling	[in,optional]	true�ɂ���ƃL���b�V�����쐬���Đ؂藣��
	 * @param bCreateCache	[in,optional]	�L���b�V�����쐬����ibRecycling��true�̏ꍇ�͂��̃t���O�Ɋ֌W�Ȃ��K���쐬�����j
	 * @return sindyErrCode
	 */
	virtual errorcode::sindyErrCode SelectByObject( _IRow* ipRow, bool bRecycling = false, bool bCreateCache = false, bool bInit = true );
};

} // sindy

#endif // ifndef _FEATURECONTAINER_H_
