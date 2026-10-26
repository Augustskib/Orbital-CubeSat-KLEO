#!/usr/bin/env bash
set -euo pipefail

HERE=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
BUILD=$(mktemp -d /tmp/kleo-power-build.XXXXXX)

g++ -std=c++14 -Wall -Wextra \
    -I"$HERE/stubs" \
    -I"$HERE/upstream" \
    "$HERE/upstream/PROVESFlightControllerReference/Components/PowerMonitor/PowerMonitor.cpp" \
    "$HERE/test.cpp" \
    -o "$BUILD/test-power"

echo "Test executable: $BUILD/test-power"
"$BUILD/test-power"