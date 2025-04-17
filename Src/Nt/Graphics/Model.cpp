// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <GL/GLEW.h>
#include <GL/GL.h>

#include <Nt/Graphics/Objects/Model.h>
#include <Nt/Graphics/Renderer.h>

namespace Nt {
	Model::Model() noexcept :
		m_pMesh(nullptr),
		m_MeshIndex(uInt(-1)),
		m_pTexture(nullptr),
		m_TextureIndex(uInt(-1))
	{
	}
	Model::Model(const uInt& meshIndex) :
		m_pTexture(nullptr),
		m_TextureIndex(uInt(-1))
	{
		SetMesh(meshIndex);
	}
	Model::Model(const uInt& meshIndex, const uInt& textureIndex) : Model() {
		SetMesh(meshIndex);
		SetTexture(textureIndex);
	}
	Model::Model(const Mesh& newMesh) : Model() {
		SetMesh(newMesh);
	}
	Model::Model(const Mesh& newMesh, const Texture& newTexture) : Model() {
		SetMesh(newMesh);
		SetTexture(newTexture);
	}
	Model::Model(const Nt::String& meshFileName) : Model() {
		LoadMeshFromFile(meshFileName);
	}
	Model::Model(const Nt::String& meshFileName, const Nt::String& textureFileName) : Model() {
		LoadMeshFromFile(meshFileName);
		LoadTextureFromFile(textureFileName);
	}
	Model::Model(const Model& newModel) :
		IObject(newModel),
		m_MeshIndex(newModel.m_MeshIndex),
		m_TextureIndex(newModel.m_TextureIndex),
		m_pMesh(nullptr),
		m_pTexture(nullptr)
	{
		if (m_MeshIndex == uInt(-1) && newModel.m_pMesh != nullptr)
			m_pMesh = new Mesh(*newModel.m_pMesh);
		if (m_TextureIndex == uInt(-1) && newModel.m_pTexture)
			m_pTexture = new Texture(*newModel.m_pTexture);
	}
	Model::Model(Model&& newModel) :
		IObject(newModel),
		m_MeshIndex(newModel.m_MeshIndex),
		m_pMesh(std::move(newModel.m_pMesh)),
		m_TextureIndex(newModel.m_TextureIndex),
		m_pTexture(std::move(newModel.m_pTexture))
	{
		newModel.m_pMesh = nullptr;
		newModel.m_pTexture = nullptr;
	}
	Model::~Model() {
		if (m_MeshIndex == uInt(-1) && m_pMesh != nullptr)
			delete(m_pMesh);
		if (m_TextureIndex == uInt(-1) && m_pTexture != nullptr)
			delete(m_pTexture);
	}

	void Model::Write(std::ostream& stream) const {
		Serialization::WriteAll(stream,
			m_Position, m_Origin, m_Angle,
			m_AngleOrigin, m_Size, m_Color,
			m_MeshIndex, m_TextureIndex);

		if (m_MeshIndex == uInt(-1))
			Serialization::WriteAll(stream, m_pMesh);

		if (m_TextureIndex == uInt(-1))
			Serialization::WriteAll(stream, m_pTexture);
	}
	void Model::Read(std::istream& stream) {
		Serialization::ReadAll(stream,
			m_Position, m_Origin, m_Angle,
			m_AngleOrigin, m_Size, m_Color,
			m_MeshIndex, m_TextureIndex);

		if (m_MeshIndex == uInt(-1))
			Serialization::ReadAll(stream, m_pMesh);

		if (m_TextureIndex == uInt(-1))
			Serialization::ReadAll(stream, m_pTexture);
	}
	constexpr uInt Model::Sizeof() const noexcept {
		return sizeof(*this);
	}
	constexpr uInt Model::ClassType() const noexcept {
		return 0;
	}
	ISerialization* Model::New([[maybe_unused]] const uInt& classType) {
		return new Model;
	}

	void Model::LoadTextureFromFile(const Nt::String& fileName) {
		if (m_TextureIndex == uInt(-1))
			SAFE_DELETE(&m_pTexture);
		else
			m_TextureIndex = uInt(-1);

		m_pTexture = new Nt::Texture(fileName);
	}
	void Model::LoadMeshFromFile(const Nt::String& fileName) {
		if (m_MeshIndex == uInt(-1))
			SAFE_DELETE(&m_pMesh);
		else
			m_MeshIndex = uInt(-1);

		m_pMesh = new Nt::Mesh(fileName);
	}

	void Model::Render(Renderer* pRenderer) const {
		if (m_pMesh == nullptr)
			return;

		if (m_pMesh->IsUsedIndexBuffer())
			Render(pRenderer, 0, m_pMesh->GetIndices().size());
		else
			Render(pRenderer, 0, m_pMesh->GetVertices().size());
	}
	void Model::Render(Renderer* pRenderer, const uInt& offset, const uInt& verticesCount) const {
		if (m_pMesh == nullptr)
			return;
		if (!IsRenderEnabled())
			return;

		RequireNotNull(pRenderer);

		if (m_pTexture != nullptr)
			m_pTexture->Bind();
		else
			glBindTexture(GL_TEXTURE_2D, 0);

		const Float4D color = pRenderer->GetColor();
		pRenderer->SetColor(GetColor());
		pRenderer->MatrixWorldPush();
		pRenderer->Transform(GetPosition(), GetOrigin(), GetAngle(), GetAngleOrigin());
		pRenderer->Render(m_pMesh, offset, verticesCount);
		pRenderer->MatrixWorldPop();
		pRenderer->SetColor(color);
	}

	Model& Model::operator = (const Model& newModel) {
		if (this == &newModel)
			return (*this);

		IObject::operator = (newModel);

		m_MeshIndex = newModel.m_MeshIndex;
		if (m_MeshIndex == uInt(-1))
			m_pMesh = (newModel.m_pMesh) ? new Mesh(*newModel.m_pMesh) : nullptr;

		m_TextureIndex = newModel.m_TextureIndex;
		if (m_TextureIndex == uInt(-1))
			m_pTexture = (newModel.m_pTexture) ? new Texture(*newModel.m_pTexture) : nullptr;
		return (*this);
	}

	Model& Model::operator = (Model&& model) {
		if (this == &model)
			return (*this);

		IObject::operator = (model);

		m_MeshIndex = model.m_MeshIndex;
		m_pMesh = std::move(model.m_pMesh);
		m_TextureIndex = model.m_TextureIndex;
		m_pTexture = std::move(model.m_pTexture);

		model.m_pMesh = nullptr;
		model.m_pTexture = nullptr;

		return *this;
	}

	uInt Model::GetTextureIndex() const noexcept {
		return m_TextureIndex;
	}
	uInt Model::GetMeshIndex() const noexcept {
		return m_MeshIndex;
	}
	Texture* Model::GetTexturePtr() const noexcept {
		return m_pTexture;
	}
	Mesh* Model::GetMeshPtr() const noexcept {
		return m_pMesh;
	}

	void Model::SetTexture(const uInt& textureIndex) {
		if (m_TextureIndex == uInt(-1))
			SAFE_DELETE(&m_pTexture);

		m_TextureIndex = textureIndex;
		m_pTexture = ResourceManager::Instance().Get<Texture>(textureIndex);
	}
	void Model::SetTexture(const Texture& newTexture) {
		if (m_TextureIndex == uInt(-1))
			SAFE_DELETE(&m_pTexture);
		else
			m_TextureIndex = uInt(-1);

		m_pTexture = new Texture(newTexture);
	}
	void Model::SetMesh(const uInt& meshIndex) {
		if (m_MeshIndex == uInt(-1))
			SAFE_DELETE(&m_pMesh);

		m_MeshIndex = meshIndex;
		m_pMesh = ResourceManager::Instance().Get<Mesh>(meshIndex);
	}
	void Model::SetMesh(const Mesh& newMesh) {
		if (m_MeshIndex == uInt(-1))
			SAFE_DELETE(&m_pMesh);
		else
			m_MeshIndex = uInt(-1);

		m_pMesh = new Mesh(newMesh);
	}
	void Model::SetMesh(const Shape& shape) {
		if (m_MeshIndex != uInt(-1)) {
			m_MeshIndex = uInt(-1);
			m_pMesh = nullptr;
		}

		if (m_pMesh == nullptr)
			m_pMesh = new Mesh(shape);
		else
			m_pMesh->SetShape(shape);
	}
}