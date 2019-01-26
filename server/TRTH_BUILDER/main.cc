#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <libdill.h>

#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

using json = nlohmann::json;
using namespace std;

string auth_token;
string ric_list;
vector<string> global_ric_list;
vector<string> search_strings;
string search_string;
int tracker = 0;

//void (json data){
//    std::string restheart_url = "http://localhost:8080/db/fundamentals";
//    auto r = cpr::Post(
//            cpr::Url{restheart_url},
//            cpr::VerifySsl(false),
//            cpr::Authentication{"admin", "changeit"},
//            cpr::Body{data.dump()},
//            cpr::Header{{"Content-Type", "application/json"}}
//    );
//
//}

string auth(){
    string auth_url = "https://hosted.datascopeapi.reuters.com/RestApi/v1/Authentication/RequestToken";

    // Building Auth Data
    json auth_data;
    json auth_data_2;
    auth_data_2["Username"] = "9017045";
    auth_data_2["Password"] = "gcG0dAVQ";
    auth_data["Credentials"] = auth_data_2;

    // Send Auth Request
    auto r = cpr::Post(
            cpr::Url{auth_url},
            cpr::VerifySsl(false),
            cpr::Body{auth_data.dump()},
            cpr::Header{
                {"Content-Type", "application/json"},
                {"Prefer", "respond-async"}
            }
    );

    // Get value and Return
    json auth_response = json::parse(r.text);
    return auth_response["value"];
}

coroutine void instrument_search(string ticker){
    string instrument_search_url = "https://hosted.datascopeapi.reuters.com/RestApi/v1/Search/InstrumentSearch";

    // Building Search Request Data
    json search_data  =  {
            {"InstrumentTypeGroups", {"Equities", "MortgageBackedSecurities", "Funds" }},
            {"IdentifierType", "Ticker"},
            {"Identifier", ticker},
            {"PreferredIdentifierType", "Ric"},
        };

    json search_req;
    search_req["SearchRequest"] = search_data;




    // Send Search Request
    auto r = cpr::Post(
            cpr::Url{instrument_search_url},
            cpr::VerifySsl(false),
            cpr::Body{search_req.dump()},
            cpr::Header{
                    {"Content-Type", "application/json"},
                    {"Prefer", "respond-async"},
                    {"Authorization", "Token "+ auth_token}
            }
    );
    // Get value and Return list of RICs (current use case)
    json search_response = json::parse(r.text);
    json value_list = search_response["value"];
    cout << value_list << endl;
    string ric_list;
    for (auto &v : value_list){
        ric_list += "RIC,";
        ric_list += v["Identifier"];
        ric_list += "\n";
    }
//    cout << ric_list << endl;
    global_ric_list.push_back(ric_list);
}


void perform_search(){
    int perform_tracker = 0;
    vector<string> local_search_strings;
    vector<thread> local_search_strings_threads;

    for (auto &ss: search_strings) {
        cout << ss << endl;
        cout << "==============" << endl;
        ++perform_tracker;
//        local_search_strings.push_back(ss);
//        if (perform_tracker == 100){
//            int bb = bundle();
//            for (int pt = 0; pt < 999; ++pt){
        go(instrument_search(ss));
//                bundle_go(bb, instrument_search(local_search_strings[pt]));
//            }
//            bundle_wait(bb, -1);
//            thread t1(instrument_search, local_search_strings[0]);
//            thread t2(instrument_search, local_search_strings[1]);
//            thread t3(instrument_search, local_search_strings[2]);
//            thread t4(instrument_search, local_search_strings[3]);
//            thread t5(instrument_search, local_search_strings[4]);
//            thread t6(instrument_search, local_search_strings[5]);
//            thread t7(instrument_search, local_search_strings[6]);
//            thread t8(instrument_search, local_search_strings[7]);
//            thread t9(instrument_search, local_search_strings[8]);
//            thread t10(instrument_search, local_search_strings[9]);
//
//            t1.join();
//            t2.join();
//            t3.join();
//            t4.join();
//            t5.join();
//            t6.join();
//            t7.join();
//            t8.join();
//            t9.join();
//            t10.join();
//




//            perform_tracker = 0;
//            local_search_strings.clear();
//            local_search_strings_threads.clear();
//        }
    }

}
int main(){

    auth_token = auth();

    //  Open IEX File
    ifstream iex_file;
    iex_file.open("IEXSymbolsList.txt");
    if (iex_file.is_open()){
        string iex_buffer;

        while (getline(iex_file, iex_buffer)){
            ++tracker;
            search_string += iex_buffer;
            search_string += ",";
            if (tracker == 10){
                tracker = 0;
                search_strings.push_back(search_string);
                search_string = "";
            }
        }
    iex_file.close();
    }
    perform_search();

    ofstream iex_trth("IEXTRTHSymbolsList.txt");
    for (auto& grl: global_ric_list){
        iex_trth << grl;
    }
    iex_trth.close();

}

