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

#ifndef SINDY_LQ_H_
#define SINDY_LQ_H_

#include <arctl/field.h>
#include <sindy/schema/common.h>

namespace sindy {

ARCTL_FIELD_VALUE_CLASS(
	lq_t,
	(2(schema::link_queue::kLinkID, long))
	(2(schema::link_queue::kLinkDir, long))
);

ARCTL_FIELD_VALUE_CLASS(
	lq3_t,
	((lq_t))
	(2(schema::link_queue::kMeshCode, long))
);

} // namespace sindy

#endif // SINDY_LQ_H_
