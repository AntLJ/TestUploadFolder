#pragma once

namespace zip_pol
{
	// �X�L�[�}�ݒ�
	namespace schema
	{
		// �X�֔ԍ��ƏZ���̕R�t���e�[�u���p
		namespace rel_zip_addr
		{
			// �e�[�u����
			const CString kTableName = _T("rel_zip_addr");
			// �t�B�[���h��
			const CString kZipCode = _T("ZIPCODE");
			const CString kPrefCode = _T("PREFCODE");
			const CString kCityCode = _T("CITYCODE");
			const CString kOazaCode = _T("OAZACODE");
			const CString kAzaCode = _T("AZACODE");
			const CString kChibanCode = _T("CHIBANCODE");
			const CString kJukyoCode = _T("JUKYOCODE");
			// �t�B�[���h��
			const long eZipCode = 7;
			const long ePrefCode = 2;
			const long eCityCode = 3;
			const long eOazaCode = 3;
			const long eAzaCode = 3;
			const long eChibanCode = 5;
			const long eJukyoCode = 4;
		}

		// �X�֔ԍ��|���S���p
		namespace zip_pol
		{
			// �e�[�u����
			const CString kTableName = _T("zip_pol");
			// �t�B�[���h��
			const CString kZipCode = _T("ZIPCODE");
			const CString kPriority = _T("PRIORITY");
			// �t�B�[���h��
			const long eZipCode = 7;
			const long ePriority = 9;
		}

		// �d�S�|�C���g�p
		namespace zip_centroid
		{
			// �e�[�u����
			const CString kTableName = _T("zip_centroid");
			// �t�B�[���h��
			const CString kZipCode = _T("ZIPCODE");
			// �t�B�[���h��
			const long eZipCode = 7;
		}

		// �s�撬����\�|�C���g�p
		namespace zip_rep
		{
			// �e�[�u����
			const CString kTableName = _T("zip_rep");
			// �t�B�[���h��
			const CString kZipCode = _T("ZIPCODE");
			const CString kPrefCode = _T("PREFCODE");
			const CString kCityCode = _T("CITYCODE");
			// �t�B�[���h��
			const long eZipCode = 7;
			const long ePrefCode = 2;
			const long eCityCode = 3;
		}
	}
}
