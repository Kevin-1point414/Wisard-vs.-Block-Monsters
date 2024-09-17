#include "Scene.h"
#include "Sprite.h"

Sprite::Sprite() {
	UpdateMomentOfInertia();
	name = "noName";
}

Sprite::Sprite(Sprite&& sprite) noexcept
	: position(sprite.position), velocity(sprite.velocity), acceleration(sprite.acceleration),
	oldVelocity(sprite.oldVelocity), mass(sprite.mass), 
	coefficientOfRestitution(sprite.coefficientOfRestitution), friction(sprite.friction),
	gravity(sprite.gravity), forces(sprite.forces), size(sprite.size), rotation(sprite.rotation),
	angluarVelocity(sprite.angluarVelocity), momentOfInertia(sprite.momentOfInertia),
	isPhysicsActive(sprite.isPhysicsActive), isMobile(sprite.isMobile), isGrounded(sprite.isGrounded),
	imageRectangle(sprite.imageRectangle), texture(sprite.texture), scenesIncludedIn(sprite.scenesIncludedIn),
	VAO(sprite.VAO), VBO(sprite.VBO), EBO(sprite.EBO), textureID(sprite.textureID), Collided(sprite.Collided),
	name(sprite.name), canCollide(sprite.canCollide)
{
	for (int i : scenesIncludedIn) {
	
		std::vector<Sprite*>::iterator iterator =
			std::find(sceneList[i]->pointerSpriteList.begin(), sceneList[i]->pointerSpriteList.end(), &sprite);
	
		if (iterator != sceneList[i]->pointerSpriteList.end()){
			*iterator = this;
		}
	}
}

Sprite::~Sprite() {
	for (int i : scenesIncludedIn) {

		std::vector<Sprite*>::iterator iterator =
			std::find(sceneList[i]->pointerSpriteList.begin(), sceneList[i]->pointerSpriteList.end(), this);

		if (iterator != sceneList[i]->pointerSpriteList.end()) {

			sceneList[i]->pointerSpriteList.erase(iterator);

			sceneList[i]->valueSpriteList.push_back(*this);
		}
	}
}

void Sprite::SetTexture(std::string _imageName) {

	imageName = _imageName;
	if (name == "noName") {
		name = _imageName.substr(0, _imageName.find('.'));
	}
	_imageName = "Textures\\" + _imageName;
	IWICImagingFactory* imageFactory = NULL;
	if (CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_ALL, IID_IWICImagingFactory,
		reinterpret_cast<void**>(&imageFactory)) != S_OK) {
		std::cout << "Error with CoCreateInstance() at line 58 in Sprite.h";
	};

	IWICBitmapDecoder* imageDecoder = NULL;
	imageFactory->CreateDecoderFromFilename(std::wstring(_imageName.begin(), _imageName.end())
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
		0.5 , -0.5, 0, 1, 1,
		0.5 , 0.5 , 0, 1, 0,
		-0.5, 0.5 , 0, 0, 0
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

void Sprite::SetRotation(float _rotation) {
	rotation = Vector2(cos(_rotation), sin(_rotation));
}

void Sprite::SetRotation(Vector2 _rotation) {
	rotation = _rotation;
}

void Sprite::SetPosition(Vector2 _position) {
	position = _position;
}

void Sprite::SetPosition(float _posX, float _posY) {
	position = Vector2(_posX, _posY);
}

void Sprite::SetSize(Vector2 _size) {
	size = _size;
	UpdateMomentOfInertia();
}

void Sprite::SetSize(float _sizeX, float _sizeY) {
	size = Vector2(_sizeX, _sizeY);
	UpdateMomentOfInertia();
}

void Sprite::SetMass(float _mass) {
	mass = _mass;
	UpdateMomentOfInertia();
}

void Sprite::SetCoefficientOfRestitution(float _coefficientOfRestitution) {
	coefficientOfRestitution = _coefficientOfRestitution;
}

void Sprite::SetGravity(float _gravity) {
	gravity = _gravity;
}

void Sprite::SetFriction(float _friction) {
	friction = _friction;
}

void Sprite::SetVelocity(Vector2 _velocity) {
	velocity = _velocity;
}

void Sprite::SetOldVelocity(Vector2 _oldVelocity) {
	oldVelocity = _oldVelocity;
}

void Sprite::SetVelocity(float _velocityX, float _velocityY) {
	velocity = Vector2(_velocityX, _velocityY);
}

void Sprite::SetAcceleration(Vector2 _acceleration) {
	acceleration = _acceleration;
}

void Sprite::SetAngluarVelocity(float _angluarVelocity) {
	angluarVelocity = _angluarVelocity;
}

void Sprite::SetIsPhysicsActive(bool _isPhysicsActive) {
	isPhysicsActive = _isPhysicsActive;
	UpdateMomentOfInertia();
}

void Sprite::SetIsMobile(bool _isMobile) {
	isMobile = _isMobile;
	UpdateMomentOfInertia();
}

void Sprite::SetIsGrounded(bool _isGrounded) {
	isGrounded = _isGrounded;
}

void Sprite::SetIsUI(bool _isUI) {
	isUI = _isUI;
}

void Sprite::SetCanCollide(bool _canCollide) {
	canCollide = _canCollide;
}

void Sprite::ApplyForce(Force _force) {
	forces.push_back(_force);
}

void Sprite::ApplyForce(Vector2 _force, Vector2 _relativePositionAppliedAt) {
	Force force(_force, _relativePositionAppliedAt);
	forces.push_back(force);
}

void Sprite::ApplyForce(float _forceX, float _forceY, Vector2 _relativePositionAppliedAt) {
	Force force(_forceX, _forceY, _relativePositionAppliedAt);
	forces.push_back(force);
}

void Sprite::ApplyForce(float _forceX, float _forceY, float _relativePositionAppliedAtX,
	float _relativePositionAppliedAtY) {

	Force force(_forceX, _forceY, _relativePositionAppliedAtX, _relativePositionAppliedAtY);
	forces.push_back(force);
}

void Sprite::ApplyForce(Vector2 _force, float _relativePositionAppliedAtX, float _relativePositionAppliedAtY) {
	Force force(_force, _relativePositionAppliedAtX, _relativePositionAppliedAtY);
	forces.push_back(force);
}

void Sprite::ClearForces() {
	forces.clear();
}

void Sprite::UpdateMomentOfInertia() {
	momentOfInertia = mass * (size.x * size.x + size.y * size.y) / 12.0f;
}

void Sprite::SetName(std::string _name) {
	name = _name;
}

void Sprite::Reset() {
	position = Vector2(0, 0);
	velocity = Vector2(0, 0);
	acceleration = Vector2(0, 0);
	oldVelocity = Vector2(0, 0);
	size = Vector2(1, 1);
	rotation = Vector2(1, 0);
	mass = 1;
	coefficientOfRestitution = 0;
	friction = 0.7f;
	gravity = 2;
	angluarVelocity = 0;
	isPhysicsActive = true;
	isMobile = true;
	isGrounded = false;
	WICRect imageRectangle = {};
	forces.clear();
	Collided = NULL;
	scenesIncludedIn.clear();
	VAO = NULL;
	VBO = NULL;
	EBO = NULL;
	textureID = NULL;
	texture = NULL;
	name = "noName";
	canCollide = true;
}

float Sprite::GetRotation() const {
	return std::atan2f(rotation.y, rotation.x);
}

Vector2 Sprite::GetRotationVector() const {
	return rotation;
}

Vector2 Sprite::GetPosition() const {
	return position;
}

Vector2 Sprite::GetAcceleration() const {
	return acceleration;
}

Vector2 Sprite::GetSize() const {
	return size;
}

float Sprite::GetAngluarVelocity() const {
	return angluarVelocity;
}

float Sprite::GetGravity() const {
	return gravity;
}

float Sprite::GetMass() const {
	return mass;
}

float Sprite::GetFriction() const {
	return friction;
}

float Sprite::GetMomentOfInertia() const {
	return momentOfInertia;
}

float Sprite::GetCoefficientOfRestitution() const {
	return coefficientOfRestitution;
}

bool Sprite::GetIsPhysicsActive() const {
	return isPhysicsActive;
}

bool Sprite::GetIsMobile() const {
	return isMobile;
}

bool Sprite::GetIsGrounded() const {
	return isGrounded;
}

bool Sprite::GetIsUI() const {
	return isUI;
}

bool Sprite::GetCanCollide() const {
	return canCollide;
}

Vector2 Sprite::GetVelocity() const {
	return velocity;
}

Vector2 Sprite::GetOldVelocity() const {
	return oldVelocity;
}

std::vector<Force> Sprite::GetForces() const {
	return forces;
}

std::string Sprite::GetImageName() const {
	return imageName;
}

std::string Sprite::GetName() const {
	return name;
}
