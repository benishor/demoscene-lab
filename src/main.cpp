#include <Window.h>
#include <Timer.h>
#include <CubeScene.h>

int main() {
    Window window(800, 600, WindowType::Windowed);

    CubeScene cubeScene;
    Timer timer;
    while (timer.secondsSinceStart() < 5) {
        window.pollEvents();
        cubeScene.update(timer.lap());
        cubeScene.render();
        window.present();
    }

    return 0;
}