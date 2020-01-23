#include "stdafx.h"
#include "CheckAll.h"

using namespace sindy;
using namespace sindy::schema;

CheckAll::CheckAll()
{
}

CheckAll::~CheckAll()
{
}

bool CheckAll::Init()
{
	bool initError = true;
	m_checkTable.SetArgument(m_args);
	initError &= m_checkTable.Init();
	m_checkTableAndMaster.SetArgument(m_args);
	initError &= m_checkTableAndMaster.Init();
	m_checkTableAndAddr.SetArgument(m_args);
	initError &= m_checkTableAndAddr.Init();
	m_checkPolygon.SetArgument(m_args);
	initError &= m_checkPolygon.Init();
	m_checkPolygonAndTable.SetArgument(m_args);
	initError &= m_checkPolygonAndTable.Init();
	m_checkPolygonAndAddr.SetArgument(m_args);
	initError &= m_checkPolygonAndAddr.Init();

	return initError;
}

bool CheckAll::Check()
{
	bool result = true;
	result &= m_checkTable.Check();
	result &= m_checkTableAndMaster.Check();
	result &= m_checkTableAndAddr.Check();
	result &= m_checkPolygon.Check();
	result &= m_checkPolygonAndTable.Check();
	result &= m_checkPolygonAndAddr.Check();
	return result;
}