# Power monitor calculation tests

This suite tests a locally modified copy of the PROVES PowerMonitor C++ component using simulated time, sensor inputs and telemetry.

The latest run passed **22/22 checks across 11 scenarios**.

## Requirements

- Bash
- g++ with C++14 support
- WSL or a compatible Linux environment

No flight controller connection is required.

## Run

From the repository root:

```bash
bash tests/power-monitor/run.sh
```

The runner builds a fresh executable under `/tmp` and runs it. A compilation error or failed check returns a nonzero exit status. Temporary build directories currently remain after execution.

## Coverage

- Constant power over one hour
- Uneven sampling intervals
- Independent consumption and generation timestamps
- Independent resets
- NaN rejection without corrupting totals
- Recovery without counting missing-data intervals
- Startup at time zero
- Reset during sensor failure preserves the invalid state
- Accumulation resumes after recovery from reset during failure

There are **22 assertions across 11 scenarios**. The comparison tolerance is **0.1 mWh**.

## Changes under test

- Separate timestamps for consumption and solar generation
- Rejection of non-finite power readings
- A fresh timing baseline after invalid readings
- Explicit timestamp-valid flags so time zero is supported
- Independent reset handling
- Resets preserve timestamp validity rather than marking failed sensor data as valid

Non-finite rejection is implemented for NaN and infinity; the current suite explicitly tests NaN.

## Files

- `test.cpp`: test scenarios and assertions
- `run.sh`: build and test runner
- `stubs/`: simulated framework interfaces
- `upstream/`: locally modified copies of the upstream component
- `upstream/LICENSE`: upstream license
- `UPSTREAM.md`: source provenance and local changes
- `results-*.txt`: saved output from earlier test milestones; these may contain fewer checks than the current suite

## Limitations

These are host-based calculation tests, not hardware validation or full F Prime integration tests.

The existing system-power-plus-solar-power formula is preserved. Its physical meaning must be checked against the sensor measurement points and current-direction conventions.

NaN injection does not verify that physical sensor failures or stale readings are correctly propagated by the real drivers.

Recovery deliberately excludes unknown intervals rather than estimating their energy.

Infinity, range boundaries, clock jumps, long gaps, repeated failure cycles, reboot persistence and long-duration accuracy still require further testing. Reset-during-failure coverage currently checks one failure and recovery cycle for each accumulator.

The modified component has not been integrated into flight-controller firmware.
