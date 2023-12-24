#pragma once

namespace Nt {
	enum GDIObjectTypes {
		GDIOBJECT_BITMAP = OBJ_BITMAP,
		GDIOBJECT_BRUSH = OBJ_BRUSH,
		GDIOBJECT_COLORSPACE = OBJ_COLORSPACE,
		GDIOBJECT_FONT = OBJ_FONT,
		GDIOBJECT_PAL = OBJ_PAL,
		GDIOBJECT_PEN = OBJ_PEN
	};

	class HandleDeviceContext {
	public:
		HandleDeviceContext() = default;
		~HandleDeviceContext() {
			Delete();
		}

		void Create(const String& driverName, const String& deviceName, const String& port, const DEVMODE* pDevMode) {
			if (pDevMode == nullptr)
				Raise("Dev mode pointer is null");

			const std::wstring wDriverName = driverName.wstr();
			const std::wstring wDeviceName = deviceName.wstr();
			const std::wstring wPort = port.wstr();

			m_Handle = CreateDC(wDriverName.c_str(), wDeviceName.c_str(), wPort.c_str(), pDevMode);
			if (m_Handle == nullptr)
				Raise("Failed to create device context");
		}
		void CreateInfoContext(const String& driverName, const String& deviceName, const String& port, const DEVMODE* pDevMode) {
			if (pDevMode == nullptr)
				Raise("Dev mode pointer is null");

			const std::wstring wDriverName = driverName.wstr();
			const std::wstring wDeviceName = deviceName.wstr();
			const std::wstring wPort = port.wstr();

			m_Handle = CreateIC(wDriverName.c_str(), wDeviceName.c_str(), wPort.c_str(), pDevMode);
			if (m_Handle == nullptr)
				Raise("Failed to create device context");
		}
		void CreateCompatible(const HandleDeviceContext& context) {
			m_Handle = CreateCompatibleDC(context.m_Handle);
			if (m_Handle == nullptr)
				Raise("Failed to create compatible device context");
		}

		// void ChangeDisplaySettings();
		// void ChangeDisplaySettingsEx();
		// void DeviceCapabilities();
		// void EnumDisplayDevices();
		// void EnumDisplaySettings();
		// void EnumDisplaySettingsEx();

		Bool Cancel() noexcept {
			return CancelDC(m_Handle);
		}

		Int DrawEscape(const Int& escapeFunctionID, const Int& inputDataSize, const Nt::String& inputData) const {
			if (!IsCreated())
				Raise("Context is not created");
			return ::DrawEscape(m_Handle, escapeFunctionID, inputDataSize, inputData.c_str());
		}

		Int EnumObjects(const Bool& isUseBrush, const GOBJENUMPROC& function, const Long& param) {
			if (!IsCreated())
				Raise("Context is not created");

			const Int objectType = (isUseBrush) ? OBJ_BRUSH : OBJ_PEN;
			return ::EnumObjects(m_Handle, objectType, function, param);
		}

		static Bool DeleteObject(const HGDIOBJ& object) {
			if (object == nullptr)
				Raise("Object handle is null");
			return ::DeleteObject(object);
		}
		Bool Delete() noexcept {
			const Bool result = DeleteDC(m_Handle);
			m_Handle = nullptr;
			return result;
		}

		HGDIOBJ GetCurrentObject(const GDIObjectTypes& objectType) const {
			if (!IsCreated())
				Raise("Context is not created");
			return ::GetCurrentObject(m_Handle, objectType);
		}
		Byte3D GetBrushColor() const {
			if (!IsCreated())
				Raise("Context is not created");
			return ColorRefToVector(::GetDCBrushColor(m_Handle));
		}
		Byte3D GetPenColor() const {
			if (!IsCreated())
				Raise("Context is not created");
			return ColorRefToVector(GetDCPenColor(m_Handle));
		}
		Bool GetOrgEx(Int2D* pPoint) const {
			if (!IsCreated())
				Raise("Context is not created");
			if (pPoint == nullptr)
				Raise("Point pointer is null");

			POINT point = (*pPoint);
			const Bool result = GetDCOrgEx(m_Handle, &point);
			(*pPoint) = point;
			return result;
		}

		Bool IsCreated() const noexcept {
			return (m_Handle != nullptr);
		}

	private:
		HDC m_Handle;
	};
}