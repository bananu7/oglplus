/*
 *  .file eglplus/enums/opengl_profile_bit_range.ipp
 *
 *  Automatically generated header file. DO NOT modify manually,
 *  edit 'source/enums/eglplus/opengl_profile_bit.txt' instead.
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

namespace enums {
EGLPLUS_LIB_FUNC aux::CastIterRange<
	const EGLenum*,
	OpenGLProfileBit
> _ValueRange(OpenGLProfileBit*)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVR_OPENGLPROFILEBIT)
#define EGLPLUS_IMPL_EVR_OPENGLPROFILEBIT
{
static const EGLenum _values[] = {
#if defined EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR
EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR,
#endif
#if defined EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT_KHR
EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT_KHR,
#endif
0
};
return aux::CastIterRange<
	const EGLenum*,
	OpenGLProfileBit
>(_values, _values+sizeof(_values)/sizeof(_values[0])-1);
}
#else
;
#endif
} // namespace enums

