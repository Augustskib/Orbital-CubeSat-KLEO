#pragma once
#include <Fw/Time/Time.hpp>

namespace Components {
class PowerMonitorComponentBase {
public:
    explicit PowerMonitorComponentBase(const char*) {}
    virtual ~PowerMonitorComponentBase() = default;

    std::uint64_t clockUs = 0;
    std::uint64_t clockStepUs = 0;
    double systemWatts = 0;
    double solarWatts = 1;
    float consumption = 0;
    float generation = 0;

    void tick(std::uint64_t timeUs) {
        clockUs = timeUs;
        run_handler(0, 0);
    }
    void resetGeneration(std::uint64_t timeUs) {
        clockUs = timeUs;
        RESET_TOTAL_GENERATION_cmdHandler(0,0);
    }

    void resetConsumption(std::uint64_t timeUs) {
        clockUs = timeUs;
        RESET_TOTAL_POWER_cmdHandler(0,0);
    }


protected:
    virtual void run_handler(FwIndexType, U32) = 0;
    virtual void RESET_TOTAL_POWER_cmdHandler(FwOpcodeType, U32) = 0;
    virtual void RESET_TOTAL_GENERATION_cmdHandler(FwOpcodeType, U32) = 0;
    virtual void GET_TOTAL_POWER_cmdHandler(FwOpcodeType, U32) = 0;

    Fw::Time getTime() {
        Fw::Time result{clockUs};
        clockUs += clockStepUs;
        return result;
    }

    double sysVoltageGet_out(unsigned) { return 0; }
    double sysCurrentGet_out(unsigned) { return 0; }
    double solVoltageGet_out(unsigned) { return 0; }
    double solCurrentGet_out(unsigned) { return 0; }
    double sysPowerGet_out(unsigned) { return systemWatts; }
    double solPowerGet_out(unsigned) { return solarWatts; }

    void tlmWrite_TotalPowerConsumption(float v) { consumption = v; }
    void tlmWrite_TotalPowerGenerated(float v) { generation = v; }
    void log_ACTIVITY_LO_TotalPowerReset() {}
    void log_ACTIVITY_LO_TotalGenerationReset() {}
    void log_ACTIVITY_LO_TotalPowerConsumptionReading(float) {}
    void cmdResponse_out(unsigned, unsigned, Fw::CmdResponse) {}
};
}
