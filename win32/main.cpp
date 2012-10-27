#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include "AString.h"

USING_NS_CC;

static void _test()
{
	unsigned short key52[56];
	unsigned short key52d[56];
	memset(key52, 0xaa, sizeof(key52));
	memset(key52d, 0x55, sizeof(key52d));

	CAString::ideaGenKey("helloe", key52);
	const char* ins = "this is a string for testing idea";
	unsigned char outs[256];

	int ret = CAString::ideaCipher(ins, strlen(ins), outs, sizeof(outs), key52);
	CAString::ideaInvKey(key52, key52d);
	CAString::ideaCipher(outs, ret, outs, sizeof(outs), key52d);
}

//#define _HRES_
#if defined(_HRES_)
#define _DEVICE_WIDTH	960
#define _DEVICE_HEIGHT	640
#else
#define _DEVICE_WIDTH	800
#define _DEVICE_HEIGHT	480
#endif

#define _DEVICE_SCALE  (8.0f / 8.0f)

#define DEVICE_WIDTH	(int)(_DEVICE_WIDTH * _DEVICE_SCALE)
#define DEVICE_HEIGHT	(int)(_DEVICE_HEIGHT * _DEVICE_SCALE)

void postGameEvent(const char* key, const char* value)
{
	_Info("Game Event:%s=%s", key, value);
}

string getConfig()
{
	return "mg=bool:false";
}

string getLanguage()
{
	return "en";
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
	_test();

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    CCEGLView& eglView = CCEGLView::sharedOpenGLView();
    eglView.setViewName("CA.WIN32.View");
    eglView.setFrameSize(DEVICE_WIDTH, DEVICE_HEIGHT);
    // set the design resolution screen size, if you want to use Design Resoulution scaled to current screen, please uncomment next line.
    // eglView.setDesignResolutionSize(480, 320);

	GUARD_FUNCTION();
    return CCApplication::sharedApplication().run();
}
