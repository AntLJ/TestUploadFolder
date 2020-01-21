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

// GetGouPointList.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

/**
 * @file	GetGouPointList.cpp
 * @brief	�s���E���������A���̉��ɂ��鍆�|�C���g���擾����T���v���R�[�h
 */

#include "stdafx.h"
#include <SiNDYLib/Workspace.h>
#include <SiNDYLib/FeatureClass.h>
#include <AddrLib/GouPoints.h>

using namespace std;
using namespace sindy;
using namespace addr;


int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale( LC_ALL, L"japanese" );

	::CoInitialize(0);
	{
		// [SiNDYLib]
		// ���[�N�X�y�[�X�擾
		CWorkspace cWorkspace( _T("..\\..\\Test\\53392442.mdb") );

		// [SiNDYLib/AddrLib]
		// �s�s�n�}�s���E�t�B�[�`���N���X�擾
		CGouPoints cGous( cWorkspace.OpenTable(schema::city_admin::kTableName) );

		// [AddrLib]
		// �w��Z���R�[�h�����s���E������
		cGous.SelectByAddrCode( _T("01101") );
		
		// [AddrLib]
		// �����q�b�g��
		wcout << L"�Z���R�[�h[01101]�̍s���E�|���S���� : " << cGous.size(cGous.GetTableType()) << endl;
		wcout << endl;

		// [AddrLib]
		// �e�s���E���ɂ��鍆�|�C���g�擾�E�Z���R�[�h�o��
		for( CCityAdmin::const_rows_iterator itAdmin = cGous.begin(cGous.GetTableType()); itAdmin != cGous.end(cGous.GetTableType()); ++itAdmin )
		{
			// �s���E���ɂ��鍆�|�C���g�擾
			const CSPCityAdmin pAdmin = boost::static_pointer_cast<CCityAdmin>(*itAdmin);
			wcout << L"[�s���E(" << pAdmin->GetOID() << L")�ȉ��̍��|�C���g " << pAdmin->size(sindyTableType::gou_point) << L"�o��]" << endl;
			for( CGouPoint::const_rows_iterator itGou = pAdmin->begin(sindyTableType::gou_point); itGou != pAdmin->end(sindyTableType::gou_point); ++itGou )
			{
				const CSPGouPoint pGou = boost::static_pointer_cast<CGouPoint>(*itGou);
				wcout << L"\t���|�C���g(" << pGou->GetOID() << L") : " << LPCTSTR(pGou->GetAddrCode()) << endl;
			}
			wcout << endl;
		}
	}
	::CoUninitialize();

	return 0;
}

