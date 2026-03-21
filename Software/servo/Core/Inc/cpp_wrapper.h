// cpp_wrapper.h
#ifndef CPP_WRAPPER_H
#define CPP_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

// 需要的C语言头文件
#include "main.h"
#include "tim.h"
#include "gpio.h"

#ifdef __cplusplus
}
#endif


// C++头文件
#include "servo.h"
#include "pwm_tim.h"
#include "pwm_pca9685.h"



#include "app_main.h"

#endif /* __CPP_WRAPPER_H */