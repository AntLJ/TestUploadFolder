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

//#include <deque>
#include <boost/shared_ptr.hpp>
#include "sindylib_base_global.h"
#include "TableTypeUtil.h"
#include "ErrorCode.h"
#include "EditDef.h"

namespace sindy {
//class CRowBase;
class CContainerBase;
class CErrorObjectsBase;
class CLogicRule;
class CGeometryBase;
class CFieldMap;
class CTableNameString;
//typedef boost::shared_ptr<CRowBase> CSPRowBase;
typedef boost::shared_ptr<CContainerBase> CSPContainerBase;
typedef boost::shared_ptr<CFieldMap> CSPFieldMap;
typedef boost::shared_ptr<CTableNameString> CSPTableNameString;

/// CContainerBase::Query�Ŏg�p�����r�I�y���[�^
enum sindyCompareOperator2 {
	sindyCompOpeEqual2,            //!< ���l�i=�j
	sindyCompOpeThanSmall2,        //!< ������菬�����i<�j
	sindyCompOpeThanBig2,          //!< �������傫���i>�j
	sindyCompOpeThanSmallOrEqual2, //!< ���l��������菬�����i<=�j
	sindyCompOpeThanBigOrEqual2,   //!< ���l���������傫���i>=�j
	sindyCompOpeNotEqual2,         //!< �������Ȃ��i!=�j
};

/**
 * @class CContainerBase
 * @brief �ėp�R���e�i�N���X
 * �ėp�R���e�i�N���X�Ə����Ă���܂����ACRowBase�n��CFeatureContainer�n��
 * �i�[���邱�Ƃ�O��ɏ�����Ă��܂��B
 * �����o�ϐ��Ƃ���shared_ptr<CContainerBase>��ێ����邽�߁ACRowBase�n��
 * CFeatureContainer�n�̂ǂ���ł����K�w�Ɋi�[���邱�Ƃ��\�ł��B
 * �������A�ꕔ�̊֐��͈�K�w�����ΏۂƂ��Ȃ����Ƃɒ��ӂ��Ă��������F
 * @li Materialization
 * @li empty
 * @li �C�e���[�^��Ԃ��֐��S�āibegin,end,find�Aerase�j
 * @li push_back�Apush_front
 * @li QueryDoughnutInteriorContainFeatures
 * 
 * erase�Ɋւ��ẮA�K�w��H����̂����݂��܂��Bfind�Ɋւ��ẮA
 * �K�w��H����CSPRowBase��Ԃ�find_row�����݂��܂��B
 */
class CContainerBase
{
//	friend class FeatureContainer;
//	friend class CRoadNetwork;

	/// �v���f�B�P�[�g
	class Equal
	{
	public:
		Equal() : m_pContainer(NULL) {}
		Equal( const CSPContainerBase& c ) : m_pContainer(&*c) {}
		bool operator()( const CSPContainerBase& c ) const;
	private:
		CContainerBase* m_pContainer;
	};
/*	class EqualTableType
	{
	public:
		EqualTableType() : m_emType(sindyTableType::unknown) {}
		EqualTableType( sindyTableType::ECode emType ) : m_emType(emType){}
		bool operator()( const CSPContainerBase& c ) const;
	private:
		sindyTableType::ECode m_emType;
	};
	class EqualTableTypeSPRowBase
	{
	public:
		EqualTableTypeSPRowBase() : m_emType(sindyTableType::unknown) {}
		EqualTableTypeSPRowBase( sindyTableType::ECode emType ) : m_emType(emType){}
		bool operator()( const CSPContainerBase& c ) const;
	private:
		sindyTableType::ECode m_emType;
	};
	class EqualTableType_and_OIDSPRowBase
	{
	public:
		EqualTableType_and_OIDSPRowBase() : m_emType(sindyTableType::unknown){}
		EqualTableType_and_OIDSPRowBase( sindyTableType::ECode emType, long lOID ) : m_emType(emType), m_lOID(lOID){}
		bool operator()( const CSPContainerBase& c ) const;
	private:
		sindyTableType::ECode m_emType;
		long m_lOID;
	};
	class EqualTableType_and_ValueSPRowBase
	{
	public:
		EqualTableType_and_ValueSPRowBase() : m_emType(sindyTableType::unknown){}
		EqualTableType_and_ValueSPRowBase(sindyTableType::ECode emType, LPCTSTR lpcszFieldName, const VARIANT& Value) : m_emType(sindyTableType::unknown), m_lpcszFieldName(lpcszFieldName), m_value(&Value){}
		bool operator()( const CSPContainerBase& c ) const;
	private:
		sindyTableType::ECode m_emType;
		LPCTSTR m_lpcszFieldName;
		const VARIANT* m_value;
	};
	class EqualRowBaseSPRowBase
	{
	public:
		EqualRowBaseSPRowBase(){}
		EqualRowBaseSPRowBase( const CRowBase& cRow ) : m_pRow(&cRow){}
		bool operator()( const CSPContainerBase& c ) const;
	private:
		const CRowBase* m_pRow;
	};*/

	typedef std::list<CSPContainerBase> TContainer;
protected:
	TContainer& GetContainer(){ return m_cContainer; }
	const TContainer& GetContainer() const { return m_cContainer; }
private:
#pragma warning(push)
#pragma warning(disable:4251)
	TContainer m_cContainer;
#pragma warning(pop)
public:
	CContainerBase(void);
	virtual ~CContainerBase(void);

	CContainerBase& operator =( const CContainerBase& obj );

	typedef TContainer::iterator iterator;
	typedef TContainer::const_iterator const_iterator;
	typedef TContainer::reverse_iterator reverse_iterator;
	typedef TContainer::const_reverse_iterator const_reverse_iterator;

	//@{ @name �������C�e���[�^
	iterator begin(){ return GetContainer().begin(); }
	iterator end(){ return GetContainer().end(); }
	const_iterator begin() const { return GetContainer().begin(); }
	const_iterator end() const { return GetContainer().end(); }
	//@}
	//@{ @name �t�����C�e���[�^
	reverse_iterator rbegin(){ return GetContainer().rbegin(); }
	reverse_iterator rend(){ return GetContainer().rend(); }
	const_reverse_iterator rbegin() const { return GetContainer().rbegin(); }
	const_reverse_iterator rend() const { return GetContainer().rend(); }
	//@}
	//@{ �R���e�i����֐�
	/**
	 * @brief �R���e�i�֒ǉ�����
	 * @param cContainer [in] �ǉ��ΏۃI�u�W�F�N�g
	 * @param bSecureUnique [in,optional] �d�������Ȃ��ꍇ��true�i�f�t�H���g�Ftrue�j
	 *
	 * CFeatureContainer�n��ǉ�����ꍇ�A��������true�Ȃ炱�̏ꍇ��CFeatureContainer�n
	 * ��T�����܂��B���̏ꍇ�̔�r�Ώۂ�CTableNameString�ł���A�����Ă���CRowBase�n�I�u�W�F�N�g
	 * �ɂ͍��E����Ȃ����Ƃɒ��ӂ��Ă��������B
	 */
	virtual void push_back( const CSPContainerBase& cContainer, bool bSecureUnique = true );
	virtual void push_front( const CSPContainerBase& cSPContainer, bool bSecureUnique = true );

	virtual iterator insert( iterator it, const CSPContainerBase& cContainer, bool bSecureUnique = true );

	virtual void clear();
	bool empty() const { return GetContainer().empty(); }
	size_t size() const { return GetContainer().size(); }
	iterator find( const CSPContainerBase& cContainer ){ return std::find_if( begin(), end(), Equal(cContainer) ); }
	const_iterator find( const CSPContainerBase& cContainer ) const { return std::find_if( begin(), end(), Equal(cContainer) ); }
//	iterator find( const CRowBase& cRowBase ){ return std::find_if( begin(), end(), EqualRowBaseSPRowBase(cRowBase) ); }
//	const_iterator find( const CRowBase& cRowBase )const { return std::find_if( begin(), end(), EqualRowBaseSPRowBase(cRowBase) ); }
//	iterator find( _IRow* ipRowBase );
//	const_iterator find( _IRow* ipRowBase ) const;
//	iterator find( sindyTableType::ECode emType, long lOID ){ return std::find_if( begin(), end(), EqualTableType_and_OIDSPRowBase(emType,lOID) ); }
//	const_iterator find( sindyTableType::ECode emType, long lOID ) const { return std::find_if( begin(), end(), EqualTableType_and_OIDSPRowBase(emType,lOID) ); }
//	iterator find( sindyTableType::ECode emType, LPCTSTR lpcszFieldName, const VARIANT& vaValue ){ return std::find_if( begin(), end(), EqualTableType_and_ValueSPRowBase(emType,lpcszFieldName,vaValue) ); }
//	const_iterator find( sindyTableType::ECode emType, LPCTSTR lpcszFieldName, const VARIANT& vaValue ) const { return std::find_if( begin(), end(), EqualTableType_and_ValueSPRowBase(emType,lpcszFieldName,vaValue) ); }

	/**
	 * @brief <b>�R���e�i���珜�O����</b>\n
	 * @param cRowBase		[in]	���O�Ώ�
	 */
//	void erase( const CSPRowBase& cRowBase );
	virtual iterator erase( iterator it );
	virtual iterator erase( iterator first, iterator last );

	/// �\�[�g
	virtual void sort(){ GetContainer().sort(); }
	template<class _Pr3> void sort(_Pr3 _Pred){ GetContainer().sort(_Pred); }

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
//	CContainer Query( sindyCompareOperator emOperator, sindyTableType::ECode emTableType ) const;
//	CContainer Query( sindyCompareOperator emOperator, const CGeometry& cGeom, sindyeSpatialRel emSpRel ) const;
//	CContainer Query( sindyCompareOperator emOperator, long lFieldIndex, const VARIANT& vaValue ) const;
//	CContainer Query( sindyCompareOperator emOperator, LPCTSTR lpcszFieldName, const VARIANT& vaValue ) const;
	CSPContainerBase Query( sindyCompareOperator2 emOperator, LPCTSTR lpcszTableName, bool bSecureUnique = true ) const;
//	void Query( sindyCompareOperator emOperator, sindyTableType::ECode emTableType, CContainer& cContainer ) const;
//	void Query( sindyCompareOperator emOperator, const CGeometry& cGeom, sindyeSpatialRel emSpRel, CContainer& cContainer ) const;
//	void Query( sindyCompareOperator emOperator, long lFieldIndex, const VARIANT& vaValue, CContainer& cContainer ) const;
//	void Query( sindyCompareOperator emOperator, LPCTSTR lpcszFieldName, const VARIANT& vaValue, CContainer& cContainer ) const;
	virtual CContainerBase& Query( sindyCompareOperator2 emOperator, LPCTSTR lpcszTableName, CContainerBase& cContainer, bool bSecureUnique = true ) const;
	//@}

	/**
	 * @brief <b>�t�B�[�`���̕ύX�����ɖ߂�</b>\n
	 */
	virtual void Reset();

	/**
	 * @brief <b>�폜����</b>\n
	 * �폜�t���O�𗧂Ă܂��B
	 * ���f������ɂ�Store()����K�v������܂��B
	 * @param lpcszTableName [in,optional] ����̃e�[�u�����������̂̂ݍ폜�������ꍇ�Ɏw�肵�܂��i�f�t�H���g�FNULL)
	 * @param bRecursive [in,optional] ������CContainerBase�ɂ��K�p����ꍇ��true�i�f�t�H���g�Ffalse�j
	 */
	virtual void Delete( /*LPCTSTR lpcszTableName = NULL*/ );
	virtual void UnDelete();

	//@{ @name �ҏW�n�֐�
	// �^����ꂽ�`��͈̔͂ŕ��s�ړ��������s��
	virtual errorcode::sindyErrCode Move( const CGeometryBase& cGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMove, CErrorObjectsBase& cErrs );
	/**
	 * @brief <b>�ҏW���e��ۑ�����</b>\n
	 * ���̊֐��ł͐������̃`�F�b�N���s���܂���B\n
	 * �K��Check�֐����Ă񂾌�ɂ��̊֐����Ă�ł��������B
	 */
	virtual errorcode::sindyErrCode Store( CErrorObjectsBase& cErrs );
	virtual errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjectsBase& cErrObjects ) const;
	virtual errorcode::sindyErrCode CheckModify( CErrorObjectsBase& cErrObjects ) const;

	/**
	 * @brief �����ɕύX�����������ǂ������`�F�b�N����
	 * 
	 * ��ł��ύX������΂���̌��ʂ�Ԃ��܂��B
	 * @param emUpdateType	[out,optional]	�ύX�^�C�v������i�f�t�H���g�FNULL�j
	 * @retval true �ύX����
	 * @retval false �ύX�Ȃ��i�L���b�V�����Ă��Ȃ��ꍇ�͏��false�j
	 */
	virtual bool Changed( schema::ipc_table::update_type::ECode* emUpdateType = NULL ) const;
	//@}
	virtual bool IsCached() const;
	virtual IEnvelopePtr GetEnvelope( ShapeMode emMode = enumLatest, bool bChangedOnly = false, long lParam = -1 ) const;

	/**
	 * @brief �`��̃R�s�[���擾����
	 *
	 * �`��̃R�s�[��l�X�ȃ��[�h�ōs���܂��B
	 * �擾�����`��Q�͑S�Č��������̌`��Ƃ��ĕԂ��܂��B
	 * �����̈قȂ�`�󂪑��݂���ꍇ�́ACoClass��GeometryBag��
	 * �Ȃ邱�Ƃɒ��ӂ��Ă��������B�����ł͂Ȃ��ꍇ��MultiPoint�A
	 * Polyline�APolygon�̂ǂꂩ��Ԃ��܂��B
	 *	 
	 * �`��̎擾��GetShapeCopy( _ISetPtr&, ShapeMode, bool, long )���g�p���A�Ō��
	 * ITopologicalOperator::ConstructUnion���g�p���Č`����������܂��B
	 * 
	 * @param emMode       [in,optional] �R�s�[���擾����ۂ̃��[�h�iShapeMode���Q�� �f�t�H���g�FemLatest�j
	 * @param bChangedOnly [in,optional] �R�s�[�擾���A�ύX���������t�B�[�`���݂̂Ɍ��肷�邩�ǂ����̃t���O�i�f�t�H���g�Ffalse�j
	 * @param lParam       [in,optional] �e�R���e�i�Ŏg�p����I�v�V�����i�ʏ�͎g�p���܂��� �f�t�H���g�F-1�j
	 * @return IGeometryPtr
	 */
	IGeometryPtr GetShapeCopy( ShapeMode emMode = enumLatest, bool bChangedOnly = false, long lParam = -1 ) const;

	/**
	 * @brief �`��̃R�s�[���擾����
	 *
	 * �`��̃R�s�[��l�X�ȃ��[�h�ōs���܂��B
	 * �R�s�[�`���ipCol�֊i�[����܂��B
	 * 
	 * ���̊֐���GetShapeCopy( ShapeMode, bool, long )�ŌĂ΂�܂��B
	 * 
	 * @param ipCol        [out] _ISetPtr�i�C���X�^���X���쐬����Ă���K�v������܂��j
	 * @param emMode       [in,optional] �R�s�[���擾����ۂ̃��[�h�iShapeMode���Q�� �f�t�H���g�FemLatest�j
	 * @param bChangedOnly [in,optional] �R�s�[�擾���A�ύX���������t�B�[�`���݂̂Ɍ��肷�邩�ǂ����̃t���O�i�f�t�H���g�Ffalse�j
	 * @param lParam       [in,optional] �e�R���e�i�Ŏg�p����I�v�V�����i�ʏ�͎g�p���܂��� �f�t�H���g�F-1�j
	 */
	virtual void GetShapeCopy( _ISetPtr& ipCol, ShapeMode emMode = enumLatest, bool bChangedOnly = false, long lParam = -1 ) const;

	virtual errorcode::sindyErrCode CreateCache( bool bDetach = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() );
	/**
	 * @brief <b>�L���b�V�����폜����</b>
	 * @param bRecursive [in,optional] CRowBase�������Ă���R���e�i�̃L���b�V�����폜����ꍇ��true�i�f�t�H���g�Ffalse�j
	 */
	void DeleteCache();

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
};

} // sindy
