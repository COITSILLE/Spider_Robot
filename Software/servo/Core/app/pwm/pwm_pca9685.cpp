#include "pwm_pca9685.h"
#include "smath.h"
bool PWM_PCA9685::global_initialized_ = false;

PWM_PCA9685::PWM_PCA9685(I2C_HandleTypeDef *i2c,uint8_t addr, uint8_t channel, float osc_freq)
: hi2c_(i2c), dev_addr_(addr), channel_(channel), osc_freq_(osc_freq), pwm_freq_(0), pre_scale_(0)
{
}
bool PWM_PCA9685::writeReg(uint8_t reg, uint8_t val)
{
    return HAL_I2C_Mem_Write(hi2c_, dev_addr_ << 1, reg, I2C_MEMADD_SIZE_8BIT, &val, 1, 100) == HAL_OK;
}
bool PWM_PCA9685::readReg(uint8_t reg, uint8_t *val)
{
    return HAL_I2C_Mem_Read(hi2c_, dev_addr_ << 1, reg, I2C_MEMADD_SIZE_8BIT, val, 1, 100) == HAL_OK;
}
void PWM_PCA9685::init(float pwm_freq)
{
    pwm_freq_ = pwm_freq;
    if (global_initialized_)
    {
        return true;
    }
    float prescale_val = osc_freq_ / (4096.0f * pwm_freq_);
    uint8_t prescale = (uint8_t)(prescale_val + 0.5f) - 1
    if (prescale < 3) prescale = 3;

    if (!writeReg(0x00, 0x00)) return false;

    uint8_t mode1;
    if (!readReg(0x00, &mode1)) return false;
    mode1 |= (1 << 4);
    if (!writeReg(0x00, mode1)) return false;

    if (!writeReg(0xFE, prescale)) return false;

    mode1 &= ~(1 << 4);
    mode1 |= (1 << 5);
    if (!writeReg(0x00, mode1)) return false;

    HAL_Delay(1);

    if (!writeReg(0x01, 0x04)) return false;
    pre_scale_ = prescale;
    global_initialized_ = true;
    return true;
}

void PWM_PCA9685::setDuty(float duty)
{
    if (duty < 0) duty = 0;
    if (duty > 1) duty = 1;

    uint16_t duty_val = (uint16_t)(duty * 4096.0f + 0.5f);
    if (duty_val > 4096) duty_val = 4096;

    uint8_t reg_base = 0x06 + 4 * channel_;
    uint8_t data[4] = {0, 0, 0, 0};   // data[0]=ON_L, [1]=ON_H, [2]=OFF_L, [3]=OFF_H


    if (duty_val == 0)
    {
    }
    else if (duty_val >= 4096)
    {
        data[1] = 0x10;
    }
    else
    {
        data[2] = duty_val & 0xFF;
        data[3] = (duty_val >> 8) & 0x0F;
    }

    writeRegs(reg_base, data, 4);
}

void PWM_PCA9685::setTime(float time_ms)
{
    if (pwm_freq_ <= 0) return;
    float period_ms = 1000.0f / pwm_freq_;
    float duty = time_ms / period_ms;
    if (duty < 0) duty = 0;
    if (duty > 1) duty = 1;
    setDuty(duty);
}
