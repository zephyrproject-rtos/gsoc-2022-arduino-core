/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

/*
 * This header is used to forcibly disable logging and assertions in the
 * MsgPack library. It is here to shadow the real DebugLog library available in
 * the index, which is currently not compatible with the latest boards.
 */

#define LOG_ERROR(...) ((void)0)
#define LOG_WARN(...)  ((void)0)
#define LOG_INFO(...)  ((void)0)
#define LOG_DEBUG(...) ((void)0)
#define LOG_TRACE(...) ((void)0)
#define ASSERT(...)    ((void)0)
#define ASSERTM(...)   ((void)0)
#define PRINT(...)     ((void)0)
#define PRINTLN(...)   ((void)0)
