#ifndef APP_CPP_PWM_PWM_PCA9685_H
#define APP_CPP_PWM_PWM_PCA9685_H

#include "pwm.h"
#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#ifdef __cplusplus
}

class PWM_PCA9685 : public PWM<PWM_PCA9685> {
public:
    PWM_PCA9685(I2C_HandleTypeDef *i2c,uint8_t addr, uint8_t channel, float osc_freq = 25000000.0f);
    void init(float pwm_freq);
    void setDuty(float duty);
    void setTime(float time_ms);

private:
    I2C_HandleTypeDef *hi2c_;
    uint8_t dev_addr_;
    uint8_t channel_;
    float osc_freq_;
    float pwm_freq_;
    uint8_t pre_scale_;

   static bool global_initialized_;

   static bool writeReg(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t reg, uint8_t val);
   static bool writeRegs(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t reg, uint8_t *data, uint8_t len);
   static bool readReg(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t reg, uint8_t val);
   }
#endif /* __cplusplus */

#endif /* APP_CPP_PWM_PWM_PCA9685_H */
