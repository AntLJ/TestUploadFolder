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

#ifndef __MPQ_FUNCTIONAL_H__
#define __MPQ_FUNCTIONAL_H__

#include <functional>

namespace mpq {

/// INに存在する点であるか
class is_inpoint : public std::unary_function<MPQ::Point, bool>
{
public:
	bool operator()(const MPQ::Point& rPoint) const { return rPoint.eIN; }
};

/// INのノード点であるか
class is_innode_point : public std::unary_function<MPQ::Point, bool>
{
public:
	bool operator()(const MPQ::Point& rPoint) const { return rPoint.eID != 0; }
};

} // namespace mpq

#endif // __MPQ_FUNCTIONAL_H__
