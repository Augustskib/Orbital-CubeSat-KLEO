#include "PROVESFlightControllerReference/Components/PowerMonitor/PowerMonitor.hpp"

#include <cmath>
#include <Fw/Time/Time.hpp>

namespace Components {

PowerMonitor ::PowerMonitor(const char* const compName)
    : PowerMonitorComponentBase(compName),
      m_totalPower_mWh(0.0f),
      m_totalGeneration_mWh(0.0f),
      m_lastUpdateTime_s(0.0),
      m_lastGenerationTime_s(0.0) {}

PowerMonitor ::~PowerMonitor() {}

void PowerMonitor ::run_handler(FwIndexType portNum, U32 context) {
    this->sysVoltageGet_out(0);
    this->sysCurrentGet_out(0);
    F64 sysPowerW = this->sysPowerGet_out(0);

    this->solVoltageGet_out(0);
    this->solCurrentGet_out(0);
    F64 solPowerW = this->solPowerGet_out(0);

    // Preserve the existing formula; electrical interpretation
    // still needs verification against the measurement points.
    this->updatePower(sysPowerW + solPowerW);
    this->updateGeneration(solPowerW);
}

void PowerMonitor ::RESET_TOTAL_POWER_cmdHandler(
    FwOpcodeType opCode, U32 cmdSeq) {
    this->m_totalPower_mWh = 0.0f;
    this->m_lastUpdateTime_s = this->getCurrentTimeSeconds();
    this->log_ACTIVITY_LO_TotalPowerReset();
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void PowerMonitor ::RESET_TOTAL_GENERATION_cmdHandler(
    FwOpcodeType opCode, U32 cmdSeq) {
    this->m_totalGeneration_mWh = 0.0f;
    this->m_lastGenerationTime_s = this->getCurrentTimeSeconds();
    this->log_ACTIVITY_LO_TotalGenerationReset();
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void PowerMonitor ::GET_TOTAL_POWER_cmdHandler(
    FwOpcodeType opCode, U32 cmdSeq) {
    this->log_ACTIVITY_LO_TotalPowerConsumptionReading(
        this->m_totalPower_mWh);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

F64 PowerMonitor ::getCurrentTimeSeconds() {
    Fw::Time t = this->getTime();
    return static_cast<F64>(t.getSeconds()) +
           static_cast<F64>(t.getUSeconds()) / 1.0e6;
}

void PowerMonitor ::updatePower(F64 powerW) {
    if (!std::isfinite(powerW) || powerW < 0.0 || powerW > 1000.0) {
        this->m_hasPowerTimestamp = false;
        return;
    }

    F64 now_s = this->getCurrentTimeSeconds();

    if (!this->m_hasPowerTimestamp) {
        this->m_lastUpdateTime_s = now_s;
        this->m_hasPowerTimestamp = true;
        this->tlmWrite_TotalPowerConsumption(this->m_totalPower_mWh);
        return;
    }

    F64 dt_s = now_s - this->m_lastUpdateTime_s;

    if (dt_s > 0.0 && dt_s < 10.0) {
        F32 energyAdded_mWh =
            static_cast<F32>(powerW * (dt_s / 3600.0) * 1000.0);
        this->m_totalPower_mWh += energyAdded_mWh;
    }

    this->m_lastUpdateTime_s = now_s;
    this->tlmWrite_TotalPowerConsumption(this->m_totalPower_mWh);
}

void PowerMonitor ::updateGeneration(F64 powerW) {
    if (!std::isfinite(powerW) || powerW < 0.0 || powerW > 1000.0) {
        this->m_hasGenerationTimestamp = false;
        return;
    }

    F64 now_s = this->getCurrentTimeSeconds();

    if (!this->m_hasGenerationTimestamp) {
        this->m_lastGenerationTime_s = now_s;
        this->m_hasGenerationTimestamp = true;
        this->tlmWrite_TotalPowerGenerated(this->m_totalGeneration_mWh);
        return;
    }

    F64 dt_s = now_s - this->m_lastGenerationTime_s;

    if (dt_s > 0.0 && dt_s < 10.0) {
        F32 energyAdded_mWh =
            static_cast<F32>(powerW * (dt_s / 3600.0) * 1000.0);
        this->m_totalGeneration_mWh += energyAdded_mWh;
    }

    this->m_lastGenerationTime_s = now_s;
    this->tlmWrite_TotalPowerGenerated(this->m_totalGeneration_mWh);
}

}  // namespace Components
