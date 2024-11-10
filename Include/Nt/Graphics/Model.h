#pragma once

namespace Nt {
	class Model : public IObject, public ISerialization {
	public:
		Model() noexcept :
			m_pMesh(nullptr),
			m_MeshIndex(uInt(-1)),
			m_pTexture(nullptr),
			m_TextureIndex(uInt(-1))
		{
		}
		Model(const uInt& meshIndex) :
			m_pTexture(nullptr),
			m_TextureIndex(uInt(-1))
		{
			SetMesh(meshIndex);
		}
		Model(const uInt& meshIndex, const uInt& textureIndex) {
			SetMesh(meshIndex);
			SetTexture(textureIndex);
		}
		Model(const Mesh& newMesh) {
			SetMesh(newMesh);
		}
		Model(const Mesh& newMesh, const Texture& newTexture) {
			SetMesh(newMesh);
			SetTexture(newTexture);
		}
		Model(const Nt::String& meshFileName) {
			LoadMeshFromFile(meshFileName);
		}
		Model(const Nt::String& meshFileName, const Nt::String& textureFileName) {
			LoadMeshFromFile(meshFileName);
			LoadTextureFromFile(textureFileName);
		}
		Model(const Model& newModel) :
			m_MeshIndex(newModel.m_MeshIndex),
			m_TextureIndex(newModel.m_TextureIndex)
		{
			if (m_MeshIndex == uInt(-1))
				m_pMesh = (newModel.m_pMesh != nullptr) ? new Mesh(*newModel.m_pMesh) : nullptr;
			if (m_TextureIndex == uInt(-1))
				m_pTexture = (newModel.m_pTexture) ? new Texture(*newModel.m_pTexture) : nullptr;
		}
		~Model() {
			if (m_MeshIndex == uInt(-1) && m_pMesh != nullptr)
				delete(m_pMesh);
			if (m_TextureIndex == uInt(-1) && m_pTexture != nullptr)
				delete(m_pTexture);
		}

		void Write(std::ostream& stream) const override {
			Serialization::WriteAll(stream,
				m_Position, m_Origin, m_Angle,
				m_AngleOrigin, m_Size, m_Color,
				m_MeshIndex, m_TextureIndex);

			if (m_MeshIndex == uInt(-1))
				Serialization::WriteAll(stream, m_pMesh);

			if (m_TextureIndex == uInt(-1))
				Serialization::WriteAll(stream, m_pTexture);
		}
		void Read(std::istream& stream) override {
			Serialization::ReadAll(stream,
				m_Position, m_Origin, m_Angle,
				m_AngleOrigin, m_Size, m_Color,
				m_MeshIndex, m_TextureIndex);

			if (m_MeshIndex == uInt(-1))
				Serialization::ReadAll(stream, m_pMesh);

			if (m_TextureIndex == uInt(-1))
				Serialization::ReadAll(stream, m_pTexture);
		}
		constexpr uInt Sizeof() const noexcept override {
			return sizeof(*this);
		}
		constexpr uInt ClassType() const noexcept {
			return 0;
		}
		static ISerialization* New([[maybe_unused]] const uInt& classType) {
			return new Model;
		}

		void LoadTextureFromFile(const Nt::String& fileName) {
			if (m_TextureIndex == uInt(-1))
				SAFE_DELETE(&m_pTexture);
			else
				m_TextureIndex = uInt(-1);

			m_pTexture = new Nt::Texture(fileName);
		}
		void LoadMeshFromFile(const Nt::String& fileName) {
			if (m_MeshIndex == uInt(-1))
				SAFE_DELETE(&m_pMesh);
			else
				m_MeshIndex = uInt(-1);

			m_pMesh = new Nt::Mesh(fileName);
		}

		void Render(Renderer* pRenderer) const override {
			if (!IsVisible())
				return;
			if (!pRenderer)
				Raise("Renderer is nullptr");

			static Bool isTextureBinded = true;
			if (m_pTexture != nullptr) {
				Assert((!m_pTexture->IsCreated()), "Texture is not created");

				m_pTexture->Bind();
				if (!isTextureBinded)
					isTextureBinded = true;
			}
			else if (isTextureBinded) {
				glBindTexture(GL_TEXTURE_2D, 0);
				isTextureBinded = false;
			}

			const Float4D color = pRenderer->GetColor();
			pRenderer->SetColor(GetColor());
			pRenderer->MatrixWorldPush();
			pRenderer->Translate(GetPosition());
			pRenderer->Rotate(GetAngleOrigin());
			pRenderer->Translate(GetOrigin());
			pRenderer->Rotate(GetAngle());
			pRenderer->Render(m_pMesh);
			pRenderer->MatrixWorldPop();
			pRenderer->SetColor(color);
		}

		Model& operator = (const Model& newModel) {
			m_MeshIndex = newModel.m_MeshIndex;
			if (m_MeshIndex == uInt(-1))
				m_pMesh = (newModel.m_pMesh) ? new Mesh(*newModel.m_pMesh) : nullptr;

			m_TextureIndex = newModel.m_TextureIndex;
			if (m_TextureIndex == uInt(-1))
				m_pTexture = (newModel.m_pTexture) ? new Texture(*newModel.m_pTexture) : nullptr;
			return (*this);
		}

		uInt GetTextureIndex() const noexcept {
			return m_TextureIndex; 
		}
		uInt GetMeshIndex() const noexcept {
			return m_MeshIndex;
		}
		Texture* GetTexturePtr() const noexcept {
			return m_pTexture;
		}
		Mesh* GetMeshPtr() const noexcept {
			return m_pMesh;
		}

		void SetTexture(const uInt& textureIndex) {
			if (m_TextureIndex == uInt(-1))
				SAFE_DELETE(&m_pTexture);

			m_TextureIndex = textureIndex;
			m_pTexture = ResourceManager::GetTexture(textureIndex);
		}
		void SetTexture(const Texture& newTexture) {
			if (m_TextureIndex == uInt(-1))
				SAFE_DELETE(&m_pTexture);
			else
				m_TextureIndex = uInt(-1);

			m_pTexture = new Texture(newTexture);
		}
		void SetMesh(const uInt& meshIndex) {
			if (m_MeshIndex == uInt(-1))
				SAFE_DELETE(&m_pMesh);

			m_MeshIndex = meshIndex;
			m_pMesh = ResourceManager::GetMesh(meshIndex);
		}
		void SetMesh(const Mesh& newMesh) {
			if (m_MeshIndex == uInt(-1))
				SAFE_DELETE(&m_pMesh);
			else
				m_MeshIndex = uInt(-1);

			m_pMesh = new Mesh(newMesh);
		}

	protected:
		Mesh* m_pMesh;
		Texture* m_pTexture;
		uInt m_MeshIndex;
		uInt m_TextureIndex;
	};
}