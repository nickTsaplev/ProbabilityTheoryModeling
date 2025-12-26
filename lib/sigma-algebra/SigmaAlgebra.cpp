#include "SigmaAlgebra.hpp"

#include <algorithm>
#include <stdexcept>

namespace ptm {
SigmaAlgebra::SigmaAlgebra(const OutcomeSpace& omega, std::vector<Event> events) :
    omega_(omega), events_(std::move(events)) {
  for (const Event& event : events_) {
    if (event.GetSize() != omega_.GetSize())
      throw std::invalid_argument("Event size not equal to omega size");
  }
}

const OutcomeSpace& SigmaAlgebra::GetOutcomeSpace() const noexcept {
  return omega_;
}

const std::vector<Event>& SigmaAlgebra::GetEvents() const noexcept {
  return events_;
}

bool SigmaAlgebra::IsSigmaAlgebra() const {
  Event full_space = Event::Full(omega_.GetSize());

  bool contains_full_event = false;
  for (const Event& event : events_) {
    if (event == full_space) {
      contains_full_event = true;
      break;
    }
  }

  if (!contains_full_event)
    return false;

  for (const Event& event : events_) {
    Event complement = Event::Complement(event);
    bool found = false;

    for (const Event& local : events_) {
      if (local == complement) {
        found = true;
        break;
      }
    }

    if (!found) {
      return false;
    }
  }

  for (size_t i = 0; i < events_.size(); ++i) {
    for (size_t j = i; j < events_.size(); ++j) {
      Event events_union = Event::Unite(events_[i], events_[j]);
      bool found = false;

      for (const Event& e : events_) {
        if (e == events_union) {
          found = true;
          break;
        }
      }

      if (!found) {
        return false;
      }
    }
  }

  return true;
}

SigmaAlgebra SigmaAlgebra::Generate(const OutcomeSpace& omega, const std::vector<Event>& generators) {
  size_t size = omega.GetSize();

  std::vector<Event> events;
  events.push_back(Event::Empty(size));
  events.push_back(Event::Full(size));

  for (const auto & generator : generators) {
    if (std::ranges::find(events.begin(), events.end(), generator) == events.end()) {
      events.push_back(generator);
    }
  }

  bool run = true;
  while (run) {
    run = false;
    size_t start_size = events.size();

    for (size_t i = 0; i < start_size; ++i) {
      Event complement = Event::Complement(events[i]);
      if (std::ranges::find(events.begin(), events.end(), complement) == events.end()) {
        events.push_back(complement);
        run = true;
      }

      for (size_t j = 0; j < start_size; ++j) {
        Event event_union = Event::Unite(events[i], events[j]);
        if (std::ranges::find(events.begin(), events.end(), event_union) == events.end()) {
          events.push_back(event_union);
          run = true;
        }
      }
    }
  }

  return {omega, std::move(events)};
}
} // namespace ptm
