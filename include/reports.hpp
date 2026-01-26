#pragma once

#include <set>
#include <ostream>

#include "factory.hpp"
#include "types.hpp"

class SpecificTurnsReportNotifier {
public:
    explicit SpecificTurnsReportNotifier(std::set<Time> turns);
    bool should_generate_report(Time t) const;

private:
    std::set<Time> turns_;
};

class IntervalReportNotifier {
public:
    explicit IntervalReportNotifier(TimeOffset interval);
    bool should_generate_report(Time t) const;

private:
    TimeOffset interval_;
};

void generate_structure_report(const Factory& f, std::ostream& os);
void generate_simulation_turn_report(
    const Factory& f,
    std::ostream& os,
    Time t
);
