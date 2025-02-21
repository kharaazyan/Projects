#ifndef ENUM_CLASS_STATUS_H
#define ENUM_CLASS_STATUS_H

#include <string>

class EnumClassStatus {
public:
    enum class Status {
        NONE, ARCA, VISA, PREMIUM, BLOCKED, ACTIVE, INACTIVE, MODER, ADMIN 
    };
    virtual ~EnumClassStatus() = default;

protected:
    std::string statusToString(Status status) const noexcept;
};

#endif
