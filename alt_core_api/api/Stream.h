/*
 * Copyright (c) 2026 TOKITA Hiroshi
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "Print.h"
#include <stream_interface.hpp>

namespace arduino {

class Stream : virtual public Print, virtual public StreamInterface {};

} // namespace arduino
