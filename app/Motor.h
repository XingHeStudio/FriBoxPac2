#ifndef MOTOR_H
#define MOTOR_H

typedef enum
{
    MOTOR_SPEED_LEVEL_1 = 1,
    MOTOR_SPEED_LEVEL_2,
    MOTOR_SPEED_LEVEL_3,
}MotorSpeedLevel_t;

void MotorSpeedSet(uint8_t ch, MotorSpeedLevel_t level);
void MotorInitialize(void);
void MotorPoll(void);
#endif

