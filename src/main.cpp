#include "mainwindow.h"
#include <stdio.h>

int main() {
    MainWindow window;
    if (!window.init_window()) {
	printf("FAILED TO CREATE GLFW WINDOW!\n");
	return 1;
    }

    window.main_loop();
    window.cleanup();

    return 0;
}
