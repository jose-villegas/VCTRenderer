/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_math_angle

template <typename T>
class Angle
{
public:
//]
//[oglplus_math_angle_ctrs
	Angle(void); /*<
	Constructs a zero angle.
	>*/
	Angle(const Angle&);

	template <typename U>
	Angle(const Angle<U>& other); /*<
	Constructs an angle from angle with different underlying type.
>*/
//]

//[oglplus_math_angle_ctr_fns
	static Angle Radians(__T val_rad); /*<
	Constructs a new angle from a value in radians.
	>*/
	static Angle Degrees(__T val_deg); /*<
	Constructs a new angle from a value in degrees.
	>*/
	static Angle ArcSin(__T x); /*<
	Constructs a new angle as the arc sine of the specified value.
	>*/
	static Angle ArcCos(__T x);  /*<
	Constructs a new angle as the arc sine of the specified value.
>*/
//]
//[oglplus_math_angle_value
	__T Value(void) const; /*<
	Returns the value of the angle in radians.
	>*/
	__T ValueInDegrees(void) const; /*<
	Returns the value of the angle in degrees.
	>*/
	__T ValueInRightAngles(void) const; /*<
	Returns the value of the angle in number of right angles.
	>*/
	__T ValueInFullCircles(void) const; /*<
	Returns the value of the angle in number of full circles.
>*/
//]
//[oglplus_math_angle_compare
	friend bool operator == (const Angle& a, const Angle& b); /*<
	Friend comparison operators.
>*/
	friend bool operator != (const Angle& a, const Angle& b);
	friend bool operator <= (const Angle& a, const Angle& b);
	friend bool operator <  (const Angle& a, const Angle& b);
	friend bool operator >= (const Angle& a, const Angle& b);
	friend bool operator >  (const Angle& a, const Angle& b);
//]
//[oglplus_math_angle_ops
	Angle& operator += (const Angle& a); /*<
	Adds angle [^a] to [^this] angle.
	>*/
	Angle& operator -= (const Angle& a); /*<
	Subtracts angle [^a] to [^this] angle.
	>*/
	Angle& operator *= (__T c); /*<
	Multiplies [^this] angle by a constant.
	>*/
	Angle& operator /= (__T c); /*<
	Divides [^this] angle by a constant.
>*/
//]
//[oglplus_math_angle_friend_ops
	friend Angle operator - (const Angle& a); /*< Negation.  >*/
	friend Angle operator + (const Angle& a, const Angle& b); /*< Addition. >*/
	friend Angle operator - (const Angle& a, const Angle& b); /*< Subtraction. >*/
	friend Angle operator * (const Angle& a, __T c); /*<
	Multiplication by a constant.
	>*/
	friend Angle operator * (__T c, const Angle& a);
	friend Angle operator / (const Angle& a, __T c); /*<
	Division by a constant.
	>*/
	friend __T operator / (const Angle& a, const Angle& b); /*<
	Ratio of two angles.
>*/
//]
//[oglplus_math_angle_trig_fns
	__T Sin(void) const; /*<
	Member trigonometric functions.
	>*/
	__T Cos(void) const;
	__T Tan(void) const;

	friend inline __T Sin(const Angle& a); /*<
	Friend trigonometric functions.
>*/
	friend inline __T Cos(const Angle& a);
	friend inline __T Tan(const Angle& a);
//]
//[oglplus_math_angle_fns
	Angle& operator Add(const Angle& a); /*<
	Adds [^a] to [^this].
	>*/
	Angle& operator Subtract(const Angle& a); /*<
	Subtracts [^a] from [^this].
	>*/
	Angle& operator Multiply(__T c); /*<
	Multiplies [^this] by [^c].
	>*/
	Angle& operator Divide(__T c); /*<
	Divides [^this] by [^c].
>*/
//]
//[oglplus_math_angle_friend_fns
	friend Angle Negated(const Angle& a);
	friend Angle Added(const Angle& a, const Angle& b);
	friend Angle Subtracted(const Angle& a, const Angle& b);
	friend Angle Multiplied(const Angle& a, __T c);
	friend Angle Divided(const Angle& a, __T c);
	friend __T Ratio(const Angle& a, const Angle& b); /*<
	Ratio of [^a] to [^b].
>*/
//]
//[oglplus_math_angle_end
}; // class Angle
//]
//[oglplus_math_angle_ext_defs
#ifdef GL_FLOAT
typedef __Angle<GLfloat> Anglef;

__Angle<GLfloat> Radians(GLfloat value); /*<
Constructs a new angle from a value in radians.
>*/
Angle<GLfloat> Degrees(GLfloat value); /*<
Constructs a new angle from a value in degrees.
>*/
Angle<GLfloat> FullCircles(GLfloat value); /*<
Constructs a new angle from a number of full turns.
>*/
Angle<GLfloat> FullCircle(void); /*<
Constructs a new angle with a value of [^__two_pi__] radians (360 degrees).
>*/
Angle<GLfloat> RightAngles(GLfloat value);
Angle<GLfloat> RightAngle(void); /*<
Constructs a new angle with a value of [^__half_pi__] radians (90 degrees).
>*/

Angle<GLfloat> ArcSin(GLfloat x); /*<
Constructs a new angle as the arc-sine of the specified value.
>*/
Angle<GLfloat> ArcCos(GLfloat x); /*<
Constructs a new angle as the arc-cosine of the specified value.
>*/
Angle<GLfloat> ArcTan(GLfloat x); /*<
Constructs a new angle as the arc-tangent of the specified value.
>*/

#endif // GL_FLOAT

template <typename T>
T SineWave(T t); /*<
Returns a value on a sine wave at the specified point.
This function returns the value of [^sin(__two_pi__ * t)], i.e.
integer values of [^t] are the ends of the previous full
sine wave and the begining of the next one.
>*/
template <typename T>
T SineWave01(T t); /*<
Returns a value on a sine wave transformed to range <0, 1>
This function returns the value of [^(sin(__two_pi__ * t)+1)/2], i.e.
integer values of [^t] are the ends of the previous full
sine wave and the begining of the next one.
>*/

template <typename T>
T CosineWave(T t); /*<
Returns a value on a cosine wave at the specified point.
This function returns the value of [^cos(__two_pi__ * t)], i.e.
integer values of [^t] are the ends of the previous full
cosine wave and the begining of the next one.
>*/
template <typename T>
T CosineWave01(T t); /*<
Returns a value on a cosine wave transformed to range <0, 1>
This function returns the value of [^(cos(__two_pi__ * t)+1)/2], i.e.
integer values of [^t] are the ends of the previous full
cosine wave and the begining of the next one.
>*/

//]

