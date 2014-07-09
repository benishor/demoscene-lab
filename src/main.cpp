#include <Window.h>
#include <Timer.h>
#include <GLheaders.h>

void drawGreenTriangle() {
    // Clear screen
    glClearColor(0.05f, 0.0f, 0.1f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Move the camera away from the triangle so it enters into the viewing fustrum
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -3.0f);

    // Draw the triangle
    glBegin(GL_TRIANGLES);
    {
        glColor3f(0, 1, 0);
        glVertex3f(0, 1, 0);
        glVertex3f(-1, -1, 0);
        glVertex3f(1, -1, 0);

    }
    glEnd();
}

int main() {
    Window window(800, 600, WindowType::Windowed);

    Timer timer;
    while (timer.secondsSinceStart() < 3) {
        drawGreenTriangle();
        window.present();
    }

    return 0;
}