#ifndef __TRANSFORM_H
#define __TRANSFORM_H

typedef struct 
{
  float x;
	float y;
	float z;
}Vec3;
//腿部坐标系向量
typedef struct
{
	Vec3 translation;
  float e0[3];
	float e1[3];
	float e2[3];
} Transform;
//腿部到身体坐标系的变换 e0 e1 e2 皆为正交单位向量
Vec3 inverse_transform_point(const Transform* tf, const Vec3* body);
//tf 变换参数 腿部到身体
//body 身体坐标系的点






#endif

