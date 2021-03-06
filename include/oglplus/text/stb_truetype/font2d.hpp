/**
 *  @file oglplus/text/stb_truetype/font2d.hpp
 *  @brief Helper wrapper around the Sean Barrett's truetype rendering lib.
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2011-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef __OGLPLUS_TEXT_STB_TRUETYPE_1102101236_HPP
#define __OGLPLUS_TEXT_STB_TRUETYPE_1102101236_HPP

#include <oglplus/config_compiler.hpp>
#include <oglplus/config_basic.hpp>
#include <oglplus/text/unicode.hpp>

#if !OGLPLUS_LINK_LIBRARY || defined(OGLPLUS_IMPLEMENTING_LIBRARY)
#include <algorithm>
#include <stdexcept>
#define STB_TRUETYPE_IMPLEMENTATION
#endif
#include <stb_truetype.h>

#include <vector>
#include <istream>

namespace oglplus {
namespace text {

class STBTTFont2D;

/// Wrapper arund the Sean Barrett's true type font glyph functionality
/**
 *  This class is instantiated by STBTTFont2D class
 */
class STBTTFont2DGlyph
{
private:
	friend class STBTTFont2D;

	const ::stbtt_fontinfo* _font;
	int _index;

	STBTTFont2DGlyph(
		const ::stbtt_fontinfo& font
	): _font(&font)
	 , _index(0)
	{ }

	void _init_index(CodePoint code_point)
	{
		_index = ::stbtt_FindGlyphIndex(_font, int(code_point));
	}

	STBTTFont2DGlyph(
		const ::stbtt_fontinfo& font,
		CodePoint code_point
	): _font(&font)
	{
		_init_index(code_point);
	}
public:
	/// Queries the horizontal glyph metrics
	void GetHMetrics(int& left_bearing, int& width) const
	{
		::stbtt_GetGlyphHMetrics(_font, _index, &width, &left_bearing);
	}

	/// Queries the vertical glyph metrics
	void GetVMetrics(int& ascent, int& descent, int& line_gap) const
	{
		::stbtt_GetFontVMetrics(_font, &ascent, &descent, &line_gap);
	}

	/// Queries the glyph bitmap box
	void GetBitmapBox(
		float xscale,
		float yscale,
		int& x0,
		int& y0,
		int& x1,
		int& y1
	) const
	{
		::stbtt_GetGlyphBitmapBox(
			_font,
			_index,
			xscale,
			yscale,
			&x0,
			&y0,
			&x1,
			&y1
		);
	}

	/// Queries the glyph bitmap box with subpixel precision
	void GetBitmapBoxSubpixel(
		float xscale,
		float yscale,
		float xoffs,
		float yoffs,
		int& x0,
		int& y0,
		int& x1,
		int& y1
	) const
	{
		::stbtt_GetGlyphBitmapBoxSubpixel(
			_font,
			_index,
			xscale,
			yscale,
			xoffs,
			yoffs,
			&x0,
			&y0,
			&x1,
			&y1
		);
	}

	/// Returns the left bearing of the glyph in pixels
	int LeftBearing(void) const
	{
		int result = 0;
		::stbtt_GetGlyphHMetrics(_font, _index, nullptr, &result);
		return result;
	}

	/// Returns the right bearing of the glyph in pixels
	int RightBearing(void) const
	{
		int w = 0, lb = 0;
		::stbtt_GetGlyphHMetrics(_font, _index, &w, &lb);
		return lb + w;
	}

	/// Returns the Width of the glyph in pixels
	int Width(void) const
	{
		int result = 0;
		::stbtt_GetGlyphHMetrics(_font, _index, &result, nullptr);
		return result;
	}

	/// Returns the ascent of the glyph in pixels
	int Ascent(void) const
	{
		int result = 0;
		::stbtt_GetFontVMetrics(_font, &result, nullptr, nullptr);
		return result;
	}

	/// Returns the descent of the glyph in pixels
	int Descent(void) const
	{
		int result = 0;
		::stbtt_GetFontVMetrics(_font, nullptr, &result, nullptr);
		return result;
	}

	/// Returns the height of the glyph in pixels
	int Height(void) const
	{
		int asc = 0, dsc = 0;
		::stbtt_GetFontVMetrics(_font, &asc, &dsc, nullptr);
		return asc - dsc;
	}

	/// Returns the line gap value in pixels
	int LineGap(void) const
	{
		int result = 0;
		::stbtt_GetFontVMetrics(_font, nullptr, nullptr, &result);
		return result;
	}

	/// Renders the glyph into a buffer at start with the specified parameters
	void Render(
		unsigned char* start,
		int frame_width,
		int frame_height,
		int stride,
		float scale
	) const
	{
		::stbtt_MakeGlyphBitmap(
			_font,
			start,
			frame_width,
			frame_height,
			stride,
			scale,
			scale,
			_index
		);
	}
};


/// Wrapper arund the Sean Barrett's true type font functionality
class STBTTFont2D
{
private:
	std::vector<unsigned char> _ttf_data;

	static std::vector<unsigned char> _load_ttf(std::istream& input);

	::stbtt_fontinfo _font;

	void _load_font(const unsigned char* ttf_buffer);
public:
	/// Creates a font from an open ttf input stream
	STBTTFont2D(std::istream&& input)
	 : _ttf_data(_load_ttf(input))
	{
		_load_font(_ttf_data.data());
	}

	/// Creates a font from an open ttf input stream
	STBTTFont2D(std::istream& input)
	 : _ttf_data(_load_ttf(input))
	{
		_load_font(_ttf_data.data());
	}

	/// Creates a font from an open ttf input data
	STBTTFont2D(std::vector<unsigned char>&& ttf_data)
	 : _ttf_data(std::move(ttf_data))
	{
		_load_font(_ttf_data.data());
	}

	/// A Glyph type
	typedef STBTTFont2DGlyph Glyph;

	/// Returns the glyph for the specified code point
	Glyph GetGlyph(CodePoint code_point) const
	{
		return Glyph(_font, code_point);
	}

	/// Gets the kerning advance for two glyphs
	static int KernAdvance(const Glyph& glyph1, const Glyph& glyph2)
	{
		assert(glyph1._font == glyph2._font);
		return ::stbtt_GetGlyphKernAdvance(
			glyph1._font,
			glyph1._index,
			glyph2._index
		);
	}

	/// Returns the scale for pixel height
	float ScaleForPixelHeight(float pixels) const
	{
		return ::stbtt_ScaleForPixelHeight(&_font, pixels);
	}

	/// A layout of glyphs from the specified code points
	typedef std::vector<Glyph> Layout;

	/// Makes a glyph sequence from code points
	Layout MakeLayout(
		const CodePoint* code_points,
		const std::size_t cp_n
	) const;

	/// Makes a glyph sequence from code points
	Layout MakeLayout(const CodePoints& code_points) const
	{
		return MakeLayout(code_points.data(), code_points.size());
	}

	/// Get the base line for the layout in pixels
	float Baseline(
		std::size_t size_in_pixels,
		const Layout& /*layout*/
	) const
	{
		int result = 0;
		::stbtt_GetFontVMetrics(&_font, &result, nullptr, nullptr);
		return result*ScaleForPixelHeight(size_in_pixels);
	}

	/// Get the height of the layout in pixels
	float Height(
		std::size_t size_in_pixels,
		const Layout& /*layout*/
	) const
	{
		int asc = 0, dsc = 0;
		::stbtt_GetFontVMetrics(&_font, &asc, &dsc, nullptr);
		return (asc - dsc)*ScaleForPixelHeight(size_in_pixels);
	}

	/// Get the width of the layout in pixels
	float Width(
		std::size_t size_in_pixels,
		const Layout& layout
	) const;

	/// Render the specified text into a buffer
	void Render(
		std::size_t size_in_pixels,
		const Layout& layout,
		unsigned char* buffer_start,
		const std::size_t buffer_width,
		const std::size_t buffer_height,
		const int xposition = 0,
		const int yposition = 0
	) const;
};

#if !OGLPLUS_LINK_LIBRARY || defined(OGLPLUS_IMPLEMENTING_LIBRARY)
OGLPLUS_LIB_FUNC
void STBTTFont2D::_load_font(const unsigned char* ttf_buffer)
{
	if(!::stbtt_InitFont(
		&_font,
		ttf_buffer,
		::stbtt_GetFontOffsetForIndex(ttf_buffer, 0)
	)) throw std::runtime_error("Error loading true type font");
}

OGLPLUS_LIB_FUNC
std::vector<unsigned char> STBTTFont2D::_load_ttf(std::istream& input)
{
	if(!input.good())
	{
		throw std::runtime_error("Error reading TTF input");
	}
	input.seekg(0, std::ios_base::end);
	std::streampos size = input.tellg();
	input.seekg(0, std::ios_base::beg);
	std::vector<unsigned char> result(size, 0x00);
	if(input.read((char*)result.data(), size).gcount() != size)
	{
		throw std::runtime_error("Error reading TTF input");
	}
	return std::move(result);
}

OGLPLUS_LIB_FUNC
STBTTFont2D::Layout STBTTFont2D::MakeLayout(
	const CodePoint* code_points,
	const std::size_t cp_n
) const
{
	Layout layout(cp_n, Glyph(this->_font));
	for(std::size_t cp=0; cp!=cp_n; ++cp)
	{
		layout[cp]._init_index(code_points[cp]);
	}
	return layout;
}

OGLPLUS_LIB_FUNC
float STBTTFont2D::Width(
	std::size_t size_in_pixels,
	const Layout& layout
) const
{
	float scale = ScaleForPixelHeight(float(size_in_pixels));
	float width = 0.0f;
	for(auto i=layout.begin(), p=i, e=layout.end(); i!=e; p = i, ++i)
	{
		if(p != i) width += KernAdvance(*p, *i);
		width += i->Width();
	}
	return width*scale;
}

OGLPLUS_LIB_FUNC
void STBTTFont2D::Render(
	std::size_t size_in_pixels,
	const STBTTFont2D::Layout& layout,
	unsigned char* buffer_start,
	const std::size_t buffer_width,
	const std::size_t buffer_height,
	const int xposition,
	const int yposition
) const
{
	if(int(buffer_height)  <   yposition) return;
	if(int(size_in_pixels) <= -yposition) return;

	std::vector<unsigned char> tmp_buffer;
	int tmp_height = int(size_in_pixels);
	int tmp_width = 0;

	float scale = ScaleForPixelHeight(float(size_in_pixels));

	float xoffset = 0.0f;
	for(auto i=layout.begin(), p=i, e=layout.end(); i!=e; ++i)
	{
		const int xo = int(std::floor(xoffset))+xposition;
		const float advance = i->Width()*scale;

		int width_in_pixels = int(std::ceil(advance));

		if(xo >= int(buffer_width))
		{
			break;
		}
		if(xo+width_in_pixels < 0)
		{
			xoffset += advance;
			continue;
		}

		if(tmp_width < width_in_pixels)
		{
			tmp_width = width_in_pixels;
			tmp_buffer.resize(tmp_width*tmp_height);
		}
		std::fill(tmp_buffer.begin(), tmp_buffer.end(), 0x00);

		if(p != i) xoffset += KernAdvance(*p, *i)*scale;
		const float xshift = xoffset - std::floor(xoffset);
		int x0, y0, x1, y1;
		i->GetBitmapBoxSubpixel(
			scale, scale,
			xshift, 0,
			x0, y0,
			x1, y1
		);
		const float yshift = std::floor((i->Ascent()*scale+y0));

		::stbtt_MakeGlyphBitmapSubpixel(
			&_font,
			tmp_buffer.data(),
			tmp_width,
			tmp_height,
			tmp_width,
			scale,
			scale,
			xshift,
			yshift,
			i->_index
		);

		const int yo = yposition;

		int gb = xo<0?-xo:0;
		int gw = int(gb+1+(x1-x0));
		if(gw > tmp_width) gw = tmp_width;
		if(gw > int(buffer_width-xo)) gw = int(buffer_width-xo);
		int gy = (std::floor(yshift));
		if(gy < -yo) gy = -yo;
		int gh = tmp_height;
		if(gh > int(buffer_height-yo)) gh = int(buffer_height-yo);

		while(gy < gh)
		{
			int gx = gb;
			while(gx < gw)
			{
				int si = gy*tmp_width+gx;
				unsigned src = tmp_buffer[si];
				if(src != 0)
				{
					int di = (gy+yo)*buffer_width+gx+xo+x0;
					unsigned dst = buffer_start[di]+src;

					if(dst > 0xFF) dst = 0xFF;
					buffer_start[di] = dst & 0xFF;
				}
				++gx;
			}
			++gy;
		}

		p = i;
		xoffset += advance;
	}
}
#endif // OGLPLUS_LINK_LIBRARY

} // namespace aux
} // namespace oglplus

#endif
