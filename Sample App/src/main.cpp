#include "eadkpp.h"
#include "palette.h"
#include <cstdio>

extern const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Sample App";
extern const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;

// Initial button color for the main window
EADK::Color buttonColor[5] = {EADK::Color(bt), EADK::Color(bt), EADK::Color(bt), EADK::Color(bt), EADK::Color(bt)};

// Button index
int selectedButtonIndex = 0;

const char* food = "";
const char* texteInitial = "Really ?! Do you REALLY like ";
// Declaration of a character string to store the complete text with the food variable
char phraseComplete[100];

// Definition of coordinates, button size and their values
int buttonDimensions[5][4] = {{290, 30, 15, 160}, {290, 30, 15, 100}, {290, 30, 15, 190}, {290, 30, 15, 130},{290, 30, 15, 70} };
const char* buttontext[5][1] = {{"Hamburger"}, {"Pasta"}, {"Kebab"}, {"Spaghetti"},{"Pizza"}};

// Dimensions, coordinates and values of the pop-up window buttons
int buttonDimensionsConfirm[2][4] = {{123, 20, 32, 145}, {123, 20, 165, 145}}; // Coordinates and dimensions
const char* buttontextConfirm[2][1] = {{"No"}, {"Yes"}}; // Values
EADK::Color buttonColorConfirm[2] = {EADK::Color(White), EADK::Color(White)}; // initial color
int selectedButtonIndexPopup = 0; // Index

// Update button colors based on selected index
void updateButtonColors() {
  for (int i = 0; i < 5; ++i) {
    int buttonWidth = buttonDimensions[i][0];
    int buttonHeight = buttonDimensions[i][1];
    int buttonX = buttonDimensions[i][2];
    int buttonY = buttonDimensions[i][3];
    const char* buttontxt = buttontext[i][0];

    EADK::Display::pushRectUniform(EADK::Rect(buttonX-1, buttonY-1, buttonWidth+2, buttonHeight+2),EADK::Color(border));

    EADK::Display::pushRectUniform(EADK::Rect(buttonX, buttonY, buttonWidth, buttonHeight),
                                   (i == selectedButtonIndex) ? EADK::Color(hover) : buttonColor[i]);
    EADK::Display::drawString(buttontxt, EADK::Point(buttonX+15, buttonY+10), false, EADK::Color(Black), (i == selectedButtonIndex) ? EADK::Color(hover) : buttonColor[i]);
  }
}

// Update button colors based on selected index for popup windows
void updateButtonColorsPopup() {
  for (int i = 0; i < 2; ++i) {
    int buttonWidth = buttonDimensionsConfirm[i][0];
    int buttonHeight = buttonDimensionsConfirm[i][1];
    int buttonX = buttonDimensionsConfirm[i][2];
    int buttonY = buttonDimensionsConfirm[i][3];
    const char* buttontxtconfirm = buttontextConfirm[i][0];

    EADK::Display::pushRectUniform(EADK::Rect(buttonX, buttonY, buttonWidth, buttonHeight),
                                   (i == selectedButtonIndexPopup) ? EADK::Color(Alert) : buttonColorConfirm[i]);
    EADK::Display::drawString(buttontxtconfirm, EADK::Point(buttonX+55, buttonY+5), false, EADK::Color(Black), (i == selectedButtonIndexPopup) ? EADK::Color(Alert) : buttonColorConfirm[i]);
  }
}

void drawmainwindow() {
  EADK::Display::pushRectUniform(EADK::Screen::Rect, EADK::Color(bg)); // Background
  EADK::Display::pushRectUniform(EADK::Rect(0, 0, 320, 18),nav); // Title Bar
  EADK::Display::drawString("SAMPLE APP", EADK::Point(125, 3), false, EADK::Color(White), EADK::Color(nav)); // Title

  EADK::Display::drawString("What do you prefer ?", EADK::Point(15, 37), false, EADK::Color(Black), EADK::Color(bg));
}

// "Wasted" window
void death() {
    EADK::Display::pushRectUniform(EADK::Screen::Rect, EADK::Color(Black));
    EADK::Timing::msleep(1500);
    while (true) {
        EADK::Display::drawString("WASTED", EADK::Point(140, 110), false, EADK::Color(red), EADK::Color(Black));
    }
}

// Popup window
int popup() {
    EADK::Display::pushRectUniform(EADK::Rect(27, 44, 266, 128), EADK::Color(Black));
    EADK::Display::drawString("Confirm", EADK::Point(135, 50), false, EADK::Color(White), EADK::Color(Black));
    EADK::Display::drawString(phraseComplete, EADK::Point(40, 100), false, EADK::Color(White), EADK::Color(Black));
    while (true) {
        updateButtonColorsPopup();
        EADK::Timing::msleep(100);
        EADK::Keyboard::State currentState = EADK::Keyboard::scan();
        if (currentState.keyDown(EADK::Keyboard::Key::Left)) {
            selectedButtonIndexPopup = (selectedButtonIndexPopup - 1 + 2) % 2;
            updateButtonColorsPopup();
        } else if (currentState.keyDown(EADK::Keyboard::Key::Right)) {
            selectedButtonIndexPopup = (selectedButtonIndexPopup + 1) % 2;
            updateButtonColorsPopup();
        } else if (currentState.keyDown(EADK::Keyboard::Key::OK)) {
            if (selectedButtonIndexPopup==0) {
                death();
            } else {
                EADK::Display::pushRectUniform(EADK::Screen::Rect, EADK::Color(White));
                drawmainwindow();
                updateButtonColors();
                break;
            }
        } else if (currentState.keyDown(EADK::Keyboard::Key::Back)) { // If back key pressed, close the app
            EADK::Display::pushRectUniform(EADK::Screen::Rect, EADK::Color(White));
            drawmainwindow();
            updateButtonColors();
            break;
        }
    }
    return 0;
}

void performButtonAction(int buttonIndex) {
    switch (buttonIndex) {
        case 0:
            EADK::Display::pushRectUniform(EADK::Rect(170, 35, 100, 18),bg);
            EADK::Display::drawString("Hamburger", EADK::Point(170, 37), false, EADK::Color(Black), EADK::Color(bg));
            break;
        case 1:
            EADK::Display::pushRectUniform(EADK::Rect(170, 35, 100, 18),bg);
            EADK::Display::drawString("Pasta", EADK::Point(170, 37), false, EADK::Color(Black), EADK::Color(bg));
            food="pasta";
            sprintf(phraseComplete, "%s%s", texteInitial, food);
            popup();
            break;
        case 2:
            EADK::Display::pushRectUniform(EADK::Rect(170, 35, 100, 18),bg);
            EADK::Display::drawString("Kebab", EADK::Point(170, 37), false, EADK::Color(Black), EADK::Color(bg));
            break;
        case 3:
            EADK::Display::pushRectUniform(EADK::Rect(170, 35, 100, 18),bg);
            EADK::Display::drawString("Spaghetti", EADK::Point(170, 37), false, EADK::Color(Black), EADK::Color(bg));
            break;
        case 4:
            EADK::Display::pushRectUniform(EADK::Rect(170, 35, 100, 18),bg);
            EADK::Display::drawString("Pizza", EADK::Point(170, 37), false, EADK::Color(Black), EADK::Color(bg));
            food="pizza";
            sprintf(phraseComplete, "%s%s", texteInitial, food);
            popup();
            break;
        default:
            break;
    }
}

int main(int argc, char * argv[]) {
  drawmainwindow();
  updateButtonColors();

  while (true) {
    EADK::Keyboard::State currentState = EADK::Keyboard::scan();
    // Button index update
    if (currentState.keyDown(EADK::Keyboard::Key::Up)) {
      selectedButtonIndex = (selectedButtonIndex - 2 + 5) % 5;
      updateButtonColors();
    } else if (currentState.keyDown(EADK::Keyboard::Key::Down)) {
      selectedButtonIndex = (selectedButtonIndex + 2) % 5;
      updateButtonColors();
    } else if (currentState.keyDown(EADK::Keyboard::Key::OK)) {
      performButtonAction(selectedButtonIndex);
    } else if (currentState.keyDown(EADK::Keyboard::Key::Home)) {
      break;
    }
    eadk_display_wait_for_vblank();
  }
  return 0;
}
