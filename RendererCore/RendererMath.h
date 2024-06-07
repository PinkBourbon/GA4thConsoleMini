#pragma once
#ifndef RENDERERMATH_H
#define RENDERERMATH_H

struct Vector3;
struct Vector4;
struct Metrix44;


namespace renderer
{
	
	//inline double Mod(double divided, double by) { return (divided - (int64)(divided / (by * 2)) * (by * 2)); }
	double Pow(double base, unsigned int exp);
	double Sin(double rad);
	double Cos(double rad);
	double Cot(double rad);
	__int64 Round(double d);

	/*-------------------------------
				Vector3
	-------------------------------*/

	struct Vector3
	{
	public:
		Vector3(double x = 0.0, double y = 0.0, double z = 0.0);
		Vector3(const Vector3& vector);
		Vector3& operator=(const Vector3& rhs);
		Vector3& operator+=(const Vector3& rhs);
		Vector3& operator-=(const Vector3& rhs);
		Vector3& operator*=(const double& scaler);
		Vector3& operator/=(const double& scaler);
		const Vector3 operator+(const Vector3& rhs) const;
		const Vector3 operator-(const Vector3& rhs) const;
		const Vector3 operator*(const double& scaler) const;
		const Vector3 operator/(const double& scaler) const;
		Vector3& Normalization();
		double Norm();

	private:
		double FastInverseNorm();


	public:
		double _x;
		double _y;
		double _z;
	};
	
	double Dot(const Vector3& lhs, const Vector3& rhs);
	const Vector3 Cross(const Vector3& lhs, const Vector3& rhs);

	/*-------------------------------
				Vector4
	-------------------------------*/
	struct Vector4
	{
	public:
		Vector4(double x = 0.0, double y = 0.0, double z = 0.0, double w = 1.0);
		Vector4(const Vector4& vector);
		Vector4& operator=(const Vector4& rhs);
		Vector4& operator=(const Vector3& rhs);
		Vector4& operator+=(const Vector4& rhs);
		Vector4& operator-=(const Vector4& rhs);
		Vector4& operator*=(const double& scaler);
		Vector4& operator/=(const double& scaler);
		const Vector4 operator+(const Vector4& rhs) const;
		const Vector4 operator-(const Vector4& rhs) const;
		const Vector4 operator*(const double& scaler) const;
		const Vector4 operator/(const double& scaler) const;
		operator Vector3();

	public:
		double _x;
		double _y;
		double _z;
		double _w;
	};
	/*-------------------------------
				Metrix
	-------------------------------*/

	struct Metrix33
	{
	public:
		Metrix33();
		Metrix33
		(
			double d0, double d1, double d2,
			double d3, double d4, double d5,
			double d6, double d7, double d8
		);
		double Determinant();
	public:
		double _element[9];
	};

	struct Metrix44
	{
	public:
		Metrix44();
		Metrix44
		(
			double d0, double d1, double d2, double d3, 
			double d4, double d5, double d6, double d7, 
			double d8, double d9, double d10, double d11, 
			double d12, double d13, double d14, double d15
		);
		Metrix44(double(&doubleArr)[16]);
		Metrix44(const Metrix44& metrix);
		const Metrix44& operator=(const Metrix44& rhs);
		const Metrix44& operator+=(const Metrix44& rhs);
		const Metrix44& operator-=(const Metrix44& rhs);
		const Metrix44& operator*=(const Metrix44& rhs);
		const Metrix44& operator*=(const double& scaler);
		const Metrix44& operator/=(const double& scaler);
		const Metrix44 operator+(const Metrix44& rhs) const;
		const Metrix44 operator-(const Metrix44& rhs) const;
		const Metrix44 operator*(const Metrix44& rhs) const;
		const Vector3 operator*(const Vector3& vector) const;
		const Vector4 operator*(const Vector4& vector) const;
		const Metrix44 operator*(const double& scaler) const;
		const Metrix44 operator/(const double& scaler) const;

		double Determinant() const;
		const Metrix44 Transpose() const;
		const Metrix44 Inverse() const;
		const Metrix44 InverseGauss() const;

	public:
		double _element[16];
	};

	Vector3 Cross(const Vector4& lhs, const Vector4& rhs);
}

#endif // RENDERERMATH_H