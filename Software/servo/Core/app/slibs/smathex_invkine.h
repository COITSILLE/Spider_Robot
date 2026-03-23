#ifndef APP_SMATHEX_INVKINE_H
#define APP_SMATHEX_INVKINE_H

#include "smath.h"
#include "vector.h"


typedef struct{
    float l1;
    float l2;
    float l3;
    //TODO 
} ik_3DOF_Params_t;

typedef enum{
    UP,
    DOWN,
} ik_3DOF_RootConfig_t;

typedef enum{
    IK_REACHABLE,
    IK_UNREACHABLE,
} ik_Status_t;

ik_Status_t ik_3DOF_checkReach(Vec3_t target, ik_3DOF_Params_t params);
Vec3_t ik_3DOF_get(Vec3_t target, ik_3DOF_Params_t params, ik_3DOF_RootConfig_t root_config);
    //TODO;


#endif /* APP_SMATHEX_INVKINE_H */