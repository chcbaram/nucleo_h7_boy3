#include <gui/screen3_screen/Screen3View.hpp>
#ifndef SIMULATOR
#include "ap.h"
#endif

extern int pnesxMain(int index);
extern void touchgfxDisable(void);


static uint32_t pre_time;


Screen3View::Screen3View()
{

}

void Screen3View::setupScreen()
{
    Screen3ViewBase::setupScreen();

    pre_time = millis();
}

void Screen3View::tearDownScreen()
{
    Screen3ViewBase::tearDownScreen();
}

void Screen3View::handleKeyEvent(uint8_t key)
{
  //Screen3ViewBase::handleKeyEvent(key);

#ifndef SIMULATOR

  if (millis()-pre_time >= 200)
  {
    pre_time = millis();

    if(2 == key)
    {
      application().gotoScreen1ScreenSlideTransitionNorth();
    }
    if(3 == key)
    {
      application().gotoScreen4ScreenSlideTransitionSouth();
    }

    if(4 == key)
    {
      touchgfxDisable();
      pnesxMain(0);
    }
  }

#endif

}
