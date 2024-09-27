#ifndef _LED_H

class Led {
    public:
        Led(int pin);
        void on();
        void off();
        void setFlickerInterval(int interval);
        void autoFlicker();
        void setFadeMount(int fadeMount);
        void setMaxBrightness();
        void setMinBrightness();
        void setBrightness();
        void autoFade();

    private:
        int mPin;
        int mFlickerInterval;
};

#endif