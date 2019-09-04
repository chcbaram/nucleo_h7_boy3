#include <gui/screen2_screen/Screen2View.hpp>
#ifndef SIMULATOR
#include "ap.h"
#endif

Screen2View::Screen2View()
{

}

void Screen2View::setupScreen()
{
  Screen2ViewBase::setupScreen();


  slider1.setValue(map(pwmRead(0), 0, 255, 0, 100));
}

void Screen2View::tearDownScreen()
{
    Screen2ViewBase::tearDownScreen();
}

void Screen2View::handleKeyEvent(uint8_t key)
{
  Screen2ViewBase::handleKeyEvent(key);

#ifndef SIMULATOR
  static uint32_t pre_time;

  if (millis()-pre_time >= 200)
  {
    pre_time = millis();

    if(6 == key)
    {
      slider1.setValue(slider1.getValue() + 10);
    }

    if(4 == key)
    {
      slider1.setValue(slider1.getValue() - 10);
    }

    pwmWrite(0, map(slider1.getValue(), 0, 100, 0, 255));
  }

#endif

}

void Screen2View::handleTickEvent()
{

}
