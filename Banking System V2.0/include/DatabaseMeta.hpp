#ifndef DATABASE_META_H
#define DATABASE_META_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include "Utility.hpp"

struct Metadata {
    int number_of_app_launches {0};
    int count_of_account {0};
    int count_of_active_users {0};
    int count_of_inactive_users {0};
    int count_of_blocked_users {0};
    int count_of_transactions {0};
    double total_amount_money {0};
    int count_of_moderators {0};
    int count_of_administrators {0};
    int count_of_arca {0};
    int count_of_visa {0};
    int count_of_premium {0};
};

class DatabaseMeta {
private:
    std::fstream metainfo;
    std::stringstream metabuff;
    Metadata m_data;

    void clearMetainfo();

public:
    enum class Type {
        NONE, READ, WRITE
    };

    DatabaseMeta();
    ~DatabaseMeta();

    void sync(Type type = Type::NONE);
    Metadata& get_metainfo();

    inline void set_launch(int tmp)      { m_data.number_of_app_launches += tmp; }
    inline void set_account(int tmp)     { m_data.count_of_account += tmp; }
    inline void set_active(int tmp)      { m_data.count_of_active_users += tmp; }
    inline void set_inactive(int tmp)    { m_data.count_of_inactive_users += tmp; }
    inline void set_blocked(int tmp)     { m_data.count_of_blocked_users += tmp; }
    inline void set_transaction(int tmp) { m_data.count_of_transactions += tmp; }
    inline void set_money(double tmp)    { m_data.total_amount_money += tmp; }
    inline void set_moder(int tmp)       { m_data.count_of_moderators += tmp; }
    inline void set_admin(int tmp)       { m_data.count_of_administrators += tmp; }
    inline void set_arca(int tmp)        { m_data.count_of_arca += tmp; }
    inline void set_visa(int tmp)        { m_data.count_of_visa += tmp; }
    inline void set_premium(int tmp)     { m_data.count_of_premium += tmp; }
};

#endif
