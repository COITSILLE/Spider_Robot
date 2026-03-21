#include "cpp_wrapper.h"

PWM_TIM pwm(&htim3, TIM_CHANNEL_1, 1000000);
PWM_PCA9685 pwm_pca;
Servo servo(&pwm);
Servo servo_pca(&pwm_pca);


void app_main(){
    pwm.init(50);
    servo.init(
        {0.5, -90},
        {2.5, 90}
    );
    servo.setDeg(0);
    HAL_Delay(10000);
    servo.setDeg(-90);
}

void loop(){
   
    // for (float angle = -90; angle <= 90; angle += 1){
    //     servo.setDeg(angle);
    //     HAL_Delay(10);
    // }
    // for (float angle = 90; angle >= -90; angle -= 1){
    //     servo.setDeg(angle);
    //     HAL_Delay(10);
    // }
}
