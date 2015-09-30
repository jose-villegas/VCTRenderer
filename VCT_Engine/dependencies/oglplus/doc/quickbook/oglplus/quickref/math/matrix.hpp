/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_math_matrix

template <typename T, size_t Rows, size_t Cols>
class Matrix
{
public:
//]
//[oglplus_math_matrix_ctrs
	Matrix(const Matrix&); /*< Matrices are copy-constructible >*/
	Matrix(void); /*< Constructs an identity matrix.  >*/

	explicit Matrix(const __T (&data)[__Rows*__Cols]); /*<
	Construction from a const C-array of [^__T] with known size.
	>*/
	Matrix(const __T* data, size_t n); /*<
	Construction from a pointer to data and number of elements.
	The memory pointed-to by [^v] must hold at least [^n] valid instances
	of type [^__T] and [^n] must be greater or equal to [^__Rows*__Cols] -
	the dimension of the vector.
	>*/

	template <typename U, size_t R, size_t C>
	explicit Matrix(const Matrix<U, R, C>& other); /*<
	Construction from a [^Matrix] with different dimensions and possibly
	different underlying data type. This constructor is available only
	if [^U] is convertible to [^__T], [^R>=__Rows] and [^C>=__Cols].
	>*/

	template <typename ... P>
	explicit Matrix(P ... p); /*<
	Construction from a pack of values. This constructor is available only
	if the number of arguments in [^p] is equal to [^__Rows*__Cols].
	>*/

	template <typename ... C>
	explicit Matrix(const __Vector<T, C> ... row); /*<
	Construction from a pack of instances of __Vector.
	This constructor is only available if the number of vectors specified
	is equal to the number of [^__Rows] and each vector has exactly [^__Cols]
	components.
>*/
//]
//[oglplus_math_matrix_fns
	void Fill(__T value); /*<
	Fills all elements of the [^Matrix] with the specified value.
	>*/
	const __T* Data(void) const; /*<
	Returns a pointer to the matrix elements in row major order.
	>*/
	size_t Size(void) const; /*<
	Returns the number of elements of the matrix.
	>*/

	__T At(size_t i, size_t j) const; /*<
	Returns the value at the position specified by [^i] and [^j].
	>*/
	void Set(size_t i, size_t j, __T v); /*<
	Sets the value of the element at the position specified by [^i] and [^j].
	>*/

	__Vector<__T, __Cols> Row(size_t i) const; /*<
	Returns the [^i]th row vector of [^this] matrix.
	>*/
	__Vector<__T, __Rows> Col(size_t j) const; /*<
	Returns the [^j]th column vector of [^this] matrix.
	>*/

	template <size_t I, size_t J, size_t R, size_t C>
	Matrix<T, R, C> Submatrix(void) const /*<
	Returns a submatrix of [^this] matrix with [^R] rows and [^C] columns
	starting from row [^I] and column [^J]. Note, that the following
	must be true: [^__Rows-I>=R] and [^__Cols-J>=C].
>*/
//]
//[oglplus_math_matrix_friend_ops
	friend bool operator == (const Matrix& a, const Matrix& b); /*<
	Equality comparison.
	>*/
	friend bool operator != (const Matrix& a, const Matrix& b); /*<
	Inequality comparison.
>*/
//]
//[oglplus_math_matrix_friend_fns
	friend const __T* Data(const Matrix& matrix); /*<
	Returns a pointer to the matrix elements in row major order.
	>*/
	friend size_t Size(const Matrix& matrix); /*<
	Returns the number of elements of the matrix.
	>*/
	friend size_t Rows(const Matrix& matrix); /*<
	Returns the number of rows of the matrix.
	>*/
	friend size_t Cols(const Matrix& matrix); /*<
	Returns the number of columns of the matrix.
	>*/

	friend bool Equal(const Matrix& a, const Matrix& b); /*<
	Returns true if the two matrices [^a] and [^b] are equal.
	>*/
	friend Matrix Negated(const Matrix& a); /*<
	Returns a copy of the matrix passed as argument with
	all elements negated.
	>*/

	friend Matrix Added(const Matrix& a, const Matrix& b); /*<
	Matrix addition.
	>*/
	friend Matrix Subtracted(const Matrix& a, const Matrix& b); /*<
	Matrix subtraction.
	>*/
	friend Matrix Multiplied(const Matrix& a, __T c); /*<
	Matrix-by-constant multiplication.
	>*/

	template <size_t N>
	friend Matrix Multiplied(
		const Matrix<__T, __Rows, N>& a,
		const Matrix<__T, N, __Cols>& b
	); /*< Matrix-by-matrix multiplication. >*/

	friend Matrix Transposed(const Matrix<__T, __Cols, __Rows>& a); /*<
	Returns a transposed copy of the matrix specified as the argument.
	>*/

	friend Matrix Inverse(Matrix m); /*<
	Returns the inverse for the specified matrix [^m].
	>*/

	template <size_t I, size_t J, size_t R, size_t C>
	friend Matrix<T, R, C> Submatrix(const Matrix& a) const; /*<
	Returns a submatrix of the specified matrix with [^R] rows and [^C]
	columns starting from row [^I] and column [^J]. Note, that the following
	must be true: [^__Rows-I>=R] and [^__Cols-J>=C].
	>*/
	friend Matrix<T, 2, 2> Sub2x2(const Matrix& a); /*<
	Returns a 2x2 submatrix of the specified matrix.
	>*/
	friend Matrix<T, 3, 3> Sub3x3(const Matrix& a); /*<
	Returns a 3x3 submatrix of the specified matrix.
>*/
//]
//[oglplus_math_matrix_friend_row_ops
	friend void RowSwap(Matrix& m, size_t a, size_t b); /*<
	Swaps the [^a]th with [^b]th row of the specified matrix.
	>*/
	friend void RowMultiply(Matrix& m, size_t i, __T c); /*<
	Multiplies the [^i]th row of the specified matrix [^m] by the
	constant [^c].
	>*/
	friend void RowAdd(Matrix& m, size_t a, size_t b, __T c); /*<
	Adds the row [^b] multiplied by the constant [^c] to the row [^a],
	of the specified matrix [^m].
	>*/

	template <size_t C>
	friend bool Gauss(Matrix& a, Matrix<__T, __Rows, C>& b); /*<
	Performs [@http://en.wikipedia.org/wiki/Gaussian_elimination
	Gaussian elimination] on the specified pair of matrices.
	Returns [^true] if the elimination was succesfully completed.
	>*/

	template <std::size_t C>
	friend bool GaussJordan(Matrix& a, Matrix<__T, __Rows, C>& b); /*<
	Performs Gauss-Jordan elimination on the specified pair of matrices.
	Returns [^true] if the elimination was succesfully completed.
>*/
//]

//[oglplus_math_matrix_end
}; // class Matrix
//]

//[oglplus_math_matrix_typedefs

#ifdef GL_FLOAT
typedef __Matrix<GLfloat, 2, 2> Mat2f; /*<
Floating-point matrices.
>*/
typedef Matrix<GLfloat, 2, 3> Mat2x3f;
typedef Matrix<GLfloat, 2, 4> Mat2x4f;
typedef Matrix<GLfloat, 3, 2> Mat3x2f;
typedef Matrix<GLfloat, 3, 3> Mat3f;
typedef Matrix<GLfloat, 3, 4> Mat3x4f;
typedef Matrix<GLfloat, 4, 2> Mat4x2f;
typedef Matrix<GLfloat, 4, 3> Mat4x3f;
typedef Matrix<GLfloat, 4, 4> Mat4f;
#endif

#ifdef GL_DOUBLE
typedef __Matrix<GLdouble, 2, 2> Mat2d; /*<
Double-precision floating-point matrices.
>*/
typedef Matrix<GLdouble, 2, 3> Mat2x3d;
typedef Matrix<GLdouble, 2, 4> Mat2x4d;
typedef Matrix<GLdouble, 3, 2> Mat3x2d;
typedef Matrix<GLdouble, 3, 3> Mat3d;
typedef Matrix<GLdouble, 3, 4> Mat3x4d;
typedef Matrix<GLdouble, 4, 2> Mat4x2d;
typedef Matrix<GLdouble, 4, 3> Mat4x3d;
typedef Matrix<GLdouble, 4, 4> Mat4d;
#endif
//]

//[oglplus_math_matrix_model
template <typename T>
class ModelMatrix
 : public __Matrix<T, 4, 4>
{
public:
	ModelMatrix(const __Matrix<T, 4, 4>& base); /*<
	Construction from a 4x4 __Matrix.
	>*/
	ModelMatrix(void); /*<
	Construction of an identity matrix.
	>*/

	static ModelMatrix Translation(T dx, T dy, T dz); /*<
	Functions constructing translation matrices with the specified offsets
	along the ['X], ['Y] and ['Z] axes.
	>*/
	static ModelMatrix Translation(const __Vector<T, 3>& dp);
	static ModelMatrix TranslationX(T dx);
	static ModelMatrix TranslationY(T dy);
	static ModelMatrix TranslationZ(T dz);

	static ModelMatrix Scale(T sx, T sy, T sz); /*<
	Function constructing scale matrix with the specified scale
	factors along the ['X], ['Y] and ['Z] axes.
	>*/

	static ModelMatrix Reflection(bool rx, bool ry, bool rz); /*<
	Function constructing reflection matrix, with [^rx], [^ry] and [^rz]
	specifying whether the ['X], ['Y] and ['Z] axes should be reflected.
	>*/

	static ModelMatrix RotationX(__Angle<T> angle); /*<
	Functions constructing matrices for rotation by the specified [^angle]
	along the ['X], ['Y] and ['Z] axes.
	>*/
	static ModelMatrix RotationY(__Angle<T> angle);
	static ModelMatrix RotationZ(__Angle<T> angle);
	static ModelMatrix RotationA(const __Vector<T, 3>& axis, __Angle<T> angle); /*<
	Function constructing matrix for rotation by the specified [^angle]
	along the specified [^axis].
	>*/

}; // ModelMatrix

#ifdef GL_FLOAT
typedef __ModelMatrix<GLfloat> ModelMatrixf;
#endif
#ifdef GL_DOUBLE
typedef ModelMatrix<GLdouble> ModelMatrixd;
#endif

//]
//[oglplus_math_matrix_camera
template <typename T>
class CameraMatrix
 : public __Matrix<T, 4, 4>
{
public:
	CameraMatrix(const __Matrix<T, 4, 4>& base);

	static CameraMatrix Perspective(
		T x_left,
		T x_right,
		T y_bottom,
		T y_top,
		T z_near,
		T z_far
	); /*<
	Creates a new perspective projection matrix with specified parameters.
	>*/
	static CameraMatrix PerspectiveX(__Angle<T> xfov, T aspect, T z_near, T z_far); /*<
	Creates a new perspective matrix from x-axis [^xfov] angle,
	x/y [^aspect] ratio and z-axis [^z_near] and [^z_far] planes.
	>*/
	static CameraMatrix PerspectiveY(__Angle<T> yfov, T aspect, T z_near, T z_far); /*<
	Creates a new perspective matrix from y-axis [^yfov] angle,
	x/y [^aspect] ratio and z-axis [^z_near] and [^z_far] planes.
	>*/

	static CameraMatrix Ortho(
		T x_left,
		T x_right,
		T y_bottom,
		T y_top,
		T z_near,
		T z_far
	); /*<
	Creates an orthographic projection matrix with the specified parameters.
	>*/
	static CameraMatrix OrthoX(T width, T aspect, T z_near, T z_far); /*<
	Creates a new orthographic matrix from x-axis [^width],
	x/y [^aspect] ratio and z-axis [^z_near] and [^z_far] planes.
	>*/
	static CameraMatrix OrthoY(T height, T aspect, T z_near, T z_far); /*<
	Creates a new orthographic matrix from y-axis [^height],
	x/y [^aspect] ratio and z-axis [^z_near] and [^z_far] planes.
	>*/

	static CameraMatrix ScreenStretch(T x_left, T x_right, T y_bottom, T y_top); /*<
	Constructs a matrix that can be used to stretch
	the normalized device coordinates after projection is applied.
	>*/
	static CameraMatrix ScreenTile(unsigned x, unsigned y, unsigned nx, unsigned ny); /*<
	Constructs a matrix, that divides the screen into
	rectangular tiles with the specified numbers of divisions [^nx] and [^ny]
	and stretches the normalized device coordinates to show the specified tile.
	>*/

	static CameraMatrix LookingAt(
		const __Vector<T, 3>& eye,
		const __Vector<T, 3>& target
	); /*<
	Constructs a matrix for a camera placed at [^eye] and looking
	at [^target] positions.
	>*/

	static CameraMatrix Orbiting(
		const __Vector<T, 3>& target,
		T radius,
		__Angle<T> azimuth,
		__Angle<T> elevation
	); /*<
	Constructs a matrix for a camera orbiting the [^target] position
	at the distance specified by [^radius], with the specified [^azimuth]
	and [^elevation] angles.
	>*/

	static CameraMatrix Pitch(__Angle<T> angle); /*<
	Constructs an X-axis rotation (Pitch/Elevation) matrix.
	The initial heading is the negative Z-axis, ['top] is the Y-axis,
	['right] is the X-axis.
	Positive [^angle] values do counter-clockwise rotation (looking up),
	negative [^angles] cause clockwise changes in pitch (looking down).
	>*/
	static CameraMatrix Yaw(__Angle<T> angle); /*<
	Constructs a Y-axis rotation (Heading/Yaw) matrix.
	The initial heading is the negative Z-axis, ['top] is the Y-axis,
	['right] is the X-axis.
	Positive [^angle] values do counter-clockwise rotation (rotating left),
	negative values cause clockwise changes in heading (rotating right).
	>*/
	static CameraMatrix Roll(__Angle<T> angle); /*<
	Constructs a Z-axis rotation (Bank/Roll) matrix.
	The initial position is that ['top] is the Y-axis,
	heading in the negative Z-axis direction, ['right] is the X-axis.
	Positive [^angle] values do counter-clockwise banking (rolling left),
	negative values do clockwise banking (rolling right).
	>*/

	__Vector<T, 3> Position(void) const; /*<
	The position of and the direction in which the camera represented
	by this matrix points to.
	>*/
	__Vector<T, 3> Direction(void) const;
}; // CameraMatrix

#ifdef GL_FLOAT
typedef __CameraMatrix<GLfloat> CamMatrixf;
#endif
#ifdef GL_DOUBLE
typedef CameraMatrix<GLdouble> CamMatrixd;
#endif

//]

