//----------------------------------------------------------------//
//                      ADAM LASTOWKA		                      //
//                    AutumnLight Engine                          //
//----------------------------------------------------------------//
#include <GLFW/glfw3.h>
#include "Tesseract.h"

int main() {
	initialize();
	do {
		loop();
	} while (!shouldClose());
	killWindow();
}