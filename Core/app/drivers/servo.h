#ifndef SERVO_H
#define SERVO_H

#include "pwm.h"

#ifdef __cplusplus

struct ServoParamsEdge{
    float period;
    float angle;
};

template <typename PWMType>
    class Servo {
    public:
        Servo(PWM<PWMType>* pwm);

        void init(ServoParamsEdge min, ServoParamsEdge max);
        void setDeg(float angle);
    private:
        PWM<PWMType>* pwm;
        float min_period;
        float min_angle;
        float max_angle;
        float angle_range;
        float period_range;
    };

template <typename PWMType>
Servo(PWM<PWMType>*) -> Servo<PWMType>;

#ifndef APP_DRIVERS_SERVO_IPP
#include "servo.ipp"
#endif


#endif

#endif /* SERVO_H */