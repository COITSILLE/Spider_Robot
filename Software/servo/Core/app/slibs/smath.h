#pragma once

#include <stdint.h>

#ifdef __FPU_PRESENT
#include <math.h>
#endif

#define PI 3.14159265358979323846f
#define TWO_PI 6.28318530717958647692f
#define HALF_PI 1.57079632679489661923f
#define QUART_PI 0.785398163397448309616f
#define INV_PI 0.318309886183790671538f
#define INV_TWO_PI 0.159154943091895335769f
#define SQRT2 1.41421356237309504880f
#define RAD_TO_DEG 57.2957795130823208768f
#define DEG_TO_RAD 0.01745329251994329577f
#define NAN    __builtin_nanf("")
#define INF    __builtin_inff()

#define ABS(x) ((x) >= 0 ? (x) : -(x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ROUND(x) ((x) >= 0 ? (int)((x) + 0.5f) : (int)((x) - 0.5f))
#define CEIL_DIV(a, b) (((a) + (b) - 1) / (b))

#ifdef __cplusplus
extern "C" {
#endif

/** 
    Accuracy:
    [inv_sqrt] n=60000 max=4.58006389e-05 mean=3.88322327e-06 rmse=8.08861079e-06 p99=3.57047424e-05 worst_x=0.010009 fast=9.99535001 ref=9.99539581
    [sqrt] n=60000 max=3.60734531e-05 mean=1.06264713e-05 rmse=1.52922087e-05 p99=3.50191211e-05 worst_x=100.000000 fast=9.99996393 ref=10.00000000
    [sin] n=120000 max=1.24946933e-03 mean=5.35166139e-04 rmse=6.51059886e-04 p99=1.23671154e-03 worst_x=296.901306 fast=0.99853422 ref=0.99978368
    [cos] n=120000 max=1.24946940e-03 mean=5.35174746e-04 rmse=6.51066719e-04 p99=1.23671071e-03 worst_x=-295.330496 fast=0.99853449 ref=0.99978396
    [tan] n=219860 max=3.32670603e-03 mean=3.74713454e-04 rmse=8.13318766e-04 p99=3.07159119e-03 worst_x=0.785402 fast=1.00333385 ref=1.00000714
    [atan] n=120000 max=3.74167769e-03 mean=2.73219088e-04 rmse=5.55896264e-04 p99=2.98872106e-03 worst_x=6.925058 fast=1.42364286 ref=1.42738454
    [asin] n=120000 max=2.65374334e-03 mean=3.35483963e-04 rmse=6.39243309e-04 p99=2.42023841e-03 worst_x=-0.749998 fast=-0.84540519 ref=-0.84805893
    [acos] n=120000 max=2.65374334e-03 mean=3.35483963e-04 rmse=6.39243309e-04 p99=2.42023841e-03 worst_x=-0.749998 fast=2.41620151 ref=2.41885526
    [atan2] n=120000 max=3.74168047e-03 mean=2.35549961e-03 rmse=2.62679603e-03 p99=3.74107522e-03 worst=(x=83.298306, y=-576.237885) fast=2.99429001 ref=2.99803169
    
    Performance on 72MHz STM32F1 (time in us):
    inv_sqrt std:10.786 s:6.407 ratio:0.59x
    sqrt     std:8.380 s:7.295 ratio:0.87x
    sin      std:16.732 s:11.451 ratio:0.68x
    cos      std:17.211 s:12.217 ratio:0.71x
    tan      std:27.883 s:12.149 ratio:0.44x
    asin     std:27.217 s:15.460 ratio:0.57x
    acos     std:25.316 s:18.420 ratio:0.73x
    atan     std:21.185 s:8.930 ratio:0.42x
    atan2    std:24.769 s:12.634 ratio:0.51x

    inv_sqrt, sqrt, asin and acos can be faster if removing the 2nd iteration in s_inv_sqrt
    */
float s_mod(float x, float y);
float s_inv_sqrt(float x);
float s_sqrt(float x);
float s_sin(float x);
float s_cos(float x);
float s_tan(float x);
float s_asin(float x);
float s_acos(float x);
float s_atan(float x);
float s_atan2(float y, float x);

#ifdef __cplusplus
}
#endif


