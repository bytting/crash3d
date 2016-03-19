#include "crash3d.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
#include <vector>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
    {
        std::vector<std::string> args;
        LPWSTR *szArgList;
        int argCount;

        szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
        if (szArgList == NULL)
        {
            MessageBox(NULL, L"Unable to parse command line", L"Error", MB_OK);
            return 1;
        }

        for(int i = 0; i < argCount; i++)
            args.push_back(szArgList[i]);

        LocalFree(szArgList);
#else
    int main(int argc, char *argv[])
    {
        std::vector<std::string> args;
        for(int i = 0; i < argc; i++)
            args.push_back(argv[i]);
#endif    
        Crash3d app;

        try
        {
            app.go(args);
        }
        catch( Ogre::Exception& e )
        {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
