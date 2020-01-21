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

// GetAttribute.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

/**
 * @file	GetAttribute,cpp
 * @brief	AddrLib��p���������擾�T���v���R�[�h
 */

#include "stdafx.h"
#include <SiNDYLib/Workspace.h>
#include <SiNDYLib/FeatureClass.h>
#include <AddrLib/Initializer.h>
#include <AddrLib/GouPoint.h>
#include <AddrLib/CityAdmin.h>

using namespace std;
using namespace sindy;
using namespace addr;


int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale( LC_ALL, L"japanese");

	::CoInitialize(0);
	{
		// [AddrLib]
		// ������
		// �� �����t���l�������Z���R�[�h�������������́A�K��Initializer.h �� Init_CodeConverter�֐����Ă�
		// �� �����t���l�������Z�����~���擾���������́A�K��Initializer.h �� Init_YomiConverter�֐����Ă�
		// �� �Z���R�[�h���疼�̎擾���������́A�K��Initializer.h �� Init_YomiConverter�֐����Ă�
		if( Init_AreaCodeHelper() && Init_CodeConverter(_T("..\\..\\Test\\CodeConvTable.mdb")) )
		{
			// [SiNDYLib]
			// ���[�N�X�y�[�X�擾
			CWorkspace cWorkspace( _T("..\\..\\Test\\53392442.mdb") );

			// [SiNDYLib]
			// �s�s�n�}�s���E�t�B�[�`���N���X�擾
			// ���s�s�n�}�s���E�t�B�[�`���N���X�擾
			CFeatureClass cAdminFC( cWorkspace.OpenTable(schema::city_admin::kTableName) );
			CFeatureClass cGouFC( cWorkspace.OpenTable(schema::gou_point::kTableName) );

			// [SiNDYLib/AddrLib]
			// �s�s�n�}�s���E�t�B�[�`���擾
			// ���|�C���g�t�B�[�`���擾
			CCityAdmin cAdmin( cAdminFC.GetFeature(70) );
			CSPFieldMap cFields( cGouFC.GetFieldMap() );
			CSPTableNameString cNames( cGouFC.GetNameString() );
			CGouPoint cGou( cGouFC.GetFeature(8), sindyTableType::gou_point, false, cFields, cNames );

			cGou.GetShape();

			// [AddrLib]
			// �s���E�����擾
			wcout << L"[�s�s�n�}�s���E�����擾]" << endl;
			wcout << L"�s�撬���R�[�h           : " << LPCTSTR(cAdmin.GetCityCode()) << endl;
			wcout << L"�厚�E���R�[�h           : " << LPCTSTR(cAdmin.GetOazaAzaCode()) << endl;
			wcout << L"�X�敄��                 : " << cAdmin.GetGaikuFugo() << endl;
			wcout << L"�g���X�敄��             : " << LPCTSTR(cAdmin.GetExGaikuFugo()) << endl;
			wcout << L"�Z���R�[�h               : " << LPCTSTR(cAdmin.GetAddrCode()) << endl;
			
			// ���Z���R�[�h�͈�x�擾����ƃL���b�V�������̂ŁA������l���ŏZ���R�[�h���擾���
			// �����t���l���ŏZ���R�[�h���擾����ꍇ�͈�x�L���b�V�����폜����K�v������
			cAdmin.DeleteCache();
			wcout << L"�Z���R�[�h(�����t���l��) : " << LPCTSTR(cAdmin.GetAddrCode(true)) << endl;

			wcout << endl;

			// [AddrLib]
			// ���|�C���g�����擾
			wcout << L"[���|�C���g�����擾]" << endl;
			wcout << L"���ԍ�                   : " << LPCTSTR(cGou.GetGouNo()) << endl;
			wcout << L"���^�C�v                 : " << cGou.GetGouType() << endl;
			wcout << L"�Z���R�[�h               : " << LPCTSTR(cGou.GetAddrCode()) << endl;
			// ���Z���R�[�h�͈�x�擾����ƃL���b�V�������̂ŁA������l���ŏZ���R�[�h���擾���
			// �����t���l���ŏZ���R�[�h���擾����ꍇ�͈�x�L���b�V�����폜����K�v������
			cGou.DeleteCache();
			wcout << L"�Z���R�[�h(�����t���l��) : " << LPCTSTR(cGou.GetAddrCode(true)) << endl;
		}
	}
	::CoUninitialize();

	return 0;
}

