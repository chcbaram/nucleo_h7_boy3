#include <gui/screen4_screen/Screen4View.hpp>

#ifndef SIMULATOR
#include "ap.h"
#endif


extern int pnesxMain(int index);
extern void touchgfxDisable(void);

static uint32_t pre_time;


Screen4View::Screen4View()
{

}

void Screen4View::setupScreen()
{
    Screen4ViewBase::setupScreen();

    pre_time = millis();
}

void Screen4View::tearDownScreen()
{
    Screen4ViewBase::tearDownScreen();
}

void Screen4View::handleKeyEvent(uint8_t key)
{
  //Screen4ViewBase::handleKeyEvent(key);

#ifndef SIMULATOR

  if (millis()-pre_time >= 200)
  {
    pre_time = millis();


    if(2 == key)
    {
      application().gotoScreen3ScreenSlideTransitionNorth();
    }

    if(4 == key)
    {
      touchgfxDisable();
      pnesxMain(1);
    }
  }

#endif

}
