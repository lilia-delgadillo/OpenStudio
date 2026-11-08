#include <iostream>
#include <windows.h> // Required for __try / __except
#include <QApplication>
#include "ui/MainWindow.h"

#include "engine/video/NDI/NDIMain.h"

// 1. Move all C++ objects into an isolated function so their destructors handle safely
int RunQtApplication(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

// 2. Keep main() completely clean of C++ objects so __try works without conflict
int main(int argc, char* argv[]) {
    __try {
        std::cout << "OpenStudio is starting up..." << std::endl;

        NDI::getNDI().StartupNDIManager();
        NDI::getNDI().NDI_init();
        NDI::getNDI().NDI_ListSources();
        // Call the helper function inside our protected block
        return RunQtApplication(argc, argv);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        DWORD exceptionCode = GetExceptionCode();

        std::cerr << "CRITICAL ERROR: The application crashed during initialization!" << std::endl;
        std::cerr << "Exception Code: 0x" << std::hex << exceptionCode << std::endl;

        // Show a native Windows pop-up dialog box to inspect the crash
        MessageBoxA(NULL,
            "OpenStudio failed to load a required system component (NDI). Please ensure the NDI Runtime is installed.",
            "Initialization Error",
            MB_ICONERROR | MB_OK);

        return 1;
    }
}
