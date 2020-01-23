#pragma once

namespace zip_pol
{
	// スキーマ設定
	namespace schema
	{
		// 郵便番号と住所の紐付けテーブル用
		namespace rel_zip_addr
		{
			// テーブル名
			const CString kTableName = _T("rel_zip_addr");
			// フィールド名
			const CString kZipCode = _T("ZIPCODE");
			const CString kPrefCode = _T("PREFCODE");
			const CString kCityCode = _T("CITYCODE");
			const CString kOazaCode = _T("OAZACODE");
			const CString kAzaCode = _T("AZACODE");
			const CString kChibanCode = _T("CHIBANCODE");
			const CString kJukyoCode = _T("JUKYOCODE");
			// フィールド長
			const long eZipCode = 7;
			const long ePrefCode = 2;
			const long eCityCode = 3;
			const long eOazaCode = 3;
			const long eAzaCode = 3;
			const long eChibanCode = 5;
			const long eJukyoCode = 4;
		}

		// 郵便番号ポリゴン用
		namespace zip_pol
		{
			// テーブル名
			const CString kTableName = _T("zip_pol");
			// フィールド名
			const CString kZipCode = _T("ZIPCODE");
			const CString kPriority = _T("PRIORITY");
			// フィールド長
			const long eZipCode = 7;
			const long ePriority = 9;
		}

		// 重心ポイント用
		namespace zip_centroid
		{
			// テーブル名
			const CString kTableName = _T("zip_centroid");
			// フィールド名
			const CString kZipCode = _T("ZIPCODE");
			// フィールド長
			const long eZipCode = 7;
		}

		// 市区町村代表ポイント用
		namespace zip_rep
		{
			// テーブル名
			const CString kTableName = _T("zip_rep");
			// フィールド名
			const CString kZipCode = _T("ZIPCODE");
			const CString kPrefCode = _T("PREFCODE");
			const CString kCityCode = _T("CITYCODE");
			// フィールド長
			const long eZipCode = 7;
			const long ePrefCode = 2;
			const long eCityCode = 3;
		}
	}
}
