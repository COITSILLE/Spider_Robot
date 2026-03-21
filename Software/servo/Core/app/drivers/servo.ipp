#ifndef APP_DRIVERS_SERVO_IPP
#define APP_DRIVERS_SERVO_IPP

#include "servo.h"


template <typename PWMType>
Servo<PWMType>::Servo(PWM<PWMType>* pwm): pwm(pwm) {}

template <typename PWMType>
void Servo<PWMType>::init(ServoParamsEdge min, ServoParamsEdge max){
	this->min_period = min.period;
	this->min_angle = min.angle;
	this->max_angle = max.angle;
	this->angle_range = max.angle - min.angle;
	this->period_range = max.period - min.period;
}

template <typename PWMType>
void Servo<PWMType>::setDeg(float angle){
	if (angle < this->min_angle){
		angle = this->min_angle;
	}
	else if (angle > this->max_angle){
		angle = this->max_angle;
	}
	this->angle_range = this->max_angle - this->min_angle;
	float period = (angle - this->min_angle) / this->angle_range * this->period_range + this->min_period;
	this->pwm->setTime(period);
}

#endif /* APP_DRIVERS_SERVO_IPP */