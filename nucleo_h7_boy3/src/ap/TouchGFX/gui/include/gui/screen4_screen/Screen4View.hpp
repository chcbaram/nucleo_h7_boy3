#ifndef SCREEN4_VIEW_HPP
#define SCREEN4_VIEW_HPP

#include <gui_generated/screen4_screen/Screen4ViewBase.hpp>
#include <gui/screen4_screen/Screen4Presenter.hpp>

class Screen4View : public Screen4ViewBase
{
public:
    Screen4View();
    virtual ~Screen4View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleKeyEvent(uint8_t key);

protected:
};

#endif // SCREEN4_VIEW_HPP
