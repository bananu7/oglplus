/**
 *  @file oglplus/shapes/sky_box.hpp
 *  @brief SkyBox builder
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_SHAPES_SKY_CUBE_1206011111_HPP
#define OGLPLUS_SHAPES_SKY_CUBE_1206011111_HPP

#include <oglplus/face_mode.hpp>
#include <oglplus/shapes/draw.hpp>

#include <oglplus/shapes/vert_attr_info.hpp>

namespace oglplus {
namespace shapes {

/// Class providing vertex attributes and instructions for drawing of a sky box
class SkyBox
 : public DrawingInstructionWriter
{
private:
public:
	/// Returns the winding direction of faces
	FaceOrientation FaceWinding(void) const
	{
		return FaceOrientation::CW;
	}

	typedef GLuint (SkyBox::*VertexAttribFunc)(std::vector<GLfloat>&) const;

	/// Makes the vertices and returns the number of values per vertex
	template <typename T>
	GLuint Positions(std::vector<T>& dest) const
	{
		const T _positions[8*3] = {
			T(-1), T(-1), T(-1),
			T(+1), T(-1), T(-1),
			T(-1), T(+1), T(-1),
			T(+1), T(+1), T(-1),
			T(-1), T(-1), T(+1),
			T(+1), T(-1), T(+1),
			T(-1), T(+1), T(+1),
			T(+1), T(+1), T(+1)
		};
		dest.assign(_positions, _positions+8*3);
		return 3;
	}

#if OGLPLUS_DOCUMENTATION_ONLY
	/// Vertex attribute information for this shape builder
	/** SkyBox provides build functions for the following named
	 *  vertex attributes:
	 *  - "Position" the vertex positions (Positions)
	 */
	typedef VertexAttribsInfo<SkyBox> VertexAttribs;
#else
	typedef VertexAttribsInfo<
		SkyBox,
		std::tuple<
			VertexPositionsTag
		>
	> VertexAttribs;
#endif

	/// Queries the bounding sphere coordinates and dimensions
	template <typename T>
	void BoundingSphere(Vector<T, 4>& center_and_radius) const
	{
		center_and_radius = Vector<T, 4>(T(0), T(0), T(0), T(1));
	}

	/// The type of the index container returned by Indices()
	typedef std::vector<GLushort> IndexArray;

	/// Returns element indices that are used with the drawing instructions
	IndexArray Indices(void) const
	{
		const GLushort _indices[6*5] = {
			1, 3, 5, 7, 9,
			4, 6, 0, 2, 9,
			2, 6, 3, 7, 9,
			4, 0, 5, 1, 9,
			5, 7, 4, 6, 9,
			0, 2, 1, 3, 9
		};
		return IndexArray(_indices, _indices+6*5);
	}

	/// Returns the instructions for rendering of faces
	DrawingInstructions Instructions(void) const
	{
		DrawOperation operation;
		operation.method = DrawOperation::Method::DrawElements;
		operation.mode = PrimitiveType::TriangleStrip;
		operation.first = 0;
		operation.count = 6*5;
		operation.restart_index = 9;
		operation.phase = 0;

		return this->MakeInstructions(operation);
	}
};

} // shapes
} // oglplus

#endif // include guard
