#include "pch.h"
#include "Type.h"
#include "Const.h"

_Check_return_ _CRT_JIT_INTRINSIC double __cdecl sqrt(_In_ double _X);

namespace renderer
{
	double Pow(double base, unsigned int exp)
	{
		double ret = 1;
		while (exp > 1)
		{
			if (exp & 1)
			{
				ret *= base;
			}
			base *= base;
			exp = exp >> 1;
		}
		return ret * base;
	}

	double Sin(double rad)
	{
		return sin(rad);
		//double 의 mod 연산
		rad -= (int64)(rad / (pi * 2)) * (pi * 2);

		double ret = rad;
		int64 divid = 1;
		double sign = 1;
		double pow = rad;

		for (int i = 1; i < 20; ++i)
		{
			divid *= (i * 2) * ((i * 2) + 1);
			sign *= -1;
			pow *= rad * rad;
			ret += (sign * pow) / divid;
		}
		return ret;
	}

	double Cos(double rad)
	{
		return cos(rad);
		//double 의 mod 연산
		rad -= (int64)(rad / (pi * 2) * (pi * 2));

		double ret = 1;
		int64 divid = 1;
		double sign = 1;
		double pow = 1;

		for (int i = 1; i < 20; ++i)
		{
			divid *= (i * 2) * ((i * 2) - 1);
			sign *= -1;
			pow *= rad * rad;
			ret += (sign * pow) / divid;
		}
		return ret;
	}

	double Cot(double rad)
	{
		return Cos(rad) / Sin(rad);
	}

	__int64 Round(double d)
	{
		__int64 sign = 1;
		if (d < 0)
			sign = -1;
		__int64 ret = d * 2 + sign;
		ret /= 2;
		return ret;
	}

	/*-------------------------------
				Vector3
	-------------------------------*/
	Vector3::Vector3(double x /*= 0.0*/, double y /*= 0.0*/, double z /*= 0.0*/)
		:_x(x), _y(y), _z(z)
	{

	}

	Vector3::Vector3(const Vector3& vector)
		: _x(vector._x), _y(vector._y), _z(vector._z)
	{

	}

	Vector3& Vector3::operator=(const Vector3& rhs)
	{
		this->_x = rhs._x;
		this->_y = rhs._y;
		this->_z = rhs._z;
		return *this;
	}

	Vector3& Vector3::operator+=(const Vector3& rhs)
	{
		this->_x += rhs._x;
		this->_y += rhs._y;
		this->_z += rhs._z;
		return *this;
	}

	Vector3& Vector3::operator-=(const Vector3& rhs)
	{
		this->_x -= rhs._x;
		this->_y -= rhs._y;
		this->_z -= rhs._z;
		return *this;
	}

	Vector3& Vector3::operator*=(const double& scaler)
	{
		this->_x *= scaler;
		this->_y *= scaler;
		this->_z *= scaler;
		return *this;
	}

	Vector3& Vector3::operator/=(const double& scaler)
	{
		this->_x /= scaler;
		this->_y /= scaler;
		this->_z /= scaler;
		return *this;
	}

	const Vector3 Vector3::operator+(const Vector3& rhs) const
	{
		return Vector3(*this) += rhs;
	}

	const Vector3 Vector3::operator-(const Vector3& rhs) const
	{
		return Vector3(*this) -= rhs;
	}

	const Vector3 Vector3::operator*(const double& scaler) const
	{
		return Vector3(*this) *= scaler;
	}

	const Vector3 Vector3::operator/(const double& scaler) const
	{
		return Vector3(*this) /= scaler;
	}

	Vector3& Vector3::Normalization()
	{
		double d = (_x * _x) + (_y * _y) + (_z * _z);
		d = sqrt(d);
		this->_x /= d;
		this->_y /= d;
		this->_z /= d;

		return *this;
	}

	double Vector3::Norm()
	{
		double d = (_x * _x) + (_y * _y) + (_z * _z);
		d = sqrt(d);
		return d;
	}


	double Vector3::FastInverseNorm()
	{
		double d;
		d = (_x * _x) + (_y * _y) + (_z * _z);
		int64 i = *(int64*)&d;

		double dHalf = d * 0.5;
		constexpr double threehalf = 1.5;
		constexpr int64 magic = 0x5fe6eb50c7b537a9;

		i = magic - (i >> 1);
		d = *(double*)&i;

		d = d * (threehalf - (dHalf * d * d));
		d = d * (threehalf - (dHalf * d * d));

		return d;
	}

	double Dot(const Vector3& lhs, const Vector3& rhs)
	{
		return ((lhs._x * rhs._x) + (lhs._y * rhs._y) + (lhs._z * rhs._z));
	}

	const Vector3 Cross(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3
		(
			(lhs._y * rhs._z - lhs._z * rhs._y),
			(lhs._z * rhs._x - lhs._x * rhs._z),
			(lhs._x * rhs._y - lhs._y * rhs._x)
		);
	}



	/*-------------------------------
				Vector4
	-------------------------------*/

	Vector4::Vector4(double x /*= 0.0*/, double y /*= 0.0*/, double z /*= 0.0*/, double w /*= 1.0*/)
		: _x(x), _y(y), _z(z), _w(w)
	{

	}

	Vector4::Vector4(const Vector4& vector)
		:_x(vector._x), _y(vector._y), _z(vector._z), _w(vector._w)
	{

	}

	Vector4& Vector4::operator=(const Vector3& rhs)
	{
		this->_x = rhs._x;
		this->_y = rhs._y;
		this->_z = rhs._z;
		this->_w = 1;
		return *this;
	}

	Vector4& Vector4::operator=(const Vector4& rhs)
	{
		this->_x = rhs._x;
		this->_y = rhs._y;
		this->_z = rhs._z;
		this->_w = rhs._w;
		return *this;
	}

	Vector4& Vector4::operator+=(const Vector4& rhs)
	{
		this->_x += rhs._x;
		this->_y += rhs._y;
		this->_z += rhs._z;
		this->_w += rhs._w;
		return *this;
	}

	Vector4& Vector4::operator-=(const Vector4& rhs)
	{
		this->_x -= rhs._x;
		this->_y -= rhs._y;
		this->_z -= rhs._z;
		this->_w -= rhs._w;
		return *this;
	}

	Vector4& Vector4::operator*=(const double& scaler)
	{
		this->_x *= scaler;
		this->_y *= scaler;
		this->_z *= scaler;
		this->_w *= scaler;
		return *this;
	}

	Vector4& Vector4::operator/=(const double& scaler)
	{
		this->_x /= scaler;
		this->_y /= scaler;
		this->_z /= scaler;
		this->_w /= scaler;
		return *this;
	}

	Vector4::operator Vector3()
	{
		return Vector3(this->_x, this->_y, this->_z);
	}

	const Vector4 Vector4::operator+(const Vector4& rhs) const
	{
		return Vector4(*this) += rhs;
	}

	const Vector4 Vector4::operator-(const Vector4& rhs) const
	{
		return Vector4(*this) -= rhs;
	}

	const Vector4 Vector4::operator*(const double& scaler) const
	{
		return Vector4(*this) *= scaler;
	}

	const Vector4 Vector4::operator/(const double& scaler) const
	{
		return Vector4(*this) /= scaler;
	}

	Vector3 Cross(const Vector4& lhs, const Vector4& rhs)
	{
		return Vector3
		(
			(lhs._y * rhs._z - lhs._z * rhs._y),
			(lhs._z * rhs._x - lhs._x * rhs._z),
			(lhs._x * rhs._y - lhs._y * rhs._x)
		);
	}
	/*-------------------------------
				Metrix
	-------------------------------*/

	Metrix33::Metrix33()
	{
		memset(this->_element, 0, sizeof(_element));
	}

	Metrix33::Metrix33(double d0, double d1, double d2, double d3, double d4, double d5, double d6, double d7, double d8)
	{
		_element[0] = d0;
		_element[1] = d1;
		_element[2] = d2;
		_element[3] = d3;
		_element[4] = d4;
		_element[5] = d5;
		_element[6] = d6;
		_element[7] = d7;
	}

	double Metrix33::Determinant()
	{
		return
			(
				(_element[0] * _element[4] * _element[8])
				+ (_element[1] * _element[5] * _element[6])
				+ (_element[2] * _element[3] * _element[7])
				- (_element[2] * _element[4] * _element[6])
				- (_element[1] * _element[3] * _element[8])
				- (_element[0] * _element[5] * _element[7])
			);
	}


	Metrix44::Metrix44()
	{
		memset(this->_element, 0, sizeof(_element));
	}

	Metrix44::Metrix44(
		double d0, double d1, double d2, double d3,
		double d4, double d5, double d6, double d7,
		double d8, double d9, double d10, double d11,
		double d12, double d13, double d14, double d15)
	{
		_element[0] = d0;
		_element[1] = d1;
		_element[2] = d2;
		_element[3] = d3;
		_element[4] = d4;
		_element[5] = d5;
		_element[6] = d6;
		_element[7] = d7;
		_element[8] = d8;
		_element[9] = d9;
		_element[10] = d10;
		_element[11] = d11;
		_element[12] = d12;
		_element[13] = d13;
		_element[14] = d14;
		_element[15] = d15;
	}

	Metrix44::Metrix44(double(&doubleArr)[16])
	{
		for (int i = 0; i < 16; ++i)
		{
			_element[i] = doubleArr[i];
		}
	}

	Metrix44::Metrix44(const Metrix44& metrix)
	{
		memcpy(this->_element, metrix._element, sizeof(_element));
	}

	const Metrix44& Metrix44::operator=(const Metrix44& rhs)
	{
		this->_element[0] = rhs._element[0];
		this->_element[1] = rhs._element[1];
		this->_element[2] = rhs._element[2];
		this->_element[3] = rhs._element[3];
		this->_element[4] = rhs._element[4];
		this->_element[5] = rhs._element[5];
		this->_element[6] = rhs._element[6];
		this->_element[7] = rhs._element[7];
		this->_element[8] = rhs._element[8];
		this->_element[9] = rhs._element[9];
		this->_element[10] = rhs._element[10];
		this->_element[11] = rhs._element[11];
		this->_element[12] = rhs._element[12];
		this->_element[13] = rhs._element[13];
		this->_element[14] = rhs._element[14];
		this->_element[15] = rhs._element[15];
		return *this;
	}

	const Metrix44& Metrix44::operator+=(const Metrix44& rhs)
	{
		this->_element[0] += rhs._element[0];
		this->_element[1] += rhs._element[1];
		this->_element[2] += rhs._element[2];
		this->_element[3] += rhs._element[3];
		this->_element[4] += rhs._element[4];
		this->_element[5] += rhs._element[5];
		this->_element[6] += rhs._element[6];
		this->_element[7] += rhs._element[7];
		this->_element[8] += rhs._element[8];
		this->_element[9] += rhs._element[9];
		this->_element[10] += rhs._element[10];
		this->_element[11] += rhs._element[11];
		this->_element[12] += rhs._element[12];
		this->_element[13] += rhs._element[13];
		this->_element[14] += rhs._element[14];
		this->_element[15] += rhs._element[15];
		return *this;
	}

	const Metrix44& Metrix44::operator-=(const Metrix44& rhs)
	{
		this->_element[0] -= rhs._element[0];
		this->_element[1] -= rhs._element[1];
		this->_element[2] -= rhs._element[2];
		this->_element[3] -= rhs._element[3];
		this->_element[4] -= rhs._element[4];
		this->_element[5] -= rhs._element[5];
		this->_element[6] -= rhs._element[6];
		this->_element[7] -= rhs._element[7];
		this->_element[8] -= rhs._element[8];
		this->_element[9] -= rhs._element[9];
		this->_element[10] -= rhs._element[10];
		this->_element[11] -= rhs._element[11];
		this->_element[12] -= rhs._element[12];
		this->_element[13] -= rhs._element[13];
		this->_element[14] -= rhs._element[14];
		this->_element[15] -= rhs._element[15];
		return *this;
	}

	const Metrix44& Metrix44::operator*=(const Metrix44& rhs)
	{
		double element[16];
		element[0] = (this->_element[0] * rhs._element[0]) + (this->_element[1] * rhs._element[4]) + (this->_element[2] * rhs._element[8]) + (this->_element[3] * rhs._element[12]);
		element[1] = (this->_element[0] * rhs._element[1]) + (this->_element[1] * rhs._element[5]) + (this->_element[2] * rhs._element[9]) + (this->_element[3] * rhs._element[13]);
		element[2] = (this->_element[0] * rhs._element[2]) + (this->_element[1] * rhs._element[6]) + (this->_element[2] * rhs._element[10]) + (this->_element[3] * rhs._element[14]);
		element[3] = (this->_element[0] * rhs._element[3]) + (this->_element[1] * rhs._element[7]) + (this->_element[2] * rhs._element[11]) + (this->_element[3] * rhs._element[15]);
		element[4] = (this->_element[4] * rhs._element[0]) + (this->_element[5] * rhs._element[4]) + (this->_element[6] * rhs._element[8]) + (this->_element[7] * rhs._element[12]);
		element[5] = (this->_element[4] * rhs._element[1]) + (this->_element[5] * rhs._element[5]) + (this->_element[6] * rhs._element[9]) + (this->_element[7] * rhs._element[13]);
		element[6] = (this->_element[4] * rhs._element[2]) + (this->_element[5] * rhs._element[6]) + (this->_element[6] * rhs._element[10]) + (this->_element[7] * rhs._element[14]);
		element[7] = (this->_element[4] * rhs._element[3]) + (this->_element[5] * rhs._element[7]) + (this->_element[6] * rhs._element[11]) + (this->_element[7] * rhs._element[15]);
		element[8] = (this->_element[8] * rhs._element[0]) + (this->_element[9] * rhs._element[4]) + (this->_element[10] * rhs._element[8]) + (this->_element[11] * rhs._element[12]);
		element[9] = (this->_element[8] * rhs._element[1]) + (this->_element[9] * rhs._element[5]) + (this->_element[10] * rhs._element[9]) + (this->_element[11] * rhs._element[13]);
		element[10] = (this->_element[8] * rhs._element[2]) + (this->_element[9] * rhs._element[6]) + (this->_element[10] * rhs._element[10]) + (this->_element[11] * rhs._element[14]);
		element[11] = (this->_element[8] * rhs._element[3]) + (this->_element[9] * rhs._element[7]) + (this->_element[10] * rhs._element[11]) + (this->_element[11] * rhs._element[15]);
		element[12] = (this->_element[12] * rhs._element[0]) + (this->_element[13] * rhs._element[4]) + (this->_element[14] * rhs._element[8]) + (this->_element[15] * rhs._element[12]);
		element[13] = (this->_element[12] * rhs._element[1]) + (this->_element[13] * rhs._element[5]) + (this->_element[14] * rhs._element[9]) + (this->_element[15] * rhs._element[13]);
		element[14] = (this->_element[12] * rhs._element[2]) + (this->_element[13] * rhs._element[6]) + (this->_element[14] * rhs._element[10]) + (this->_element[15] * rhs._element[14]);
		element[15] = (this->_element[12] * rhs._element[3]) + (this->_element[13] * rhs._element[7]) + (this->_element[14] * rhs._element[11]) + (this->_element[15] * rhs._element[15]);

		this->_element[0] = element[0];
		this->_element[1] = element[1];
		this->_element[2] = element[2];
		this->_element[3] = element[3];
		this->_element[4] = element[4];
		this->_element[5] = element[5];
		this->_element[6] = element[6];
		this->_element[7] = element[7];
		this->_element[8] = element[8];
		this->_element[9] = element[9];
		this->_element[10] = element[10];
		this->_element[11] = element[11];
		this->_element[12] = element[12];
		this->_element[13] = element[13];
		this->_element[14] = element[14];
		this->_element[15] = element[15];

		return *this;
	}

	const Metrix44& Metrix44::operator*=(const double& scaler)
	{
		this->_element[0] *= scaler;
		this->_element[1] *= scaler;
		this->_element[2] *= scaler;
		this->_element[3] *= scaler;
		this->_element[4] *= scaler;
		this->_element[5] *= scaler;
		this->_element[6] *= scaler;
		this->_element[7] *= scaler;
		this->_element[8] *= scaler;
		this->_element[9] *= scaler;
		this->_element[10] *= scaler;
		this->_element[11] *= scaler;
		this->_element[12] *= scaler;
		this->_element[13] *= scaler;
		this->_element[14] *= scaler;
		this->_element[15] *= scaler;
		return *this;
	}

	const Metrix44& Metrix44::operator/=(const double& scaler)
	{
		this->_element[0] /= scaler;
		this->_element[1] /= scaler;
		this->_element[2] /= scaler;
		this->_element[3] /= scaler;
		this->_element[4] /= scaler;
		this->_element[5] /= scaler;
		this->_element[6] /= scaler;
		this->_element[7] /= scaler;
		this->_element[8] /= scaler;
		this->_element[9] /= scaler;
		this->_element[10] /= scaler;
		this->_element[11] /= scaler;
		this->_element[12] /= scaler;
		this->_element[13] /= scaler;
		this->_element[14] /= scaler;
		this->_element[15] /= scaler;
		return *this;
	}



	const Metrix44 Metrix44::operator+(const Metrix44& rhs) const
	{
		return Metrix44(*this) += rhs;
	}

	const Metrix44 Metrix44::operator-(const Metrix44& rhs) const
	{
		return Metrix44(*this) -= rhs;
	}

	const Metrix44 Metrix44::operator*(const Metrix44& rhs) const
	{
		return Metrix44(*this) *= rhs;
	}

	const Vector3 Metrix44::operator*(const Vector3& vector) const
	{
		Vector3 returnVector;
		returnVector._x = (this->_element[0] * vector._x) + (this->_element[1] * vector._y) + (this->_element[2] * vector._z) + (this->_element[3] * 1);
		returnVector._y = (this->_element[4] * vector._x) + (this->_element[5] * vector._y) + (this->_element[6] * vector._z) + (this->_element[7] * 1);
		returnVector._z = (this->_element[8] * vector._x) + (this->_element[9] * vector._y) + (this->_element[10] * vector._z) + (this->_element[11] * 1);

		return returnVector;
	}

	const Vector4 Metrix44::operator*(const Vector4& vector) const
	{
		Vector4 returnVector;
		returnVector._x = (this->_element[0] * vector._x) + (this->_element[1] * vector._y) + (this->_element[2] * vector._z) + (this->_element[3] * vector._w);
		returnVector._y = (this->_element[4] * vector._x) + (this->_element[5] * vector._y) + (this->_element[6] * vector._z) + (this->_element[7] * vector._w);
		returnVector._z = (this->_element[8] * vector._x) + (this->_element[9] * vector._y) + (this->_element[10] * vector._z) + (this->_element[11] * vector._w);
		returnVector._w = (this->_element[12] * vector._x) + (this->_element[13] * vector._y) + (this->_element[14] * vector._z) + (this->_element[15] * vector._w);

		return returnVector;
	}

	const Metrix44 Metrix44::operator*(const double& scaler) const
	{
		return Metrix44(*this) *= scaler;
	}

	const Metrix44 Metrix44::operator/(const double& scaler) const
	{
		return Metrix44(*this) /= scaler;
	}

	double Metrix44::Determinant() const
	{
		double det =
			(_element[3] * _element[6] * _element[9] * _element[12])
			- (_element[2] * _element[7] * _element[9] * _element[12])
			- (_element[3] * _element[5] * _element[10] * _element[12])
			+ (_element[1] * _element[7] * _element[10] * _element[12])
			+ (_element[2] * _element[5] * _element[11] * _element[12])
			- (_element[1] * _element[6] * _element[11] * _element[12])
			- (_element[3] * _element[6] * _element[8] * _element[13])
			+ (_element[2] * _element[7] * _element[8] * _element[13])
			+ (_element[3] * _element[4] * _element[10] * _element[13])
			- (_element[0] * _element[7] * _element[10] * _element[13])
			- (_element[2] * _element[4] * _element[11] * _element[13])
			+ (_element[0] * _element[6] * _element[11] * _element[13])
			+ (_element[3] * _element[5] * _element[8] * _element[14])
			- (_element[1] * _element[7] * _element[8] * _element[14])
			- (_element[3] * _element[4] * _element[9] * _element[14])
			+ (_element[0] * _element[7] * _element[9] * _element[14])
			+ (_element[1] * _element[4] * _element[11] * _element[14])
			- (_element[0] * _element[5] * _element[11] * _element[14])
			- (_element[2] * _element[5] * _element[8] * _element[15])
			+ (_element[1] * _element[6] * _element[8] * _element[15])
			+ (_element[2] * _element[4] * _element[9] * _element[15])
			- (_element[0] * _element[6] * _element[9] * _element[15])
			- (_element[1] * _element[4] * _element[10] * _element[15])
			+ (_element[0] * _element[5] * _element[10] * _element[15]);

		return det;
	}

	const Metrix44 Metrix44::Transpose() const
	{
		return Metrix44
		(
			_element[0], _element[4], _element[8], _element[12],
			_element[1], _element[5], _element[9], _element[13],
			_element[2], _element[6], _element[10], _element[14],
			_element[3], _element[7], _element[11], _element[15]
		);
	}

	const Metrix44 Metrix44::Inverse() const
	{
		double det = this->Determinant();
		Metrix44 metrix;
		if (det == 0)
		{
			return metrix;
		}

		int startSign = 1;
		for (int i = 0; i < 4; ++i)
		{
			int sign = startSign;
			for (int j = 0; j < 4; ++j)
			{
				int k = 0;
				int row = 0;
				int col = 0;
				Metrix33 metrix33;
				while (k < 9)
				{
					if (row == i)
					{
						++row;
						continue;
					}
					if (col == j)
					{
						++col;
						if (col > 3)
						{
							++row;
							col = 0;
						}
						continue;
					}
					metrix33._element[k] = _element[row * 4 + col];
					++k;
					++col;
					if (col > 3)
					{
						++row;
						col = 0;
					}
				}
				double det3x3 = metrix33.Determinant();
				metrix._element[i * 4 + j] = sign * det3x3;

				sign *= -1;
			}
			startSign *= -1;
		}
		metrix = metrix.Transpose();

		metrix /= det;

		return metrix;
	}

	const Metrix44 Metrix44::InverseGauss() const
	{


		Metrix44 unit
		(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
		Metrix44 temp(*this);

		for (int i = 0; i < 4; i++)
		{
			double t = temp._element[i * 4 + i];

			for (int j = i; j < 4; j++)
			{
				temp._element[i * 4 + j] /= t;
				unit._element[i * 4 + j] /= t;
			}
			for (int j = 0; j < 4; j++)
			{
				if (i == j)
				{
					continue;
				}

				t = temp._element[j * 4 + i];
				for (int k = 0; k < 4; k++)
				{
					temp._element[j * 4 + k] = temp._element[j * 4 + k] - t * temp._element[i * 4 + k];
					unit._element[j * 4 + k] = unit._element[j * 4 + k] - t * unit._element[i * 4 + k];
				}
			}
		}

		return unit;
	}
}