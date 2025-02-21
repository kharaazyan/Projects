#include "../include/DatabaseMeta.hpp"

DatabaseMeta::DatabaseMeta(){
    metainfo.open("metainfo.txt", std::ios::in | std::ios::out | std::ios::app);
    if (!metainfo.is_open()) {
        std::cerr << "Open meta file error!" << std::endl;
        exit(1);
    }
    metainfo.close();

    metainfo.open("metainfo.txt", std::ios::in | std::ios::out);
    metainfo.seekg(0, std::ios::end); 
    if (metainfo.tellg() == 0) {
        sync(Type::WRITE);
    } else {
        sync(Type::READ);
        set_launch(1);
    }
}

DatabaseMeta::~DatabaseMeta() {
    sync(Type::WRITE);
    if (metainfo.is_open()) {
        metainfo.close();
    }
}

Metadata& DatabaseMeta::get_metainfo(){
    return m_data;
}

void DatabaseMeta::sync(Type type){
    switch(type){
        case Type::READ:{
            std::string tmp;
            metainfo.seekg(0);  
            std::getline(metainfo, tmp);
            set_launch(Utility::myatoi(tmp));  
            std::getline(metainfo, tmp);
            set_account(Utility::myatoi(tmp));
            std::getline(metainfo, tmp);
            set_active(Utility::myatoi(tmp));
            std::getline(metainfo, tmp);
            set_inactive(Utility::myatoi(tmp));
            std::getline(metainfo, tmp);
            set_blocked(Utility::myatoi(tmp));
            std::getline(metainfo, tmp);
            set_arca(Utility::myatoi(tmp));
            std::getline(metainfo, tmp);
            set_visa(Utility::myatoi(tmp));
            std::getline(metainfo, tmp);
            set_premium(Utility::myatoi(tmp));
            std::getline(metainfo, tmp);
            set_transaction(Utility::myatoi(tmp));
            std::getline(metainfo, tmp);
            set_moder(Utility::myatoi(tmp));
            std::getline(metainfo, tmp);
            set_admin(Utility::myatoi(tmp));
            std::getline(metainfo, tmp);
            set_money(Utility::myatoi(tmp));
            metainfo.flush();
            return;
        }    

        case Type::WRITE:{
            metabuff.str("");
            metabuff.clear();
            clearMetainfo();
            metabuff << "Number of App Launches: " << m_data.number_of_app_launches << "\n";
            metabuff << "Count of Accounts: " << m_data.count_of_account << "\n";
            metabuff << "Count of Active Users: " << m_data.count_of_active_users << "\n";
            metabuff << "Count of Inactive Users: " << m_data.count_of_inactive_users << "\n";
            metabuff << "Count of Blocked Users: " << m_data.count_of_blocked_users << "\n";
            metabuff << "Count of Arca Users: " << m_data.count_of_arca << "\n";
            metabuff << "Count of Visa Users: " << m_data.count_of_visa << "\n";
            metabuff << "Count of Premium Users: " << m_data.count_of_premium << "\n";
            metabuff << "Count of Transactions: " << m_data.count_of_transactions << "\n";
            metabuff << "Count of Moderators: " << m_data.count_of_moderators << "\n";
            metabuff << "Count of Administrators: " << m_data.count_of_administrators << "\n";
            metabuff << "Total Amount of Money: " << m_data.total_amount_money << "\n";
            metainfo << metabuff.str();
            metainfo.flush();
            return;
        }
        
        default: exit(1);
    }
}

void DatabaseMeta::clearMetainfo(){
    metainfo.close();
    metainfo.open("metainfo.txt", std::ios::trunc | std::ios::out);
    metainfo.flush();
    metainfo.close();
    metainfo.open("metainfo.txt", std::ios::in | std::ios::out);
}
