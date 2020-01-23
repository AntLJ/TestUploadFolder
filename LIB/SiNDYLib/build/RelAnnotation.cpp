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
 * @file RelAnnotation.cpp
 * @brief <b>CRelAnnotation�N���X�����t�@�C��</b>
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @vertion $Id$
 */
#include "stdafx.h"
#include "AnnotationClass.h"
#include "RelAnnotation.h"
#include "Table.h"
#include "util.h"
#include "ErrorObject.h"
#include "AnnoGlobals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;

double g_dTotalSetDispLineTime = 0.0;	//!< SetDispLine�ɂ����鑍����

// �\���ʒu���Z�b�g����
sindyErrCode CRelAnnotation::SetDispLine( sindyTableType::ECode emTableType, IFeature* ipDispLineFeature, bool bDetatch/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNameString/* = CSPTableNameString()*/ )
{
	double dStart = GetTickCount();	// SetDispLine�J�n���ԁi�g���[�X�p�j

	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERT_IF( ipDispLineFeature, sindyErr_COMInterfaceIsNull )
	{
		CSPDispLine cDisp( new CDispLine( ipDispLineFeature, emTableType, false, spFields, spNameString ) );
		if( bDetatch )
			cDisp->CreateCache( true, spFields, spNameString );
		push_front( cDisp, false ); // ���j�[�N���ǂ����͂����Ŕ��肵�Ȃ��i�X�s�[�h�d���j
	}

	g_dTotalSetDispLineTime += GetTickCount()-dStart;	// SetDispLine�ɂ����鎞�Ԃ��v��

	return emErr;
}

// �\���^�C�v���`�F�b�N����
errorcode::sindyErrCode CRelAnnotation::CheckDispType() const
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	switch( GetTableType() )
	{
		case sindyTableType::city_annotation:
			emErr = CAnnotation::CheckDispType( sindyTableType::city_disp_line );
			break;
		case sindyTableType::d1_annotation:
			emErr = CAnnotation::CheckDispType( sindyTableType::c_sc1disp_line );
			break;
		case sindyTableType::base_annotation:
			if( sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::b_sc4disp_line ) ) &&
				sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::b_sc3disp_line ) ) &&
				sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::b_sc2disp_line ) ) &&
				sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::b_sc1disp_line ) ) )
				emErr = sindyErr_NoErr;
			break;
		case sindyTableType::middle_annotation:
			if( sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::m_sc4disp_line ) ) &&
				sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::m_sc3disp_line ) ) &&
				sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::m_sc2disp_line ) ) &&
				sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::m_sc1disp_line ) ) )
				emErr = sindyErr_NoErr;
			break;
		case sindyTableType::top_annotation:
			if( sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::t_sc4disp_line ) ) &&
				sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::t_sc3disp_line ) ) &&
				sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::t_sc2disp_line ) ) &&
				sindyErr_NoErr == ( emErr = CAnnotation::CheckDispType( sindyTableType::t_sc1disp_line ) ) )
				emErr = sindyErr_NoErr;
			break;
		default:
			emErr = sindyErr_UnknownTableType;
			break;
	}
	return emErr;
}

// ���L������Ɛ^�ʒu�̈ʒu�֌W���`�F�b�N����
sindyErrCode CRelAnnotation::CheckPosition() const
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	CPoint cAnnoPoint( (IPointPtr)(IGeometry*)*GetShape() );		// �^�ʒu�|�C���g
	schema::annotation::annotation_code::ECode emAnnoCode = GetAnnoCode();	// ���L���

	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator itDisp = it->second.begin(); itDisp != it->second.end(); ++itDisp )
		{
			const CDispLine& cDisp( static_cast<const CDispLine&>(**itDisp) );
			// �p�����[�^�v�Z
			long lSecondLineStartIndex = 0;
			WKSEnvelope pRect;
			IGeometryPtr ipGeom;
			sindyAnnotationLineType emLineType = cDisp.CalcParameter( cDisp.IsAltitudeValue( emAnnoCode ), &lSecondLineStartIndex, NULL, &pRect, &ipGeom );	// �����Ńp�����[�^�v�Z����͖̂��ʂ��Ȃ��c

			// �\���ʒu�^�C�v�擾
			sindyAnnotationPosition emPos = cDisp.CalcPosition( cAnnoPoint, emLineType, pRect );

			sindyErrCode emErr = sindyErr_AnnoDispPositionInvalid;
			switch( emAnnoCode )
			{
				case schema::annotation::annotation_code::kPass:		// ��
					if( emPos == sindyAnnoPos_UpperVertical  ||
						emPos == sindyAnnoPos_LowerVertical  ||
						emPos == sindyAnnoPos_RightHorizonal || 
						emPos == sindyAnnoPos_LeftHorizonal )
						emErr = sindyErr_NoErr;
					break;
				case schema::annotation::annotation_code::kMountain:	// �R�x
					if( emPos == sindyAnnoPos_UpperHorizonal ||
						emPos == sindyAnnoPos_LowerHorizonal ||
						emPos == sindyAnnoPos_RightHorizonal || 
						emPos == sindyAnnoPos_LeftHorizonal )
						emErr = sindyErr_NoErr;
					break;
				default:
					if( emPos != (sindyAnnotationPosition)-1 )
						emErr = sindyErr_NoErr;
					break;
			}
			if( emErr != sindyErr_NoErr ) break;	// �G���[�Ȃ�r���ł�߂�
		}
	}
	return emErr;
}

// �L���b�V�����쐬����
sindyErrCode CRelAnnotation::CreateCache( bool bDetach/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNameString/* = CSPTableNameString()*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERT_IF( sindyErr_NoErr == ( emErr = CRow::CreateCache( bDetach, spFields, spNameString ) ), emErr )
	{
		// ���L�I�u�W�F�N�g�쐬
		boost::shared_ptr<CFieldMap> spFields;
		boost::shared_ptr<CTableNameString> spDispNameString;
		for( iterator it = begin(); it != end(); ++it )
		{
			for( rows_iterator itDisp = it->second.begin(); itDisp != it->second.end(); ++itDisp )
			{
				if( sindyErr_NoErr != ( emErr = (*itDisp)->CreateCache( bDetach ) ) )
				{
					ERRORLOG(emErr);
					SASSERT(false);
					break;
				}
			}
		}
	}
	return emErr;
}

// �X�P�[���ɂ������\���ʒu�t�B�[�`���̌����擾����
long CRelAnnotation::GetDispLineCount( sindyTableType::ECode emTableType ) const // TODO: CContainer::size
{
	const_iterator it = find( emTableType );
	if( it != end() )
		return (long)it->second.size();
	return 0;
}

// �\���ʒu���擾����
CDispLine* CRelAnnotation::GetDispLine( long lIndex, sindyTableType::ECode emTableType ) // TODO: CContainer::find���H���������C���f�b�N�X�w�肷��H
{
	iterator it = find( emTableType );
	if( it != end() )
	{
		if( (long)it->second.size() > lIndex )
		{
			rows_iterator itDisp = it->second.begin();
			std::advance( itDisp, lIndex );
			return static_cast<CDispLine*>(&**itDisp);
		}
	}
	ERRORLOG(sindyErr_ArgLimitOver);
	SASSERT(false);

	return NULL;
}

// �\���ʒu���擾����
const CDispLine* CRelAnnotation::GetDispLine( long lIndex, sindyTableType::ECode emTableType ) const  // TODO: CContainer::find���H���������C���f�b�N�X�w�肷��H
{
	const_iterator it = find( emTableType );
	if( it != end() )
	{
		if( (long)it->second.size() > lIndex )
		{
			const_rows_iterator itDisp = it->second.begin();
			std::advance( itDisp, lIndex );
			return static_cast<const CDispLine*>(&**itDisp);
		}
	}
	ERRORLOG(sindyErr_ArgLimitOver);
	SASSERT(false);

	return NULL;
}

// �X�P�[���ɂ��������L��������擾����
LPCTSTR CRelAnnotation::GetNameString( long lIndex, sindyTableType::ECode emTableType, int nLen, LPTSTR lpszName ) const
{
	const CDispLine* pDisp = GetDispLine( lIndex, emTableType );

	LOGASSERT_IF( pDisp, sindyErr_ArgLimitOver )
	{
		switch( pDisp->GetTargetStr() )
		{
			case schema::disp_line::target_str::kStr1: GetNameString1( nLen, lpszName ); break;
			case schema::disp_line::target_str::kStr2: GetNameString2( nLen, lpszName ); break;
			case schema::disp_line::target_str::kStr3: GetNameString3( nLen, lpszName ); break;
			default: ERRORLOG(sindyErr_AnnoDispTargetStrInvalid); break; // �^�[�Q�b�g������R�[�h����������
		}
	}
	return lpszName;
}

// �X�P�[���ɂ��������L���������擾����
long CRelAnnotation::GetStringNum( long lIndex, sindyTableType::ECode emTableType ) const
{
	long lNum = -1;
	const CDispLine* pDisp = GetDispLine( lIndex, emTableType );

	LOGASSERT_IF( pDisp, sindyErr_ArgLimitOver )
	{
		switch( pDisp->GetTargetStr() )
		{
			case schema::disp_line::target_str::kStr1: lNum = GetStringNum1(); break;
			case schema::disp_line::target_str::kStr2: lNum = GetStringNum2(); break;
			case schema::disp_line::target_str::kStr3: lNum = GetStringNum3(); break;
			default: ERRORLOG(sindyErr_AnnoDispTargetStrInvalid); break; // �^�[�Q�b�g������R�[�h����������
		}
	}
	return lNum;
}

// �R�s�[���L�����݂��邩�ǂ���
bool CRelAnnotation::HasCopyDispLine( sindyTableType::ECode emDispTableType ) const
{
	// �����e�[�u���^�C�v�������̂��������݂��邩�ǂ����Ŕ���
	const_iterator it = find( emDispTableType );
	if( it != end() )
		return ( 1 < it->second.size() ) ? true : false;

	return false;
}

// �w��I�u�W�F�N�g���폜����
sindyErrCode CRelAnnotation::Delete( _IRow* ipRow, CErrorObjects& cErrs )
{
	_ASSERTE(false); // ������
	// �����`�F�b�N
	if( ! ipRow )
		return sindyErr_ArgIsNull;

	// �w��I�u�W�F�N�g���^�ʒu�H
	if( *this == ipRow )
	{
		CAnnotation::Delete();
		// �S�Ă̕\���ʒu���폜�Ώۂ�
		CContainer::Delete( NULL, true );
	}
	else {
		// �\���ʒu�H
		CSPRow cRow = find( ipRow );
		if( cRow )
		{
			// �\���ʒu���폜�����ꍇ�A�R�s�[���L��������ΐ^�ʒu�������Ă���\���^�C�v
			// �͋L���̂ݖ��͕\�������ƂȂ�i�L���݂̂̕����D��x���j�B
			// �����X�P�[�����ɃR�s�[������Ȃ�\���^�C�v�̕ύX�Ȃ�
			// �����X�P�[�����ɃR�s�[���Ȃ��Ȃ�\���^�C�v��ύX
			// �\���^�C�v��ύX�����ꍇ�́A�������̐��������m�F����
			//
			// �R�s�[�̑��݂��m�F
			if( HasCopyDispLine( cRow->GetTableType() ) )
			{
				// �R�s�[������Ȃ炻�̂܂܍폜
				cRow->Delete();
			}
			// �R�s�[���Ȃ��ꍇ
			else {
				// �폜��̕\���^�C�v�𒲂ׂ�
				schema::annotation::disp_type::ECode emAfterDispType = (schema::annotation::disp_type::ECode)-1; // �폜��̕\���^�C�v
				const annotation_class::CAnnotationClass* pRule = GetRule();
				LOGASSERTE_IF( pRule, sindyErr_RuleNotFound )
				{
					schema::annotation_class::scaledisp_rule::ECode emDispRuleCode = pRule->GetDispRule( cRow->GetTableType() ); // �\���^�C�v���[��
					if( AheCheckDispTypeCodeAndScaleDispRule( schema::annotation::disp_type::kSym, emDispRuleCode ) )
						emAfterDispType = schema::annotation::disp_type::kSym;
					else if( AheCheckDispTypeCodeAndScaleDispRule( schema::annotation::disp_type::kNone, emDispRuleCode ) )
						emAfterDispType = schema::annotation::disp_type::kNone;
				}
				if( (schema::annotation::disp_type::ECode)-1 != emAfterDispType )
				{
					bool bConsistencyOK = false; // �\���^�C�v�̐��������Ă��邩�ǂ���
					if( ! pRule->GetExceptScale( m_emTableType ) )
					{
						// �������ł��Ȃ��Ȃ��ʃX�P�[���̕\���^�C�v�Ɣ�r���Ē������ɂȂ�Ȃ����`�F�b�N
						// ��ʃX�P�[���̕\���^�C�v���擾
						schema::annotation::disp_type::ECode emUpperDispType = GetDispType( *(++DispIterator( cRow->GetTableType() )) );
						if( AheCheckDispTypeAndUpperDispType( emAfterDispType, emUpperDispType ) )
							bConsistencyOK = true;
					}
					else
						bConsistencyOK = true;

					// �폜���Ă��������ۂĂ�Ȃ�폜
					if( bConsistencyOK )
					{
						// �\���^�C�v������ύX
						SetLongValueByFieldName( schema::annotation::kDispType, emAfterDispType );
						// �폜
						cRow->Delete();
					}
					else
						// ��������ԂɂȂ�̂ō폜�s��
						cErrs.push_back( CErrorObject( *cRow, CErrorInfo( sindyErr_AnnoDispExceptExist, sindyErrLevel_ERR ) ) );
				}
				else
					// �\���^�C�v��ύX���邱�Ƃ��ł��Ȃ��̂ō폜�s��
					cErrs.push_back( CErrorObject( *cRow, CErrorInfo( sindyErr_AnnoDispLineShouldExist, sindyErrLevel_ERR ) ) );
			}
			// �^�ʒu����
		}
		else
			return sindyErr_ObjectNotExist; // �^�[�Q�b�g��������Ȃ�
	}

	return cErrs.GetDefaultErrCode();
}

// ���L�^�ʒu�ɍ������\���ʒu���C������Ԃ�
int CRelAnnotation::GetDispLayerCount() const
{
	int nRet = -1;	// �Ԃ�l

	switch( GetTableType() )
	{
	case sindyTableType::city_annotation:
	case sindyTableType::d1_annotation:   nRet = 1; break;
	case sindyTableType::base_annotation:
	case sindyTableType::middle_annotation:
	case sindyTableType::top_annotation:  nRet = 4; break;
	default:
		LOGASSERT(false,sindyErr_UnknownTableType); break;
	}
	return nRet;
}

// �\���ʒu���C���̃e�[�u���^�C�v��Ԃ�
sindyTableType::ECode CRelAnnotation::GetDispLayer( int index ) const
{
	sindyTableType::ECode emType = sindyTableType::unknown;	// �Ԃ�l

	switch( GetTableType() )
	{
	case sindyTableType::city_annotation: emType = sindyTableType::city_disp_line; break;
	case sindyTableType::d1_annotation:   emType = sindyTableType::c_sc1disp_line; break;
	case sindyTableType::base_annotation:
	{
		switch( index )
		{
		case 0: emType = sindyTableType::b_sc4disp_line; break;
		case 1: emType = sindyTableType::b_sc3disp_line; break;
		case 2: emType = sindyTableType::b_sc2disp_line; break;
		case 3: emType = sindyTableType::b_sc1disp_line; break;
		default: LOGASSERT(false,sindyErr_ArgLimitOver); break;
		}
		break;
	}
	case sindyTableType::middle_annotation:
	{
		switch( index )
		{
		case 0: emType = sindyTableType::m_sc4disp_line; break;
		case 1: emType = sindyTableType::m_sc3disp_line; break;
		case 2: emType = sindyTableType::m_sc2disp_line; break;
		case 3: emType = sindyTableType::m_sc1disp_line; break;
		default: LOGASSERT(false,sindyErr_ArgLimitOver); break;
		}
		break;
	}
	case sindyTableType::top_annotation:
	{
		switch( index )
		{
		case 0: emType = sindyTableType::t_sc4disp_line; break;
		case 1: emType = sindyTableType::t_sc3disp_line; break;
		case 2: emType = sindyTableType::t_sc2disp_line; break;
		case 3: emType = sindyTableType::t_sc1disp_line; break;
		default: LOGASSERT(false,sindyErr_ArgLimitOver); break;
		}
		break;
	}
	default:
		LOGASSERT(false,sindyErr_UnknownTableType); break;
	}

	return emType;
}

} // sindy
