#include "smathex_invkine.h"

ik_Status_t ik_3DOF_checkReach(Vec3_t target, ik_3DOF_Params_t* params){
    //TODO

}

Vec3_t ik_3DOF_get(Vec3_t target, ik_3DOF_Params_t params, ik_3DOF_RootConfig_t root_config){
    //TODO
    float a1 = s_atan2(target.y, target.x);

    float r = s_sqrt(target.x * target.x + target.y * target.y);
    float x_ = r - params.l1;
    float l = s_sqrt(x_ * x_ + target.z * target.z);

    float m3 = (l * l - params.l2 * params.l2 - params.l3 * params.l3) / (2 * params.l2 * params.l3);
    if (ABS(m3) > 1.0f){
        return (Vec3_t){NAN, NAN, NAN}; 
    }
    float a3 = root_config == DOWN ? s_acos(m3) : -s_acos(m3);
    
    float a2 = s_atan2(target.z, x_) - s_atan2(params.l3 * s_sin(a3), (params.l2 + params.l3 * s_cos(a3)));

    return (Vec3_t){a1, a2, a3};
}