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

// SiNDYLogHeader.h: SiNDYLogHeader �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYLOGHEADER_H__D9B2099B_780B_475A_B97E_2F6631DAF6C4__INCLUDED_)
#define AFX_SINDYLOGHEADER_H__D9B2099B_780B_475A_B97E_2F6631DAF6C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <set>
#include <map>
#include <vector>
#include <list>
#include "SiNDYTableName.h"
#include "SiNDYTableFinder.h"

#pragma warning(disable : 4290) // ��O�\���x������

class Executor;

/**
 * @brief SiNDY���R�`�����O�@�\�w�b�_�N���X
 *
 * @note �vCString�N���X�B
 */
class SiNDYLogHeader  
{
public:
// ��`
	typedef std::vector<CString> LogRow;
	typedef std::set<int> FlagColSet;
	typedef std::set<int> EditableColSet;
	typedef SiNDYTableName TableName;

	class FieldValues : public std::list<CComVariant>
	{
	public:
	// �\�z/����
		FieldValues(IFieldPtr ipField = 0) : m_ipField(ipField) {}

	// �ݒ�/�擾
		CString getValuesString() const;

	// �v���p�e�B
		IFieldPtr m_ipField;
	};

	/// �I�u�W�F�N�g�ꗗ
	class RowMap : public std::map<long ,_IRowPtr>
	{
	public:
		RowMap(LPCTSTR lpszTableName);
		RowMap(const TableName& rTableName);

		bool insert(const RowMap& rRowMap);

		TableName m_cTableName;
	};

	class LogRowInfo
	{
	public:
	// �\�z/����
		LogRowInfo(LogRow& rLogRow, const SiNDYLogHeader& rHeader);

	// �ݒ�/�擾
		void clearInfo();
		void initInfo(const SiNDYLogHeader& rHeader);

		void updateCellsString(const std::set<int>& rColSet, LPCTSTR lpszNew, std::set<int>* pUpdatedColSet);
		bool updateCellString(int nCol, LPCTSTR lpszNew, std::set<int>* pUpdatedColSet);

		LogRow::size_type size() const { return m_rLogRow.size(); }
		LogRow::const_reference operator [] (LogRow::size_type  n) const;

	// �v���p�e�B
		TableName m_cTableName; ///< �e�[�u����
		long m_nObjectId; ///< �I�u�W�F�N�gID
		CString m_strWhereClause; ///< Where��
		IGeometryPtr m_ipGeometry; ///< �`��

	private:
	// �ÓI�֐�
		static bool updateCellString(CString& rCell, LPCTSTR lpszNew);

	// �v���p�e�B
		LogRow& m_rLogRow;
	};

	/**
	 * @brief �@�\�֐�
	 */
	class Function
	{
	public:
	// ��`

		typedef std::map<CString, Function*> FunctionMap;
		typedef std::set<int> ColumnSet;
		typedef std::auto_ptr<Executor> ExecutorPtr;

	// �\�z/����
		virtual ~Function();

	// �ݒ�/�擾
		virtual bool addNextFunctions(int nCol, LPCTSTR lpszFunctionList);
		virtual Function* addNextFunction(Function* pNextFunction);

		const FunctionMap& nextFunctionMap() const { return m_cNextFunctionMap; }
		const ColumnSet& columnSet() const { return m_cColSet; }

		void getUpdateColumnSet(ColumnSet& rColSet) const;

		virtual bool empty() const { return m_cNextFunctionMap.empty(); }

		virtual LPCTSTR getClassName() const = 0;
		virtual CString getKey() const = 0;
		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error) = 0;

	// ����
		void fillAllCells(LogRowInfo& rLogRow, LPCTSTR lpszNew, ColumnSet* pUpdatedColSet) const;

	private:
	// �v���p�e�B
		FunctionMap m_cNextFunctionMap;
		ColumnSet m_cColSet;
	};

	/**
	 * @brief �����l�擾�֐��N���X
	 */
	class FieldFunction : public Function
	{
	public:
	// �\�z/����
		FieldFunction(LPCTSTR lpszFieldName);

	// �ݒ�/�擾
		const CString& fieldName() const { return m_strFieldName; }

	// ���z�֐��̃I�[�o�[���C�h
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const;

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// �ÓI�֐�
		static LPCTSTR className() { return _T("FIELD"); }

	protected:
	// �v���p�e�B
		CString m_strFieldName; ///< �t�B�[���h��
	};

	class SubFieldFunction : public FieldFunction
	{
	public:
	// �\�z/����
		SubFieldFunction(LPCTSTR lpszFieldName) : FieldFunction(lpszFieldName)
		{
		}

	// ���z�֐��̃I�[�o�[���C�h
		virtual LPCTSTR getClassName() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// �ÓI�֐�
		static LPCTSTR className() { return _T("SUBFIELD"); }
	};

	class PointFunction : public Function
	{
	public:
	// �\�z/����
		PointFunction(LPCTSTR lpszNo);

	// �ݒ�/�擾
		IPointPtr getPoint(IPointCollection* ipPointCol) const;

	// ���z�֐��̃I�[�o�[���C�h
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const;

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// �ÓI�֐�
		static LPCTSTR className() { return _T("POINT"); }

	protected:
	// �v���p�e�B
		int m_nNo; ///< �_�ԍ�
		bool m_bLast; ///< �Ō�̓_�H
	};

	class BufferFunction : public Function
	{
	public:
	// �R���X�g���N�^�ƃf�X�g���N�^
		BufferFunction(LPCTSTR lpszDistance);

	// ����
		IGeometryPtr buffer(IGeometry* ipGeometry) const;

	// ���z�֐��̃I�[�o�[���C�h
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const;

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// �ÓI�֐�
		static LPCTSTR className() { return _T("BUFFER"); }

	private:
		double m_dDistance;
	};

	class LengthFunction : public Function
	{
	public:
	// ���z�֐��̃I�[�o�[���C�h
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// �ÓI�֐�
		static LPCTSTR className() { return _T("LENGTH"); }
		static LPCTSTR shortClassName() { return _T("LEN"); }
	};

	/**
	 * @brief �o�x�擾�֐��N���X
	 */
	class LongitudeFunction : public Function
	{
	public:
	// ���z�֐��̃I�[�o�[���C�h
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// �ÓI�֐�
		static LPCTSTR className() { return _T("LONGITUDE"); }
		static LPCTSTR shortClassName() { return _T("LON"); }
	};

	/**
	 * @brief �ܓx�擾�֐��N���X
	 */
	class LatitudeFunction : public Function
	{
	public:
	// ���z�֐��̃I�[�o�[���C�h
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// �ÓI�֐�
		static LPCTSTR className() { return _T("LATITUDE"); }
		static LPCTSTR shortClassName() { return _T("LAT"); }
	};

	class BaseMeshFunction : public Function
	{
	public:
	// ���z�֐��̃I�[�o�[���C�h
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// �ÓI�֐�
		static LPCTSTR className() { return _T("BASEMESH"); }
		static LPCTSTR shortClassName() { return _T("BASE"); }
	};

	class MeshcodeFunction : public Function
	{
	public:
	// ���z�֐��̃I�[�o�[���C�h
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// �ÓI�֐�
		static LPCTSTR className() { return _T("MESHCODE"); }
		static LPCTSTR shortClassName() { return _T("MESH"); }
	};

	class MeshXFunction : public Function
	{
	public:
	// ���z�֐��̃I�[�o�[���C�h
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// �ÓI�֐�
		static LPCTSTR className() { return _T("X"); }
	};

	class MeshYFunction : public Function
	{
	public:
	// ���z�֐��̃I�[�o�[���C�h
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// �ÓI�֐�
		static LPCTSTR className() { return _T("Y"); }
	};

	/**
	 * @brief ���j�[�N�E�\�[�g�֐��N���X
	 */
	class DistinctFunction : public Function
	{
	public:
	// ���z�֐��̃I�[�o�[���C�h
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// �ÓI�֐�
		static LPCTSTR className() { return _T("DISTINCT"); }
	};

	class AvgFunction : public Function
	{
	public:
	// ���z�֐��̃I�[�o�[���C�h
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// �ÓI�֐�
		static LPCTSTR className() { return _T("AVG"); }
	};

	class MaxFunction : public Function
	{
	public:
	// ���z�֐��̃I�[�o�[���C�h
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// �ÓI�֐�
		static LPCTSTR className() { return _T("MAX"); }
	};

	class MinFunction : public Function
	{
	public:
	// ���z�֐��̃I�[�o�[���C�h
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// �ÓI�֐�
		static LPCTSTR className() { return _T("MIN"); }
	};

	class SumFunction : public Function
	{
	public:
	// ���z�֐��̃I�[�o�[���C�h
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// �ÓI�֐�
		static LPCTSTR className() { return _T("SUM"); }
	};

	/**
	 * @brief �J�E���g�֐��N���X
	 */
	class CountFunction : public Function
	{
	public:
	// ���z�֐��̃I�[�o�[���C�h
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const { return className(); }

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// �ÓI�֐�
		static LPCTSTR className() { return _T("COUNT"); }
	};

	class SelectFunction : public Function
	{
	public:
	// �\�z/����
		SelectFunction();
		SelectFunction(LPCTSTR lpszLayerName, LPCTSTR lpszWhereClause);

	// �ݒ�/�擾
		void setTableName(const CString& rTableName) { m_cTableName = rTableName; }
		void setTableName(const TableName& rTableName) { m_cTableName = rTableName; }

		const TableName& tableName() const { return m_cTableName; }
		const CString& whereClause() const { return m_strWhereClause; }

	private:
	// �v���p�e�B
		TableName m_cTableName; ///< �I�����s�����C����
		CString m_strWhereClause; ///< �đI�����s���ۂɎg�p����Where�� (option)
	};

	/**
	 * @brief �֘A�I�u�W�F�N�g�đI���N���X
	 */
	class BelongFunction : public SelectFunction
	{
	public:
	// �\�z/����
		BelongFunction(LPCTSTR lpszLayerName, LPCTSTR lpszWhereClause);

	// ���z�֐��̃I�[�o�[���C�h
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const;

		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// �ÓI�֐�
		static LPCTSTR className() { return _T("BELONG"); }

	};

	class RootFunction : public SelectFunction
	{
	public:
	// �ݒ�/�擾
		virtual ExecutorPtr executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, ColumnSet* pUpdatedColSet) const throw (std::logic_error);

	// ���z�֐��̃I�[�o�[���C�h
		virtual LPCTSTR getClassName() const { return className(); }
		virtual CString getKey() const;

	// �ÓI�֐�
		static LPCTSTR className() { return _T("ROOT"); }
	};

	typedef std::map<int, CString> FieldColMap;
	typedef std::map<int, CString> ComboMap;
	typedef std::map<int, CString> OpenColMap;
	typedef std::map<int, CString> AliasMap;

// �\�z/����
	SiNDYLogHeader();
	virtual ~SiNDYLogHeader();

// �ݒ�/�擾
	void clear();

	bool setHeader(const CString& strHeader, LPCTSTR sep = 0);
	bool setHeader(const LogRow& rLogRow);
	bool setColumnHeader(const CString& rColumnHeader, int nCol);

	bool isEditableColumn(int nCol) const { return m_cEditableColSet.find(nCol) != m_cEditableColSet.end(); }
	bool isFlagColumn(int nCol) const { return m_cFlagColSet.find(nCol) != m_cFlagColSet.end(); }
	bool canGetLayerName() const { return m_nLayerCol >= 0 || ! m_strLayerName.IsEmpty(); }
	bool canIdentifyObject() const { return m_nObjectIdCol >= 0 && canGetLayerName(); }
	bool canIdentifyVdbPrimitive() const { return m_nVdbMeshcodeCol >= 0 && m_nVdbPrimitiveIdCol >= 0 && canGetLayerName(); }
	bool canSelectObjects() const;
	bool canGetLonLat() const { return m_nLongitudeCol >= 0 && m_nLatitudeCol >= 0; }
	bool canGetMeshXY() const { return m_nMeshcodeCol >= 0 && m_nXCol >= 0 && m_nYCol >= 0; }
	bool haveScaleColumn() const { return m_nScaleCol >= 0; }
	bool haveUpdateColumn() const { return ! (m_cRootFunction.empty()); }

	CString getColumnHeaderAlias(int nCol);

	bool getLayerName(const LogRow& rRow, CString& rLayerName) const;
	bool getObjectIdentifier(const LogRow& rRow, CString& rLayerName, long& rObjectId) const;
	bool getVdbMeshcode(const LogRow& rRow, long& rVdbMeshcode) const;
	bool getObjectsSelector(const LogRow& rRow, CString& rLayerName, CString& rWhereClause) const;
	bool getLonLat(const LogRow& rRow, double& rLongitude, double& rLatitude) const;
	bool getMeshXY(const LogRow& rRow, unsigned int& rMeshcode, int& rX, int& rY) const;
	bool getScale(const LogRow& rRow, double& rScale) const;

	_IRowPtr getIdentifiedObject(const LogRow& rRow, SiNDYTableFinder& rTableCarrier) const;
	IFeaturePtr getIdentifiedVdbPrimitive(const LogRow& rRow, SiNDYTableFinder& rTableCarrier) const;

	virtual LPCTSTR getId() const { return 0; }
	virtual LPCTSTR getDescription() const { return 0; }
	virtual LPCTSTR getSeps() const { return _T("\t"); }

// �v���p�e�B
	int m_nLayerCol; ///< ���C������ԍ�
	int m_nObjectIdCol; ///< �I�u�W�F�N�gID��ԍ�
	int m_nVdbMeshcodeCol; ///< VDB���b�V���R�[�h��ԍ�
	int m_nVdbPrimitiveIdCol; ///< VDB�v���~�e�B�uID��ԍ�
	int m_nWhereClauseCol; ///< Where���ԍ�
	int m_nScaleCol; ///< �X�P�[����ԍ�
	int m_nLongitudeCol; ///< �o�x��ԍ�
	int m_nLatitudeCol; ///< �ܓx��ԍ�
	int m_nMeshcodeCol; ///< ���b�V���R�[�h��ԍ�
	int m_nXCol; ///< X���W��ԍ�
	int m_nYCol; ///< Y���W��ԍ�
	FlagColSet m_cFlagColSet; ///< �t���O��ԍ�
	EditableColSet m_cEditableColSet; ///< �R�����g��ԍ�
	ComboMap m_cComboMap; ///< �R�����g��ɋ����ꂽ�v�f�ꗗ
	OpenColMap m_cOpenColMap; ///< OPEN��ƑO�u���ꗗ
	FieldColMap m_cFieldColMap; ///< �t�B�[���h��ԍ��ƃt�B�[���h��

	RootFunction m_cRootFunction;

	CString m_strLayerName; ///< �Œ背�C����

private:
	bool setFunctionColumnHeader(const CString& strColumnHeader, int nCol);

	CString getObjectsSelectorByObjectId(const LogRow& rRow) const;
	CString getObjectsSelectorByVdbPrimitiveId(const LogRow& rRow) const;
	CString getObjectsSelectorByField(const LogRow& rRow) const;
	CString getObjectsSelectorByWhereClause(const LogRow& rRow) const;

	CString m_strFunctionHeader;
};

#endif // !defined(AFX_SINDYLOGHEADER_H__D9B2099B_780B_475A_B97E_2F6631DAF6C4__INCLUDED_)
