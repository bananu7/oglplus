/**
 *  @file oglplus/shapes/obj_mesh.hpp
 *  @brief Loader of meshes stored in .obj file format
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_SHAPES_OBJ_MESH_1304161247_HPP
#define OGLPLUS_SHAPES_OBJ_MESH_1304161247_HPP

#include <oglplus/face_mode.hpp>
#include <oglplus/shapes/draw.hpp>

#include <oglplus/shapes/vert_attr_info.hpp>

#include <oglplus/auxiliary/any_iter.hpp>

#include <vector>
#include <iostream>
#include <cctype>
#include <string>

#if !OGLPLUS_LINK_LIBRARY || defined(OGLPLUS_IMPLEMENTING_LIBRARY)
#include <algorithm>
#include <sstream>
#endif

namespace oglplus {
namespace shapes {

/// Class providing attributes and instructions for drawing of mesh loaded from obj
class ObjMesh
 : public DrawingInstructionWriter
{
private:
	struct _loading_options
	{
		bool load_normals;
		bool load_tangents;
		bool load_bitangents;
		bool load_texcoords;
		bool load_materials;

		_loading_options(bool load_all = true)
		{
			All(load_all);
		}

		_loading_options& All(bool load_all = true)
		{
			load_normals = load_all;
			load_tangents = load_all;
			load_bitangents = load_all;
			load_texcoords = load_all;
			load_materials = load_all;
			return *this;
		}

		_loading_options& Nothing(void)
		{
			return All(false);
		}

		_loading_options& Normals(bool load = true)
		{
			load_normals = load;
			return *this;
		}

		_loading_options& Tangents(bool load = true)
		{
			load_tangents = load;
			return *this;
		}

		_loading_options& Bitangents(bool load = true)
		{
			load_bitangents = load;
			return *this;
		}

		_loading_options& TexCoords(bool load = true)
		{
			load_texcoords = load;
			return *this;
		}

		_loading_options& Materials(bool load = true)
		{
			load_materials = load;
			return *this;
		}
	};

	// vertex positions
	std::vector<double> _pos_data;
	// vertex normals
	std::vector<double> _nml_data;
	// vertex tangents
	std::vector<double> _tgt_data;
	// vertex bitangents
	std::vector<double> _btg_data;
	// vertex tex coords
	std::vector<double> _tex_data;
	// material numbers
	std::vector<GLuint> _mtl_data;
	// material names
	std::vector<std::string> _mtl_names;

	struct _vert_indices
	{
		GLuint _pos;
		GLuint _nml;
		GLuint _tex;
		GLuint _mtl;

		_vert_indices(void)
		 : _pos(0)
		 , _nml(0)
		 , _tex(0)
		 , _mtl(0)
		{ }
	};

	// the vertex offsets and counts for individual meshes
	std::vector<std::string> _mesh_names;
	std::vector<GLuint> _mesh_offsets;
	std::vector<GLuint> _mesh_counts;

	bool _load_index(
		GLuint& value,
		std::string::const_iterator& i,
		std::string::const_iterator& e
	);

	bool _load_indices(
		_vert_indices& indices,
		std::string::const_iterator& i,
		std::string::const_iterator& e
	);

	void _load_meshes(
		const _loading_options& opts,
		aux::AnyInputIter<const char*> names_begin,
		aux::AnyInputIter<const char*> names_end,
		std::istream& input
	);

	void _call_load_meshes(
		std::istream& input,
		aux::AnyInputIter<const char*> names_begin,
		aux::AnyInputIter<const char*> names_end,
		_loading_options opts
	);
public:
	typedef _loading_options LoadingOptions;

	ObjMesh(
		std::istream& input,
		LoadingOptions opts = LoadingOptions()
	)
	{
		const char** p = nullptr;
		_call_load_meshes(input, p, p, opts);
	}

	template <typename NameStr, std::size_t NN>
	ObjMesh(
		std::istream& input,
		const std::array<NameStr, NN>& names,
		LoadingOptions opts = LoadingOptions()
	)
	{
		_call_load_meshes(
			input,
			names.begin(),
			names.end(),
			opts
		);
	}

	/// Returns the winding direction of faces
	FaceOrientation FaceWinding(void) const
	{
		return FaceOrientation::CCW;
	}

	typedef GLuint (ObjMesh::*VertexAttribFunc)(std::vector<GLfloat>&) const;

	/// Makes the vertex positions and returns the number of values per vertex
	template <typename T>
	GLuint Positions(std::vector<T>& dest) const
	{
		dest.clear();
		dest.insert(dest.begin(), _pos_data.begin(), _pos_data.end());
		return 3;
	}

	/// Makes the vertex normals and returns the number of values per vertex
	template <typename T>
	GLuint Normals(std::vector<T>& dest) const
	{
		dest.clear();
		dest.insert(dest.begin(), _nml_data.begin(), _nml_data.end());
		return 3;
	}

	/// Makes the vertex tangents and returns the number of values per vertex
	template <typename T>
	GLuint Tangents(std::vector<T>& dest) const
	{
		dest.clear();
		dest.insert(dest.begin(), _tgt_data.begin(), _tgt_data.end());
		return 3;
	}

	/// Makes the vertex bi-tangents and returns the number of values per vertex
	template <typename T>
	GLuint Bitangents(std::vector<T>& dest) const
	{
		dest.clear();
		dest.insert(dest.begin(), _btg_data.begin(), _btg_data.end());
		return 3;
	}

	/// Makes the texture coordinates returns the number of values per vertex
	template <typename T>
	GLuint TexCoordinates(std::vector<T>& dest) const
	{
		dest.clear();
		dest.insert(dest.begin(), _tex_data.begin(), _tex_data.end());
		return 3;
	}

	/// Makes the material numbers returns the number of values per vertex
	template <typename T>
	GLuint MaterialNumbers(std::vector<T>& dest) const
	{
		dest.clear();
		dest.insert(dest.begin(), _mtl_data.begin(), _mtl_data.end());
		return 1;
	}

	/// Returns the name of the i-th material
	const std::string& MaterialName(GLuint mat_num) const
	{
		return _mtl_names[mat_num];
	}

	/// Queries the index of the mesh with the specified name
	bool QueryMeshIndex(const std::string& name, GLuint& index) const;

	/// Gets the index of the mesh with the specified name, throws on error
	GLuint GetMeshIndex(const std::string& name) const;

#if OGLPLUS_DOCUMENTATION_ONLY
	/// Vertex attribute information for this shape builder
	/** ObjMesh provides build functions for the following named
	 *  vertex attributes:
	 *  - "Position" the vertex positions
	 *  - "Normal" the vertex normals
	 *  - "Tangent" the vertex tangents
	 *  - "TexCoords" the vertex texture coordinates
	 *  - "Material" the vertex material numbers
	 */
	typedef VertexAttribsInfo<ObjMesh> VertexAttribs;
#else
	typedef VertexAttribsInfo<
		ObjMesh,
		std::tuple<
			VertexPositionsTag,
			VertexNormalsTag,
			VertexTangentsTag,
			VertexBitangentsTag,
			VertexTexCoordinatesTag,
			VertexMaterialNumbersTag
		>
	> VertexAttribs;
#endif

	/// Queries the bounding sphere coordinates and dimensions
	template <typename T>
	void BoundingSphere(Vector<T, 4>& center_and_radius) const
	{
		GLfloat min_x = _pos_data[3], max_x = _pos_data[3];
		GLfloat min_y = _pos_data[4], max_y = _pos_data[4];
		GLfloat min_z = _pos_data[5], max_z = _pos_data[5];
		for(std::size_t v=0, vn=_pos_data.size()/3; v!=vn; ++v)
		{
			GLfloat x = _pos_data[v*3+0];
			GLfloat y = _pos_data[v*3+1];
			GLfloat z = _pos_data[v*3+2];

			if(min_x > x) min_x = x;
			if(min_y > y) min_y = y;
			if(min_z > z) min_z = z;
			if(max_x < x) max_x = x;
			if(max_y < y) max_y = y;
			if(max_z < z) max_z = z;
		}

		Vector<T, 3> c(
			T((min_x + max_x) * 0.5),
			T((min_y + max_y) * 0.5),
			T((min_z + max_z) * 0.5)
		);

		center_and_radius = Vector<T, 4>(
			c.x(),
			c.y(),
			c.z(),
			Distance(c, Vector<T, 3>(T(min_x), T(min_y), T(min_z)))
		);
	}

	/// The type of the index container returned by Indices()
	typedef std::vector<GLuint> IndexArray;

	/// Returns element indices that are used with the drawing instructions
	IndexArray Indices(void) const
	{
		return IndexArray();
	}

	/// Returns the instructions for rendering of faces
	DrawingInstructions Instructions(void) const
	{
		DrawingInstructions instr = this->MakeInstructions();
		for(std::size_t m=0; m!=_mesh_offsets.size(); ++m)
		{
			DrawOperation operation;
			operation.method = DrawOperation::Method::DrawArrays;
			operation.mode = PrimitiveType::Triangles;
			operation.first = _mesh_offsets[m];
			operation.count = _mesh_counts[m];
			operation.restart_index = DrawOperation::NoRestartIndex();
			operation.phase = m;
			this->AddInstruction(instr, operation);
		}
		return std::move(instr);
	}
};

#if !OGLPLUS_LINK_LIBRARY || defined(OGLPLUS_IMPLEMENTING_LIBRARY)


OGLPLUS_LIB_FUNC
bool ObjMesh::_load_index(
	GLuint& value,
	std::string::const_iterator& i,
	std::string::const_iterator& e
)
{
	if((i != e) && (*i >= '0') && (*i <= '9'))
	{
		value = 0;
		while((i != e) && (*i >= '0') && (*i <= '9'))
		{
			value *= 10;
			value += *i-'0';
			++i;
		}
		return true;
	}
	return false;
}

OGLPLUS_LIB_FUNC
bool ObjMesh::_load_indices(
	_vert_indices& indices,
	std::string::const_iterator& i,
	std::string::const_iterator& e
)
{
	indices = _vert_indices();
	while((i != e) && (std::isspace(*i))) ++i;
	if(_load_index(indices._pos, i, e))
	{
		if(i == e) return true;
		if(std::isspace(*i)) return true;
		if(*i == '/')
		{
			++i;
			if(i == e) return false;
			if((*i >= '0') && (*i <= '9'))
			{
				if(!_load_index(indices._tex, i, e))
					return false;
			}
			if(*i == '/')
			{
				++i;
				if(i == e) return false;
				if(std::isspace(*i)) return false;
				if(!_load_index(indices._nml, i, e))
					return false;
			}
			return (*i == *e) || std::isspace(*i);
		}
	}
	return false;
}

OGLPLUS_LIB_FUNC
void ObjMesh::_load_meshes(
	const _loading_options& opts, //TODO
	aux::AnyInputIter<const char*> names_begin,
	aux::AnyInputIter<const char*> names_end,
	std::istream& input
)
{
	if(!input.good())
	{
		throw std::runtime_error("Obj file loader: Unable to read input.");
	}

	const double unused[3] = {0.0, 0.0, 0.0};
	// unused position
	std::vector<double> pos_data(unused, unused+3);
	// unused normal
	std::vector<double> nml_data(unused, unused+3);
	// unused tex. coord.
	std::vector<double> tex_data(unused, unused+3);
	// unused material
	std::vector<double> mtl_data(1, 0);
	// unused index
	std::vector<_vert_indices> idx_data(1, _vert_indices());
	_mtl_names.push_back(std::string());

	std::vector<std::string> mesh_names;
	std::vector<GLuint> mesh_offsets;
	std::vector<GLuint> mesh_counts;

	GLuint curr_mtl = 0;
	std::string mtllib;

	const std::string vert_tags(" tnp");
 	std::string line;
	while(std::getline(input, line))
	{
		std::string::const_iterator b = line.begin(), i = b, e = line.end();
		// ltrim
		while((i != e) && std::isspace(*i)) ++i;
		// skip empty lines
		if(i == e) continue;
		// skip comments
		if(*i == '#') continue;
		//
		// if it is a material library statement
		if(*i == 'm')
		{
			const char* s = "mtllib";
			if(std::find_end(i, e, s, s+6) != i)
			{
				throw std::runtime_error(
					"Obj file loader: Unknown tag at line: "+
					line
				);
			}
			i += 6;
			while((i != e) && std::isspace(*i)) ++i;
			std::string::const_iterator f = i;
			while((f != e) && !std::isspace(*f)) ++f;
			mtllib = std::string(i, f);
		}
		// if it is a use material statement
		else if(*i == 'u')
		{
			const char* s = "usemtl";
			if(std::find_end(i, e, s, s+6) != i)
			{
				throw std::runtime_error(
					"Obj file loader: Unknown tag at line: "+
					line
				);
			}
			i += 6;
			while((i != e) && std::isspace(*i)) ++i;
			std::string::const_iterator f = i;
			while((f != e) && !std::isspace(*f)) ++f;

			std::string material;
			if(!mtllib.empty()) material = mtllib + '#';
			material.append(std::string(i, f));

			curr_mtl = GLuint(_mtl_names.size());
			_mtl_names.push_back(material);
		}
		// if the line contains vertex data
		else if(*i == 'v')
		{
			++i;
			if(i == e)
			{
				throw std::runtime_error(
					"Obj file loader: Unexpected end of line: "+
					line
				);
			}
			char t = *i;
			++i;
			std::stringstream str(line.c_str()+distance(b, i));
			// if it is a known tag
			if(vert_tags.find(t) != std::string::npos)
			{
				double v[3] = {0.0, 0.0, 0.0};
				str >> v[0];
				str >> v[1];
				str >> v[2];
				if(t == ' ') pos_data.insert(pos_data.end(), v, v+3);
				if(t == 'n') nml_data.insert(nml_data.end(), v, v+3);
				if(t == 't') tex_data.insert(tex_data.end(), v, v+3);
			}
		}
		else if(*i == 'f')
		{
			++i;
			while((i != e) && std::isspace(*i)) ++i;
			_vert_indices vi1[3];
			for(std::size_t n=0; n!=3; ++n)
			{
				if(!_load_indices(vi1[n], i, e))
				{
					throw std::runtime_error(
						"Obj file loader: Error reading indices: "+
						line
					);
				}
				vi1[n]._mtl = curr_mtl;
			}
			idx_data.insert(idx_data.end(), vi1, vi1+3);
			_vert_indices vi2[3] = {vi1[0], vi1[2], _vert_indices()};
			while(_load_indices(vi2[2], i, e))
			{
				vi2[2]._mtl = curr_mtl;
				idx_data.insert(idx_data.end(), vi2, vi2+3);
				vi2[1] = vi2[2];
			}
		}
		else if(*i == 'o')
		{
			++i;
			while((i != e) && std::isspace(*i)) ++i;
			if(!mesh_offsets.empty())
			{
				mesh_counts.push_back(
					idx_data.size()-
					mesh_offsets.back()
				);
			}
			mesh_names.push_back(std::string(i, e));
			mesh_offsets.push_back(idx_data.size());
		}
	}
	// the last mesh element count
	if(mesh_offsets.empty())
	{
		if(!idx_data.empty())
		{
			mesh_offsets.push_back(1);
			mesh_counts.push_back(idx_data.size()-1);
		}
	}
	else
	{
		mesh_counts.push_back(idx_data.size()-mesh_offsets.back());
	}

	if(mesh_names.empty())
		mesh_names.push_back(std::string());
	assert(mesh_names.size() == mesh_offsets.size());
	assert(mesh_names.size() == mesh_counts.size());

	std::size_t ni = idx_data.size()-1;
	std::size_t mo = 0;

	_pos_data.resize(ni*3);
	_nml_data.resize(ni*3);
	_tex_data.resize(ni*3);
	_mtl_data.resize(ni*1);

	std::vector<std::size_t> meshes_to_load;

	if(names_begin == names_end)
	{
		meshes_to_load.resize(mesh_names.size());
		_mesh_names.resize(mesh_names.size());
		for(std::size_t m = 0; m!=mesh_names.size(); ++m)
		{
			meshes_to_load[m] = m;
			_mesh_names[m] = std::move(mesh_names[m]);
		}
	}
	else
	{
		while(names_begin != names_end)
		{
			for(std::size_t m=0; m!=mesh_names.size(); ++m)
			{
				if(*names_begin == mesh_names[m])
				{
					meshes_to_load.push_back(m);
					_mesh_names.push_back(mesh_names[m]);
					break;
				}
			}
			++names_begin;
		}
	}

	for(std::size_t l = 0; l!=meshes_to_load.size(); ++l)
	{
		std::size_t m = meshes_to_load[l];
		std::size_t ii = mesh_offsets[m];
		std::size_t mc = mesh_counts[m];
		ni = ii + mc;
		while(ii != ni)
		{
			for(std::size_t c=0; c!=3; ++c)
			{
				std::size_t oi = (ii-1)*3+c;
				_pos_data[oi] = pos_data[idx_data[ii]._pos*3+c];
				_nml_data[oi] = nml_data[idx_data[ii]._nml*3+c];
				_tex_data[oi] = tex_data[idx_data[ii]._tex*3+c];
			}
			_mtl_data[ii-1] = idx_data[ii]._mtl;
			++ii;
		}
		_mesh_offsets.push_back(mo);
		_mesh_counts.push_back(mc);
		mo += mc;
	}

	assert(_pos_data.size() % 9 == 0);
	assert(_pos_data.size() == _tex_data.size());

	if(opts.load_tangents)
	{
		if(opts.load_tangents)
			_tgt_data.resize(_pos_data.size());
		if(opts.load_bitangents)
			_btg_data.resize(_pos_data.size());
		for(std::size_t f=0, nf = _pos_data.size()/9; f != nf; ++f)
		{
			for(std::size_t v=0; v!=3; ++v)
			{
				std::size_t j[3] = {
					v,
					(v+1)%3,
					(v+2)%3
				};

				Vec3f p[3];
				Vec2f uv[3];
				for(size_t k=0; k!=3; ++k)
				{
					p[k] = Vec3f(
						_pos_data[f*9+j[k]*3+0],
						_pos_data[f*9+j[k]*3+1],
						_pos_data[f*9+j[k]*3+2]
					);
					uv[k] = Vec2f(
						_tex_data[f*9+j[k]*3+0],
						_tex_data[f*9+j[k]*3+1]
					);
				}

				Vec3f v0 = p[1] - p[0];
				Vec3f v1 = p[2] - p[0];

				Vec2f duv0 = uv[1] - uv[0];
				Vec2f duv1 = uv[2] - uv[0];

				float d = duv0.x()*duv1.y()-duv0.y()*duv1.x();
				if(d != 0.0f) d = 1.0f/d;

				if(opts.load_tangents)
				{
					Vec3f t = (duv1.y()*v0 - duv0.y()*v1)*d;
					Vec3f nt = Normalized(t);

					for(std::size_t tv=0; tv!=3; ++tv)
					{
						_tgt_data[f*9+v*3+0] = nt.x();
						_tgt_data[f*9+v*3+1] = nt.y();
						_tgt_data[f*9+v*3+2] = nt.z();
					}
				}

				if(opts.load_bitangents)
				{
					Vec3f b = (duv0.x()*v1 - duv1.x()*v0)*d;
					Vec3f nb = Normalized(b);

					for(std::size_t tv=0; tv!=3; ++tv)
					{
						_btg_data[f*9+v*3+0] = nb.x();
						_btg_data[f*9+v*3+1] = nb.y();
						_btg_data[f*9+v*3+2] = nb.z();
					}
				}
			}
		}
	}
}

OGLPLUS_LIB_FUNC
void ObjMesh::_call_load_meshes(
	std::istream& input,
	aux::AnyInputIter<const char*> names_begin,
	aux::AnyInputIter<const char*> names_end,
	_loading_options opts
)
{
	opts.load_tangents |= opts.load_bitangents;
	opts.load_bitangents |= opts.load_tangents;
	opts.load_texcoords |= opts.load_tangents;

	_load_meshes(opts, names_begin, names_end, input);
}

OGLPLUS_LIB_FUNC
bool ObjMesh::QueryMeshIndex(const std::string& name, GLuint& index) const
{
	auto p = std::find(_mesh_names.begin(), _mesh_names.end(), name);
	if(p == _mesh_names.end()) return false;
	index = GLuint(std::distance(_mesh_names.begin(), p));
	return true;
}

OGLPLUS_LIB_FUNC
GLuint ObjMesh::GetMeshIndex(const std::string& name) const
{
	GLuint result = 0;
	if(!QueryMeshIndex(name, result))
	{
		throw std::runtime_error(
			"ObjMesh: Unable to find index of mesh '"+
			name +
			"'"
		);
	}
	return result;
}

#endif // OGLPLUS_LINK_LIBRARY

} // shapes
} // oglplus

#endif // include guard
