#pragma once

#include <map>

#include <Nt/Core/Math/Vector.h>
#include <Nt/Core/Math/Matrix4x4.h>
#include <Nt/Graphics/Buffer.h>

#ifndef GLAPIENTRY
#	define GLAPIENTRY __stdcall
#endif

namespace Nt {
	enum UniformType : uInt {
		UNIFORM_DOUBLE,
		UNIFORM_FLOAT,
		UNIFORM_INT,
		UNIFORM_UINT,
		UNIFORM_TYPES_COUNT
	};
	enum UniformMatrixSize : uInt {
		UNIFORM_MATRIX_2X2,
		UNIFORM_MATRIX_2X3,
		UNIFORM_MATRIX_2X4,

		UNIFORM_MATRIX_3X2,
		UNIFORM_MATRIX_3X3,
		UNIFORM_MATRIX_3X4,

		UNIFORM_MATRIX_4X2,
		UNIFORM_MATRIX_4X3,
		UNIFORM_MATRIX_4X4,

		UNIFORM_MATRIX_COUNT
	};
	enum UniformDimension : uInt {
		UNIFORM_1D,
		UNIFORM_2D,
		UNIFORM_3D,
		UNIFORM_4D,
		UNIFORM_DIMENSIONS_COUNT
	};

	class Shader {
	public:
		enum class Types {
			VERTEX = 0x8B31,
			COMPUTE = 0x91B9,
			TESS_CONTROL = 0x8E88,
			TESS_EVALUATION = 0x8E87,
			GEOMETRY = 0x8DD9,
			FRAGMENT = 0x8B30,
		};

	public:
		NT_API Shader();
		NT_API ~Shader() noexcept;

		NT_API void Initialize();
		NT_API void Create();
		NT_API void CompileFromFile(const Shader::Types& shaderType, const String& fileName);
		NT_API void CompileCode(const Shader::Types& shaderType, const std::string& code);

		NT_API void Link();

		template <typename _Ty>
		void SetUniform1D(const String& semanticName, const _Ty& value) const {
			_SetUniform(semanticName, value);
		}

		template <typename _Ty>
		void SetUniform2D(const String& semanticName, const Vector2D<_Ty>& vector) const {
			_SetUniform(semanticName, vector);
		}

		template <typename _Ty>
		void SetUniform3D(const String& semanticName, const Vector3D<_Ty>& vector) const {
			_SetUniform(semanticName, vector);
		}

		template <typename _Ty>
		void SetUniform4D(const String& semanticName, const Vector4D<_Ty>& vector) const {
			_SetUniform(semanticName, vector);
		}


		template <typename _Ty>
		void SetUniformArray1D(const String& semanticName, const UniformType& type, const uInt& count, _Ty* pArray) const {
			_SetUniformArray(semanticName, type, count, pArray);
		}

		template <typename _Ty>
		void SetUniformArray2D(const String& semanticName, const UniformType& type, const uInt& count, Vector2D<_Ty>* pArray) const {
			_SetUniformArray(semanticName, type, count, pArray);
		}

		template <typename _Ty>
		void SetUniformArray3D(const String& semanticName, const UniformType& type, const uInt& count, Vector3D<_Ty>* pArray) const {
			_SetUniformArray(semanticName, type, count, pArray);
		}

		template <typename _Ty>
		void SetUniformArray4D(const String& semanticName, const UniformType& type, const uInt& count, Vector4D<_Ty>* pArray) const {
			_SetUniformArray(semanticName, type, count, pArray);
		}

		NT_API void SetUniformMatrix4x4(const String& semanticName, const uInt& type, const Matrix4x4& Mat) const;

		NT_API void Use() const noexcept;

		NT_API void Delete() const;

		NT_API void EnableStrict() noexcept;
		NT_API void DisableStrict() noexcept;

		NT_API void UniformBlockBinding(const String& semanticName, const uInt& uniformBlockBinding);
		NT_API void BindBufferBase(const Buffer& buffer, const uInt& index);

	private:
		Void* m_UniformFunctions[2][UNIFORM_TYPES_COUNT][UNIFORM_DIMENSIONS_COUNT] = { };
		Void* m_UniformMatrixFunctions[2][UNIFORM_TYPES_COUNT] = { };
		std::map<Shader::Types, uInt> m_Shaders;
		uInt m_ProgramID = 0;
		Bool m_fStrict;

	private:
		template <typename _Ty> requires std::is_arithmetic_v<_Ty>
		_CONSTEXPR23 UniformType _GetUniformType() const noexcept {
			if constexpr (std::is_same_v<_Ty, Double>)
				return UNIFORM_DOUBLE;
			else if constexpr (std::is_same_v<_Ty, Float>)
				return UNIFORM_FLOAT;
			else if constexpr (std::is_same_v<_Ty, Int> || std::is_same_v<_Ty, Bool>)
				return UNIFORM_INT;
			else if constexpr (std::is_same_v<_Ty, uInt>)
				return UNIFORM_UINT;
			else
				static_assert(false, "Error uniform type");
		}

		template <typename _Ty> requires (std::is_arithmetic_v<_Ty>)
		void _SetUniform(const String& semanticName, const _Ty& value) const {
			const auto Uniform = m_UniformFunctions[0][_GetUniformType<_Ty>()];
			const auto Uniform1D = reinterpret_cast<void (GLAPIENTRY*) (Int, _Ty)>(Uniform[UNIFORM_1D]);

			if constexpr (std::is_arithmetic_v<_Ty>)
				Uniform1D(_GetUniformLocation(semanticName), value);
		}
		template <class _Ty> requires (is_Vector_v<_Ty>)
		void _SetUniform(const String& semanticName, const _Ty& value) const {
			using _U = typename _Ty::ValueType;

			const auto Uniform = m_UniformFunctions[0][_GetUniformType<_U>()];
			const auto Uniform2D = reinterpret_cast<void (GLAPIENTRY*) (Int, _U, _U)>(Uniform[UNIFORM_2D]);
			const auto Uniform3D = reinterpret_cast<void (GLAPIENTRY*) (Int, _U, _U, _U)>(Uniform[UNIFORM_3D]);
			const auto Uniform4D = reinterpret_cast<void (GLAPIENTRY*) (Int, _U, _U, _U, _U)>(Uniform[UNIFORM_4D]);

			const Int location = _GetUniformLocation(semanticName);

			if constexpr (std::is_same_v<_Ty, Vector2D<_U>>)
				Uniform2D(location, value.x, value.y);
			else if constexpr (std::is_same_v<_Ty, Vector3D<_U>>)
				Uniform3D(location, value.x, value.y, value.z);
			else if constexpr (std::is_same_v<_Ty, Vector4D<_U>>)
				Uniform4D(location, value.x, value.y, value.z, value.w);
		}

		template <typename _Ty>
		void _SetUniformArray(const String& semanticName, const UniformType& type, const uInt& count, _Ty* pValue) const {
			const auto Uniform = 
				reinterpret_cast<void (*) (Int, Int, _Ty*)>(m_UniformFunctions[1][type][UNIFORM_1D]);

			const Int location = _GetUniformLocation(semanticName);
			Uniform(location, count, pValue);
		}

		NT_API uInt _GetUniformLocation(const String& semanticName) const;
		NT_API uInt _CompileShader(const uInt& ShaderID, const cString& code) const;
		NT_API void _VarifyResult(const uInt& ShaderID, const Shader::Types& shaderType, const cString& fileName = nullptr);
	};
}