#pragma once

#include "Kinematics.h"

namespace Kinematics {
    class Polar : public KinematicSystem {
        float _last_theta = 0.0f;
        float _accumulated_theta = 0.0f;
        bool  _initialized = false;

    public:
        Polar(const char* name) : KinematicSystem(name) {}

        void init() override {}
        void init_position() override;

        bool cartesian_to_motors(float* target, plan_line_data_t* pl_data, float* position) override;
        void motors_to_cartesian(float* cartesian, float* motors, axis_t n_axis) override;
        bool transform_cartesian_to_motors(float* motors, float* cartesian) override;

        bool canHome(AxisMask axisMask) override { return true; }
    };
}
