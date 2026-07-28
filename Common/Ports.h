#pragma once
#include <iostream>
#include <array>;

using namespace std;

namespace Ports {
	constexpr uint16_t BusController = 5000;

	constexpr uint16_t BusMonitor = 5001;

	constexpr uint16_t BUS = 5100;


	constexpr array<uint16_t, 5> RemoteTerminals = { 7000,7001,7002,7003,7004 };
}