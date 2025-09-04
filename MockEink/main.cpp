#include "MockEpaperDisplay.h"
#include "MockDataRetriever.h"

#include "Secrets.h"

#include "Application.h"
#include "GoogleScriptApi.h"

int WIDTH = 800;
int HEIGHT = 480;

int main(int argc, char **argv) {

    // Create an instance of the mock display with the specified dimensions.
    MockEpaperDisplay display(WIDTH, HEIGHT);

    MockDataRetriever retriever;
    Secrets secrets("../../../secrets.json");

    Application app(retriever, secrets);
    app.renderScreen(display);

    display.saveToFile("epaper_output.png");
    
    return 0;
}
