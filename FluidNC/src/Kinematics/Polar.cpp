#include "Polar.h"
#include <cmath>
#include "Machine/MachineConfig.h"

namespace Kinematics {
    void Polar::init_position() {
        // Position initialize karte waqt hum current motor state se Cartesian calculate karte hain
        float motors[MAX_N_AXIS];
        for (int i = 0; i < MAX_N_AXIS; i++) {
            motors[i] = get_motor_pos()[i];
        }
        
        _accumulated_theta = motors[X_AXIS];
        _last_theta = fmodf(_accumulated_theta, 360.0f);
        if (_last_theta < 0) _last_theta += 360.0f;
        _initialized = true;
    }

    bool Polar::transform_cartesian_to_motors(float* motors, float* cartesian) {
        float x = cartesian[X_AXIS];
        float y = cartesian[Y_AXIS];

        // Radius calculation
        float r = sqrtf(x * x + y * y);
        
        // Target Angle (0 to 360)
        float target_theta = atan2f(y, x) * 180.0f / M_PI;
        if (target_theta < 0) target_theta += 360.0f;

        if (!_initialized) {
            _accumulated_theta = target_theta;
            _last_theta = target_theta;
            _initialized = true;
        } else {
            // Continuous rotation logic: shortest path find karna
            float delta = target_theta - _last_theta;
            if (delta > 180.0f) delta -= 360.0f;
            else if (delta < -180.0f) delta += 360.0f;
            
            _accumulated_theta += delta;
            _last_theta = target_theta;
        }

        motors[X_AXIS] = _accumulated_theta; // Rotation Motor
        motors[Y_AXIS] = r;                  // Radius Motor
        
        // Agar Z axis ho toh usay waise hi rehne dein
        for (int i = 2; i < MAX_N_AXIS; i++) {
            motors[i] = cartesian[i];
        }

        return true;
    }

    bool Polar::cartesian_to_motors(float* target, plan_line_data_t* pl_data, float* position) {
        return transform_cartesian_to_motors(target, target);
    }

    void Polar::motors_to_cartesian(float* cartesian, float* motors, axis_t n_axis) {
        float theta_rad = motors[X_AXIS] * M_PI / 180.0f;
        float r = motors[Y_AXIS];

        cartesian[X_AXIS] = r * cosf(theta_rad);
        cartesian[Y_AXIS] = r * sinf(theta_rad);
        
        for (int i = 2; i < n_axis; i++) {
            cartesian[i] = motors[i];
        }
    }

    // Register this kinematic system so it can be used in config.yaml
    namespace {
        KinematicsFactory::InstanceBuilder<Polar> registration("Polar");
    }
}
