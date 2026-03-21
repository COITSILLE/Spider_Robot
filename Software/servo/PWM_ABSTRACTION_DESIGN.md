# 零开销多态实现指南：以 Servo/PWM 为例

## 1. 背景与问题

在嵌入式实时系统中，我希望用基类接口编写上层业务逻辑（例如 Servo 依赖 PWM 接口），而底层具体实现（如 PWM_TIM、PWM_PCA9685）在编译时完全确定，整个程序是封闭的，不会在运行时引入新的派生类。因此，编译器理应能够自动将基类引用/指针上的调用优化为直接调用（甚至内联），既保留面向对象的抽象设计，又实现零运行时开销。

然而，C++ 的虚函数机制会强制引入间接开销，即使类型在编译时已知也无法自动消除；而模板虽然能实现零开销，却要求手动传递具体类型参数，破坏了接口的统一性和简洁性。

我需要的是一种既保持基类接口的抽象性，又能让编译器在编译时自动生成直接调用的方案，同时语法上不显式暴露具体类型，使上层代码看起来如同使用普通对象一样自然。

## 2. 解决方案演进11

### 方案0：虚函数基类（❌ 被否决）
```cpp
class PWMBase {
    virtual void init(int freq) = 0;
    virtual void setDuty(float val) = 0;
};

class Servo {
    PWMBase* pwm;  // 指针 → 虚表查找 → 开销
};
```
**问题**：需要指针/引用存储，虚函数调用间接层；无法在栈上分配。

### 方案1：CRTP/静态多态（方向正确）
把运行时多态改成编译期多态，让调用在编译阶段就绑定到具体实现，消除虚表开销。这个阶段的核心价值是“零开销调用路径”，但调用端通常仍需要显式写模板实参，易读性一般。

示例（调用端要显式写模板参数）：
```cpp
template <typename Derived>
class PWM{
    void setDeg(flaot deg) {static_cast<Derived*>this->setDeg(deg)}
}
```
```cpp
template <typename PWMType>
class Servo {
public:
    Servo(PWMType& pwm): pwm_(pwm) {}
    void setDeg(float deg);
};

PWM_TIM pwm_tim(...);
Servo<PWM_TIM> servo(pwm_tim);  // 显式模板参数
```

### 方案2：引入 CTAD，统一调用写法（体验升级）
通过类模板实参推导（CTAD），把调用写法统一成 `Servo servo(pwm);`。这样既保持静态多态的零开销，又避免在业务代码里显式写出模板参数。

示例（同样是模板类，但调用更自然）：
```cpp
template <typename PWMType>
Servo(PWMType&) -> Servo<PWMType>;
```
```cpp

PWM_TIM pwm_tim(...);
PWM_PCA9685 pwm_pca(...);

Servo servo1(pwm_tim);  // 推导为 Servo<PWM_TIM>
Servo servo2(pwm_pca);  // 推导为 Servo<PWM_PCA9685>
```

### 方案3：可选增强：编译期断言/约束（小优化）
为了更早暴露接口不一致问题，可以加一层编译期约束机制（不是零开销多态成立的前提）：
- 早期可用 `static_assert` + type_traits 做检测
- 最终采用 C++20 Concepts + `requires`，语义更直接、错误信息更清晰、可复用性更好

示例A（早期做法：`static_assert`）：
```cpp
template <typename T>
struct is_pwm_like {
    static constexpr bool value = requires(T& pwm, int f, float v) {
        pwm.init(f);
        pwm.setDuty(v);
        pwm.setTime(v);
    };
};

template <typename PWMType>
class Servo {
    static_assert(is_pwm_like<PWMType>::value, "PWMType must implement init/setDuty/setTime");
};
```

示例B（最终做法：Concepts + `requires`）：
```cpp
template <typename T>
concept PWMInterface = requires(T& pwm, int f, float v) {
    pwm.init(f);
    pwm.setDuty(v);
    pwm.setTime(v);
};

template <typename PWMType>
    requires PWMInterface<PWMType>
class Servo {
    // ...
};
```

最终形成的主链路是：虚函数基类 -> CRTP/静态多态 -> CTAD 统一调用。
编译期断言/约束属于可选增强层，不改变主链路的零开销本质。

## 3. 核心设计：静态多态 + CTAD（Concepts 为可选增强）

### 步骤1：定义可复用接口约束（可选） （`pwm.h`）

```cpp
#pragma once

#include "main.h"
#include "smath.h"

// C++20 Concept: 定义什么是合格的PWM类型
template <typename T>
concept PWMInterface = requires(T& pwm, int freq, float val) {
    pwm.init(freq);           // 必须有 init(int) 方法
    pwm.setDuty(val);         // 必须有 setDuty(float) 方法
    pwm.setTime(val);         // 必须有 setTime(float) 方法
};
```

**作用说明（可选增强）**：
- `concept` 关键字定义编译期类型约束
- `requires` 子句列出类型必须满足的操作
- 如果类型不满足，编译器拒绝实例化，给出清晰错误

### 步骤2：Servo模板设计 （`servo.h`）

```cpp
#pragma once
#include "pwm.h"

struct ServoParamsEdge {
    float period;
    float angle;
};

// Servo 模板类：核心是模板静态绑定；requires 约束可按需启用
template <typename PWMType>
    requires PWMInterface<PWMType>  // ← 编译期约束，不满足拒绝实例化
class Servo {
private:
    PWMType& pwm;  // ← 引用存储，支持栈分配，零额外开销
    
    float min_period = 500;
    float max_period = 2500;
    float min_angle = 0;
    float max_angle = 180;
    float angle_range = 180;
    float period_range = 2000;

public:
    Servo(PWMType& pwm);
    void init(ServoParamsEdge min, ServoParamsEdge max);
    void setDeg(float angle);
};
```

**设计亮点**：
- 模板静态绑定 + 引用持有，调用可在编译期直接解析
- `requires` 子句可选启用，用于更早发现接口不一致
- 存储具体类型的引用（不是虚基类指针）——零虚表开销
- 头文件只依赖 `pwm.h`，不知道 `pwm_tim.h` 或 `pwm_pca9685.h` 的存在——解耦设计

### 步骤3：模板定义与显式实例化 （`servo.cpp`）

```cpp
#include "servo.h"
#include "pwm_tim.h"
#include "pwm_pca9685.h"

// 模板定义（注意每个定义都要重复requires约束）
template <typename PWMType>
    requires PWMInterface<PWMType>
Servo<PWMType>::Servo(PWMType& pwm): pwm(pwm) {}

template <typename PWMType>
    requires PWMInterface<PWMType>
void Servo<PWMType>::init(ServoParamsEdge min, ServoParamsEdge max) {
    this->min_period = min.period;
    this->min_angle = min.angle;
    this->max_angle = max.angle;
    this->angle_range = max.angle - min.angle;
    this->period_range = max.period - min.period;
}

template <typename PWMType>
    requires PWMInterface<PWMType>
void Servo<PWMType>::setDeg(float angle) {
    if (angle < this->min_angle) angle = this->min_angle;
    else if (angle > this->max_angle) angle = this->max_angle;
    
    float period = (angle - this->min_angle) / this->angle_range 
                 * this->period_range + this->min_period;
    this->pwm.setTime(period);
}

// 显式实例化：告诉编译器为这两个具体类型生成代码
template class Servo<PWM_TIM>;
template class Servo<PWM_PCA9685>;
```

**优势**：
- 显式实例化在`.cpp`文件中，而非头文件——减少编译时间和对象文件膨胀
- 只为实际使用的PWM类型生成代码（无模板膨胀）

### 步骤4：具体PWM实现 （例如 `pwm_tim.h`）

```cpp
#pragma once
#include "pwm.h"
#include "main.h"

class PWM_TIM : public PWM {
public:
    PWM_TIM(TIM_HandleTypeDef* tim_handle);
    void init(int freq) override;
    void setDuty(float duty_pct);
    void setTime(float time_us) override;
    
private:
    TIM_HandleTypeDef* tim;
};
```

**注意**：`PWM_TIM` 仍然可以继承空的 `PWM` 基类（用于向下兼容或标记），但Servo不依赖多态。

### 步骤5：应用代码 （`app_main.cpp`）

```cpp
#include "servo.h"      // Servo 知道 PWMInterface
#include "pwm_tim.h"    // 具体实现 PWM_TIM

int main(void) {
    PWM_TIM pwm(&htim1);  // 创建具体 PWM 实现
    Servo servo(pwm);     // Servo 模板自动推导为 Servo<PWM_TIM>
    
    ServoParamsEdge min{500, 0}, max{2500, 180};
    servo.init(min, max);
    servo.setDeg(90);     // ← 编译時解析为直接调用 PWM_TIM::setTime()
    
    return 0;
}
```

**神奇之处**：`Servo servo(pwm);` 利用类模板实参推导（CTAD），编译器自动判断 `pwm` 的类型是 `PWM_TIM`。

## 4. 使用的C++特性清单

本设计里最关键的语言特性是 CTAD（类模板实参推导），因为它让调用侧保持统一写法 `Servo servo(pwm);`，同时仍保留静态多态和零开销。

| 特性 | C++版本 | 用途 |
|------|--------|------|
| **Templates** | C++98 | 参数化类型，支持泛型编程 |
| **Template Specialization** | C++98 | 为具体类型生成特化版本 |
| **Class Template Argument Deduction (CTAD)** | C++17 | `Servo servo(pwm)` 自动推导 `PWMType`，是调用层统一接口体验的关键 |
| **Concepts** | C++20 | 可选：编译期接口约束，提升可维护性 |
| **`requires` 子句** | C++20 | 可选：在模板参数中表达约束 |

## 5. 编译期vs运行期对比

### 虚函数方案（运行期开销）
```
Servo::setDeg()
  └─> pwm->setTime()  // 虚函数指针查表 (~4-8 cycles)
       └─> PWM_TIM::setTime()
```

### 本方案（零开销）
```
Servo<PWM_TIM>::setDeg()
  └─> pwm.setTime()  // 编译期已知为 PWM_TIM::setTime
       └─> 直接调用，编译器可内联 (0 cycles)
```

## 6. 编译流程

```
┌─────────────────────────────────────────┐
│ servo.h (Servo<PWMType> 声明)          │
│ + pwm.h (PWMInterface concept 定义)    │
└────────────┬────────────────────────────┘
             │
             ├─→ app_main.cpp: 实例化 Servo<PWM_TIM>
             │   └─→ servo.cpp 读取显式实例化指令
             │       └─→ 为 PWM_TIM 版本生成代码
             │
             ├─→ 另一个文件：可以实例化 Servo<PWM_PCA9685>
             │   └─→ servo.cpp 已预先显式实例化
             │
             └─→ 生成代码：
                 1. CTAD 推导具体 PWMType
                 2. 按实例化策略生成目标类型代码
                 3. 生成直接调用路径，无虚调用
                 4. 若启用 Concepts，再额外执行接口一致性检查
```

## 7. 关键设计原则

### 7.1 抽象洁癖（Clean Abstraction）
- ✅ **单一职责**：pwm.h 定义接口，servo.h 定义消费者，具体类分离
- ✅ **解耦**：servo.h 不含 `#include "pwm_tim.h"`，仅依赖 concept
- ✅ **可组合**：任何满足 `PWMInterface` 的类都可用于 Servo，无修改

### 7.2 零开销抽象
- ✅ 无指针间接、无虚表查找
- ✅ 编译器可内联所有PWM调用
- ✅ 二进制代码大小：等同于手写的非模板代码

### 7.3 可选增强：编译期错误检查
```cpp
// 如果你创建不满足接口的类：
class BadPWM {
    void setup() { }  // 没有 init, setDuty, setTime
};

Servo servo(BadPWM());  // ❌ 编译错误：
// error: use of 'BadPWM' requires that it satisfy 'PWMInterface'
// note: 'BadPWM' does not satisfy 'PWMInterface' because...
//   pwm.init(freq);  // ← required method not found
```

## 8. 完整例子：新增下层PWM类型（上层Servo逻辑不改）

要扩展到新的PWM类型（如 `PWM_SPI`），核心原则是：
- 上层 `Servo` 算法代码不变
- 只在下层实现 `init/setDuty/setTime`，满足 `PWMInterface`

```cpp
// 下层新增类型：pwm_spi.h
class PWM_SPI : public PWM {
public:
    void init(int freq) override;
    void setDuty(float val);
    void setTime(float us) override;  // 满足 PWMInterface
};

// 上层调用保持不变：app_main.cpp
#include "pwm_spi.h"
PWM_SPI pwm_spi(...);
Servo servo_spi(pwm_spi);  // CTAD 自动推导
```

实例化落点说明（与你的工程策略相关）：
- 若采用“使用点隐式实例化”（常见于模板定义可见场景），上层和 `servo.cpp` 都不用改。
- 若采用当前文档里的“非 header-only + 显式实例化”策略，`Servo` 算法仍然不用改，但可能需要在实例化汇总处补一行类型实例化声明。

本质上，新增类型的工作都在下层接口实现，`Servo` 的业务逻辑保持稳定。

## 9. 小结

| 维度 | 实现方式 |
|------|--------|
| 多态 | 编译期模板特化（静态多态）而非虚函数（动态多态） |
| 统一调用体验 | CTAD 让 `Servo servo(pwm)` 在不同 PWM 实现下保持同一写法 |
| 接口约束 | 可选增强：C++20 concepts 的 `requires` 子句 |
| 类型检查 | 编译期完成，错误信息清晰 |
| 开销 | 零运行时开销（虚函数零成本） |
| 代码组织 | 声明在`.h`，定义与实例化在`.cpp`（显式实例化） |
| 设计模式 | Curiously Recurring Template Pattern (CRTP) 的现代替代品 |

这就是用现代 C++（C++20）实现零开销多态的方法：Servo/PWM 只是一个可复用的示例，核心思想可迁移到任何“上层逻辑 + 多种底层实现”的场景。
