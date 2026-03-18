#define ARM_MATH_CM3
#define __FPU_PRESENT 0
#include "arm_math.h"
#include "Transform.h"

//包含dsp库，deepseek推荐的计算数学库

Vec3 inverse_transform_point(const Transform* tf, const Vec3* body)
{
float32_t v[3];//说是dsp的浮点型
//	减去平移向量
	v[0]= body->x - tf->translation.x;
  v[1]= body->y - tf->translation.y;
  v[2]= body->z - tf->translation.z;
//逆旋转
Vec3 leg;

	arm_dot_prod_f32((float32_t*)tf->e0, v, 3, &leg.x);
//	点乘e0，存到legx里面
	arm_dot_prod_f32((float32_t*)tf->e1, v, 3, &leg.y);
	//	点乘e1，存到legy里面
	arm_dot_prod_f32((float32_t*)tf->e2, v, 3, &leg.z);
	//	点乘e2，存到legz里面
	return leg;
//	得到腿部坐标系的点
	
	
	
	
}




