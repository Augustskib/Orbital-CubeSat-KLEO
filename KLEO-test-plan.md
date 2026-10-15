# KLEO test overview

Draft — October 14, 2026. Target: PROVES V5c.

No hardware available now. Start with simulation; bench tests come later.
Unchecked items are planned, not completed. Confirm optional hardware before adding its tests.

## Build and board setup

- [ ] Verify V5c pins, I2C addresses, SPI chip selects and interrupt lines against the schematic.
- [ ] Resolve inherited board conflicts: INA219/INA260, I2C/UART/watchdog pins and magnetometer selection.
- [ ] Check generated devicetree, enabled drivers and startup output states.
- [ ] Fix full-deployment RAM overflow, stack settings and time/linker errors.
- [ ] Verify flash size, partitions, bootloader and image offsets.
- [ ] Rebuild from pinned dependencies; generate the matching telemetry dictionary.

## Drivers and interfaces

For each driver: test normal operation, invalid input, missing device, transaction failure, timeout and recovery.

- [x] INA260: core tests and five Zephyr tests passed using simulated I2C.
- [ ] INA260: delayed readiness, deadline boundaries, startup failure, concurrent callers and repeated fault/recovery cycles.
- [ ] IMU and magnetometer: identity, signed axes, units, orientation mapping, stale data and saturation.
- [ ] Temperature sensors: negative readings, scaling, missing sensors and separate face/battery instances.
- [ ] Light sensors: dark/saturated readings, scaling and correct face selection.
- [ ] RTC: invalid time, date rollover, time changes, alarms and read/write errors.
- [ ] Internal temperature/ADC: scaling, invalid readings and channel selection.
- [ ] I2C/SPI: failed transfers, bus contention, device isolation and bounded timeouts.
- [ ] I2C mux/buffer: channel selection, repeated addresses, failed branches and recovery.
- [ ] GPIO expander: pin mapping, polarity, startup defaults and unrelated-bit preservation.
- [ ] USB/UART: fragmented data, disconnect/reconnect, buffer limits and backpressure.
- [ ] Radios: packet limits, configuration errors, TX/RX timeouts and retry limits. Confirm S-band scope.

## Power and outputs

- [ ] Sensor managers: one sampling owner; correct units, validity, age and instance labels.
- [ ] Power monitor: energy totals, variable sampling intervals, current direction and invalid samples.
- [ ] Load switches: correct output, duplicate requests, startup state and power-up delays.
- [ ] Thermal control: thresholds, hysteresis, missing temperature and heater timeout.
- [ ] Actuator drivers: bounded demand, stop command, power loss and failed writes.
- [ ] Antenna/burnwire: arming, inhibits, duplicate commands, abort and restart behaviour. Simulated outputs only.
- [ ] ADCS/detumbling: axis/sign conventions, invalid sensors, saturation and control timing.

## F′ services and application

- [ ] Startup: device order, missing optional devices and required-device failures.
- [ ] Modes: low power, system faults, lost communications and recovery; conflicting requests.
- [ ] Scheduler: correct rates, late ticks, queue saturation and overruns.
- [ ] Commands: registration, valid/unknown commands, malformed arguments and responses.
- [ ] Telemetry/events: IDs, values, timestamps, invalid data and rate limits.
- [ ] Health/watchdog/reset: missed replies, stalled tasks, reset reasons and escalation.
- [ ] Parameters: defaults, invalid values, corrupt storage and persistence.
- [ ] Buffers/queues: exhaustion, ownership, dropped messages and leaks.
- [ ] Sequencers: timing, cancellation, failed commands and missing/corrupt sequence files.
- [ ] Version/resource reporting: build identity, uptime, memory and load.

## Storage, payload and updates

- [ ] Filesystem: missing/full/corrupt storage, failed reads/writes and interrupted operations.
- [ ] Files/data products: partial transfers, integrity, duplicate IDs and buffer exhaustion.
- [ ] Payload/camera: malformed or partial responses, timeouts, capture failure and power loss.
- [ ] Firmware update: invalid image, partition bounds, failed write/verify and rollback.
- [ ] Persistent state: restart during a write; reject incomplete or incompatible data.

## Communications and ground system

- [ ] Framing: known packets, CRC errors, truncation, combined frames and sequence rollover.
- [ ] Routing: correct destination, invalid packets, delays, retries and dropped links.
- [ ] Authentication: valid/invalid credentials and replay rejection using test-only keys.
- [ ] Dictionary: firmware/channel/command agreement; detect mismatches.
- [ ] End-to-end: simulated sensor → F′ telemetry → ground display with correct units and validity.
- [ ] Commands: ground command → intended component → matching response.
- [ ] Yamcs: verify mappings with recorded packets; compare with known decoded results.

## System scenarios

- [ ] Cold start with all devices present, then with one missing.
- [ ] Sensor fails after good readings, then recovers with fresh data.
- [ ] One mux branch fails while other devices remain active.
- [ ] Low power and sensor failure occur together.
- [ ] Ground link disconnects while telemetry continues.
- [ ] Storage fills during payload activity.
- [ ] Clock changes during energy accumulation.
- [ ] Task stalls or the system restarts during an operation.
- [ ] Representative mission sequence and extended fault/recovery run.

## Hardware checks later

- [ ] Verify rails, clocks, reset states, connector pinouts, jumpers and inhibits.
- [ ] Measure I2C/SPI/UART signals and actual bus speed.
- [ ] Compare INA260 and other sensors against reference instruments.
- [ ] Check flash, storage and optional external RAM using designated test regions.
- [ ] Test power switches and actuators with dummy loads first.
- [ ] Verify watchdog timing and the backup/restore procedure.
- [ ] Test radios with a suitable RF setup.
- [ ] Confirm battery/EPS, solar faces, payload, harness and mechanical requirements with the team.

