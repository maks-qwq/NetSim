#pragma once

#include "factory.hpp"
#include "reports.hpp"

void simulate(
    Factory& f,
    Time max_time,
    std::ostream& os,
    const SpecificTurnsReportNotifier* specific = nullptr,
    const IntervalReportNotifier* interval = nullptr
);
