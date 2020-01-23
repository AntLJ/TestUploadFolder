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

#include <boost/shared_ptr.hpp>
#include <deque>
#include "define_enum.h"
#include "TableTypeUtil.h"
#include "ErrorCode.h"
#include "EditDef.h"
#include "sindy/schema.h"

namespace sindy {

class CRow;
typedef boost::shared_ptr<CRow> CSPRow;
class CErrorObjects;
class CLogicRule;
class CGeometry;

/// CContainer::Query�Ŏg�p�����r�I�y���[�^
enum sindyCompareOperator {
	sindyCompOpeEqual,            //!< ���l�i=�j
	sindyCompOpeThanSmall,        //!< ������菬�����i<�j
	sindyCompOpeThanBig,          //!< �������傫���i>�j
	sindyCompOpeThanSmallOrEqual, //!< ���l��������菬�����i<=�j
	sindyCompOpeThanBigOrEqual,   //!< ���l���������傫���i>=�j
	sindyCompOpeNotEqual,         //!< �������Ȃ��i!=�j
};

class SINDYLIB_API CContainer
{
	friend class FeatureContainer;
	friend class CRoadNetwork;
public:
	CContainer(void);
	virtual ~CContainer(void);

	CContainer& operator =( const CContainer& obj );

	errorcode::sindyErrCode Materialization( ITable* ipTable );

	//@{ �R���e�i����֐�
	typedef std::deque<CSPRow> TRows;
	typedef std::map<sindyTableType::ECode,TRows> TRowsContainer;
#ifdef _USE_FIND_INDEX
	typedef std::map<sindyTableType::ECode, std::map<long,long> > TRowsContainerOIDIndex;
#endif //_USE_FIND_INDEX
	typedef TRowsContainer::iterator iterator;
	typedef TRowsContainer::const_iterator const_iterator;
	typedef TRows::iterator rows_iterator;
	typedef TRows::const_iterator const_rows_iterator;
	typedef TRowsContainer::reverse_iterator reverse_iterator;
	typedef TRowsContainer::const_reverse_iterator const_reverse_iterator;
	typedef TRows::reverse_iterator reverse_rows_iterator;
	typedef TRows::const_reverse_iterator const_reverse_rows_iterator;
	// ������
	iterator begin(){ return m_mapRowsContainer.begin(); }
	iterator end(){ return m_mapRowsContainer.end(); }
	const_iterator begin() const { return m_mapRowsContainer.begin(); }
	const_iterator end() const { return m_mapRowsContainer.end(); }
	rows_iterator begin(sindyTableType::ECode emType){ return m_mapRowsContainer[emType].begin(); }
	rows_iterator end(sindyTableType::ECode emType){ return m_mapRowsContainer[emType].end(); }
	const_rows_iterator begin(sindyTableType::ECode emType) const {
		return (*const_cast<TRowsContainer*>(&m_mapRowsContainer))[emType].begin();
	}
	const_rows_iterator end(sindyTableType::ECode emType) const {
		return (*const_cast<TRowsContainer*>(&m_mapRowsContainer))[emType].end();
	}
	// �t����
	reverse_iterator rbegin(){ return m_mapRowsContainer.rbegin(); }
	reverse_iterator rend(){ return m_mapRowsContainer.rend(); }
	const_reverse_iterator rbegin() const { return m_mapRowsContainer.rbegin(); }
	const_reverse_iterator rend() const { return m_mapRowsContainer.rend(); }
	reverse_rows_iterator rbegin(sindyTableType::ECode emType){ return m_mapRowsContainer[emType].rbegin(); }
	reverse_rows_iterator rend(sindyTableType::ECode emType){ return m_mapRowsContainer[emType].rend(); }
	const_reverse_rows_iterator rbegin(sindyTableType::ECode emType) const {
		return (*const_cast<TRowsContainer*>(&m_mapRowsContainer))[emType].rbegin();
	}
	const_reverse_rows_iterator rend(sindyTableType::ECode emType) const {
		return (*const_cast<TRowsContainer*>(&m_mapRowsContainer))[emType].rend();
	}

	void push_back( const CSPRow& cRow, bool bSecureUnique = true );
	void push_front( const CSPRow& cRow, bool bSecureUnique = true );
	virtual void clear();
	bool empty() const { return m_mapRowsContainer.empty(); }
	bool empty(sindyTableType::ECode emType) const {
		const_iterator it = m_mapRowsContainer.find(emType);
		if( it != end() ) 
			return it->second.empty();
		return true;
	}
	size_t size() const { return m_mapRowsContainer.size(); }
	size_t size(sindyTableType::ECode emType) const {
		const_iterator it = m_mapRowsContainer.find(emType);
		if( it != end() )
			return it->second.size();
		return (size_t)0;
	}
	CSPRow find( _IRow* ipRow ) const;
	CSPRow find( const CRow& cRow ) const;
	iterator find( sindyTableType::ECode emType ){ return m_mapRowsContainer.find(emType); }
	const_iterator find( sindyTableType::ECode emType ) const { return m_mapRowsContainer.find(emType); }
	rows_iterator find( sindyTableType::ECode emType, long lOID );
	const_rows_iterator find( sindyTableType::ECode emType, long lOID ) const;
	rows_iterator find( sindyTableType::ECode emType, LPCTSTR lpcszFieldName, const VARIANT& vaValue );
	const_rows_iterator find( sindyTableType::ECode emType, LPCTSTR lpcszFieldName, const VARIANT& vaValue ) const;

	/**
	 * @brief <b>�R���e�i���珜�O����</b>\n
	 * @param cRow		[in]	���O�Ώ�
	 */
	void erase( const CSPRow& cRow );
	rows_iterator erase( sindyTableType::ECode emType, rows_iterator position );
	rows_iterator erase( sindyTableType::ECode emType, rows_iterator first, rows_iterator last );

	CSPRow operator[]( size_t pos );
	CSPRow operator[]( size_t pos ) const;
	//@}

	/**
	 * @brief <b>�R���e�i���I�u�W�F�N�g�̂����A������������cGeom��������̂𔲂��o��</b>\n
	 * @param emTableType   [in] ��ܑΏۃe�[�u���^�C�v
	 * @param cGeom         [in] �`�F�b�N�`��
	 * @param cContains	[in] �`�F�b�N�`����܂��Ă���I�u�W�F�N�g�Q
	 * @note �w�肵���e�[�u���^�C�v�̃R���e�i���I�u�W�F�N�g�̂����A������������cGeom��������̂𔲂��o��
	 */
	bool QueryDoughnutInteriorContainFeatures( sindyTableType::ECode emTableType, const CGeometry& cGeom, CContainer& cContains ) const;

	/**
	 * @brief �R���e�i���I�u�W�F�N�g�����̏����Œ��o����
	 * 
	 * �Ԃ����CContainer�͂����̃R���e�i�ł���A�i�[�����CRow�I�u�W�F�N�g��shared_ptr��
	 * ��܂�Ă���ׁA�R�s�[�̃R�X�g�͂�����������܂���B
	 * ���ꂼ��̊֐��͒P���̏��������ݒ�ł��܂��񂪁A�Ԃ��ꂽCContainer�ɑ΂��čX��Query
	 * ���ĂԂ��Ƃ��ł���̂ŁA�����̏�������s�Ŏw�肷�邱�Ƃ��\�ł��B
	 * 
	 * void��Ԃ��ł̓R�s�[�R�X�g���ŏ����ɂƂǂ߂����ꍇ��A�ǉ��ŃR���e�i�ɓ��ꂽ���ꍇ�Ɏg�p���܂��B
	 * �܂��A�����I�ɊK�w��H��Ƃ��ɂ���������g�p���܂��B
	 *
	 * ��FcContainer����A�s�s�n�}���L��OBJECTID=100�𒊏o����\n
	 * CConatiner cResult( cContainer.Query( sindyeTableType::city_annotation ).Query( sindyCompOpeEqual, _T("OBJECTID"), CComVariant(100L) ) );
	 *
	 * @param emTableType		[in]    �e�[�u���^�C�v
	 * @param ipFilterGeom		[in]	�t�B���^�W�I���g���iNULL�̏ꍇ�͑S�Ď擾�j
	 * @param emSpRel			[in]	�֘A�t�B���^�iesriSpatialIntersects�����Ή����Ă��܂���j
	 * @param emOperator		[in]	��r���Z�q
	 * @param lpcszFieldName	[in]	�t�B�[���h��
	 * @param vaValue			[in]	��r�l
	 * @param lpcszTableName	[in]	�e�[�u����
	 * @retval ���o���ꂽ�I�u�W�F�N�g�Q
	 */
	CContainer Query( sindyCompareOperator emOperator, sindyTableType::ECode emTableType ) const;
	CContainer Query( sindyCompareOperator emOperator, const CGeometry& cGeom, sindyeSpatialRel emSpRel ) const;
	CContainer Query( sindyCompareOperator emOperator, long lFieldIndex, const VARIANT& vaValue ) const;
	CContainer Query( sindyCompareOperator emOperator, LPCTSTR lpcszFieldName, const VARIANT& vaValue ) const;
	CContainer Query( sindyCompareOperator emOperator, LPCTSTR lpcszTableName ) const;
	void Query( sindyCompareOperator emOperator, sindyTableType::ECode emTableType, CContainer& cContainer ) const;
	void Query( sindyCompareOperator emOperator, const CGeometry& cGeom, sindyeSpatialRel emSpRel, CContainer& cContainer ) const;
	void Query( sindyCompareOperator emOperator, long lFieldIndex, const VARIANT& vaValue, CContainer& cContainer ) const;
	void Query( sindyCompareOperator emOperator, LPCTSTR lpcszFieldName, const VARIANT& vaValue, CContainer& cContainer ) const;
	void Query( sindyCompareOperator emOperator, LPCTSTR lpcszTableName, CContainer& cContainer ) const;

	/**
	 * @brief <b>�t�B�[�`���̕ύX�����ɖ߂�</b>\n
	 */
	void Reset( bool bRecursive = false );

	/**
	 * @brief <b>�폜����</b>\n
	 * �폜�t���O�𗧂Ă܂��B
	 * ���f������ɂ�Store()����K�v������܂��B
	 * @param lpcszTableName [in,optional] ����̃e�[�u�����������̂̂ݍ폜�������ꍇ�Ɏw�肵�܂��i�f�t�H���g�FNULL)
	 * @param bRecursive [in,optional] ������CContainer�ɂ��K�p����ꍇ��true�i�f�t�H���g�Ffalse�j
	 */
	void Delete( LPCTSTR lpcszTableName = NULL, bool bRecursive = false );

	//@{ @name �ҏW�n�֐�
	errorcode::sindyErrCode Move( const CGeometry& cGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMove, CErrorObjects& cErrObjects );
	/**
	 * @brief <b>�ҏW���e��ۑ�����</b>\n
	 * ���̊֐��ł͐������̃`�F�b�N���s���܂���B\n
	 * �K��Check�֐����Ă񂾌�ɂ��̊֐����Ă�ł��������B
	 */
	virtual errorcode::sindyErrCode Store( CErrorObjects& cErrObjects );
	virtual errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const;
	virtual errorcode::sindyErrCode CheckModify( CErrorObjects& cErrObjects ) const;
	//@}
	virtual bool IsCached() const;
	virtual IEnvelopePtr GetEnvelope( bool bOriginal = true, bool bChanged = true, bool bFeatureChangedOriginal = true ) const;
	virtual IGeometryPtr GetShape(  bool bLatest = true, bool bOriginal = false, bool bFeatureChangedOriginal = false, long lParam = -1 ) const;
	virtual errorcode::sindyErrCode CreateCache( bool bDetatch = false );
	/**
	 * @brief <b>�L���b�V�����폜����</b>
	 * @param bRecursive [in,optional] CRow�������Ă���R���e�i�̃L���b�V�����폜����ꍇ��true�i�f�t�H���g�Ffalse�j
	 */
	void DeleteCache( bool bRecursive = false );

	/**
	 * @brief �L���b�V���ȊO�̃p�����[�^���X�V����
	 * @param bForce [in,optional] �����I�ɍ�蒼���ꍇ��true�i�f�t�H���g�Ffalse�j
	 * 
	 * �����E�`��ɕt������e��p�����[�^���X�V���܂��B
	 * �f�t�H���g�ł͉������삵�܂���B���L�̋L���E������
	 * ��`��A�����N��`��Ȃǂ�z�肵�Ă��܂��B
	 */
	virtual errorcode::sindyErrCode UpdateParameter( bool bForce = false );
	virtual void DeleteParameter();

	virtual void Trace( bool bAttribute = true, bool bGeometry = true ) const;

	/**
	 * @brief <b>�ύX���������I�u�W�F�N�g�̕ύX�̈���擾</b>\n
	 */
	IGeometryPtr GetEditArea() const;

	/// find�p��OID�C���f�b�N�X���č\�z����
	void UpdateIndex();
private:
#pragma warning(push)
#pragma warning(disable:4251)
	TRowsContainer m_mapRowsContainer;
#ifdef _USE_FIND_INDEX
	TRowsContainerOIDIndex m_mapRowsContainerOIDIndex;
#endif // _USE_FIND_INDEX
#pragma warning(pop)
};

} // sindy
