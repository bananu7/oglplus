/*
 *  .file oalplus/enums/source_type_range.ipp
 *
 *  Automatically generated header file. DO NOT modify manually,
 *  edit 'source/enums/oalplus/source_type.txt' instead.
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

namespace enums {
OALPLUS_LIB_FUNC aux::CastIterRange<
	const ALenum*,
	SourceType
> _ValueRange(SourceType*)
#if (!OALPLUS_LINK_LIBRARY || defined(OALPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(OALPLUS_IMPL_EVR_SOURCETYPE)
#define OALPLUS_IMPL_EVR_SOURCETYPE
{
static const ALenum _values[] = {
#if defined AL_UNDETERMINED
AL_UNDETERMINED,
#endif
#if defined AL_STATIC
AL_STATIC,
#endif
#if defined AL_STREAMING
AL_STREAMING,
#endif
0
};
return aux::CastIterRange<
	const ALenum*,
	SourceType
>(_values, _values+sizeof(_values)/sizeof(_values[0])-1);
}
#else
;
#endif
} // namespace enums

