#pragma once
#include <iostream>

class Vector4 {
public:
	Vector4(float _x, float _y, float _z, float _w) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
	Vector4(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	Vector4(float values[]) {
		x = values[0];
		y = values[1];
		z = values[2];
		w = values[3];
	}
	Vector4(float _x, float _y) {
		x = _x;
		y = _y;
	}
	Vector4() {
	}
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;
	Vector4 operator+(Vector4 vector) {
		Vector4 result;
		result.x = x + vector.x;
		result.y = y + vector.y;
		result.z = z + vector.z;
		result.w = w + vector.w;
		return result;
	}
	Vector4 operator-(Vector4 vector) {
		Vector4 result;
		result.x = x - vector.x;
		result.y = y - vector.y;
		result.z = z - vector.z;
		result.w = w - vector.w;
		return result;
	}
	float operator*(Vector4 vector) {
		return x * vector.x + y * vector.y + z * vector.z + w * vector.w;
	}
	Vector4 operator*(float scaler) {
		Vector4 result;
		result.x = x * scaler;
		result.y = y * scaler;
		result.z = z * scaler;
		result.w = w * scaler;
		return result;
	}
};

static std::ostream& operator<<(std::ostream& ostream, Vector4 vector) {
	ostream << "(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";
	return ostream;
}

class Vector3 {
public:
	Vector3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	Vector3(float values[]) {
		x = values[0];
		y = values[1];
		z = values[2];
	}
	Vector3(float _x, float _y) {
		x = _x;
		y = _y;
	}
	Vector3() {
	}
	float x = 0;
	float y = 0;
	float z = 0;

	float Magnitude() const{
		return sqrtf(x * x + y * y + z * z);
	}

	Vector3 operator+(Vector3 vector) {
		Vector3 result;
		result.x = x + vector.x;
		result.y = y + vector.y;
		result.z = z + vector.z;
		return result;
	}
	Vector3 operator-(Vector3 vector) {
		Vector3 result;
		result.x = x - vector.x;
		result.y = y - vector.y;
		result.z = z - vector.z;
		return result;
	}
	float operator*(Vector3 vector) {
		return x * vector.x + y * vector.y + z * vector.z;
	}
	Vector3 operator%(Vector3 vector) {
		Vector3 result;
		result.x = y * vector.z - z * vector.y;
		result.y = z * vector.x - x * vector.z;
		result.z = x * vector.y - y * vector.x;
		return result;
	}
	Vector3 operator*(float scaler) {
		Vector3 result;
		result.x = x * scaler;
		result.y = y * scaler;
		result.z = z * scaler;
		return result;
	}
};

static std::ostream& operator<<(std::ostream& ostream, Vector3 vector) {
	ostream << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
	return ostream;
}

class Vector2 {
public:
	Vector2(float _x, float _y) {
		x = _x;
		y = _y;
	}
	Vector2(float values[2]) {
		x = values[0];
		y = values[1];
	}
	Vector2() {
	}
	float x = 0;
	float y = 0;

	float Magnitude() const {
		return sqrtf(x * x + y * y);
	}

	float MagnitudeSquared() const {
		return x * x + y * y;
	}

	Vector2 operator+(Vector2 vector) {
		Vector2 result;
		result.x = x + vector.x;
		result.y = y + vector.y;
		return result;
	}
	Vector2& operator+=(Vector2 vector) {
		x += vector.x;
		y += vector.y;
		return *this;
	}
	Vector2 operator-(Vector2 vector) {
		Vector2 result;
		result.x = x - vector.x;
		result.y = y - vector.y;
		return result;
	}
	float operator*(Vector2 vector) {
		return x * vector.x + y * vector.y;
	}
	Vector2 operator*(float scaler) {
		Vector2 result;
		result.x = x * scaler;
		result.y = y * scaler;
		return result;
	}
	Vector2 operator/(float scaler) {
		Vector2 result;
		result.x = x / scaler;
		result.y = y / scaler;
		return result;
	}
};

static std::ostream& operator<<(std::ostream& ostream, Vector2 vector) {
	ostream << "(" << vector.x << ", " << vector.y << ")";
	return ostream;
}

class Matrix4x4 {
public:
	float values[4][4];
	Matrix4x4() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				values[i][j] = 0;
			}
		}
	}
	Matrix4x4(float _values[4][4]) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				values[i][j] = _values[i][j];
			}
		}
	}
	Matrix4x4(float _values[16]) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				values[i][j] = _values[i * 4 + j];
			}
		}
	}
	Matrix4x4 operator+(Matrix4x4 matrix) {
		Matrix4x4 result;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result.values[i][j] = values[i][j] + matrix.values[i][j];
			}
		}
		return result;
	}
	Matrix4x4 operator-(Matrix4x4 matrix) {
		Matrix4x4 result;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result.values[i][j] = values[i][j] - matrix.values[i][j];
			}
		}
		return result;
	}
	Matrix4x4 operator*(Matrix4x4 matrix) {
		Matrix4x4 result;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result.values[i][j] = Vector4(values[i]) * Vector4(matrix.values[0][j],
					matrix.values[1][j], matrix.values[2][j], matrix.values[3][j]);
			}
		}
		return result;
	}
	Vector4 operator*(Vector4 vector) {
		Vector4 result;
		result.x = Vector4(values[0]) * vector;
		result.y = Vector4(values[1]) * vector;
		result.z = Vector4(values[2]) * vector;
		result.w = Vector4(values[3]) * vector;
		return result;
	}
};

static std::ostream& operator<<(std::ostream& ostream, Matrix4x4 matrix) {
	ostream << "|";
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			ostream << matrix.values[i][j];
			if (j != 3) {
				ostream << " ";
			}
			if (j == 3 && i != 3) {
				ostream << "|" << "\n" << "|";
			}
			else if (j == 3) {
				ostream << "|" << "\n";
			}
		}
	}
	return ostream;
}

class Matrix3x3 {
public:
	float values[3][3];
	Matrix3x3() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				values[i][j] = 0;
			}
		}
	}
	Matrix3x3(float _values[3][3]) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				values[i][j] = _values[i][j];
			}
		}
	}
	Matrix3x3(float _values[9]) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				values[i][j] = _values[i * 3 + j];
			}
		}
	}
	Matrix3x3 operator+(Matrix3x3 matrix) {
		Matrix3x3 result;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				result.values[i][j] = values[i][j] + matrix.values[i][j];
			}
		}
		return result;
	}
	Matrix3x3 operator-(Matrix3x3 matrix) {
		Matrix3x3 result;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				result.values[i][j] = values[i][j] - matrix.values[i][j];
			}
		}
		return result;
	}
};

static std::ostream& operator<<(std::ostream& ostream, Matrix3x3 matrix) {
	ostream << "|";
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			ostream << matrix.values[i][j];
			if (j != 2) {
				ostream << " ";
			}
			if (j == 2 && i != 2) {
				ostream << "|" << "\n" << "|";
			}
			else if (j == 2) {
				ostream << "|" << "\n";
			}
		}
	}
	return ostream;
}

class Matrix2x2 {
public:
	float values[2][2];
	Matrix2x2() {
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				values[i][j] = 0;
			}
		}
	}
	Matrix2x2(float _values[2][2]) {
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				values[i][j] = _values[i][j];
			}
		}
	}
	Matrix2x2(float _values[4]) {
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				values[i][j] = _values[i * 2 + j];
			}
		}
	}
	Matrix2x2 operator+(Matrix2x2 matrix) {
		Matrix2x2 result;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				result.values[i][j] = values[i][j] + matrix.values[i][j];
			}
		}
		return result;
	}
	Matrix2x2 operator-(Matrix2x2 matrix) {
		Matrix2x2 result;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				result.values[i][j] = values[i][j] - matrix.values[i][j];
			}
		}
		return result;
	}
	Matrix2x2 operator*(Matrix2x2 matrix) {
		Matrix2x2 result;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				result.values[i][j] = Vector2(values[i]) * Vector2(matrix.values[0][j],
					matrix.values[1][j]);
			}
		}
		return result;
	}
	Vector2 operator*(Vector2 vector) {
		Vector2 result;
		result.x = Vector2(values[0]) * vector;
		result.y = Vector2(values[1]) * vector;
		return result;
	}
};

static std::ostream& operator<<(std::ostream& ostream, Matrix2x2 matrix) {
	ostream << "|";
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			ostream << matrix.values[i][j];
			if (j != 1) {
				ostream << " ";
			}
			if (j == 1 && i != 1) {
				ostream << "|" << "\n" << "|";
			}
			else if (j == 1) {
				ostream << "|" << "\n";
			}
		}
	}
	return ostream;
}

const float Pi = 3.1415927f;
