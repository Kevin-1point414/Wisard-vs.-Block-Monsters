#pragma once
#include <iostream>

struct Vector4 {
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
};

static std::ostream& operator<<(std::ostream& ostream, Vector4 vector) {
	ostream << "(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";
	return ostream;
}

struct Vector2 {
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
	Vector2 operator+(Vector2 vector) {
		Vector2 result;
		result.x = x + vector.x;
		result.y = y + vector.y;
		return result;
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
};

const double pi = 3.14159265358979323846;

static std::ostream& operator<<(std::ostream& ostream, Vector2 vector) {
	ostream << "(" << vector.x << ", " << vector.y << ")";
	return ostream;
}

struct Matrix4x4 {
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
	Matrix4x4(float _values[9]) {
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
