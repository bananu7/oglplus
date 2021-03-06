/*
 *  .file eglplus/enums/render_buffer_range.ipp
 *
 *  Automatically generated header file. DO NOT modify manually,
 *  edit 'source/enums/eglplus/render_buffer.txt' instead.
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

namespace enums {
EGLPLUS_LIB_FUNC aux::CastIterRange<
	const EGLenum*,
	RenderBuffer
> _ValueRange(RenderBuffer*)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVR_RENDERBUFFER)
#define EGLPLUS_IMPL_EVR_RENDERBUFFER
{
static const EGLenum _values[] = {
#if defined EGL_SINGLE_BUFFER
EGL_SINGLE_BUFFER,
#endif
#if defined EGL_BACK_BUFFER
EGL_BACK_BUFFER,
#endif
0
};
return aux::CastIterRange<
	const EGLenum*,
	RenderBuffer
>(_values, _values+sizeof(_values)/sizeof(_values[0])-1);
}
#else
;
#endif
} // namespace enums

