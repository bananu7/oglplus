/**
 *  @file eglplus/texture_target.hpp
 *  @brief EGL texture target enumeration
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef EGLPLUS_TEXTURE_TARGET_1303292057_HPP
#define EGLPLUS_TEXTURE_TARGET_1303292057_HPP

#include <eglplus/enumerations.hpp>

namespace eglplus {

/// EGL texture target enumeration
/**
 *  @ingroup eglplus_enumerations
 */
EGLPLUS_ENUM_CLASS_BEGIN(TextureTarget, EGLint)
#include <eglplus/enums/texture_target.ipp>
EGLPLUS_ENUM_CLASS_END(TextureTarget)

#if !EGLPLUS_NO_ENUM_VALUE_NAMES
#include <eglplus/enums/texture_target_names.ipp>
#endif

#if !EGLPLUS_ENUM_VALUE_RANGES
#include <eglplus/enums/texture_target_range.ipp>
#endif

} // namespace oglplus

#endif // include guard
