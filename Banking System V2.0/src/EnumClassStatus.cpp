#include "../include/EnumClassStatus.hpp"

std::string EnumClassStatus::statusToString(Status status) const noexcept {
    switch (status) {
        case Status::NONE:     return "None";
        case Status::ARCA:     return "Arca";
        case Status::VISA:     return "Visa";
        case Status::PREMIUM:  return "Premium";
        case Status::BLOCKED:  return "Blocked";
        case Status::ACTIVE:   return "Active";
        case Status::INACTIVE: return "Inactive";
        case Status::MODER:    return "Moder";
        case Status::ADMIN:    return "Admin";
        default:               return "NULL";
    }
}
