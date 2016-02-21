/*
 * Copyright (c) 2016      Los Alamos National Security, LLC
 *                         All rights reserved.
 *
 * This file is part of the Gladius project. See the LICENSE.txt file at the
 * top-level directory of this distribution.
 */

#pragma once

#include "mrnet/Types.h"

#define PROTO_CONN (FirstApplicationTag)
#define PROTO_PING (FirstApplicationTag + 1)
#define PROTO_EXIT (FirstApplicationTag + 2)
