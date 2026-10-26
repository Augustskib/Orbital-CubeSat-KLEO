#include <cmath>
#include <iostream>
#include <limits>
#include "PROVESFlightControllerReference/Components/PowerMonitor/PowerMonitor.hpp"

int failures = 0;

void check(const char* name, double actual, double expected) {
    // Allow small rounding errors from the component's float totals.
    bool passed = std::isfinite(actual)
                  && std::abs(actual - expected) <= 0.1;
    std::cout << (passed ? "PASS: " : "FAIL: ")
              << name << " | actual=" << actual
              << " mWh, expected=" << expected << " mWh\n";
    if (!passed) ++failures;
}

int main() {
    {
        Components::PowerMonitor m("constant");
        // Start above zero to avoid the initialization sentinel.
        m.tick(1000000);
        for (unsigned i = 1; i <= 3600; ++i)
            m.tick((1ULL + i) * 1000000);

        check("1 W for 1 hour - combined total",
              m.consumption, 1000);
        check("1 W for 1 hour - solar total",
              m.generation, 1000);
    }

    {
        Components::PowerMonitor m("variable");
        std::uint64_t timeUs = 1000000;
        m.tick(timeUs);
        // Each pair is 4 seconds: 900 pairs = 1 hour.
        for (unsigned i = 0; i < 900; ++i) {
            timeUs += 500000;
            m.tick(timeUs);
            timeUs += 3500000;
            m.tick(timeUs);
        }
        check("Uneven intervals - combined total",
              m.consumption, 1000);
        check("Uneven intervals - solar total",
              m.generation, 1000);
    }

    {
        Components::PowerMonitor m("independent");
        m.systemWatts = 2;
        m.solarWatts = 1;
        // Simulate 1 ms between successive clock reads.
        m.clockStepUs = 1000;
        m.tick(1000000);
        for (unsigned i = 1; i <= 3600; ++i)
            m.tick((1ULL + i) * 1000000);

        // This checks the code's existing system + solar formula,
        // not whether that formula matches the electrical design.
        check("Independent totals - combined 3 W",
              m.consumption, 3000);
        check("Independent totals - solar 1 W",
              m.generation, 1000);
    }

        {
        Components::PowerMonitor m("reset-generation");
        m.systemWatts = 2;
        m.solarWatts = 1;

        m.tick(1000000);
        m.tick(5000000);
        m.resetGeneration(7000000);
        m.tick(9000000);

        // Solar reset at 7 seconds: count only the next 2 seconds.
        check("Solar reset restarts accumulation",
              m.generation, 1.0 * 2.0 / 3.6);

        // Consumption was not reset: retain all 8 seconds.
        check("Solar reset preserves consumption",
              m.consumption, 3.0 * 8.0 / 3.6);
    }

    {
        Components::PowerMonitor m("reset-consumption");
        m.systemWatts = 2;
        m.solarWatts = 1;

        m.tick(1000000);
        m.tick(5000000);
        m.resetConsumption(7000000);
        m.tick(9000000);

        // Consumption reset at 7 seconds: count the next 2 seconds.
        check("Consumption reset restarts accumulation",
              m.consumption, 3.0 * 2.0 / 3.6);

        // Solar was not reset: retain all 8 seconds.
        check("Consumption reset preserves solar",
              m.generation, 1.0 * 8.0 / 3.6);
    }

        {
        Components::PowerMonitor m("invalid-reading");
        m.systemWatts = 2;
        m.solarWatts = 1;

        m.tick(1000000);
        m.tick(5000000);

        const double savedConsumption = m.consumption;
        const double savedGeneration = m.generation;

        m.solarWatts = std::numeric_limits<double>::quiet_NaN();
        m.tick(6000000);

        check("Invalid reading preserves consumption",
              m.consumption, savedConsumption);
        check("Invalid reading preserves solar",
              m.generation, savedGeneration);

        // New check:
        // the first valid reading after a failure 
        // establishes a new starting time; accumulation resumes at the following reading.
                m.solarWatts = 1;
        m.tick(7000000);

        check("Recovery does not backfill consumption",
              m.consumption, savedConsumption);
        check("Recovery does not backfill solar",
              m.generation, savedGeneration);

        m.tick(9000000);

        check("Consumption resumes after recovery",
              m.consumption, savedConsumption + 3.0 * 2.0 / 3.6);
        check("Solar resumes after recovery",
              m.generation, savedGeneration + 1.0 * 2.0 / 3.6);
    }

        {
        Components::PowerMonitor m("startup-at-zero");
        m.systemWatts = 2;
        m.solarWatts = 1;

        m.tick(0);
        m.tick(4000000);

        check("Time-zero startup counts consumption",
              m.consumption, 3.0 * 4.0 / 3.6);
        check("Time-zero startup counts solar",
              m.generation, 1.0 * 4.0 / 3.6);
    }

        {
        Components::PowerMonitor m("solar-reset-during-failure");
        m.systemWatts = 2;
        m.solarWatts = 1;
        m.tick(1000000);
        m.tick(5000000);

        m.solarWatts = std::numeric_limits<double>::quiet_NaN();
        m.tick(6000000);
        m.resetGeneration(7000000);

        m.solarWatts = 1;
        m.tick(8000000);
        check("Solar reset during failure does not backfill",
              m.generation, 0.0);

        m.tick(10000000);
        check("Solar resumes after reset during failure",
              m.generation, 1.0 * 2.0 / 3.6);
    }

    {
        Components::PowerMonitor m("consumption-reset-during-failure");
        m.systemWatts = 2;
        m.solarWatts = 1;
        m.tick(1000000);
        m.tick(5000000);

        m.systemWatts = std::numeric_limits<double>::quiet_NaN();
        m.tick(6000000);
        m.resetConsumption(7000000);

        m.systemWatts = 2;
        m.tick(8000000);
        check("Consumption reset during failure does not backfill",
              m.consumption, 0.0);

        m.tick(10000000);
        check("Consumption resumes after reset during failure",
              m.consumption, 3.0 * 2.0 / 3.6);
    }
    
    std::cout << "\nFailed checks: " << failures << "\n";
    return failures ? 1 : 0;
}
