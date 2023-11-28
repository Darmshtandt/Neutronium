#pragma once

/*
All uniforms:

glUniform1f
glUniform1fv
glUniform1i
glUniform1iv
glUniform1d
glUniform1dv
glUniform1ui
glUniform1uiv

glUniform2f
glUniform2fv
glUniform2i
glUniform2iv
glUniform2d
glUniform2dv
glUniform2ui
glUniform2uiv

glUniform3f
glUniform3fv
glUniform3i
glUniform3iv
glUniform3d
glUniform3dv
glUniform3ui
glUniform3uiv

glUniform4f
glUniform4fv
glUniform4i
glUniform4iv
glUniform4d
glUniform4dv
glUniform4ui
glUniform4uiv

glUniformMatrix2fv
glUniformMatrix2dv
glUniformMatrix3fv
glUniformMatrix3dv
glUniformMatrix4fv
glUniformMatrix4dv

glUniformMatrix2x3dv
glUniformMatrix2x4dv
glUniformMatrix3x2dv
glUniformMatrix3x4dv
glUniformMatrix4x2dv
glUniformMatrix4x3dv

*/

namespace Nt {
	NT_API Bool IsGLEWInitialized() noexcept;

	class Shader {
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
		Shader() :
			m_fStrict(true)
		{ }
		~Shader() noexcept {
			if (IsGLEWInitialized())
				glDeleteProgram(m_Program);
		}


		void Initialize() {
			m_Program = glCreateProgram();
			if (m_Program == 0)
				Raise(L"Failed to create program");
		}
		void CompileFromFile(const Shader::Types& ShaderType, const String& FileName) {
			std::ifstream File(FileName);
			if (!File.is_open())
				Raise(String("Failed to open file: ") + FileName);

			std::string Code(std::istreambuf_iterator<char>(File.rdbuf()), std::istreambuf_iterator<char>());
			File.close();

			const uInt ShaderID = glCreateShader(uInt(ShaderType));
			_CompileShader(ShaderID, Code.c_str());
			_VarifyResult(ShaderID, ShaderType, FileName);

			glAttachShader(m_Program, ShaderID);
		}

		void CompileCode(const Shader::Types& ShaderType, const std::string& Code) {
			const uInt ShaderID = glCreateShader(uInt(ShaderType));
			_CompileShader(ShaderID, Code.c_str());
			_VarifyResult(ShaderID, ShaderType);

			glAttachShader(m_Program, ShaderID);
		}

		void Link() {
			glLinkProgram(m_Program);

			Int Result;
			glGetProgramiv(m_Program, GL_LINK_STATUS, &Result);
			if (Result == GL_FALSE) {
				Int Length;
				glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &Length);

				GLchar* LogMessage = new GLchar[Length];
				glGetProgramInfoLog(m_Program, Length, &Length, LogMessage);

				Raise(LogMessage);
			}
		}

		template <typename _Ty>
		void SetUniform(const String SemanticName, const uInt& Type, const _Ty& Value) const {
			const Int Location = GetUniformLocation(SemanticName);
			switch (Type) {
			case GL_FLOAT:
				glUniform1f(Location, Value);
				break;
			case GL_DOUBLE:
				glUniform1d(Location, Value);
				break;
			case GL_INT:
				glUniform1i(Location, Value);
				break;
			case GL_UNSIGNED_INT:
				glUniform1ui(Location, Value);
				break;
			}
		}
		template <typename _Ty>
		void SetUniformVec2(const String SemanticName, const uInt& Type, const Vector2D<_Ty>& Vec) const {
			const Int Location = GetUniformLocation(SemanticName);
			switch (Type) {
			case GL_FLOAT:
				glUniform2f(Location, float(Vec.x), float(Vec.y));
				break;
			case GL_DOUBLE:
				glUniform2d(Location, double(Vec.x), double(Vec.y));
				break;
			case GL_INT:
				glUniform2i(Location, int(Vec.x), int(Vec.y));
				break;
			case GL_UNSIGNED_INT:
				glUniform2ui(Location, uInt(Vec.x), uInt(Vec.y));
				break;
			}
		}
		template <typename _Ty>
		void SetUniformVec3(const String SemanticName, const uInt& Type, const Vector3D<_Ty>& Vec) const {
			const Int Location = GetUniformLocation(SemanticName);
			switch (Type) {
			case GL_FLOAT:
				glUniform3f(Location, float(Vec.x), float(Vec.y), float(Vec.z));
				break;
			case GL_DOUBLE:
				glUniform3d(Location, double(Vec.x), double(Vec.y), double(Vec.z));
				break;
			case GL_INT:
				glUniform3i(Location, int(Vec.x), int(Vec.y), int(Vec.z));
				break;
			case GL_UNSIGNED_INT:
				glUniform3ui(Location, uInt(Vec.x), uInt(Vec.y), uInt(Vec.z));
				break;
			}
		}
		template <typename _Ty>
		void SetUniformVec4(const String SemanticName, const uInt& Type, const Vector4D<_Ty>& Vec) const {
			const Int Location = GetUniformLocation(SemanticName);
			switch (Type) {
			case GL_FLOAT:
				glUniform4f(Location, float(Vec.x), float(Vec.y), float(Vec.z), float(Vec.w));
				break;
			case GL_DOUBLE:
				glUniform4d(Location, double(Vec.x), double(Vec.y), double(Vec.z), double(Vec.w));
				break;
			case GL_INT:
				glUniform4i(Location, int(Vec.x), int(Vec.y), int(Vec.z), int(Vec.w));
				break;
			case GL_UNSIGNED_INT:
				glUniform4ui(Location, uInt(Vec.x), uInt(Vec.y), uInt(Vec.z), uInt(Vec.w));
				break;
			}
		}
		void SetUniformMatrix4x4(const String SemanticName, const uInt& Type, const Matrix4x4& Mat) const {
			const Int Location = GetUniformLocation(SemanticName);
			switch (Type) {
			case GL_FLOAT:
				glUniformMatrix4fv(Location, 1, GL_FALSE, Mat.Matrix);
				break;
			case GL_DOUBLE:
			{
				GLdouble Matrix[16];
				for (uInt i = 0; i < 16; ++i)
					Matrix[i] = Mat.Matrix[i];
				glUniformMatrix4dv(Location, 1, GL_FALSE, Matrix);
				break;
			}
			case GL_INT:
				break;
			case GL_UNSIGNED_INT:
				break;
			}
		}

		void Use() const noexcept {
			glUseProgram(m_Program);
		}

		void EnableStrict() noexcept {
			m_fStrict = true;
		}
		void DisableStrict() noexcept {
			m_fStrict = false;
		}

	private:
		uInt m_Program;
		bool m_fStrict;

	private:
		uInt GetUniformLocation(const String& SemanticName) const {
			const Int Location = glGetUniformLocation(m_Program, SemanticName.c_str());
			if (Location == -1 && m_fStrict)
				Raise("Failed to locate " + SemanticName);
			return Location;
		}

		uInt _CompileShader(const uInt& ShaderID, LPCSTR Code) const {
			glShaderSource(ShaderID, 1, &Code, nullptr);
			glCompileShader(ShaderID);
			return ShaderID;
		}

		void _VarifyResult(const uInt& ShaderID, const Shader::Types& ShaderType, const cString& FileName = nullptr) {
			Int Result;
			glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
			if (Result == GL_FALSE) {
				String ErrorMsg;
				if (FileName) {
					ErrorMsg += "File name: ";
					ErrorMsg += FileName;
				}

				ErrorMsg += "\nShader type: ";
				switch (ShaderType) {
				case Types::VERTEX:
					ErrorMsg += "Vertex shader";
					break;
				case Types::COMPUTE:
					ErrorMsg += "Compute shader";
					break;
				case Types::TESS_CONTROL:
					ErrorMsg += "Tess control shader";
					break;
				case Types::TESS_EVALUATION:
					ErrorMsg += "Tess evaluation shader";
					break;
				case Types::GEOMETRY:
					ErrorMsg += "Geometry shader";
					break;
				case Types::FRAGMENT:
					ErrorMsg += "Fragment shader";
					break;
				default:
					ErrorMsg += "Unknown";
					break;
				}
				ErrorMsg += "\n\n";

				Int Length;
				glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &Length);
				if (Length > 0) {
					GLchar* Log = new GLchar[Length];
					glGetShaderInfoLog(ShaderID, Length, &Length, Log);

					ErrorMsg += Log;
					delete(Log);
				}
				Raise(ErrorMsg);
			}
		}
	};
}