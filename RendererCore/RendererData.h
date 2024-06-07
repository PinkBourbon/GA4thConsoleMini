#pragma once

#include "Type.h"

namespace renderer
{
	enum class outputScheme
	{
		dot,
		line,
		mesh
	};

	struct AABB
	{
		Vector4 _pos;
		double _x;
		double _y;
		double _z;
	};

	struct BoundingSphere
	{
		Vector4 _pos;
		double _radius;
	};

	struct Vertex
	{
	public:
		Vertex(double x = 0.0, double y = 0.0, double z = 0.0);
		Vertex(const Vertex& vertex);

		Vertex& operator/=(const double& rhs);
	public:
		Vector4 _pos;
		//Vector4 _nomal;
		bool _isDraw = true;
	};

	struct UV
	{
	public:
		UV& operator=(const UV& rhs);
		UV& operator+=(const UV& rhs);
		UV& operator*=(const double rhs);
		UV& operator/=(const double rhs);
		const UV operator+(const UV& rhs);
		const UV operator*(const double rhs);
		const UV operator/(const double rhs);

		double _u;
		double _v;
	};

	struct Face
	{
	public:
		Face(uint32 a = 0.0, uint32 b = 0.0, uint32 c = 0.0, 
			uint32 uvA = 0.0, uint32 uvB = 0.0, uint32 uvC = 0.0);

		Face& operator=(const Face& rhs);
	public:
		uint32 _vertexIndex[3];
		uint32 _uvIndex[3];
		Vector3 _nomal;
		bool _isDraw;
	};

	struct RenderFace
	{
	public:
		RenderFace& operator=(const RenderFace& rhs);
		
	public:
		Vertex _vertex[3];
		double _w[3];
		UV _uv[3];
		Vector3 _nomal;
		bool _isDraw;
		outputScheme _scheme = outputScheme::line;
	};

	struct Mesh
	{
	public:
		Mesh(Vertex* vertexBuffer = nullptr, uint32 vertexBufferSize = 0, Face* indexBuffer = nullptr, uint32 indexBufferSize = 0);
		Mesh(const Mesh& mesh);
		Mesh(const Mesh* mesh);
		~Mesh();

		bool ImportObjFile(const char* fileName);
		void DeleteData();

	private:
		void SetMinMax(double* min, double* max, double Num);

	public:
		Vertex* _vertexBuffer;//stl vector로도
		uint32 _vertexBufferSize;
		UV* _uvBuffer;
		uint32 _uvBufferSize;
		Face* _faceBuffer;
		uint32 _faceBufferSize;

		AABB _aabb;
		BoundingSphere _boundingSphere;
		bool isRender = true;
	};

	struct Position
	{
	public:
		Position(double x = 0.0, double y = 0.0, double z = 0.0);
		void Set(double x, double y, double z);
		Vector3 Get() const;
		void DeltaSet(double x, double y, double z);
		void SetX(double x);
		double GetX();
		void SetY(double y);
		double GetY();
		void SetZ(double z);
		double GetZ();

	private:
		Metrix44 _data;

		friend class Renderer;
	};

	struct Rotation
	{
	public:
		Rotation(double x = 0.0, double y = 0.0, double z = 0.0);
		void Set(double x, double y, double z);
		void DeltaSet(double x, double y, double z);
		double GetX();
		double GetY();
		double GetZ();

	private:
		void SetX(double euler);
		void SetY(double euler);
		void SetZ(double euler);
		double EulerToRadian(double euler);
		double RadianToEuler(double radian);

	private:
		Metrix44 _metrixX;
		double _x;
		Metrix44 _metrixY;
		double _y;
		Metrix44 _metrixZ;
		double _z;
		Metrix44 _metrixR;

		friend class Renderer;
		friend class Camera;
	};

	struct Scale
	{
	public:
		Scale(double x = 1.0, double y = 1.0, double z = 1.0);
		void Set(double x, double y, double z);
		void SetX(double x);
		double GetX();
		void SetY(double y);
		double GetY();
		void SetZ(double z);
		double GetZ();

	private:
		Metrix44 _data;

		friend class Renderer;
	};

	struct Transform
	{
		Position _position;
		Rotation _rotation;
		Scale _scale;
		Metrix44 _transformationMatrix;
		Metrix44 _normalTransformationMatrix;
	};

	struct Object
	{
	public:
		~Object();
		bool ImportObjFileToMesh(const char* fileName);
		void DeleteMesh();

	public:
		Transform _transform;
		Mesh* _mesh;
	};

	struct RenderableObject
	{
	public:
		RenderableObject(Object& object);
		RenderableObject(Object* object);

	public:
		outputScheme _scheme = outputScheme::line;
		Object* _object;
	};

	//바라보는 방향은 내 로컬 Z축의 역방향
	class Camera
	{
	public:
		Camera(double aspect = (198.0 / 108.0), double fovY = 0.5, double n = 5.0, double f = 200.0);

		Vector3 GetEye() const;
		Vector3 GetU() const;
		Vector3 GetV() const;
		Vector3 GetN() const;

	public:
		Transform _transform;

		double _fovY;
		double _aspect;
		double _n;
		double _f;
	};

	//class Light
	//{
	//public:
	//	Light();
	//public:
	//	Transform _transform;
	//	Vector3 _direction;
	//	double _lux;
	//	RGB _color;

	//	friend class Renderer;
	//};

	struct Pixel
	{
		RGB _rgb;
		double _lightIntensity = 0;
		Vector3 _nomal;
		double _z;
	};
}

