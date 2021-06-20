#pragma once

class TGamepad
{
    public:
    TGamepad(){
        mCurrentInput = 0;
        mPreviousInput = 0;
    };

    virtual void readGamePad(void);
    u16 getCurrentInput(void);
    bool checkPressed(u16);
    bool checkHeld(u16);
    bool checkReleased(u16);

    protected:
    u16 mCurrentInput;
    u16 mPreviousInput;

    bool mFolded;
};

class TVirtualGamepad : public TGamepad
{
    public:
    TVirtualGamepad(){
        mCurrentInput = 0;
        mPreviousInput = 0;
    };
    ~TVirtualGamepad(){};

    virtual void readGamePad(void);
};