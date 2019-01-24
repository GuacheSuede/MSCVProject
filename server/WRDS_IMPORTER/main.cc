#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <ctime>
#include <unistd.h>

#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include<boost/tokenizer.hpp>

#include <nlohmann/json.hpp>
#include <cpr/cpr.h>


using json = nlohmann::json;
using namespace std;
using boost::tokenizer;
using boost::escaped_list_separator;

string ticker = "";
string prev_ticker = "";

vector<vector<string>> ticker_rows;
vector<string> transient_row;
int ticker_index = 9;

int tracker = 0;

vector<string> titles;
bool titled = false;

void commit_document(json data){
    std::string restheart_url = "http://localhost:8080/db/fundamentals";
    auto r = cpr::Post(
            cpr::Url{restheart_url},
            cpr::VerifySsl(false),
            cpr::Authentication{"admin", "changeit"},
            cpr::Body{data.dump()},
            cpr::Header{{"Content-Type", "application/json"}}
    );

}

void sort_and_commit_rows(string wrds_buffer){
    ++tracker;
    // FORMAT ROW
    typedef tokenizer<escaped_list_separator<char> > so_tokenizer;

    so_tokenizer tok(wrds_buffer, escaped_list_separator<char>('\\', ',', '\"'));

    for(so_tokenizer::iterator beg=tok.begin(); beg!=tok.end(); ++beg){
        transient_row.push_back(*beg);
    }

    ticker = transient_row[9];
    // actio keidl and lme signappreo saf army

    /// peo w lnto suaoppsoe pap and army osidotic,dicatatiorship wihto reaosn,sutpidity system
    //woamn hve otperonsalsie
    // sjut lie kgycncocentrism in lfie ,teh pcats arefavoured fvedr the dogs for looks and affection apeal,sfiotnes

    //wshitellbeor ana leakers uspally do for good of people
//    /stupdi woman mean leak andcheat on toehguys steial fro jgirfiend
//.//// lol hahaha, aloycisou pang kield by amry ,.. acitidnetlal padn
// //erely on ahdnsoem guys lol. ijmerelyl chai obnixious
//
//
// /hj ian wr
// // jiulia rich eocsoyte mlriabreia and support
//jlaii shoud lstare to be used by hedge funds
//gsg no surival ready
//dare to say and stop
//'m doing some pretty simple machine learning with Flux.jl. The thing I like most about doing ML in Julia is not so much the ML library itself but the fact that working in a fast language changes everything. Most of the work in ML (at least in my experience) is in collecting data, preparing data, analyzing data, etc., and having a language where simple constructs like loops and functions are fast means that all of my data processing can be fast and simple. I don't have to jump through Google's protobuf hoops just to feed my data into TensorFlow. Instead, I just have a Julia Vector of my custom Sample type and iterate through it in a loop, which gives me performance as good as C with very little code. My actual training loop is literally just

// no need work so har nejoy ?
// //dotn dvie to ofasrt
// nwo mroe pseignaproe nwilspeka out agisnt he en nd for it,diotici signapoe takek a alrg accient before the ycan doanythjing... senslels luselsl army pwihjto u eaosn nro logica ndmenaing
//
//onyl popaulr peop iwl lthe ysl;eak and tlak about
//wehn you do it smothly tyou dotn ahte it, change msient ,elarnt olvowe the prose and sytle of it

//Julia Language Co-Creators Win James H. Wilkinson Prize for Numerical Software (siam.org) you can win rpiszes fo goodo software
//    cout << prev_ticker << "=====" << ticker << endl;
// chiense afraid admit faults
    // IF END, CLEAN and COMMIT
    if (ticker != "" && ticker != prev_ticker && ticker_rows.size() != 0){
        transient_row.clear();

        //send to db and the ncontinuel
        json data;
        for (int ticker_index = 0; ticker_index < ticker_rows.size(); ++ticker_index) {
            vector<string> ticker_row = ticker_rows[ticker_index];
            json ticker_row_json;
            for (int ticker_index_row = 0; ticker_index_row < ticker_row.size(); ++ticker_index_row) {
                ticker_row_json[titles[ticker_index_row]] = ticker_row[ticker_index_row];
            }
            data[ticker].push_back(ticker_row_json);
        }
        commit_document(data);
        cout << time(0) << endl;
        //        cout << data.dump() << endl;
        //learn to smoke peope ,whait onyl nenes to look as usch..
//        cout << "===========================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================" << endl;

        ticker_rows.clear();
        ticker = "";
// ahve cofndience 9in ur discoveries
// womane only appeal via sex
//the yvisually ugly whe nsex
//dare to ahv eocnfidnece
        /*
         * TSLA: [
         *  {
         *   k:v
         *  }
         *  {
         *  }
         *  {
         *  }
         * ]
         *
         *
         *
         *
         */
    // ELSE CONTINUE
    }else{
        ticker_rows.push_back(transient_row);
        transient_row.clear();
        prev_ticker = ticker;

    }


    //jsoeph lvoe change htigns when peope arebusy..dotn stuppify self by follwogn toehrs
    //narcarcism will brign u dow ncos uthink uare abvoe othe and ahrdhsip is fine.. or perhpa laziness
//    the smart man edits hsi coe to serve hismelf nto try to coempsate for it or be lazy
//fast grpase fo scipritn fjs fast .. can see as c++ fst to dotn hin sk omuch
//do tnhink os much no nend ovoe htink
// dont be narcsitist, try thing
//bash tosl as apis hmm
}

int main(){



    ifstream wrds_file;
    wrds_file.open("iex_na_all_variables_fundamentals_quarterly.csv");
    if (wrds_file.is_open()){
        string wrds_buffer;
        while (getline(wrds_file, wrds_buffer)){
            if(titled == true){
                sort_and_commit_rows(wrds_buffer);
            }else{
                boost::split(titles, wrds_buffer, boost::is_any_of(","));
                titled = true;
            }
        }
        wrds_file.close();
    }
}

