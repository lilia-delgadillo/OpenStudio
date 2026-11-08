#include <Processing.NDI.Lib.h>

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

class NDI {
public:

	NDI(const NDI&) = delete;
	NDI& operator=(const NDI&) = delete;

	NDI(NDI&&) = delete;
	NDI& operator=(NDI&&) = delete;

	static NDI& getNDI() {
		static NDI instance;
		return instance;
	}

	const NDIlib_v5* p_NDIlib;

    bool StartupNDIManager() {
        char* runtimePath = nullptr;
        size_t len = 0;

        // 1. Instantly check system variables for an installed NDI distribution
        if (_dupenv_s(&runtimePath, &len, "NDI_RUNTIME_DIR_V6") == 0 && runtimePath != nullptr) {
            SetDllDirectoryA(runtimePath);
            free(runtimePath);
        }
        else if (_dupenv_s(&runtimePath, &len, "NDI_RUNTIME_DIR_V5") == 0 && runtimePath != nullptr) {
            SetDllDirectoryA(runtimePath);
            free(runtimePath);
        }
        else {
            std::cerr << "Startup Check: NDI Runtime environment variables are missing." << std::endl;
            return false;
        }

        // 2. Safely call the library initialization using delay-load error safety hooks
        __try {
            if (!NDIlib_initialize()) {
                return false;
            }
            std::cout << "NDI System Core loaded and running at application start!" << std::endl;
            return true;
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            return false;
        }
    }

	void NDI_init() {
		p_NDIlib = NDIlib_v5_load();

		if (!p_NDIlib) {
			throw std::runtime_error("Please install the NDI runtime to use this appication.");
			return;
		}

		if (!p_NDIlib->initialize()) {
			throw std::runtime_error("Failed to initialize NDI.");
			return;
		}
	}

    const NDIlib_source_t* NDI_ListSources() {
        NDIlib_find_create_t find_config = { true, nullptr, nullptr }; // show local sources
        NDIlib_find_instance_t pNDI_find = NDIlib_find_create_v2(&find_config);

        if (NDIlib_find_wait_for_sources(pNDI_find, 5000)) {
            uint32_t no_sources = 0;

            // 3. Get the array of discovered sources
            const NDIlib_source_t* p_sources = NDIlib_find_get_current_sources(pNDI_find, &no_sources);

            std::cout << "Found " << no_sources << " source(s):" << std::endl;
            for (uint32_t i = 0; i < no_sources; i++) {
                std::cout << i + 1 << ". Name: " << p_sources[i].p_ndi_name
                    << " | IP: " << p_sources[i].p_url_address << std::endl;
            }
            return p_sources;
        }
        else {
            std::cout << "No NDI sources found or network timed out." << std::endl;
            return {};
        }

        // 4. Cleanup
        NDIlib_find_destroy(pNDI_find);
        NDIlib_destroy();
    }

private:
	NDI() {};
	~NDI() = default;
};