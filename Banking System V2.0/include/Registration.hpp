#ifndef REGISTRATION_HPP
#define REGISTRATION_HPP

#include <string>
#include <memory>
#include <cstdlib>
#include <unistd.h> 
#include <iostream> 

#include "Account.hpp"
#include "DatabaseData.hpp"
#include "DatabaseMeta.hpp"
#include "CubeEncoder.hpp"
#include "Utility.hpp"
#include "EnumClassStatus.hpp"
#include "LogService.hpp"

class RandGeneration {
public:
    std::string generator(DatabaseData &data);
};

class Registration : public Account, public RandGeneration {
public:
    Registration(DatabaseData &data, DatabaseMeta &meta, CubeEncoder &encode, LogService& _log);
    virtual ~Registration() = default;  

protected:
    DatabaseData &m_data;
    DatabaseMeta &m_meta;
    CubeEncoder &m_encode;
    LogService &log;
};

class RegArca : public Registration {
public:
    RegArca(DatabaseData &data, DatabaseMeta &meta, CubeEncoder &encode, LogService& _log);
};

class RegVisa : public Registration {
public:
    RegVisa(DatabaseData &data, DatabaseMeta &meta, CubeEncoder &encode, LogService& _log);
};

class RegPremium : public Registration {
public:
    RegPremium(DatabaseData &data, DatabaseMeta &meta, CubeEncoder &encode, LogService& _log);
};

class RegistrationFactory {
public:
    virtual ~RegistrationFactory() = default;
    virtual void createRegistration() const = 0;
};

class RegArcaFactory : public RegistrationFactory {
private:
    DatabaseData &data;
    DatabaseMeta &meta;
    CubeEncoder &encode;
    LogService& log;
public:
    RegArcaFactory(DatabaseData &d, DatabaseMeta &m, CubeEncoder &e, LogService& _log)
        : data(d), meta(m), encode(e), log(_log) {}
    void createRegistration() const override {
        RegArca r(data, meta, encode, log);
        (void)r;
    }
};

class RegVisaFactory : public RegistrationFactory {
private:
    DatabaseData &data;
    DatabaseMeta &meta;
    CubeEncoder &encode;
    LogService &log;
public:
    RegVisaFactory(DatabaseData &d, DatabaseMeta &m, CubeEncoder &e, LogService& _log)
        : data(d), meta(m), encode(e), log(_log) {}
    void createRegistration() const override {
        RegVisa r(data, meta, encode, log);
        (void)r;
    }
};

class RegPremiumFactory : public RegistrationFactory {
private:
    DatabaseData &data;
    DatabaseMeta &meta;
    CubeEncoder &encode;
    LogService &log;
public:
    RegPremiumFactory(DatabaseData &d, DatabaseMeta &m, CubeEncoder &e, LogService& _log)
        : data(d), meta(m), encode(e), log(_log) {}
    void createRegistration() const override {
        RegPremium r(data, meta, encode, log);
        (void)r;
    }
};

#endif // REGISTRATION_HPP
