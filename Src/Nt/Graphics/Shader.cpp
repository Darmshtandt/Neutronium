// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Core/WinMinimal.h>

#include <Windows.h>
#include <GL/GLEW.h>
#include <GL/GL.h>
#include <fstream>

#include <Nt/Graphics/Shader.h>


namespace Nt {
	Shader::Shader() :
		m_fStrict(true)
	{
		m_Shaders[Types::VERTEX] = 0;
		m_Shaders[Types::COMPUTE] = 0;
		m_Shaders[Types::TESS_CONTROL] = 0;
		m_Shaders[Types::TESS_EVALUATION] = 0;
		m_Shaders[Types::GEOMETRY] = 0;
		m_Shaders[Types::FRAGMENT] = 0;
	}
	Shader::~Shader() noexcept {
		if (glDeleteShader == nullptr)
			return;

		for (const std::pair<Shader::Types, uInt> shaders : m_Shaders) {
			if (shaders.second != 0)
				glDeleteShader(shaders.second);
		}

		glDeleteProgram(m_ProgramID);
	}


	void Shader::Initialize() {
		if (glUniform1d == nullptr)
			Raise("GLEW not initialized");

		void* glUniforms[2][UNIFORM_TYPES_COUNT][UNIFORM_DIMENSIONS_COUNT] = {
			{
				{ glUniform1d, glUniform2d, glUniform3d, glUniform4d },
				{ glUniform1f, glUniform2f, glUniform3f, glUniform4f },
				{ glUniform1i, glUniform2i, glUniform3i, glUniform4i },
				{ glUniform1ui, glUniform2ui, glUniform3ui, glUniform4ui },
			},
			{
				{ glUniform1dv, glUniform2dv, glUniform3dv, glUniform4dv },
				{ glUniform1fv, glUniform2fv, glUniform3fv, glUniform4fv },
				{ glUniform1iv, glUniform2iv, glUniform3iv, glUniform4iv },
				{ glUniform1uiv, glUniform2uiv, glUniform3uiv, glUniform4uiv },
			},
		};

		memcpy(m_UniformFunctions, glUniforms, sizeof(Void*) * 2 * UNIFORM_TYPES_COUNT * UNIFORM_DIMENSIONS_COUNT);


		//void* glUniformsMatrix[2][UNIFORM_MATRIX_COUNT] = {
		//	{ glUniformMatrix2dv, glUniformMatrix2x3dv, glUniformMatrix2x4dv },
		//	{ glUniformMatrix2fv, glUniformMatrix2x3fv, glUniformMatrix2x4fv }
		//};

		//memcpy(m_UniformMatrixFunctions, glUniformsMatrix, sizeof(Void*) * 2 * UNIFORM_MATRIX_COUNT);
	}
	void Shader::Create() {
		if (glCreateProgram == nullptr)
			Raise("GLEW not initialized");

		if (m_ProgramID != 0)
			Raise("Shader already created");

		m_ProgramID = glCreateProgram();
		if (m_ProgramID == 0)
			Raise("Failed to create program");
	}
	void Shader::CompileFromFile(const Shader::Types& shaderType, const String& fileName) {
		if (m_ProgramID == 0)
			Raise("Shader not created");

		if (m_Shaders[shaderType] != 0) {
			glDetachShader(m_ProgramID, m_Shaders[shaderType]);
			glDeleteShader(m_Shaders[shaderType]);
		}

		std::ifstream file(fileName);
		if (!file.is_open())
			Raise("Failed to open file: " + fileName);

		std::string code(std::istreambuf_iterator<char>(file.rdbuf()), std::istreambuf_iterator<char>());
		file.close();

		const uInt shaderID = glCreateShader(uInt(shaderType));
		_CompileShader(shaderID, code.c_str());
		_VarifyResult(shaderID, shaderType, fileName);

		glAttachShader(m_ProgramID, shaderID);
	}

	void Shader::CompileCode(const Shader::Types& shaderType, const std::string& code) {
		const uInt shaderID = glCreateShader(uInt(shaderType));

		_CompileShader(shaderID, code.c_str());
		_VarifyResult(shaderID, shaderType);

		glAttachShader(m_ProgramID, shaderID);
	}

	void Shader::Link() {
		glLinkProgram(m_ProgramID);

		Int result;
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &result);

		if (result == GL_FALSE) {
			Int length;
			glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &length);

			GLchar* logMessage = new GLchar[length];
			glGetProgramInfoLog(m_ProgramID, length, &length, logMessage);

			Raise(logMessage);
		}
	}

	NT_API void Nt::Shader::SetUniformMatrix4x4(const String& semanticName, const uInt& type, const Matrix4x4& matrix) const {
		const Int location = _GetUniformLocation(semanticName);

		switch (type) {
		case UNIFORM_FLOAT:
			glUniformMatrix4fv(location, 1, GL_FALSE, matrix.Matrix);
			break;

		case UNIFORM_DOUBLE: {
			Double array2D[16];
			for (uInt i = 0; i < 16; ++i)
				array2D[i] = Double(matrix.Matrix[i]);

			glUniformMatrix4dv(location, 1, GL_FALSE, array2D);
			break;
		}
		case UNIFORM_INT:
			break;

		case UNIFORM_UINT:
			break;
		}
	}

	void Shader::Use() const noexcept {
		glUseProgram(m_ProgramID);
	}

	void Shader::Delete() const {
		if (glDeleteShader == nullptr)
			Raise("GLEW not initialized");

		for (const std::pair<Shader::Types, uInt> shaders : m_Shaders) {
			if (shaders.second != 0)
				glDeleteShader(shaders.second);
		}

		glDeleteProgram(m_ProgramID);
	}

	void Shader::EnableStrict() noexcept {
		m_fStrict = true;
	}
	void Shader::DisableStrict() noexcept {
		m_fStrict = false;
	}

	void Shader::UniformBlockBinding(const String& semanticName, const uInt& uniformBlockBinding) {
		const Int location = _GetUniformLocation(semanticName);
		glUniformBlockBinding(m_ProgramID, location, uniformBlockBinding);
	}
	void Shader::BindBufferBase(const Buffer& buffer, const uInt& index) {
		glBindBufferBase(uInt(buffer.GetTarget()), index, buffer.GetID());
	}

	uInt Shader::_GetUniformLocation(const String& SemanticName) const {
		const Int location = glGetUniformLocation(m_ProgramID, SemanticName.c_str());
		if (location == -1 && m_fStrict)
			Raise("Failed to locate " + SemanticName);

		return location;
	}

	uInt Shader::_CompileShader(const uInt& shaderID, const cString& code) const {
		glShaderSource(shaderID, 1, &code, nullptr);
		glCompileShader(shaderID);
		return shaderID;
	}

	void Shader::_VarifyResult(const uInt& shaderID, const Shader::Types& shaderType, const cString& fileName) {
		Int result;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE) {
			String errorMsg;
			if (fileName != nullptr) {
				errorMsg += "File name: ";
				errorMsg += fileName;
			}

			errorMsg += "\nShader type: ";

			switch (shaderType) {
			case Types::VERTEX:
				errorMsg += "Vertex shader";
				break;

			case Types::COMPUTE:
				errorMsg += "Compute shader";
				break;

			case Types::TESS_CONTROL:
				errorMsg += "Tess control shader";
				break;

			case Types::TESS_EVALUATION:
				errorMsg += "Tess evaluation shader";
				break;

			case Types::GEOMETRY:
				errorMsg += "Geometry shader";
				break;

			case Types::FRAGMENT:
				errorMsg += "Fragment shader";
				break;

			default:
				errorMsg += "Unknown";
				break;
			}

			errorMsg += "\n\n";

			Int length;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);

			if (length > 0) {
				GLchar* logData = new GLchar[length];
				glGetShaderInfoLog(shaderID, length, &length, logData);

				if (length > 0)
					errorMsg += logData;

				delete[](logData);
			}

			Raise(errorMsg);
		}

		m_Shaders[shaderType] = shaderID;
	}
}