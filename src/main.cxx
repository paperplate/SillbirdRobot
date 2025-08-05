#include "main.hxx"

void Main::run() {
    led.setLevel(button.read());
}

extern "C" void app_main() {
    Main app;

    while (true) {
	app.run();
    }
}
