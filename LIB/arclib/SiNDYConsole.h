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

#ifndef __SINDY_CONSOLE_H__
#define __SINDY_CONSOLE_H__

#include <iostream>
#include "SiNDYTraits.h"
#include "SiNDYConnectionFile.h"
#include "exportimport.h"

namespace sindy {

/**
 * @brief �W���G���[�o�͋@�\�t�������N���X
 */
class SINDYLIB_API Console
{
public:
// ��`
	/**
	 * @brief ���b�Z�[�W�t���Ńe�[�u�����J���e���v���[�g�֐��N���X
	 *
	 * - TTable : ITable ���邢�� IFeatureClass �B
	 */
	template <typename TTable>
	class OpenTable
	{
	public:
	// ��`
		typedef _COM_SMARTPTR<_COM_SMARTPTR_LEVEL2<TTable, &__uuidof(TTable)> > TableInterfacePtr; ///< �X�}�[�g�|�C���^�B

	// �\�z
		OpenTable() {}

	// ����
		/**
		 * @brief �󋵕񍐕t���Ńe�[�u�����J��
		 *
		 * @param rCerr [in] ���b�Z�[�W�o�͐�
		 * @param ipFeatureWorkspace [in] �t�B�[�`�����[�N�X�y�[�X�B
		 * @param bstrTableName [in] �e�[�u�����B
		 * @return �e�[�u���B�J���Ȃ������ꍇ��null�B
		 */
		TableInterfacePtr operator()(std::ostream& rCerr, IFeatureWorkspace* ipFeatureWorkspace, BSTR bstrTableName)
		{
			USES_CONVERSION;

			(rCerr << '\"' << OLE2CA(bstrTableName) << "\" �W�J ... ").flush();

			TableInterfacePtr ipTable;
			rCerr
				<< ((sindy::traits<IFeatureWorkspace>::accessor<TableInterfacePtr>::get(ipFeatureWorkspace, bstrTableName, &ipTable) == S_OK && ipTable != 0) ? "OK" : "NG")
				<< std::endl;

			return ipTable;
		}
	};

// �\�z
	Console(std::ostream& rCerr);

// ����
	IWorkspacePtr openWorkspace(const ConnectionFile::Folder& rFolder, LPCTSTR lpszPath);
	IWorkspacePtr openWorkspace(LPCTSTR lpszPath);
	IWorkspacePtr openWorkspace(IWorkspaceFactory* ipWorkspaceFactory, IPropertySet* ipPropertySet);
	IWorkspacePtr openWorkspace(IPropertySet* ipPropertySet);

	IFeatureClassPtr openFeatureClass(IFeatureWorkspace* ipFeatureWorkspace, BSTR bstrTableName);
	ITablePtr openTable(IFeatureWorkspace* ipFeatureWorkspace, BSTR bstrTableName);

// shape�t�@�C���n����
	IWorkspacePtr openShapefileWorkspace(BSTR bstrShapeFolderPath);

	IFeatureClassPtr createShapefile(LPCTSTR lpszShapeFilePath, IFields* ipFields);
	IFeatureClassPtr createShapefile(IFeatureWorkspace* ipShapeWorkspace, BSTR bstrShapeFileName, IFields* ipFields);

private:
// �ϐ�
	std::ostream& m_rCerr;
};

SINDYLIB_EXPIMP template class SINDYLIB_API Console::OpenTable<ITable>;
SINDYLIB_EXPIMP template class SINDYLIB_API Console::OpenTable<IFeatureClass>;

} // namespace sindy

#endif // __SINDY_CONSOLE_H__
