#include "gamepad.hpp"

#include <nitro.h>

void TGamepad::readGamePad(void){
    mPreviousInput = mCurrentInput;
    mCurrentInput = PAD_Read();
    mFolded = (bool)PAD_DetectFold();
}
u16 TGamepad::getCurrentInput(void){
    return mCurrentInput;
}
bool TGamepad::checkPressed(u16 button){
    return (button & mCurrentInput) != 0 && (button & mPreviousInput) == 0;
}
bool TGamepad::checkHeld(u16 button){
    return (button & mCurrentInput) != 0;
}
bool TGamepad::checkReleased(u16 button){
    return (button & mCurrentInput) == 0 && (button & mPreviousInput) != 0;
}

void TVirtualGamepad::readGamePad(void){
    mPreviousInput = mCurrentInput;
    mCurrentInput = 0;
}