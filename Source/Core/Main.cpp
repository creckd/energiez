#include "CDPRGameInstance.h"

int main(int argc, char *argv[])
{
	CDPRGameInstance app;
	app.initApp();
	app.getRoot()->startRendering();
	app.closeApp();
	return 0;
}
