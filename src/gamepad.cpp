#include "gamepad.hpp"

#include <nitro.h>

void TGamepad::ReadGamePad(void){
    mPreviousInput = mCurrentInput;
    mCurrentInput = PAD_Read();
    mFolded = (bool)PAD_DetectFold();
}
u16 TGamepad::GetCurrentInput(void){
    return mCurrentInput;
}
bool TGamepad::CheckPressed(u16 button){
    return (button & mCurrentInput) != 0 && (button & mPreviousInput) == 0;
}
bool TGamepad::CheckHeld(u16 button){
    return (button & mCurrentInput) != 0;
}
bool TGamepad::CheckReleased(u16 button){
    return (button & mCurrentInput) == 0 && (button & mPreviousInput) != 0;
}

void TVirtualGamepad::ReadGamePad(void){
    mPreviousInput = mCurrentInput;
    mCurrentInput = 0;
}