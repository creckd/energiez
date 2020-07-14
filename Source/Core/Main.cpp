#include "EnergiezApp.h"

int main(int argc, char *argv[])
{
	EnergiezApp app;
	app.initApp();
	app.getRoot()->startRendering();
	app.closeApp();
	return 0;
}
