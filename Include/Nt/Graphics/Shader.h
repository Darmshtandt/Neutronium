#pragma once

/*
All uniforms:

m_GLEW.Uniform1F
m_GLEW.Uniform1FV
m_GLEW.Uniform1I
m_GLEW.Uniform1IV
m_GLEW.Uniform1D
m_GLEW.Uniform1DV
m_GLEW.Uniform1UI
m_GLEW.Uniform1uIV

m_GLEW.Uniform2F
m_GLEW.Uniform2FV
m_GLEW.Uniform2I
m_GLEW.Uniform2IV
m_GLEW.Uniform2D
m_GLEW.Uniform2DV
m_GLEW.Uniform2UI
m_GLEW.Uniform2uIV

m_GLEW.Uniform3F
m_GLEW.Uniform3FV
m_GLEW.Uniform3I
m_GLEW.Uniform3IV
m_GLEW.Uniform3D
m_GLEW.Uniform3DV
m_GLEW.Uniform3UI
m_GLEW.Uniform3uIV

m_GLEW.Uniform4F
m_GLEW.Uniform4FV
m_GLEW.Uniform4I
m_GLEW.Uniform4IV
m_GLEW.Uniform4D
m_GLEW.Uniform4DV
m_GLEW.Uniform4UI
m_GLEW.Uniform4uIV

m_GLEW.UniformMatrix2FV
m_GLEW.UniformMatrix2DV
m_GLEW.UniformMatrix3FV
m_GLEW.UniformMatrix3DV
m_GLEW.UniformMatrix4FV
m_GLEW.UniformMatrix4DV

m_GLEW.UniformMatrix2x3DV
m_GLEW.UniformMatrix2x4DV
m_GLEW.UniformMatrix3x2DV
m_GLEW.UniformMatrix3x4DV
m_GLEW.UniformMatrix4x2DV
m_GLEW.UniformMatrix4x3DV

*/

namespace Nt {
	enum class UniformType {
		FLOAT, DOUBLE, INT, UINT
	};
	enum class UniformDimension {
		_1D, _2D, _3D, _4D
	};

	class Shader {
	private:
		struct GLEWFunctions {
			PFNGLUNIFORM1FPROC Uniform1F;
			PFNGLUNIFORM1DPROC Uniform1D;
			PFNGLUNIFORM1IPROC Uniform1I;
			PFNGLUNIFORM1UIPROC Uniform1UI;

			PFNGLUNIFORM2FPROC Uniform2F;
			PFNGLUNIFORM2DPROC Uniform2D;
			PFNGLUNIFORM2IPROC Uniform2I;
			PFNGLUNIFORM2UIPROC Uniform2UI;

			PFNGLUNIFORM3FPROC Uniform3F;
			PFNGLUNIFORM3DPROC Uniform3D;
			PFNGLUNIFORM3IPROC Uniform3I;
			PFNGLUNIFORM3UIPROC Uniform3UI;

			PFNGLUNIFORM4FPROC Uniform4F;
			PFNGLUNIFORM4DPROC Uniform4D;
			PFNGLUNIFORM4IPROC Uniform4I;
			PFNGLUNIFORM4UIPROC Uniform4UI;
		};

	public:
		enum class Types {
			VERTEX = GL_VERTEX_SHADER,
			COMPUTE = GL_COMPUTE_SHADER,
			TESS_CONTROL = GL_TESS_CONTROL_SHADER,
			TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
			GEOMETRY = GL_GEOMETRY_SHADER,
			FRAGMENT = GL_FRAGMENT_SHADER,
		};

	public:
		NT_API Shader();
		NT_API ~Shader() noexcept;


		NT_API void Initialize();
		NT_API void CompileFromFile(const Shader::Types& ShaderType, const String& FileName);
		NT_API void CompileCode(const Shader::Types& ShaderType, const std::string& Code);

		NT_API void Link();

		template <typename _Ty>
		void SetUniform(const String SemanticName, const uInt& Type, const _Ty& Value) const {
			const Int Location = _GetUniformLocation(SemanticName);
			switch (Type) {
			case GL_FLOAT:
				m_GLEW.Uniform1F(Location, Value);
				break;
			case GL_DOUBLE:
				m_GLEW.Uniform1D(Location, Value);
				break;
			case GL_INT:
				m_GLEW.Uniform1I(Location, Value);
				break;
			case GL_UNSIGNED_INT:
				m_GLEW.Uniform1UI(Location, Value);
				break;
			}
		}
		template <typename _Ty>
		void SetUniformVec2(const String SemanticName, const uInt& Type, const Vector2D<_Ty>& Vec) const {
			const Int Location = _GetUniformLocation(SemanticName);
			switch (Type) {
			case GL_FLOAT:
				m_GLEW.Uniform2F(Location, float(Vec.x), float(Vec.y));
				break;
			case GL_DOUBLE:
				m_GLEW.Uniform2D(Location, double(Vec.x), double(Vec.y));
				break;
			case GL_INT:
				m_GLEW.Uniform2I(Location, int(Vec.x), int(Vec.y));
				break;
			case GL_UNSIGNED_INT:
				m_GLEW.Uniform2UI(Location, uInt(Vec.x), uInt(Vec.y));
				break;
			}
		}
		template <typename _Ty>
		void SetUniformVec3(const String SemanticName, const uInt& Type, const Vector3D<_Ty>& Vec) const {
			const Int Location = _GetUniformLocation(SemanticName);
			switch (Type) {
			case GL_FLOAT:
				m_GLEW.Uniform3F(Location, float(Vec.x), float(Vec.y), float(Vec.z));
				break;
			case GL_DOUBLE:
				m_GLEW.Uniform3D(Location, double(Vec.x), double(Vec.y), double(Vec.z));
				break;
			case GL_INT:
				m_GLEW.Uniform3I(Location, int(Vec.x), int(Vec.y), int(Vec.z));
				break;
			case GL_UNSIGNED_INT:
				m_GLEW.Uniform3UI(Location, uInt(Vec.x), uInt(Vec.y), uInt(Vec.z));
				break;
			}
		}
		template <typename _Ty>
		void SetUniformVec4(const String SemanticName, const uInt& Type, const Vector4D<_Ty>& Vec) const {
			const Int Location = _GetUniformLocation(SemanticName);
			switch (Type) {
			case GL_FLOAT:
				m_GLEW.Uniform4F(Location, float(Vec.x), float(Vec.y), float(Vec.z), float(Vec.w));
				break;
			case GL_DOUBLE:
				m_GLEW.Uniform4D(Location, double(Vec.x), double(Vec.y), double(Vec.z), double(Vec.w));
				break;
			case GL_INT:
				m_GLEW.Uniform4I(Location, int(Vec.x), int(Vec.y), int(Vec.z), int(Vec.w));
				break;
			case GL_UNSIGNED_INT:
				m_GLEW.Uniform4UI(Location, uInt(Vec.x), uInt(Vec.y), uInt(Vec.z), uInt(Vec.w));
				break;
			}
		}

		NT_API void SetUniformMatrix4x4(const String semanticName, const uInt& type, const Matrix4x4& Mat) const;


		NT_API void Use() const noexcept;

		NT_API void EnableStrict() noexcept;
		NT_API void DisableStrict() noexcept;

	private:
		uInt m_Program;
		Bool m_fStrict;
		GLEWFunctions m_GLEW;

	private:
		NT_API uInt _GetUniformLocation(const String& semanticName) const;
		NT_API uInt _CompileShader(const uInt& ShaderID, LPCSTR Code) const;
		NT_API void _VarifyResult(const uInt& ShaderID, const Shader::Types& ShaderType, const cString& FileName = nullptr);
	};
}