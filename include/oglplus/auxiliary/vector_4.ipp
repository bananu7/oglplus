/**
 *  .file oglplus/auxiliary/vector_4.ipp
 *  .brief Specialization of Vector for 4D vectors
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

template <typename T>
class Vector<T, 4>
 : public VectorBase<T, 4>
{
private:
	typedef VectorBase<T, 4> Base;
public:
	Vector(void)
	{ }

	template <typename U>
	Vector(const Vector<U, 4>& vector)
	 : Base(vector)
	{ }

	Vector(const T (&v)[4])
	 : Base(v)
	{ }

	Vector(const T* v, std::size_t n)
	 : Base(v, n)
	{ }

	explicit Vector(T v0)
	 : Base(v0)
	{ }

	Vector(T v0, T v1, T v2, T v3)
	 : Base(oglplus::Nothing())
	{
		this->_elem[0] = v0;
		this->_elem[1] = v1;
		this->_elem[2] = v2;
		this->_elem[3] = v3;
	}

	Vector(const Vector<T, 1>& v, T v1, T v2, T v3)
	 : Base(oglplus::Nothing())
	{
		this->_elem[0] = v[0];
		this->_elem[1] = v1;
		this->_elem[2] = v2;
		this->_elem[3] = v3;
	}

	Vector(const Vector<T, 2>& v, T v2, T v3)
	 : Base(oglplus::Nothing())
	{
		this->_elem[0] = v[0];
		this->_elem[1] = v[1];
		this->_elem[2] = v2;
		this->_elem[3] = v3;
	}

	Vector(const Vector<T, 3>& v, T v3)
	 : Base(oglplus::Nothing())
	{
		this->_elem[0] = v[0];
		this->_elem[1] = v[1];
		this->_elem[2] = v[2];
		this->_elem[3] = v3;
	}

	explicit Vector(const Matrix<T, 1, 4>& matrix)
	 : Base(matrix)
	{ }

	explicit Vector(const Matrix<T, 4, 1>& matrix)
	 : Base(matrix)
	{ }

	T x(void) const
	{
		return this->At(0);
	}

	T y(void) const
	{
		return this->At(1);
	}

	T z(void) const
	{
		return this->At(2);
	}

	T w(void) const
	{
		return this->At(3);
	}

	Vector<T, 2> xy(void) const
	{
		return Vector<T, 2>(this->At(0), this->At(1));
	}

	Vector<T, 3> xyz(void) const
	{
		return Vector<T, 3>(this->At(0), this->At(1), this->At(2));
	}

	friend Vector Negated(const Vector& a)
	{
		return Vector(-a[0], -a[1], -a[2], -a[3]);
	}

	friend Vector Added(const Vector& a, const Vector& b)
	{
		return Vector(a[0]+b[0], a[1]+b[1], a[2]+b[2], a[3]+b[3]);
	}

	Vector& operator += (const Vector& v)
	{
		this->Add(v);
		return *this;
	}

	friend Vector Subtracted(const Vector& a, const Vector& b)
	{
		return Vector(a[0]-b[0], a[1]-b[1], a[2]-b[2], a[3]-b[3]);
	}

	Vector& operator -= (const Vector& v)
	{
		this->Subtract(v);
		return *this;
	}

	friend Vector Multiplied(const Vector& a, T v)
	{
		return Vector(a[0]*v, a[1]*v, a[2]*v, a[3]*v);
	}

	Vector& operator *= (T v)
	{
		this->MultiplyBy(v);
		return *this;
	}

	friend Vector Divided(const Vector& a, T v)
	{
		return Vector(a[0]/v, a[1]/v, a[2]/v, a[3]/v);
	}

	Vector& operator /= (T v)
	{
		this->DivideBy(v);
		return *this;
	}
};

