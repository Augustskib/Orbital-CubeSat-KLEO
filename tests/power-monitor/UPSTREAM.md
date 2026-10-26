# Upstream source and local changes

## Source

Repository: https://github.com/Open-Source-Space-Foundation/proves-core-reference

Commit: 3541730be559127b592620f41b093e2f60f164c2

Original component:
PROVESFlightControllerReference/Components/PowerMonitor

The reference checkout had no tracked changes in this component
when provenance was checked.

The upstream license is preserved in upstream/LICENSE.

## Copied files

- PowerMonitor.cpp
- PowerMonitor.hpp
- PowerMonitor.fpp

The files under upstream/ are locally modified test copies.
They must not be described as unchanged upstream source.

## Local modifications

- Separate solar-generation and consumption timestamps.
- Reject non-finite power readings.
- Clear timestamp validity after an invalid reading.
- Resume accumulation only after establishing a fresh baseline.
- Use explicit validity flags to support startup at time zero.
- Maintain independent timestamps and validity flags during resets.

The existing system-power-plus-solar-power formula is preserved.
Its electrical interpretation remains to be verified.

## Test infrastructure

test.cpp, run.sh and stubs/ provide host-based regression tests.
The stubs replace framework interfaces for testing; they are not
production F Prime generated code.

These changes have not been integrated into flight firmware.