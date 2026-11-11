#include <Processing.NDI.Lib.h>

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

//Backend of the NDI (Network Device Interface) system for OpenStudio
class NDI {
public:
	//Deletes the default constructors and = operations to ensure no copys can be made.

	NDI(const NDI&) = delete;
	NDI& operator=(const NDI&) = delete;
	NDI(NDI&&) = delete;
	NDI& operator=(NDI&&) = delete;

	//Get the singular static instance of the NDI backend.
	static NDI& getNDI() {
		static NDI instance;
		return instance;
	}

	const NDIlib_v5* p_NDIlib;

    bool StartupNDIManager();
	void NDI_init();
	const NDIlib_source_t* NDI_ListSources();

private:
	NDI() {};
	~NDI() = default;
};