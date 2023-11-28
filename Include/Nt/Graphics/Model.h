#pragma once

namespace Nt {
	class Model : public IObject, public ISerialization {
	public:
		Model() noexcept :
			m_pMesh(nullptr),
			m_MeshIndex(-1),
			m_pTexture(nullptr),
			m_TextureIndex(-1)
		{
		}
		Model(const uInt& MeshIndex) :
			m_pTexture(nullptr),
			m_TextureIndex(-1) 
		{
			SetMesh(MeshIndex);
		}
		Model(const uInt& MeshIndex, const uInt& TextureIndex) {
			SetMesh(MeshIndex);
			SetTexture(TextureIndex);
		}
		Model(const Mesh& NewMesh) {
			SetMesh(NewMesh);
		}
		Model(const Mesh& NewMesh, const Texture& NewTexture) {
			SetMesh(NewMesh);
			SetTexture(NewTexture);
		}
		Model(const Model& newModel) :
			m_MeshIndex(newModel.m_MeshIndex),
			m_TextureIndex(newModel.m_TextureIndex)
		{
			if (m_MeshIndex == uInt(-1))
				m_pMesh = (newModel.m_pMesh) ? new Mesh(*newModel.m_pMesh) : nullptr;
			if (m_TextureIndex == uInt(-1))
				m_pTexture = (newModel.m_pTexture) ? new Texture(*newModel.m_pTexture) : nullptr;
		}
		~Model() {
			if (m_MeshIndex == uInt(-1) && m_pMesh)
				delete(m_pMesh);
			if (m_TextureIndex == uInt(-1) && m_pTexture)
				delete(m_pTexture);
		}

		void Write(std::ostream& Stream) const override {
			Serialization::WriteAll(Stream,
				m_Position, m_Origin, m_Angle,
				m_AngleOrigin, m_Size, m_Color,
				m_MeshIndex, m_TextureIndex);

			if (m_MeshIndex == uInt(-1))
				Serialization::WriteAll(Stream, m_pMesh);

			if (m_TextureIndex == uInt(-1))
				Serialization::WriteAll(Stream, m_pTexture);
		}
		void Read(std::istream& Stream) override {
			Serialization::ReadAll(Stream,
				m_Position, m_Origin, m_Angle,
				m_AngleOrigin, m_Size, m_Color,
				m_MeshIndex, m_TextureIndex);

			if (m_MeshIndex == uInt(-1))
				Serialization::ReadAll(Stream, m_pMesh);

			if (m_TextureIndex == uInt(-1))
				Serialization::ReadAll(Stream, m_pTexture);
		}
		constexpr uInt Sizeof() const noexcept override {
			return sizeof(*this);
		}
		constexpr uInt ClassType() const noexcept {
			return 0;
		}
		static ISerialization* New(const uInt& ClassType) {
			return new Model;
		}

		void Render(Renderer* pRenderer) const override {
			if (!IsVisible())
				return;
			if (!pRenderer)
				Raise("Renderer is nullptr");

			static Bool IsTextureBinded = true;
			if (m_pTexture) {
				m_pTexture->CheckIfItsLoaded();
				m_pTexture->Set();
				if (!IsTextureBinded)
					IsTextureBinded = true;
			}
			else if (IsTextureBinded) {
				glBindTexture(GL_TEXTURE_2D, 0);
				IsTextureBinded = false;
			}

			const Float4D Color = pRenderer->GetColor();
			pRenderer->SetColor(GetColor());
			pRenderer->MatrixWorldPush();
			pRenderer->Translate(GetPosition());
			pRenderer->Rotate(GetAngleOrigin());
			pRenderer->Translate(GetOrigin());
			pRenderer->Rotate(GetAngle());
			pRenderer->Render(m_pMesh);
			pRenderer->MatrixWorldPop();
			pRenderer->SetColor(Color);
		}

		Model& operator = (const Model& newModel) {
			m_MeshIndex = newModel.m_MeshIndex;
			m_TextureIndex = newModel.m_TextureIndex;
			if (m_MeshIndex == uInt(-1))
				m_pMesh = (newModel.m_pMesh) ? new Mesh(*newModel.m_pMesh) : nullptr;
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

		void SetTexture(const uInt& TextureIndex) {
			if (m_TextureIndex == uInt(-1))
				delete(m_pTexture);
			m_TextureIndex = TextureIndex;
			m_pTexture = ResourceManager::GetTexture(TextureIndex);
		}
		void SetTexture(const Texture& NewTexture) {
			if (m_TextureIndex == uInt(-1))
				delete(m_pTexture);
			else
				m_TextureIndex = uInt(-1);
			m_pTexture = new Texture(NewTexture);
		}
		void SetMesh(const uInt& MeshIndex) {
			if (m_MeshIndex == uInt(-1))
				delete(m_pMesh);
			m_MeshIndex = MeshIndex;
			m_pMesh = ResourceManager::GetMesh(MeshIndex);
		}
		void SetMesh(const Mesh& NewMesh) {
			if (m_MeshIndex == uInt(-1))
				delete(m_pMesh);
			else
				m_MeshIndex = uInt(-1);
			m_pMesh = new Mesh(NewMesh);
		}

	protected:
		Mesh* m_pMesh;
		Texture* m_pTexture;
		uInt m_MeshIndex;
		uInt m_TextureIndex;
	};
}