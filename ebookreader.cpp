//
//  03.Качество кода
//  Тема 12.Философия хороших функций
//  Урок 04.Функция vs метод класса - ebook.cpp
//
//  created by ps on 01.05.2024
//

#include <iostream>
#include <numeric>
#include <string>
#include <vector>

//#include "log_duration.h"

using namespace std;

const int MAX_PAGES = 1'000;
const int MAX_USERS = 100'000;

class EbookReader {
public:
    EbookReader()
    : pages_to_users_(MAX_PAGES+1, 0)
    , users_to_pages_(MAX_USERS+1, -1){
    }
    
    void Read(int user, int page) {
        //Если пользователь уже читал книгу, удалить прошлую запись
        if(users_to_pages_[user] > 0) {
            int page = users_to_pages_[user];
            --pages_to_users_[page];
        } else {
            //новый пользователь
            ++total_users_;
        }
        ++pages_to_users_[page];
        users_to_pages_[user] = page;
    }
    
    double Cheer(int user) {
        //Не было ни одного события Read
        if(users_to_pages_[user] < 0) {
            return 0;
        } //Пользователь на данный момент единственный
        else if(total_users_ == 1) {
            return 1;
        }
        //Посчитать количество пользователей на page < user_page
        int users_behind = std::accumulate(pages_to_users_.begin(), 
                                           pages_to_users_.begin() + users_to_pages_[user],
                                           0, [](int sum, const int& users_on_page) {
                                                 return sum += users_on_page;
                                           });
        //Делить на общее число пользователей минус текущий user
        return 1.0 * users_behind / (total_users_ - 1);
    }
    
    
private:
    int total_users_ = 0;
    vector<int> pages_to_users_;
    vector<int> users_to_pages_;
};

int main() {
    //оптимизация потоков
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    EbookReader rdr;
    
    int n;
    cin >> n;
    
    for(int i = 0; i < n; ++i) {
        string command;
        cin >> command;
        
        if(command == "READ"s) {
            int user, page;
            
            cin >> user >> page;
            rdr.Read(user, page);
        } else if(command == "CHEER"s) {
            int user;
            
            cin >> user;
            cout << rdr.Cheer(user) << '\n';
        }
    }
}
    

//Testing:
/*
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n = 10e5; // = 10^6
    
    random_device rd;
    std::default_random_engine g(rd());
    
    std::uniform_int_distribution<int> rand_distr(0, 10e5);
    {
        LOG_DURATION("Using map") //-> 3570 ms >_<
        EbookReader_umap rdr;
        for(int i = 0; i < n; ++i) {
            int user_no = rand_distr(g);
            int cmd = user_no % 2;
            
            if(cmd == 0) {
                rdr.Read(user_no, user_no % 1001);
            } else {
                rdr.Cheer(user_no);
            }
        }
    }
    
    {
        LOG_DURATION("Using 1 vec")  //-> 1775 ms!
        EbookReader_vec rdr;
        for(int i = 0; i < n; ++i) {
            int user_no = rand_distr(g);
            int cmd = user_no % 2;
            
            if(cmd == 0) {
                rdr.Read(user_no, user_no % 1001);
            } else {
                rdr.Cheer(user_no);
            }
        }
    }
    
    {
        LOG_DURATION("Using 2 vec") //-> 482 ms !!
        EbookReader_2vec rdr;
        for(int i = 0; i < n; ++i) {
            int user_no = rand_distr(g);
            if (user_no > 10e5) {
                cerr << "user num is: " << user_no << endl;
            }
            int cmd = user_no % 2;
            
            if(cmd == 0) {
                rdr.Read(user_no, user_no % 1001);
            } else {
                rdr.Cheer(user_no);
            }
        }
    }
*/
