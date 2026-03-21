#include "pwm_tim.h"

#ifdef __cplusplus
PWM_TIM::PWM_TIM(TIM_HandleTypeDef *PWMTIM, int Channel, int UNIT_FREQ, int APB1_FREQ)
: APB1_FREQ(APB1_FREQ),
  UNIT_FREQ(UNIT_FREQ),
  _PWM_TIM_(PWMTIM),
  Channel(Channel)
{
}
void PWM_TIM::init(int pwm_freq){
    if (pwm_freq <= 0 || pwm_freq > this->UNIT_FREQ || this->APB1_FREQ % this->UNIT_FREQ != 0){
        return;
    }
    __HAL_TIM_SET_PRESCALER(this->_PWM_TIM_, (this->APB1_FREQ / this->UNIT_FREQ) - 1);
    __HAL_TIM_SET_AUTORELOAD(this->_PWM_TIM_, this->UNIT_FREQ / pwm_freq - 1);

    HAL_TIM_PWM_Start(this->_PWM_TIM_, this->Channel);
    __HAL_TIM_SET_COMPARE(this->_PWM_TIM_, this->Channel, 0);
}
void PWM_TIM::setDuty(float duty){
    __HAL_TIM_SET_COMPARE(this->_PWM_TIM_, this->Channel, (ROUND(duty * __HAL_TIM_GET_AUTORELOAD(this->_PWM_TIM_))));
}
void PWM_TIM::setTime(float time_ms){
    uint32_t period = (__HAL_TIM_GET_AUTORELOAD(this->_PWM_TIM_) + 1) * 1000 / UNIT_FREQ;
    this->setDuty(time_ms / period);
}

#endif

