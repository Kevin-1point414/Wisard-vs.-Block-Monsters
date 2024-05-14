#pragma once
#include <wincodec.h>
#include "OpenWindow.h"

#pragma comment(lib, "windowscodecs.lib")

class Sprite
{
public:
	Sprite(std::string imageName, Vector2 _position, float _rotation) {
		position = _position;
		rotation = _rotation;
		SetTexture(imageName);
	}
	Sprite(std::string imageName, Vector2 _position) {
		position = _position;
		SetTexture(imageName);
	}
	Sprite(std::string imageName, float _posX, float _posY, float _rotation) {
		position = Vector2(_posX, _posY);
		rotation = _rotation;
		SetTexture(imageName);
	}
	Sprite(std::string imageName, float _posX, float _posY) {
		position = Vector2(_posX, _posY);
		SetTexture(imageName);
	}
	Sprite(std::string imageName) {
		SetTexture(imageName);
	}
	Sprite(Vector2 _position, float _rotation) {
		position = _position;
		rotation = _rotation;
	}
	Sprite(Vector2 _position) {
		position = _position;
	}
	Sprite(float _posX, float _posY, float _rotation) {
		position = Vector2(_posX, _posY);
		rotation = _rotation;
	}
	Sprite(float _posX, float _posY) {
		position = Vector2(_posX, _posY);
	}
	Sprite() {}

	Vector2 position = Vector2(0, 0);
	Vector2 scale = Vector2(1, 1);
	float rotation = 0;

	unsigned int VAO = NULL, VBO = NULL, EBO = NULL, textureID = NULL;

	void SetTexture(std::string imageName) {

		imageName = "Textures\\" + imageName;

		IWICImagingFactory* imageFactory = NULL;
		if (CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_ALL, IID_IWICImagingFactory,
			reinterpret_cast<void**>(&imageFactory)) != S_OK) {
			std::cout << "Error with CoCreateInstance() at line 58 in Sprite.h";
		};

		IWICBitmapDecoder* imageDecoder = NULL;
		imageFactory->CreateDecoderFromFilename(std::wstring(imageName.begin(), imageName.end())
			.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &imageDecoder);

		IWICBitmapFrameDecode* unConvertedImage = NULL;
		imageDecoder->GetFrame(0, &unConvertedImage);

		IWICFormatConverter* convertedImage = NULL;
		imageFactory->CreateFormatConverter(&convertedImage);
		convertedImage->Initialize(unConvertedImage, GUID_WICPixelFormat32bppRGBA,
			WICBitmapDitherTypeNone, NULL, 0, WICBitmapPaletteTypeCustom);

		IWICBitmap* bitmap = NULL;
		imageFactory->CreateBitmapFromSource(convertedImage, WICBitmapNoCache, &bitmap);

		convertedImage->GetSize((unsigned int*)&imageRectangle.Width,
			(unsigned int*)&imageRectangle.Height);
		IWICBitmapLock* lock = NULL;
		bitmap->Lock(&imageRectangle, WICBitmapLockRead, &lock);

		unsigned int bufferSize = NULL;
		lock->GetDataPointer(&bufferSize, &texture);

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageRectangle.Width, imageRectangle.Height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, texture);
		glGenerateMipmap(GL_TEXTURE_2D);

		float vertices[] =
		{
			-0.5, -0.5, 0, 0, 1,
			0.5, -0.5, 0, 1, 1,
			0.5, 0.5, 0, 1, 0,
			-0.5, 0.5, 0, 0, 0
		};
		unsigned int indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	WICInProcPointer texture = NULL;
	WICRect imageRectangle = {};
};
