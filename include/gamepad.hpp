#pragma once

class TGamepad
{
    public:
    TGamepad(){
        mCurrentInput = 0;
        mPreviousInput = 0;
    };
    ~TGamepad(){};

    virtual void ReadGamePad(void);
    u16 GetCurrentInput(void);
    bool CheckPressed(u16);
    bool CheckHeld(u16);
    bool CheckReleased(u16);

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

    virtual void ReadGamePad(void);
};