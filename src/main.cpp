#include <Window.h>
#include <Timer.h>
#include <CubeScene.h>

int main() {
    Window window(800, 600, WindowType::Windowed);

    CubeScene cubeScene;
    Timer timer;
    while (!window.shouldQuit() && timer.secondsSinceStart() < 20) {
        cubeScene.update(timer.lap());
        cubeScene.render();
        window.present();
    }

    return 0;
}