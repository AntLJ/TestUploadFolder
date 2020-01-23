#include "stdafx.h"
#include "ArcHelperEx/AheGeometryOp.h"

extern crd_cnv g_cnv;

// ジオメトリからR-Tree用のボックスを得る
BGBox GeoClass::GetBox(double range) const
{
	IEnvelopePtr ipEnvelope;
	geometry->get_Envelope(&ipEnvelope);
	WKSPoint minPoint = {}, maxPoint = {};
	if(range == 0)
	{
		minPoint.X = ipEnvelope->GetXMin(); minPoint.Y = ipEnvelope->GetYMin();
		maxPoint.X = ipEnvelope->GetXMax(); maxPoint.Y = ipEnvelope->GetYMax();
	}
	else
	{
		g_cnv.GetLLPt(ipEnvelope->GetXMin(), ipEnvelope->GetYMin(), -range, -range, &(minPoint.X), &(minPoint.Y));
		g_cnv.GetLLPt(ipEnvelope->GetXMax(), ipEnvelope->GetYMax(),  range,  range, &(maxPoint.X), &(maxPoint.Y));
	}
	return BGBox(BGPoint(minPoint.X, minPoint.Y), BGPoint(maxPoint.X, maxPoint.Y));
}