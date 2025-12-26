#include "Event.hpp"

#include <stdexcept>

namespace ptm {
    Event::Event(std::vector<bool> mask) : mask_(std::move(mask)) {}

    size_t Event::GetSize() const noexcept {
        return mask_.size();
    }

    bool Event::Contains(OutcomeSpace::OutcomeId id) const {
        if (id >= mask_.size()) {
            throw std::out_of_range("Outcome id is out of range");
        }
        
        return mask_[id];
    }

    const std::vector<bool>& Event::GetMask() const noexcept {
        return mask_;
    }

    Event Event::Empty(std::size_t n) {
        return Event(std::vector<bool>(n, false));
    }

    Event Event::Full(std::size_t n) {
        return Event(std::vector<bool>(n, true));
    }

    Event Event::Complement(const Event& e) {
        std::vector<bool> result_mask = e.mask_;
        for (size_t i = 0; i < e.mask_.size(); ++i) {
            result_mask[i] = !result_mask[i];
        }

        return Event(std::move(result_mask));
    }

    Event Event::Unite(const Event& a, const Event& b) {
        if (a.mask_.size() != b.mask_.size())
            throw std::out_of_range("Different mask sizes");

        std::vector<bool> result_mask(a.mask_.size());
        for (size_t i = 0; i < a.mask_.size(); ++i) {
            result_mask[i] = a.mask_[i] || b.mask_[i];
        }

        return Event(std::move(result_mask));
    }

    Event Event::Intersect(const Event& a, const Event& b) {
        if (a.mask_.size() != b.mask_.size())
            throw std::out_of_range("Different mask sizes");

        std::vector<bool> result_mask(a.mask_.size());
        for (size_t i = 0; i < a.mask_.size(); ++i) {
            result_mask[i] = a.mask_[i] && b.mask_[i];
        }

        return Event(std::move(result_mask));
    }

    bool Event::operator==(const Event& other) const noexcept {
        if (mask_.size() != other.mask_.size()) {
            return false;
        }

        for (size_t i = 0; i < mask_.size(); ++i) {
            if (mask_[i] != other.mask_[i]) {
                return false;
            }
        }

        return true;
    }
}