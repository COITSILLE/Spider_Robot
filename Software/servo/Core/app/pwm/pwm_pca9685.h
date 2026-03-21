#ifndef APP_CPP_PWM_PWM_PCA9685_H
#define APP_CPP_PWM_PWM_PCA9685_H

#include "pwm.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#ifdef __cplusplus
}

class PWM_PCA9685 : public PWM<PWM_PCA9685> {
public:
    PWM_PCA9685();
    void init(/*TODO*/);
    void setDuty(float duty);
    void setTime(float time_ms);
};

#endif /* __cplusplus */

#endif /* APP_CPP_PWM_PWM_PCA9685_H */