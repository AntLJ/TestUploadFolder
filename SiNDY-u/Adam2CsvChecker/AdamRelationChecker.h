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

#pragma once

#include "LogicalChecker.h"

namespace sindy  {
namespace adam2  {
namespace format {


/**
 * @brief リンク間の関連チェッカー
 */
class CLinkRelationChecker : public CAdam2LoglcalChecker
{
public:
	CLinkRelationChecker() {}
	virtual ~CLinkRelationChecker() {}

private:
	virtual bool run() override;

protected:
	virtual bool checkAttr(const Row&, const Feature&, const Feature&){ return true; }
};

/**
* @brief 車線リンク-区画線リンク間の関連チェッカー
*/
class CLaneCompartLinkRelationChecker : public CLinkRelationChecker
{
public:
	CLaneCompartLinkRelationChecker() {}
	virtual ~CLaneCompartLinkRelationChecker() {}

protected:
	bool checkAttr(const Row&, const Feature&, const Feature&) override;
};


/**
* @brief 車線ノードと地物間の関連チェッカー
*/
class CNodeRelationChecker : public CAdam2LoglcalChecker
{
public:
	CNodeRelationChecker() {}
	virtual ~CNodeRelationChecker() {}

private:
	virtual bool run() override;

protected:
	virtual bool checkAttr(
		const Row& row, 
		const Feature& lane_node, 
		const Feature& object_point,
		const ImportFileList& filelist
		) { return true; }
};

/**
* @brief 車線ノードと信号機ポイント間の関連チェッカー
*/
class CLaneNodeSignalRelationChecker : public CNodeRelationChecker
{
public:
	CLaneNodeSignalRelationChecker() {}
	virtual ~CLaneNodeSignalRelationChecker() {}

protected:
	bool checkAttr(const Row& row, const Feature& lane_node, const Feature& object_point, const ImportFileList& filelist) override;
};


} // format
} // adam2
} // sindy
