#include "stdafx.h"
#include "ErrInfoRec.h"
#include "AddrPointRec.h"

ErrInfoRec::ErrInfoRec(
		const AddrPointRec& ipcPoint,
		const AddrPointRec& midPoint,
		const CString& diffPattern,
		const CString& midLayerName
		) : m_diffPattern(diffPattern)
{
	// 差分パターンごとに、メンバ変数に格納する値を変える
	switch( _tstol(diffPattern) )
	{
	case 1:
	case 4:
		setIpcAndMidPoints(ipcPoint, midPoint, midLayerName, false, true);
		break;
	case 2:
	case 6:
		setOnlyMidPoint(midPoint, midLayerName);
		break;
	case 3:
	case 5:
		setOnlyIpcPoint(ipcPoint, true);
		break;
	case 7:
	case 8:
	case 9:
		setIpcAndMidPoints(ipcPoint, midPoint, midLayerName, true, false);
		break;
	case 10:
		( -1 == midPoint.oid ) ? 
			setOnlyIpcPoint(ipcPoint, false) : setIpcAndMidPoints(ipcPoint, midPoint, midLayerName, true, false);
		break;
	case 11:
			setOnlyMidPoint(midPoint, midLayerName);
		break;
	}
}


void ErrInfoRec::setIpcAndMidPoints(
		const AddrPointRec& ipcPoint,
		const AddrPointRec& midPoint,
		const CString& midLayerName,
		bool isExportDist,
		bool isExportGeoBldId
		)
{
	m_layer = getLayerName( ipcPoint.addrPointType );
	m_oid = uh::str_util::ToString(ipcPoint.oid);
	m_layerOther = midLayerName;
	m_oidOther = uh::str_util::ToString(midPoint.oid);
	m_geoBldId = ( isExportGeoBldId ) ? ipcPoint.geoBldId : _T("");
	m_ipcAddrcode = ipcPoint.addrcode;
	m_ipcAddress = ipcPoint.address;
	m_midAddrcode = midPoint.addrcode;
	m_midAddress = midPoint.address;
	m_dist = ( isExportDist && midPoint.geo ) ?
				uh::str_util::ToString(AheGetMeterLength((IPointPtr)ipcPoint.geo, (IPointPtr)midPoint.geo), "%.2f") :
				_T("");
	m_type = ipcPoint.addrPointType;
}

void ErrInfoRec::setOnlyIpcPoint(
		const AddrPointRec& ipcPoint,
		bool isExportGeoBldId
		)
{
	m_layer = getLayerName( ipcPoint.addrPointType );
	m_oid = uh::str_util::ToString(ipcPoint.oid);
	m_layerOther = _T("");
	m_oidOther = _T("");
	m_geoBldId = ( isExportGeoBldId ) ? ipcPoint.geoBldId : _T("");;
	m_ipcAddrcode = ipcPoint.addrcode;
	m_ipcAddress = ipcPoint.address;
	m_midAddrcode = _T("");
	m_midAddress = _T("");
	m_dist = _T("");
	m_type = ipcPoint.addrPointType;
}

void ErrInfoRec::setOnlyMidPoint(
		const AddrPointRec& midPoint,
		const CString& midLayerName
		)
{
	m_layer = midLayerName;
	m_oid = uh::str_util::ToString(midPoint.oid);
	m_layerOther = _T("");
	m_oidOther = _T("");
	m_geoBldId = midPoint.geoBldId;
	m_ipcAddrcode = _T("");
	m_ipcAddress = _T("");
	m_midAddrcode = midPoint.addrcode;
	m_midAddress = midPoint.address;
	m_dist = _T("");
	m_type = midPoint.addrPointType;
}