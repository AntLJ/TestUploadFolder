#include "stdafx.h"
#include "AddrRecHelper.h"
#include "AddrRec.h"

#include <TDC/useful_headers/str_util.h>

#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGlobalsMisc.h>
#include <boost/assign.hpp>

namespace addr
{

using namespace cv_schema;
using namespace sindy;
using namespace sindy::schema;
using namespace AddrRecHelperEnum;

bool AddrRecHelper::makeAddrNames(AddrRec& record)
{
	bool ret = true;

	// �n�Ԗ��̐���
	ret &= makeChibanName(record);

	// �����̐���
	ret &= makeGouName(record);

	return ret;
}

bool AddrRecHelper::makeChibanName(AddrRec& record)
{
	// �n�ԃR�[�h���Ȃ��ꍇ�A���̂̐����̓X�L�b�v
	if( record.getChibancode().IsEmpty() )
		return true;

	// �����t���ł͂Ȃ��ꍇ
	if(!record.isChibanChar())
	{
		// �n�ԃR�[�h�𐔒l���������̂��Z�b�g
		long code = _tstol(record.getChibancode());
		record.setChibanName(uh::str_util::ToString(code));
		return true;
	}

	// �����t���̏ꍇ
	uh::tstring sql = uh::str_util::format
						(
							_T("select %s from %s where %s = '%s' and %s = '%s'"), 
							pnt_conv::kPntName,
							pnt_conv::kTableName,
							pnt_conv::kAddrCode, record.getAdrcode11(), 
							pnt_conv::kPntCode, record.getChibancode()
						);

	CString resultName;
		
	// �����t���n�Ԗ��̂��擾
	if( getName(sql, resultName) )
	{
		record.setChibanName(resultName);
		return true;
	}

	return false;
}

bool AddrRecHelper::makeGouName(AddrRec& record)
{
	// ���R�[�h���Ȃ��ꍇ�A���̂̐����̓X�L�b�v
	if( record.getGoucode().IsEmpty() )
		return true;

	// �����t���ł͂Ȃ��ꍇ�i�P���ɑS�p������j
	if( !record.isGouChar() )
	{
		long code = _tstol(record.getGoucode());
		record.setGouName(uh::str_util::ToString(code));
		return true;
	}

	// �����t���̏ꍇ
	uh::tstring sql = uh::str_util::format
						(
							_T("select %s from %s where %s = '%s' and %s = '%s'"), 
							gou_conv::kGouName,
							gou_conv::kTableName,
							gou_conv::kAddrCode, record.getAdrcode11(), 
							gou_conv::kGouCode, record.getGoucode()
						);

	CString resultName;
	std::vector<AttributeValue> attrValues = boost::assign::pair_list_of
			( gou_conv::kAddrCode, record.getAdrcode11() )
			( gou_conv::kGouCode, record.getGoucode() );
		
	// �����t�������̂��擾
	if( getName(sql, resultName) )
	{
		record.setGouName(resultName);
		return true;
	}

	return false;
}

bool AddrRecHelper::getName(
						const uh::tstring& sql,
						CString& resultName)
{
	// �܂��͕����t���Z�����R�[�h�ϊ��e�[�u��������
	if( searchStrValue(m_charConvAdo, CT2CA(sql.c_str()), resultName) )
		return true;

	// �Ȃ���Ε����t���Z���R�[�h�ϊ����e�[�u��������
	if( searchStrValue(m_charConvTmpAdo, CT2CA(sql.c_str()), resultName) )
		return true;

	return false;
}

void AddrRecHelper::getCityAdminRecs(
						CTableContainer& cityAdminT,
						const CString& addrcode11,
						AddrRecPairs& caAddrRecPairs)
{
	// �s�s�n�}�s���E�|���S������11���R�[�h����v����I�u�W�F�N�g���擾
	auto query = AheInitQueryFilter
					(
						nullptr,
						nullptr,
						_T("%s = '%s' and (%s = '%s' or %s = '%s')"),
						city_admin::kCityCode, addrcode11.Mid(0, 5),
						city_admin::kAddrCode, addrcode11.Mid(5, 6),
						city_admin::kAddrCode2, addrcode11.Mid(5, 6)
						);

	cityAdminT._Select(query, true);

	for(const auto& ca : cityAdminT)
	{
		const auto& row = *CAST_ROWC(ca);

		// �Z�����R�[�h�𐶐�
		AddrRec adrRec(addrcode11, row.CRowBase::GetShapeCopy());
		
		// �n�Ԗ��̂�ݒ�
		// city_admin��
		//   �g���X�敄���ɒl������ꍇ  �F�g���X�敄��
		//   �X�敄���ɒl������ꍇ      �F�X�敄��
		//   ����ȊO�i�����l���Ȃ��ꍇ�j�F��
		CString gaikuFugo = row.GetValue(city_admin::kGaikuFugo, CString()) ;
		CString extGaikuFugo = row.GetValue(city_admin::kExtGaikuFugo, CString()) ;
		CString chibanName = ( !gaikuFugo.IsEmpty() ) ? gaikuFugo : extGaikuFugo;
		int addrClass = row.GetValue(city_admin::kAddrClass, 0L);
	
		// �ʏ�Z���A�����t���Z�����ꂼ��ŁA���R�[�h�����Ώۂ�city_admin���𔻒�
		bool isTgtAreaByNormal = isMakeNormalAddrRec(addrClass, gaikuFugo);
		bool isTgtAreaByChar = isMakeCharAddrRec(addrClass, gaikuFugo, extGaikuFugo, _T("0"), false);
		
		// �ʏ�E�����t���Z������������R�[�h�Ώۂł͂Ȃ��ꍇ�Afalse��ݒ�
		adrRec.setIsMatchTarget( isTgtAreaByNormal || isTgtAreaByChar );

		adrRec.setChibanName(chibanName);

		caAddrRecPairs.emplace_back(std::make_pair(adrRec, row));
	}
}

void AddrRecHelper::makeAdrRecordsFromGp(
						CTableContainer& gouPointT,
						const AddrRecPairs& caAdrRecPairs,
						AddrRecs& gpAdrRecs)
{
	for (const auto& caAdrRecPair : caAdrRecPairs)
	{
		makeAdrRecordsFromGp(gouPointT, caAdrRecPair.first.getAdrcode11(), caAdrRecPair.second, gpAdrRecs);
	}
}

void AddrRecHelper::makeAdrRecordsFromGp(
						CTableContainer& gouPointT,
						const CString& addrcode11,
						const CRowContainer& caRow,
						AddrRecs& gpAdrRecs)
{
	// �����Ώۂ̓s�s�n�}�s���E�|���S���̌`��ɐڐG���鍆�|�C���g���擾
	auto query = AheInitSpatialFilter( nullptr, caRow.CRowBase::GetShapeCopy());
	auto query2 = AheInitQueryFilter
			(
				(IQueryFilterPtr)AheGetClone(query),
				nullptr
			);
	gouPointT._Select(query2, false);

	for(const auto& gp : gouPointT)
	{
		const auto& row = *CAST_ROWC(gp);

		// ���|�C���g����Z�����R�[�h�iAdrRecord�j�Q�𐶐�
		makeAdrRecordsFromGp(caRow, row, addrcode11, gpAdrRecs);
	}

	// �������ʂ��N���A
	gouPointT.clear();
}

void AddrRecHelper::makeAdrRecordsFromGp(
						const CRowContainer& caRow,
						const CRowContainer& gpRow,
						const CString& addrcode11,
						AddrRecs& gpAdrRecs)
{
	bool ret = true;

	// ���|�C���g����K�v�ȑ����l���擾�i���ԍ��擾���A[#]�͎�菜���j
	CString gouNo = gpRow.GetValue(gou_point::kGouNo, CString());
	bool expGouNo = (1 == gpRow.GetValue(gou_point::kExpGouNo, 0L)) ? true : false;
	CString gaikuFugo = caRow.GetValue(city_admin::kGaikuFugo, CString());
	CString extGaikuFugo = caRow.GetValue(city_admin::kExtGaikuFugo, CString());
	IGeometryPtr geo = gpRow.CRowBase::GetShapeCopy();
	int addrClass = caRow.GetValue(city_admin::kAddrClass, 0L);

	// ���ԍ����u*�v�̏ꍇ�́A�������Ȃ�
	if( _T("*") == gouNo )
		return;

	// �ʏ�Z�������Ώۂ̏ꍇ�A�ʏ�Z���𐶐�
	if( isMakeNormalAddrRec(addrClass, gaikuFugo) )
		makeNormalAdrRecordFromGp(gpAdrRecs, addrcode11, gaikuFugo, gouNo, expGouNo, geo);

	// �����t���Z�������Ώۂ̏ꍇ�A�����t���Z���𐶐�
	if( isMakeCharAddrRec(addrClass, gaikuFugo, extGaikuFugo, gouNo, expGouNo) )
		makeCharAdrRecordFromGp(gpAdrRecs, addrcode11, extGaikuFugo, gaikuFugo, gouNo, expGouNo, geo);

	return;
}

bool AddrRecHelper::isMakeNormalAddrRec(int addrClass, const CString& gaikuFugo)
{
	// �Z��������ʂ��u1�v�u2�v�ł���̂ɁA�X�敄���ɒl���Ȃ��ꍇ�A�ʏ�Z���͐������Ȃ�
	if( gaikuFugo.IsEmpty() &&
		( city_admin::addr_class::kChibanMaintenance == addrClass ||
		  city_admin::addr_class::kResidenceIndicationMaintenance == addrClass ) )
		  return false;

	return true;
}

bool AddrRecHelper::isMakeCharAddrRec(
						int addrClass,
						const CString& gaikuFugo,
						const CString& extGaikuFugo, 
						const CString& gouNo,
						bool expGouNo)
{
	// �����t���Z���ł͂Ȃ��ꍇ�A�������Ȃ�
	if( AddrStatus::kNormal == getAddrStatus(gaikuFugo, extGaikuFugo, gouNo, expGouNo) )
		return false;

	// city_admin�̏Z��������ʂ��u0�v�u2�v�̏ꍇ�A�����t���Z���͐������Ȃ�
	if( addrClass == city_admin::addr_class::kAzaUndeveloped ||
		addrClass == city_admin::addr_class::kChibanMaintenance )
		return false;

	return true;
}

void AddrRecHelper::makeCharAdrRecordFromGp(
						AddrRecs& addrRecs,
						const CString& addrcode,
						const CString& extGaikuFugo,
						const CString& gaikuFugo,
						const CString& gouNo,
						bool expGouNo,
						IGeometryPtr& geo)
{
	// CharAddrConvert.exe��CMakeAddrRec::MakeGouRecordByDB���Q�l�ɍ쐬

	AddrRec rec(addrcode, geo);
	// ���ԍ�����[#]���������ď������s��
	CString gouNoRmShp = uh::str_util::replace(gouNo, _T("#"), _T(""));

	// city_admin�̊g���X�敄������ł͂Ȃ��Agou_point�̊g�����ԍ��t���O��false(0)�̏ꍇ
	if( !expGouNo && !extGaikuFugo.IsEmpty() )
	{
		// �n�Ԗ��́Fcity_admin�̊g���X�敄��
		// �����́@�Fgou_point�̍��ԍ�
		rec.setAddrNames(extGaikuFugo, gouNoRmShp);
	}
	else
	{
		// city_admin�̊X�敄���ɒl���ݒ肳��Ă��āAgou_point�̊g�����ԍ��t���O��false(0)�̏ꍇ
		if( !expGouNo && !gaikuFugo.IsEmpty() )
		{
			// �n�Ԗ��́Fcity_admin�̊X�敄��
			// �����́@�Fgou_point�̍��ԍ�
			rec.setAddrNames(gaikuFugo, gouNoRmShp);
		}
		else // ����ȊO�̏ꍇ
		{
			auto& splitGouNo = uh::str_util::split(gouNoRmShp, SEP_HYPHEN, true);

			// �n�Ԗ���
			// gou_point�̍��ԍ��̒l���n�C�t����؂����ꍇ�A
			//   2�ȏ�ɋ�؂��F�������1�v�f��
			//   ��؂�Ȃ��@�@�@ �Fgou_point�̍��ԍ�
			const CString strChibanName = ( 1 == splitGouNo.size() ) ? gouNoRmShp : splitGouNo[0]; 

			// ������
			// gou_point�̍��ԍ��̒l���n�C�t����؂����ꍇ�A
			//   2�ȏ�ɋ�؂��Fgou_point�̍��ԍ���1�ڂ̃n�C�t�������̕����S��
			//   ��؂�Ȃ�       �F��
			CString strGouName;
			if( 1 != splitGouNo.size() )
			{
				splitGouNo.erase(splitGouNo.begin());
				strGouName = uh::str_util::join(splitGouNo, SEP_HYPHEN);
			}

			rec.setAddrNames(strChibanName, strGouName);
		}
	}
	addrRecs.emplace_back(rec);
}

void AddrRecHelper::makeNormalAdrRecordFromGp(
						AddrRecs& addrRecs,
						const CString& addrcode,
						const CString& gaikuFugo,
						const CString& gouNo,
						bool expGouNo,
						IGeometryPtr& geo)
{
	// CreateAddrWorkTxt.exe��CGP2WorkTxt::WriteData���Q�l�ɍ쐬

	// ���ԍ����A�󂩁u*�v�̏ꍇ�͐������Ȃ�
	if( gouNo.IsEmpty() || _T("*") == gouNo )
		return;
	
	// AddrLib��CGouPoint::GetAddrCode_NoChar���烍�W�b�N���R�s�[
	// TODO:AddrLib�Ƌ��ʉ��o����悤�ɂ���

	// ���ԍ��𕪗�(��ʂQ��)
	long lGouNo[2] = {0};
	CString strTmpGouNo( gouNo );

	// ���lor�n�C�t�����܂܂�Ȃ��ꍇ�́A�������Ȃ�
	int nPos = strTmpGouNo.FindOneOf( _T("0123456789-") );
	if( nPos == -1 )
		return;

	strTmpGouNo = strTmpGouNo.Mid( nPos );
	int nHaihunPos = strTmpGouNo.Find( _T("-") );
	if( 0 != nHaihunPos )
	{
		lGouNo[0] = _tstol( strTmpGouNo );
		if( nHaihunPos > 0 )
		{
			strTmpGouNo = strTmpGouNo.Mid( nHaihunPos );
			nPos = strTmpGouNo.FindOneOf( _T("0123456789") );
			if( nPos >= 0 )
				lGouNo[1] = _tstol( strTmpGouNo.Mid(nPos) );
		}
	}
	else
		return;

	AddrRec rec(addrcode, geo);
	CString gouNoFirst = (lGouNo[0] > 0) ? uh::str_util::ToString(lGouNo[0]) : _T("");
	CString gouNoSecond = (lGouNo[1] > 0) ? uh::str_util::ToString(lGouNo[1]) : _T("");

	bool isAddRec = true;
	//�n�ԃR�[�h/���R�[�h�̌����ȏ�̂��̂����݂���ꍇ�A���X�g�ɂ͒ǉ����Ȃ�
	if( !gaikuFugo.IsEmpty() && !expGouNo && _tstol(gouNoFirst) <= 9999 )
		rec.setAddrNames(gaikuFugo, gouNoFirst);
	else if( !gouNoFirst.IsEmpty() && _tstol(gouNoFirst) <= 99999 && _tstol(gouNoSecond) <= 9999)
		rec.setAddrNames(gouNoFirst, gouNoSecond);
	else // �����ȊO�̏ꍇ�A�Z�����R�[�h���X�g�ɂ͒ǉ����Ȃ�
		isAddRec = false;

	if( isAddRec )
		addrRecs.emplace_back(rec);
}

void AddrRecHelper::makeAdrRecordsFromCs(
						CTableContainer& csPointT,
						const CString& whereClause,
						std::map<CString, AddrRecs>& addrRecs)
{
	// 11���R�[�h�ƍ��v����s�s�n�}�s���E�|���S�����擾
	auto query = AheInitQueryFilter
					(
						nullptr,
						nullptr,
						( !whereClause.IsEmpty() ) ? whereClause : _T("")
					);

	csPointT._Select(query, true);

	for(const auto& cs : csPointT)
	{
		const auto& row = *CAST_ROWC(cs);

		AddrRec addrRec(row.GetValue(cs_addr_point::kAddrCode, CString()), row.CRowBase::GetShapeCopy());

		// �n�Ԗ���+�����̂��擾
		CString chibanName = ( !addrRec.getChibancode().IsEmpty() ) ? 
								uh::str_util::ToString(_tstol(addrRec.getChibancode())) : _T("");
		CString gouName = ( !addrRec.getGoucode().IsEmpty() ) ? 
								uh::str_util::ToString(_tstol(addrRec.getGoucode())) : _T("");

		addrRec.setAddrNames(chibanName, gouName);
		addrRecs[addrRec.getAdrcode11()].emplace_back(addrRec);
	}

	// �������ʂ��N���A
	csPointT.clear();
}

AddrStatus AddrRecHelper::getAddrStatus(
							const CString& gaikuFugo,
							const CString& extGaikuFugo, 
							const CString& gouNo,
							bool expGouNo)
{
	// ���ԍ��̐擪[#]�Ȃ當���t��
	if( 0 == gouNo.Find( _T('#') ) )
		return ( !expGouNo && !extGaikuFugo.IsEmpty() ) ? kMojiGou_ExtGaiku : kMojiGou;

	const auto& splitGouNo = uh::str_util::split(gouNo, SEP_HYPHEN, true);
	const long lGouNoFirst = ( splitGouNo.size() > 0 && !splitGouNo[0].IsEmpty() ) ? _tstol(splitGouNo[0]) : 0;
	const long lGouNoSecond = ( splitGouNo.size() > 1 && !splitGouNo[1].IsEmpty() ) ? _tstol(splitGouNo[1]) : 0;

	if( !expGouNo && !extGaikuFugo.IsEmpty() )	// �g���X�敄������
	{
		// ��؂肪2�ȏ�Ȃ當���t����
		if( splitGouNo.size() > 1 )
			return kSepOver_ExtGaiku;

		// ����10,000�ȏ�Ȃ當���t����
		return ( lGouNoFirst > 9999 ) ? kNoOver_ExtGaiku : kExtGaiku;
	}

	// �E�X�敄������A���A�g�����ł͂Ȃ��A���ԍ��̋�؂�i���p�n�C�t���j��2�ȏ�
	// �E�g�����ŁA�����ԍ��̋�؂肪3�ȏ�
	// �����ꂩ�̏ꍇ�A�����t���Ƃ��Ĉ���
	long lGaikuFugo = (gaikuFugo.IsEmpty() ) ? 0 : _tstol(gaikuFugo);
	if( (!expGouNo && lGaikuFugo > 0 && splitGouNo.size() > 1) || (expGouNo && splitGouNo.size() > 2) )
		return kSepOver;

	// �X�敄���Ȃ��ŋ�؂肪3�ȏ�Ȃ當���t���Ƃ��Ĉ���
	if( lGaikuFugo == 0 && splitGouNo.size() > 2 )
		return kSepOver;

	// �Ō�ɒn�Ԃ�100,000�ȏ� ����10,000�ȏ�Ȃ當���t���Ƃ��Ĉ���
	if( !expGouNo && lGaikuFugo > 0 )	// �X�敄������
		return ( _tstol(splitGouNo[0]) > 9999 ) ? kNoOver : kNormal;

	return ( (lGouNoFirst > 99999) || (splitGouNo.size() > 1 && lGouNoSecond > 9999) )
				? kNoOver : kNormal;
}

bool AddrRecHelper::connectCharConvFile(const CString& charConvFile)
{
	return openDb(m_charConvAdo, charConvFile);
}

bool AddrRecHelper::connectCharConvTmpFile(const CString& charConvTmpFile)
{
	return openDb(m_charConvTmpAdo, charConvTmpFile);
}

bool AddrRecHelper::openDb(CADOBase& ado, const CString& file)
{
	// ADO�g�p
	try
	{
		ado.disconnect();

		_ConnectionPtr ipConn( _uuidof(Connection) );

		CString strCon;
		strCon.Format( _T("Driver={Microsoft Access Driver (*.mdb)}; DBQ=%s;"), file );
		if( SUCCEEDED(ipConn->Open(_bstr_t(strCon), _T(""), _T(""), adConnectUnspecified)) )
		{
			ado.attach( ipConn );
			return true;
		}
		else
		{
			return false; // �ڑ����s
		}
	}
	catch( _com_error& e )
	{
		return false; // �ڑ����s
	}
}

bool AddrRecHelper::searchStrValue(const CADOBase& ado, const LPCSTR& sqlStr, CString& value)
{
	bool ret = true;

	CADOCursor cCur( ado.search(sqlStr));

	if( S_OK != cCur.err() )
		return false;

	if( cCur.begin() == cCur.end() )
		return false;

	CComVariant vaValue = (*cCur.begin())[0];
	value = SUCCEEDED(vaValue.ChangeType(VT_BSTR)) ? CString(vaValue.bstrVal) : _T("");
	return true;
}

} 	// namespace addr