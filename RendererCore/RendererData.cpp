#include "pch.h"
#include "RendererData.h"
#include <fstream>
#include <sstream>

namespace renderer
{
	/*-------------------------------
			 	 Vertex
	------------------------------*/
	Vertex::Vertex(double x/* = 0.0*/, double y/* = 0.0*/, double z/* = 0.0*/)
		: _pos(x, y, z), _isDraw(true)
	{

	}

	Vertex::Vertex(const Vertex& vertex)
		: _pos(vertex._pos), _isDraw(vertex._isDraw)
	{

	}

	Vertex& Vertex::operator/=(const double& rhs)
	{
		_pos /= rhs;
		return *this;
	}

	/*-------------------------------
			   VertexIndex
	------------------------------*/
	Face::Face(uint32 a /*= 0.0*/, uint32 b /*= 0.0*/, uint32 c /*= 0.0*/, 
		uint32 uvA /*= 0.0*/, uint32 uvB /*= 0.0*/, uint32 uvC /*= 0.0*/)
	{
		_vertexIndex[0] = a;
		_vertexIndex[1] = b;
		_vertexIndex[2] = c;

		_uvIndex[0] = uvA;
		_uvIndex[0] = uvB;
		_uvIndex[0] = uvC;

		_isDraw = true;
	}

	Face& Face::operator=(const Face& rhs)
	{
		_vertexIndex[0] = rhs._vertexIndex[0];
		_vertexIndex[1] = rhs._vertexIndex[1];
		_vertexIndex[2] = rhs._vertexIndex[2];

		_uvIndex[0] = rhs._uvIndex[0];
		_uvIndex[1] = rhs._uvIndex[1];
		_uvIndex[2] = rhs._uvIndex[2];

		return *this;
	}

	/*-------------------------------
			      Mesh
	------------------------------*/
	Mesh::Mesh(Vertex* vertexBuffer /*= nullptr*/, uint32 vertexBufferSize /*= 0*/, Face* indexBuffer /*= nullptr*/, uint32 indexBufferSize /*= 0*/)
		: _vertexBuffer(vertexBuffer), _vertexBufferSize(vertexBufferSize), _faceBuffer(indexBuffer), _faceBufferSize(indexBufferSize) 
	{

	}

	Mesh::Mesh(const Mesh& mesh)
		: _vertexBuffer(mesh._vertexBuffer), _vertexBufferSize(mesh._vertexBufferSize), _faceBuffer(mesh._faceBuffer), _faceBufferSize(mesh._faceBufferSize)
	{

	}

	Mesh::Mesh(const Mesh* mesh)
		: _vertexBuffer(mesh->_vertexBuffer), _vertexBufferSize(mesh->_vertexBufferSize), _faceBuffer(mesh->_faceBuffer), _faceBufferSize(mesh->_faceBufferSize)
	{

	}

	Mesh::~Mesh()
	{
		DeleteData();
	}

	bool Mesh::ImportObjFile(const char* fileName)
	{
		uint32 vertexNum = 0;
		uint32 normalNum = 0;
		uint32 indexNum = 0;
		uint32 uvNum = 0;
		std::string line;
		line.resize(100);
		std::stringstream sStream;

		std::ifstream file(fileName);

		if (!file.is_open())
		{
			file.close();
			return false;
		}

		while (getline(file, line))
		{
			if (line[0] == 'v')
			{
				if (line[1] == ' ')
				{
					++vertexNum;
				}
				else if (line[1] == 'n')
				{
					++normalNum;
				}
				else if (line[1] == 't')
				{
					++uvNum;
				}
			}
			else if (line[0] == 'f' && line[1] == ' ')
			{
				++indexNum;
			}
		}

		_vertexBufferSize = vertexNum;
		_vertexBuffer = new Vertex[_vertexBufferSize];
		vertexNum = 0;

		_uvBufferSize = uvNum;
		_uvBuffer = new UV[_uvBufferSize];
		uvNum = 0;

		_faceBufferSize = indexNum;
		_faceBuffer = new Face[_faceBufferSize];
		indexNum = 0;

		Vector3* normalArr = new Vector3[normalNum];
		normalNum = 0;

		file.clear();
		file.seekg(0);

		double minX = DBL_MAX;
		double maxX = -(DBL_MAX);
		double minY = DBL_MAX;
		double maxY = -(DBL_MAX);
		double minZ = DBL_MAX;
		double maxZ = -(DBL_MAX);

		while (getline(file, line))
		{
			if (line[0] == 'v' && line[1] == ' ')
			{
				double bufDouble = 0.0;
				char trash;
				sStream.clear();
				sStream.str(line);
				sStream >> trash;

				sStream >> bufDouble;
				_vertexBuffer[vertexNum]._pos._x = bufDouble;
				SetMinMax(&minX, &maxX, bufDouble);

				sStream >> bufDouble;
				_vertexBuffer[vertexNum]._pos._y = bufDouble;
				SetMinMax(&minY, &maxY, bufDouble);

				sStream >> bufDouble;
				_vertexBuffer[vertexNum]._pos._z = bufDouble;
				SetMinMax(&minZ, &maxZ, bufDouble);
				++vertexNum;
			}
			else if (line[0] == 'v' && line[1] == 't')
			{
				double temp = 0.0;
				char trash;
				sStream.clear();
				sStream.str(line);
				sStream >> trash;
				sStream >> trash;

				sStream >> temp;
				_uvBuffer[uvNum]._u = temp;

				sStream >> temp;
				_uvBuffer[uvNum]._v = temp;

				++uvNum;
			}
			else if (line[0] == 'v' && line[1] == 'n')
			{
				double temp = 0.0;
				char trash;
				sStream.clear();
				sStream.str(line);
				sStream >> trash;
				sStream >> trash;

				sStream >> temp;
				normalArr[normalNum]._x = temp;

				sStream >> temp;
				normalArr[normalNum]._y = temp;

				sStream >> temp;
				normalArr[normalNum]._z = temp;
				++normalNum;
			}
			else if (line[0] == 'f' && line[1] == ' ')
			{
				int temp;
				char trash;
				sStream.clear();
				sStream.str(line);
				sStream >> trash;

				for (int i = 0; i < 3; ++i)
				{
					sStream >> temp;
					_faceBuffer[indexNum]._vertexIndex[i] = temp - 1;
					sStream >> trash;
					sStream >> temp;
					_faceBuffer[indexNum]._uvIndex[i] = temp - 1;
					sStream >> trash;
					sStream >> temp;
					_faceBuffer[indexNum]._nomal = normalArr[temp];
				}

				++indexNum;
			}
		}

		_aabb._pos._x = (minX + maxX) / 2;
		_aabb._x = _aabb._pos._x - minX;
		_aabb._pos._y = (minY + maxY) / 2;
		_aabb._y = _aabb._pos._y - minY;
		_aabb._pos._z = (minZ + maxZ) / 2;
		_aabb._z = _aabb._pos._z - minZ;

		_boundingSphere._pos = _aabb._pos;
		_boundingSphere._radius = _aabb._x > _aabb._y ? _aabb._x : _aabb._y;
		_boundingSphere._radius = _boundingSphere._radius > _aabb._z ? _boundingSphere._radius : _aabb._z;

		file.close();
		return true;
	}

	void Mesh::DeleteData()
	{
		if (_vertexBuffer != nullptr)
		{
			delete[] _vertexBuffer;
			_vertexBuffer = nullptr;
		}
		if (_faceBuffer != nullptr)
		{
			delete[] _faceBuffer;
			_faceBuffer = nullptr;
		}
	}

	void Mesh::SetMinMax(double* min, double* max, double Num)
	{
		if (*min > Num)
		{
			*min = Num;
		}
		if (*max < Num)
		{
			*max = Num;
		}
	}

	/*-------------------------------
			    Position
	------------------------------*/
	Position::Position(double x /*= 0.0*/, double y /*= 0.0*/, double z /*= 0.0*/)
	{
		_data._element[3] = x;
		_data._element[7] = y;
		_data._element[11] = z;
		_data._element[0] = 1.0;
		_data._element[5] = 1.0;
		_data._element[10] = 1.0;
		_data._element[15] = 1.0;
	}

	void Position::Set(double x, double y, double z)
	{
		_data._element[3] = x;
		_data._element[7] = y;
		_data._element[11] = z;
	}

	Vector3 Position::Get() const
	{
		return Vector3(_data._element[3], _data._element[7], _data._element[11]);
	}

	void Position::DeltaSet(double x, double y, double z)
	{
		_data._element[3] += x;
		_data._element[7] += y;
		_data._element[11] += z;
	}

	void Position::SetX(double x)
	{
		_data._element[3] = x;
	}

	double Position::GetX()
	{
		return _data._element[3];
	}

	void Position::SetY(double y)
	{
		_data._element[7] = y;
	}

	double Position::GetY()
	{
		return _data._element[7];
	}

	void Position::SetZ(double z)
	{
		_data._element[11] = z;
	}

	double Position::GetZ()
	{
		return _data._element[11];
	}

	/*-------------------------------
			    Rotation
	------------------------------*/
	Rotation::Rotation(double x /*= 0.0*/, double y /*= 0.0*/, double z /*= 0.0*/)
	{
		_x = x;
		_y = y;
		_z = z;
		_metrixX._element[0] = 1.0;
		_metrixX._element[15] = 1.0;
		SetX(x);
		_metrixY._element[5] = 1.0;
		_metrixY._element[15] = 1.0;
		SetY(y);
		_metrixZ._element[10] = 1.0;
		_metrixZ._element[15] = 1.0;
		SetZ(z);

		_metrixR = (_metrixZ * (_metrixY * _metrixX));
	}

	void Rotation::Set(double x, double y, double z)
	{
		SetX(x);
		SetY(y);
		SetZ(z);

		_metrixR = (_metrixZ * _metrixY * _metrixX);
	}

	void Rotation::DeltaSet(double x, double y, double z)
	{
		Set(_x + x, _y + y, _z + z);
		_metrixR = (_metrixZ * (_metrixY * _metrixX));
	}

	void Rotation::SetX(double euler)
	{
		//euler -= (int64)(euler / 360) * 360;

		_x = euler;
		double rad = EulerToRadian(euler);
		_metrixX._element[5] = Cos(rad);
		_metrixX._element[6] = Sin(rad) * -1;
		_metrixX._element[9] = Sin(rad);
		_metrixX._element[10] = Cos(rad);
	}

	double Rotation::GetX()
	{
		return _x;
	}

	void Rotation::SetY(double euler)
	{
		//euler -= (int64)(euler / 360) * 360;

		_y = euler;
		double rad = EulerToRadian(euler);
		_metrixY._element[0] = Cos(rad);
		_metrixY._element[2] = Sin(rad);
		_metrixY._element[8] = Sin(rad) * -1;
		_metrixY._element[10] = Cos(rad);
	}

	double Rotation::GetY()
	{
		return _y;
	}

	void Rotation::SetZ(double euler)
	{
		//euler -= (int64)(euler / 360) * 360;

		_z = euler;
		double rad = EulerToRadian(euler);
		_metrixZ._element[0] = Cos(rad);
		_metrixZ._element[1] = Sin(rad) * -1;
		_metrixZ._element[4] = Sin(rad);
		_metrixZ._element[5] = Cos(rad);
	}

	double Rotation::GetZ()
	{
		return _z;
	}

	double Rotation::EulerToRadian(double euler)
	{
		return euler / 180 * pi;
	}

	double Rotation::RadianToEuler(double radian)
	{
		return radian / pi * 180;
	}
	/*-------------------------------
				  Scale
	------------------------------*/

	Scale::Scale(double x /*= 1.0*/, double y /*= 1.0*/, double z /*= 1.0*/)
	{
		_data._element[0] = x;
		_data._element[5] = y;
		_data._element[10] = z;
		_data._element[15] = 1;
	}

	void Scale::Set(double x, double y, double z)
	{
		_data._element[0] = x;
		_data._element[5] = y;
		_data._element[10] = z;
	}

	void Scale::SetX(double x)
	{
		_data._element[0] = x;
	}

	double Scale::GetX()
	{
		return _data._element[0];
	}

	void Scale::SetY(double y)
	{
		_data._element[5] = y;
	}

	double Scale::GetY()
	{
		return _data._element[5];
	}

	void Scale::SetZ(double z)
	{
		_data._element[10] = z;
	}

	double Scale::GetZ()
	{
		return _data._element[10];
	}

	/*-------------------------------
				Transform
	------------------------------*/



	/*-------------------------------
				 Object
	------------------------------*/

	Object::~Object()
	{
		DeleteMesh();
	}

	bool Object::ImportObjFileToMesh(const char* fileName)
	{
		return _mesh->ImportObjFile(fileName);
	}

	void Object::DeleteMesh()
	{
		delete _mesh;
	}

	/*-------------------------------
			RenderableObject
	------------------------------*/

	RenderableObject::RenderableObject(Object& object)
		:_object(&object)
	{
		//_renderMesh._vertexBufferSize = object._mesh->_vertexBufferSize;
		//_renderMesh._faceBufferSize = object._mesh->_faceBufferSize;
		//_renderMesh._vertexBuffer = new Vertex[_renderMesh._vertexBufferSize];
		//_renderMesh._faceBuffer = new Face[_renderMesh._faceBufferSize];
		//for (int i = 0; i < _renderMesh._faceBufferSize; ++i)
		//{
		//	_renderMesh._faceBuffer[i] = object._mesh->_faceBuffer[i];
		//}
	}

	RenderableObject::RenderableObject(Object* object)
		:_object(object)
	{
		//_renderMesh._vertexBufferSize = object->_mesh->_vertexBufferSize;
		//_renderMesh._faceBufferSize = object->_mesh->_faceBufferSize;
		//_renderMesh._vertexBuffer = new Vertex[_renderMesh._vertexBufferSize];
		//_renderMesh._faceBuffer = new Face[_renderMesh._faceBufferSize];

		//for (int i = 0; i < _renderMesh._faceBufferSize; ++i)
		//{
		//	_renderMesh._faceBuffer[i] = object->_mesh->_faceBuffer[i];
		//}
	}

	/*-------------------------------
				Camera
	------------------------------*/
	Camera::Camera(double aspect /*= (1980.0 / 1080.0)*/, double fovY, double n, double f)
		:_n(n), _f(f), _aspect(aspect), _fovY(fovY)
	{
		_transform._position.Set(2, 2, 2);
		_transform._rotation.Set(45, 45, 45);
	}

	Vector3 Camera::GetEye() const
	{
		return _transform._position.Get();
	}

	Vector3 Camera::GetU() const
	{
		Vector3 localX = {1, 0, 0};
		localX = _transform._rotation._metrixR * localX;
		localX.Normalization();
		return localX;
	}

	Vector3 Camera::GetV() const
	{
		Vector3 localY = {0, 1, 0};
		localY = _transform._rotation._metrixR * localY;
		localY.Normalization();
		return localY;
	}

	Vector3 Camera::GetN() const
	{
		Vector3 localZ = {0, 0, 1};
		localZ = _transform._rotation._metrixR * localZ;
		localZ.Normalization();
		return localZ;
	}

	renderer::UV& UV::operator=(const UV& rhs)
	{
		_u = rhs._u;
		_v = rhs._v;
		return *this;
	}

	renderer::UV& UV::operator+=(const UV& rhs)
	{
		_u += rhs._u;
		_v += rhs._v;
		return *this;
	}

	const renderer::UV UV::operator+(const UV& rhs)
	{
		return (*this) += rhs;
	}

	renderer::UV& UV::operator*=(const double rhs)
	{
		_u *= rhs;
		_v *= rhs;
		return *this;
	}

	const renderer::UV UV::operator*(const double rhs)
	{
		return (*this) *= rhs;
	}

	UV& UV::operator/=(const double rhs)
	{
		_u /= rhs;
		_v /= rhs;
		return *this;
	}

	const renderer::UV UV::operator/(const double rhs)
	{
		return (*this) /= rhs;
	}

	RenderFace& RenderFace::operator=(const RenderFace& rhs)
	{
		for (int i = 0; i < 3; ++i)
		{
			_vertex[i] = rhs._vertex[i];
			_uv[i] = rhs._uv[i];
			_isDraw = rhs._isDraw;
			_scheme = rhs._scheme;
		}
		return *this;
	}



}