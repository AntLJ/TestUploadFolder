#include "stdafx.h"
#include "ConvRelZipAddr.h"
#include "Log.h"
#include "Common.h"
#include <useful_headers/directory_util.h>
#include <boost/regex.hpp>

bool CConvRelZipAddr::createFieldIndex()
{
	if(!setFieldIndex(sindy::schema::rel_addr::kTableName20, m_ipRelAddr, m_mapFieldIndexRelAddr) ||
		!setFieldIndex(m_strShapefileName, m_ipOutShapefile, m_mapFieldIndexOutShapefile)){
			return false;
	}
	return true;
}

bool CConvRelZipAddr::createFields(IFieldsPtr& ipFields)
{
	if(!addAttrField(m_strShapefileName, zip_pol::schema::rel_zip_addr::kZipCode, esriFieldType::esriFieldTypeString, VARIANT_FALSE, VARIANT_FALSE, zip_pol::schema::rel_zip_addr::eZipCode, 0, ipFields) ||
		!addAttrField(m_strShapefileName, zip_pol::schema::rel_zip_addr::kPrefCode, esriFieldType::esriFieldTypeString, VARIANT_FALSE, VARIANT_FALSE, zip_pol::schema::rel_zip_addr::ePrefCode, 0, ipFields) ||
		!addAttrField(m_strShapefileName, zip_pol::schema::rel_zip_addr::kCityCode, esriFieldType::esriFieldTypeString, VARIANT_FALSE, VARIANT_FALSE, zip_pol::schema::rel_zip_addr::eCityCode, 0, ipFields) ||
		!addAttrField(m_strShapefileName, zip_pol::schema::rel_zip_addr::kOazaCode, esriFieldType::esriFieldTypeString, VARIANT_FALSE, VARIANT_FALSE, zip_pol::schema::rel_zip_addr::eOazaCode, 0, ipFields) ||
		!addAttrField(m_strShapefileName, zip_pol::schema::rel_zip_addr::kAzaCode, esriFieldType::esriFieldTypeString, VARIANT_FALSE, VARIANT_FALSE, zip_pol::schema::rel_zip_addr::eAzaCode, 0, ipFields) ||
		!addAttrField(m_strShapefileName, zip_pol::schema::rel_zip_addr::kChibanCode, esriFieldType::esriFieldTypeString, VARIANT_FALSE, VARIANT_FALSE, zip_pol::schema::rel_zip_addr::eChibanCode, 0, ipFields) ||
		!addAttrField(m_strShapefileName, zip_pol::schema::rel_zip_addr::kJukyoCode, esriFieldType::esriFieldTypeString, VARIANT_FALSE, VARIANT_FALSE, zip_pol::schema::rel_zip_addr::eJukyoCode, 0, ipFields)){
		return false;
	}

	return true;
}

bool CConvRelZipAddr::loadData()
{
	m_mapZipAddr.clear();
	m_setAddr11Acc2.clear();

	// 住所データディレクトリが設定されていれば精度2の11桁住所を取得
	if(!loadAddr11Acc2()){
		return false;
	}

	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
	CString strWhere;
	// 整備状態が使用中のものだけ取得
	strWhere.Format(_T("%s=%ld"), sindy::schema::rel_addr::kModifyStatus, sindy::schema::rel_addr::ModifyStatusCode::Used11digit);
	if(S_OK != ipQFilter->put_WhereClause(CComBSTR(strWhere))){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutWhereClause, sindy::schema::rel_addr::kTableName20, _T(""));
		return false;
	}

	_ICursorPtr ipCursor;
	if(S_OK != m_ipRelAddr->Search(ipQFilter, VARIANT_FALSE, &ipCursor)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetCursor, sindy::schema::rel_addr::kTableName20, _T(""));
		return false;
	}
	bool bRet = true;
	_IRowPtr ipRow;
	while(S_OK == ipCursor->NextRow(&ipRow) && ipRow){
		// 郵便番号と20桁コードを取得
		CComVariant vaZipCode, vaAddrCode;
		if(S_OK != ipRow->get_Value(m_mapFieldIndexRelAddr[sindy::schema::rel_addr::kZipCode], &vaZipCode) ||
			S_OK != ipRow->get_Value(m_mapFieldIndexRelAddr[sindy::schema::rel_addr::kAddrCode], &vaAddrCode)){
				CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetValue, sindy::schema::rel_addr::kTableName20, getID(ipRow));
				return false;
		}
		// 20桁コードを都道府県、市区町村コード以下に分解
		CString strZipCode(vaZipCode.bstrVal), strAddrCode(vaAddrCode.bstrVal);
		long lAddrCodeLen = strAddrCode.GetLength();
		CRelZipAddr CRelZipAddr;
		switch(lAddrCodeLen){
		case ADDR_DIGIT_JUKYO_LEVEL:
			CRelZipAddr.strJukyoCode = strAddrCode.Mid(ADDR_DIGIT_CHIBAN_LEVEL, zip_pol::schema::rel_zip_addr::eJukyoCode);
		case ADDR_DIGIT_CHIBAN_LEVEL:
			CRelZipAddr.strChibanCode = strAddrCode.Mid(ADDR_DIGIT_AZA_LEVEL, zip_pol::schema::rel_zip_addr::eChibanCode);
		case ADDR_DIGIT_AZA_LEVEL:
			CRelZipAddr.strAzaCode = strAddrCode.Mid(ADDR_DIGIT_OAZA_LEVEL, zip_pol::schema::rel_zip_addr::eAzaCode);
			CRelZipAddr.strOazaCode = strAddrCode.Mid(ADDR_DIGIT_CITY_LEVEL, zip_pol::schema::rel_zip_addr::eOazaCode);
			CRelZipAddr.strCityCode = strAddrCode.Mid(ADDR_DIGIT_PREF_LEVEL, zip_pol::schema::rel_zip_addr::eCityCode);
			CRelZipAddr.strPrefCode = strAddrCode.Mid(0, zip_pol::schema::rel_zip_addr::ePrefCode);
			break;
		default:
			// 住所コードの桁数がおかしい
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgAddrCode, sindy::schema::rel_addr::kTableName20, getID(ipRow), strZipCode, strAddrCode);
			bRet = false;
			continue;
			break;
		}
		// 11桁コードの場合、精度2のレコードは出力対象外とする
		if(m_setAddr11Acc2.cend() != m_setAddr11Acc2.find(strAddrCode)){
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::warning, err_code::NgAddr11AccCode2, sindy::schema::rel_addr::kTableName20, getID(ipRow), strZipCode, strAddrCode);
			continue;
		}

		if(!m_mapZipAddr[strZipCode].insert(CRelZipAddr).second){
			// 郵便番号と住所コードが同じレコードがあるのはNG
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgDuplicateRelAddr, sindy::schema::rel_addr::kTableName20, getID(ipRow), strZipCode, strAddrCode);
			bRet = false;
			continue;
		}
	}

	return bRet;
}

bool CConvRelZipAddr::loadAddr11Acc2()
{
	// 住所データディレクトリが設定されていなければ終了
	if(m_strAddr11Dir.IsEmpty()){
		return true;
	}

	// ディレクトリ直下のファイルを取得
	std::vector<uh::tstring> vecAddr11;
	uh::getFileNames(m_strAddr11Dir, _T("*.txt"), vecAddr11);

	// ファイルが無ければNG
	if(0 == vecAddr11.size()){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgAddr11Dir, m_strAddr11Dir);
		return false;
	}

	bool bReturn = true;

	for(const auto& filePath : vecAddr11){
		// addrxx.txtでなければスルー
		boost::wregex reg(ADDR11_FILENAME);
		std::wstring str = CT2CW(filePath.c_str());
		if(!boost::regex_match(str, reg)){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::warning, err_code::NgAddr11File, CString(filePath.c_str()));
			continue;
		}

		CString strFilePath;
		strFilePath.Format(_T("%s\\%s"), m_strAddr11Dir, filePath.c_str());
		std::ifstream ifs(strFilePath);
		if(ifs.fail()){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgAddr11FileOpen, strFilePath);
			bReturn = false;
			continue;
		}
		
		std::string aLine;
		while(std::getline(ifs, aLine)){
			// EUC->SHIFT-JIS変換
			CString strLine = convEUC2SJIS(aLine.c_str());

			// addr**.txtのフォーマットが正しいか簡易確認
			std::vector<CString> vecLine = uh::str_util::split(strLine, _T("\t"), true);
			if(ADDR11_TAB_COUNT != vecLine.size()){
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgAddr11Record, strFilePath);
				bReturn = false;
				break;
			}
			// 精度コード2のレコードを確保
			if(0 == vecLine[ADDR11_POS_ACCCODE].Compare(ACCCODE_2)){
				CString strAddr11;
				strAddr11.Format(_T("%s%s%s%s"), vecLine[ADDR11_POS_PREFCODE], vecLine[ADDR11_POS_CITYCODE], vecLine[ADDR11_POS_AZACODE], vecLine[ADDR11_POS_OAZACODE]);
				// 重複はないはず
				if(!m_setAddr11Acc2.insert(strAddr11).second){
					CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgAddr11Duplicate, strAddr11);
					bReturn = false;
					continue;
				}
			}
		}
	}
	return bReturn;
}

bool CConvRelZipAddr::outputData()
{
	_ICursorPtr ipCursor;
	if(S_OK != m_ipOutShapefile->Insert(VARIANT_FALSE, &ipCursor)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgInsertCursor, m_strShapefileName, _T(""));
		return false;
	}

	long n = 0;
	for(const auto& relZipAddr : m_mapZipAddr){
		for(const auto& addr : relZipAddr.second){
			++n;
			IRowBufferPtr ipRowBuffer;
			if(S_OK != m_ipOutShapefile->CreateRowBuffer(&ipRowBuffer)){
				CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgCreateRowBuffer, m_strShapefileName, _T(""));
				return false;
			}
			if(S_OK != ipRowBuffer->put_Value(m_mapFieldIndexOutShapefile[zip_pol::schema::rel_zip_addr::kZipCode], CComVariant(relZipAddr.first)) ||
				S_OK != ipRowBuffer->put_Value(m_mapFieldIndexOutShapefile[zip_pol::schema::rel_zip_addr::kPrefCode], CComVariant(addr.strPrefCode)) ||
				S_OK != ipRowBuffer->put_Value(m_mapFieldIndexOutShapefile[zip_pol::schema::rel_zip_addr::kCityCode], CComVariant(addr.strCityCode)) ||
				S_OK != ipRowBuffer->put_Value(m_mapFieldIndexOutShapefile[zip_pol::schema::rel_zip_addr::kOazaCode], CComVariant(addr.strOazaCode)) ||
				S_OK != ipRowBuffer->put_Value(m_mapFieldIndexOutShapefile[zip_pol::schema::rel_zip_addr::kAzaCode], CComVariant(addr.strAzaCode)) ||
				S_OK != ipRowBuffer->put_Value(m_mapFieldIndexOutShapefile[zip_pol::schema::rel_zip_addr::kChibanCode], CComVariant(addr.strChibanCode)) ||
				S_OK != ipRowBuffer->put_Value(m_mapFieldIndexOutShapefile[zip_pol::schema::rel_zip_addr::kJukyoCode], CComVariant(addr.strJukyoCode))){
					CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutValue, m_strShapefileName, _T(""));
					return false;
			}
			CComVariant vaOID;
			if(S_OK != ipCursor->InsertRow(ipRowBuffer, &vaOID)){
				CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgInsertRow, m_strShapefileName, _T(""));
				return false;
			}
			// Flushはこまめに
			if(0 == n % FLUSH_COUNT){
				if(FAILED(ipCursor->Flush())){
					CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgFlush, m_strShapefileName, _T(""));
					return false;
				}
			}
		}
	}
	if(FAILED(ipCursor->Flush())){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgFlush, m_strShapefileName, _T(""));
		return false;
	}

	return true;
}
