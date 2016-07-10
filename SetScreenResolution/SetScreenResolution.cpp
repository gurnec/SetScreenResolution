// SetScreenResolution - a Windows utility to change the screen resolution via the command line
// Copyright (C) 2016 Christopher Gurnee
//
// Please refer to LICENSE.txt for details about distribution and modification

#include "stdafx.h"

int wmain(int argc, wchar_t* argv[])
{
    std::ios_base::sync_with_stdio(false);

    DEVMODE dm;
    dm.dmSize = sizeof dm;
    dm.dmDriverExtra = 0;

    try {
        if (argc != 3)
            throw std::runtime_error("wrong number of arguments - must specify two: width and height (in pixels)");

        if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm) == 0)
            throw std::runtime_error("can't retrieve current display settings");

        DWORD new_width  = boost::lexical_cast<DWORD>(argv[1]),
              new_height = boost::lexical_cast<DWORD>(argv[2]);
        if (dm.dmPelsWidth == new_width && dm.dmPelsHeight == new_height) {
            std::wcout << L"The display is already set to " << new_width << L"x" << new_height << L".\n";
            return 0;
        }

        dm.dmPelsWidth  = new_width;
        dm.dmPelsHeight = new_height;
        dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
        
        if (ChangeDisplaySettings(&dm, CDS_TEST) != DISP_CHANGE_SUCCESSFUL)
            throw std::runtime_error("unsupported resolution");

        switch (ChangeDisplaySettings(&dm, CDS_UPDATEREGISTRY)) {
        case DISP_CHANGE_SUCCESSFUL:
            break;
        case DISP_CHANGE_RESTART:
            std::wcout << L"Please restart your computer to complete this change.\n";
            break;
        default:
            throw std::runtime_error("resolution change failed");
        }

    } catch (const std::exception& e) {
        std::wcerr << L"Error: " << e.what() << L"\nPress any key to exit...";
        _getch();
        return 1;
    }

    std::wcout << L"Done.\n";
}
