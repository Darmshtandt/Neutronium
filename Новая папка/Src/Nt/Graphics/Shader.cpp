#include <Windows.h>
#include <GL\GLEW.h>
#include <GL\GL.h>
#include <fstream>

// ============================================================================
//	Neutronium
// ----------------------------------------------------------------------------
#include <Nt/Core/Defines.h>
#include <Nt/Core/NtTypes.h>
#include <Nt/Core/String.h>
#include <Nt/Core/Utilities.h>
#include <Nt/Core/Log.h>

#include <Nt/Core/Math/Vectors.h>
#include <Nt/Core/Math/Matrix3x3.h>
#include <Nt/Core/Math/Matrix4x4.h>

#include <Nt/Graphics/Shader.h>



namespace Nt {
	Shader::Shader() :
		m_fStrict(true)
	{
	}
	Shader::~Shader() noexcept {
		if (glDeleteProgram != nullptr)
			glDeleteProgram(m_Program);
	}


	void Shader::Initialize() {
		if (glUniform1d == nullptr)
			Raise("GLEW not initialized");

		m_GLEW.Uniform1D = glUniform1d;
		m_GLEW.Uniform1F = glUniform1f;
		m_GLEW.Uniform1I = glUniform1i;
		m_GLEW.Uniform1UI = glUniform1ui;

		m_GLEW.Uniform2D = glUniform2d;
		m_GLEW.Uniform2F = glUniform2f;
		m_GLEW.Uniform2I = glUniform2i;
		m_GLEW.Uniform2UI = glUniform2ui;

		m_GLEW.Uniform3D = glUniform3d;
		m_GLEW.Uniform3F = glUniform3f;
		m_GLEW.Uniform3I = glUniform3i;
		m_GLEW.Uniform3UI = glUniform3ui;

		m_GLEW.Uniform4D = glUniform4d;
		m_GLEW.Uniform4F = glUniform4f;
		m_GLEW.Uniform4I = glUniform4i;
		m_GLEW.Uniform4UI = glUniform4ui;

		m_Program = glCreateProgram();
		if (m_Program == 0)
			Raise(L"Failed to create program");
	}
	void Shader::CompileFromFile(const Shader::Types& ShaderType, const String& FileName) {
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

	void Shader::CompileCode(const Shader::Types& ShaderType, const std::string& Code) {
		const uInt ShaderID = glCreateShader(uInt(ShaderType));
		_CompileShader(ShaderID, Code.c_str());
		_VarifyResult(ShaderID, ShaderType);

		glAttachShader(m_Program, ShaderID);
	}

	void Shader::Link() {
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

	void Shader::SetUniformMatrix4x4(const String SemanticName, const uInt& Type, const Matrix4x4& Mat) const {
		const Int Location = _GetUniformLocation(SemanticName);
		switch (Type) {
		case GL_FLOAT:
			glUniformMatrix4fv(Location, 1, GL_FALSE, Mat.Matrix);
			break;
		case GL_DOUBLE:
		{
			Double Matrix[16];
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

	void Shader::Use() const noexcept {
		glUseProgram(m_Program);
	}

	void Shader::EnableStrict() noexcept {
		m_fStrict = true;
	}
	void Shader::DisableStrict() noexcept {
		m_fStrict = false;
	}

	uInt Shader::_GetUniformLocation(const String& SemanticName) const {
		const Int Location = glGetUniformLocation(m_Program, SemanticName.c_str());
		if (Location == -1 && m_fStrict)
			Raise("Failed to locate " + SemanticName);
		return Location;
	}

	uInt Shader::_CompileShader(const uInt& ShaderID, LPCSTR Code) const {
		glShaderSource(ShaderID, 1, &Code, nullptr);
		glCompileShader(ShaderID);
		return ShaderID;
	}

	void Shader::_VarifyResult(const uInt& ShaderID, const Shader::Types& ShaderType, const cString& FileName) {
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
}