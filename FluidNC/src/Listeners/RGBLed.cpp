#if 1
#    include "RGBLed.h"

#    include "System.h"

namespace Listeners {
    RGBLed::RGBLed(const char* name) : SysListener(name) {}

    void RGBLed::init() {
        log_info("Initializing RGB Led on gpio " << pin_ << ", index " << index_);

        if (this->pin_.defined()) {
            auto thepin = this->pin_.getNative(Pin::Capabilities::Native | Pin::Capabilities::Output);

            pixels_ = new Adafruit_NeoPixel(index_ + 1, thepin, NEO_GRB + NEO_KHZ800);
            pixels_->begin();
            pixels_->clear();
            for (uint16_t i = 0; i <= index_; i++) {
                pixels_->setPixelColor(i, pixels_->Color(32, 0, 0));  // Booting is dark red.
            }
            pixels_->show();

            sys.register_change_handler(handleChange, this);
        }
    }

    void RGBLed::handleChangeDetail(SystemDirty changes, const system_t& state) {
        if ((int(changes) & int(SystemDirty::State)) != 0) {
            uint32_t index = index_;
            int32_t value = -1;

            switch (state.state()) {
                case State::Idle:
                    value = this->idle;
                    break;
                case State::Alarm:
                    value = this->alarm;
                    break;
                case State::CheckMode:
                    value = this->checkMode;
                    break;
                case State::Homing:
                    value = this->homing;
                    break;
                case State::Cycle:
                    value = this->cycle;
                    break;
                case State::Hold:
                    value = this->hold;
                    break;
                case State::Jog:
                    value = this->jog;
                    break;
                case State::SafetyDoor:
                    value = this->safetyDoor;
                    break;
                case State::Sleep:
                    value = this->sleep;
                    break;
                case State::ConfigAlarm:
                    value = this->configAlarm;
                    break;
                default:
                    value = -1;
                    break;
            }

            // log_info("Updating RGB led to " << (value < 0 ? "none" : getColor(value)));

            if (value >= 0) {
                if (state.state() == State::Idle) {
                    // Idle state: Poori strip par static Rainbow lagayen
                    for (uint16_t i = 0; i <= index; i++) {
                        uint16_t hue = (i * 65535L) / (index == 0 ? 1 : index);
                        pixels_->setPixelColor(i, pixels_->ColorHSV(hue, 255, 255));
                    }
                } else {
                    // Baqi states: Single color poori strip par
                    for (uint16_t i = 0; i <= index; i++) {
                        pixels_->setPixelColor(i, pixels_->Color((value >> 16) & 0xFF, (value >> 8) & 0xFF, value & 0xFF));
                    }
                }
                pixels_->show();
            }
        }
    }

    // Configuration registration
    namespace {
        SysListenerFactory::InstanceBuilder<RGBLed> registration("rgbled");
    }
}
#endif
