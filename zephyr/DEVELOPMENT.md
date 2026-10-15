# Development checkpoint

This personal repository preserves the initial F Prime + Zephyr learning scaffold.
It is not a validated flight-software release.

## Initial scaffold

- F Prime v4.3.0 with fprime-zephyr pinned as a Git submodule.
- West manifest requests Zephyr v4.3.0.
- The original build uses `rpi_pico2/rp2350a/m33` and a USB console overlay.
  This generic build target is not a complete PROVES V5c board definition.
- Previous linking failed from RP2350 RAM overflow (including a large telemetry
  store) and an unresolved `gettimeofday`. These issues are not fixed by this checkpoint.
- Component stacks still specify 64 KiB while the Zephyr thread-pool configuration
  specifies 8 KiB. These must be reconciled before deployment.

## INA260 module

`modules/kleo-ina260` is independently tested against the separate
`proves-core-reference` checkout at
`3541730be559127b592620f41b093e2f60f164c2`, using Zephyr 4.4.2.
It is not yet wired into the F Prime topology above.
Its README describes build/test commands, limitations and hardware validation still needed.

## Repository contents

Commit source, configuration, dependency manifests and tests. Keep downloaded
Zephyr workspaces, virtual environments, build products, telemetry captures and
FC flash backups outside Git. Flash backups may contain device configuration or keys.
The local build scripts currently use the developer's existing WSL reference
checkout and toolchain; they are not a fresh-machine setup installer.
