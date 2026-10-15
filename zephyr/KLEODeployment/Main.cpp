#include <KLEODeployment/Top/KLEODeploymentTopology.hpp>
#include <Os/Os.hpp>

int main() {
    Os::init();

    KLEO::TopologyState inputs{};
    inputs.uartDevice = nullptr;
    inputs.baudRate = 115200;

    KLEO::setupTopology(inputs);
    KLEO::startRateGroups();

    return 0;
}
