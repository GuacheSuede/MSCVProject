#ifndef includes_hub
#define includes_hub

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <r3/r3.hpp>
#include <nlohmann/json.hpp>
#include <sstream>
#include <regex>
#include <boost/regex.hpp>

using namespace std;

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using http_response =  http::response<http::string_body>;
using http_response_none =  http::response<http::empty_body>;

using json = nlohmann::json;

// Report a failure
void fail(beast::error_code ec, char const* what){
    std::cerr << what << ": " << ec.message() << "\n";
}

// Return a reasonable mime type based on the extension of a file.
beast::string_view
mime_type(beast::string_view path)
{
    using beast::iequals;
    auto const ext = [&path]
    {
        auto const pos = path.rfind(".");
        if(pos == beast::string_view::npos)
            return beast::string_view{};
        return path.substr(pos);
    }();
    if(iequals(ext, ".htm"))  return "text/html";
    if(iequals(ext, ".html")) return "text/html";
    if(iequals(ext, ".php"))  return "text/html";
    if(iequals(ext, ".css"))  return "text/css";
    if(iequals(ext, ".txt"))  return "text/plain";
    if(iequals(ext, ".js"))   return "application/javascript";
    if(iequals(ext, ".json")) return "application/json";
    if(iequals(ext, ".xml"))  return "application/xml";
    if(iequals(ext, ".swf"))  return "application/x-shockwave-flash";
    if(iequals(ext, ".flv"))  return "video/x-flv";
    if(iequals(ext, ".png"))  return "image/png";
    if(iequals(ext, ".jpe"))  return "image/jpeg";
    if(iequals(ext, ".jpeg")) return "image/jpeg";
    if(iequals(ext, ".jpg"))  return "image/jpeg";
    if(iequals(ext, ".gif"))  return "image/gif";
    if(iequals(ext, ".bmp"))  return "image/bmp";
    if(iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
    if(iequals(ext, ".tiff")) return "image/tiff";
    if(iequals(ext, ".tif"))  return "image/tiff";
    if(iequals(ext, ".svg"))  return "image/svg+xml";
    if(iequals(ext, ".svgz")) return "image/svg+xml";
    return "application/text";
}



string init_code = " ";
string code = "";

string gen_syntax(string code_line);
string init_code_gen();
string loop_gen();
string loop_code = "";

vector<string> tickers;
vector<string> algos;

void load_script(string input_code){
    tickers.clear();

    init_code += "window.tickers = []; \n";
    init_code += "window.price_page = 0; \n";
    init_code += "window.portfolio = 10000; \n";
    init_code += "window.buys = [] \n"; // for now buy and sell  a stock only limited
    init_code += "window.sells = []; \n"; //push thoguth so u see the good


    istringstream f(input_code);
    string code_script_buf;
    while (getline(f, code_script_buf)){
        code += gen_syntax(code_script_buf) + "\n";
    }

    code.insert(0, loop_gen());
    code.insert(0, init_code_gen());
    code.insert(0, init_code);

}


string init_code_gen(){
    string i_code = "function price(ticker, container){  \n fetch( window.murl + ticker + window.url_end + \"&page=\"+window.price_page, { \n method: \'get\', \n headers:{ \n \'Authorization\': \'Basic '+btoa(\'admin:changeit\'), \n \'Content-Type\': \'application/json\' \n }\n}).then(response => {\nreturn response.json(); \n }).then(data => {";
        i_code += "container.push(data\[\"_embedded\"\]\[0\]\[\"price\"\]);\n";
    i_code += "}); \n } \n";

    i_code +=  "function fetch_funda(){ \n fetch(window.murl  + ";

    for (auto &t: tickers) { // this happens after all tickers have been examined
        i_code += "var " + t + "_line = new window.TimeSeries;\n";
    }
    i_code += " setInterval(function() { \n";
    for(auto &tt: tickers){

        i_code += tt + "_line.append(new Date().getTime(), window."+ tt +"_values.slice(-1).pop()); \n";
    }
    i_code += "}, 1000); \n";
    for(auto &ttt: tickers) {
        i_code += "window.smoothie.addTimeSeries(" + ttt + "_line,  { strokeStyle: window.random_rgba(), lineWidth: 8 }); \n";
    }
    i_code += "window.smoothie.streamTo(document.getElementById(\"graphviewid\"), 1001);";
    return i_code;
}


string loop_gen(){
    string ret;
    ret += "function loop(){ ++window.price_page;\n";
    for(auto &ttt: tickers) {
        ret += "price(\"" + ttt + "\"," + "window." + ttt + "_values);";
    }
    ret += "\n } \n setInterval(loop, 1000); \n";
    return ret;
}

string get_string(char s){

}

/* 1. Normal Loop Code - normal code
 * 2. Init Loop Code - start code that assigns next value
 * 3. Init code - global start code like websocekts
 */
string gen_syntax(string code_line){
//    /* Comment Declaration
//     * # This will do this thing
//     */
//    if (code_line.find("#") != string::npos){
//        // Do nothing
//    }
//
//    /* Variable Declaration
//     * variable(graft,1)
//     */
//    if (code_line.find("variable") != string::npos){
//        boost::replace_all(code_line, "variable(", "window.");
//        boost::replace_all(code_line, ",", "=");
//        boost::replace_all(code_line, ")", ";");
//    }
//
//    /* Array Init Declaration
//     * array(graft,1)
//     */
//    if (code_line.find("array(") != string::npos){
//        boost::replace_all(code_line, "array(", "window.");
//        boost::replace_first(code_line, ",", "=[\"");
//        boost::replace_all(code_line, ",", "\",\"");
//        boost::replace_all(code_line, ")", "\"];");
//    }
//
//    /* Array Push Declaration
//     * array_push(graft,1)
//     */
//    if (code_line.find("array_push") != string::npos){
//        boost::replace_all(code_line, "array_push(", "");
//        boost::replace_all(code_line, ",", ".push(\"");
//        boost::replace_all(code_line, ")", "\")");
//        code_line.insert(0, "window.");
//    }
//
//    /* Array Pop Declaration
//     * array_pop(graft)
//     */
//    if (code_line.find("array_pop") != string::npos){
//        boost::replace_all(code_line, "array_pop(", "");
//        boost::replace_all(code_line, ")", ".pop();");
//        code_line.insert(0, "window.");
//    }
//
//
//    /* Array Has Declaration
//     * array_has(graft, "1")
//     */
//    if (code_line.find("array_has") != string::npos){
//        boost::replace_all(code_line, "array_has(", "");
//        boost::replace_first(code_line, ",", ".includes(\"");
//        boost::replace_all(code_line, ")", "\")");
//        code_line += ";";
//        code_line.insert(0, "window.");
//    }
//
//    // more than function
//    if(code_line.find("more_than(") != string::npos) {
//        int occurrences;
//        int pos;
//
//        string target = "more_than(";
//        while ((pos = code_line.find(target, pos )) != std::string::npos) {
//            ++occurrences;
//            pos += target.length();
//        }
//
//        cout << occurrences  << endl;
//        for(int oc = 0; oc < occurrences; ++oc) {
//
//            // always find and start with first, can we ddo multiple instructions ?
//            bool index_ended = false;
//            int line_end = code_line.length() - 1;
//            int index = code_line.find("more_than(");
//            int index_end = index + 9;
//
//            string parameter_s;
//            while (index_ended == false) {
//                if (++index_end <= line_end && code_line.at(index_end) != ')') {
//                    parameter_s += code_line.at(index_end);
//                } else {
//                    index_ended = true;
//                }
//            }
//
//            cout << parameter_s << endl;
//            vector <string> params;
//            boost::split(params, parameter_s, boost::is_any_of(","));
//            // only 2 paramaters
//            boost::replace_first(code_line, "more_than(" + params[0] + "," + params[1] + ")", "");
//            code_line.insert(index, "(" + params[0] + " > " + params[1] + ")");
//        }
//    }
//
//    // more than or equals function
//    if(code_line.find("more_than_or_equals(") != string::npos) {
//        int index = code_line.find("more_than_or_equals(");
//
//        string parameter_s = code_line;
//        vector<string> params;
//        boost::replace_first(parameter_s, "more_than_or_equals(", "");
//        boost::replace_first(parameter_s, ")", "");
//        boost::split(params, parameter_s, boost::is_any_of(","));
//        code_line.insert(index, "(" + params[0] + " >= " + params[1] + ")");
//    }
//
//    // less than function
//    if(code_line.find("less_than(") != string::npos) {
//        int index = code_line.find("less_than(");
//        string parameter_s = code_line;
//        vector<string> params;
//        boost::replace_first(parameter_s, "less_than(", "");
//        boost::replace_first(parameter_s, ")", "");
//        boost::split(params, parameter_s, boost::is_any_of(","));
//        code_line.insert(index, "(" + params[0] + " < " + params[1] + ")");
//    }
//
//
//    // less than or equals function
//    if(code_line.find("less_than_or_equals(") != string::npos) {
//        int index = code_line.find("less_than_or_equals(");
//        string parameter_s = code_line;
//        vector<string> params;
//        boost::replace_first(parameter_s, "less_than_or_equals(", "");
//        boost::replace_first(parameter_s, ")", "");
//        boost::split(params, parameter_s, boost::is_any_of(","));
//        code_line.insert(index, "(" + params[0] + " <= " + params[1] + ")");
//    }
//
//    // equals function
//    if(code_line.find("equals(") != string::npos) {
//        int index = code_line.find("equals(");
//        string parameter_s = code_line;
//        vector<string> params;
//        boost::replace_first(parameter_s, "equals(", "");
//        boost::replace_first(parameter_s, ")", "");
//        boost::split(params, parameter_s, boost::is_any_of(","));
//        code_line.insert(index, "(" + params[0] + " == " + params[1] + ")");
//    }
//
//    // not equals function
//    if(code_line.find("not_equals(") != string::npos) {
//        int index = code_line.find("not_equals(");
//        string parameter_s = code_line;
//        vector<string> params;
//        boost::replace_first(parameter_s, "equals(", "");
//        boost::replace_first(parameter_s, ")", "");
//        boost::split(params, parameter_s, boost::is_any_of(","));
//        code_line.insert(index, "(" + params[0] + " != " + params[1] + ")");
//
//    }
//// its all abt how u feel
//
//    // or function
//    if(code_line.find("or(") != string::npos) {
//        int index = code_line.find("or(");
//
//        string parameter_s = code_line;
//        vector<string> params;
//        boost::replace_first(parameter_s, "or(", "");
//        boost::replace_first(parameter_s, ")", "");
//        boost::split(params, parameter_s, boost::is_any_of(","));
//        code_line.insert(index, "(" + params[0] + " || " + params[1] + ")");
//    }
//
//    // and function
//    if(code_line.find("and(") != string::npos) {
//        int index = code_line.find("and(")+3;
//        int index_end = 0;
//        string parameter_s = code_line;
//        vector<string> params;
//        boost::replace_first(parameter_s, "and(", "");
//        boost::replace_first(parameter_s, ")", "");
//        boost::split(params, parameter_s, boost::is_any_of(","));
//        code_line.insert(index, "(" + params[0] + " && " + params[1] + ")");
//    }
//
//    if(code_line.find("buy_") != string::npos){
//        string alphabets = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//        string stock_name;
//        bool stock_name_end = false;
//        int stock_name_start = code_line.find("buy_")+3;
//        int stock_name_had_start = code_line.find("buy_");
//
//        int stcok_name_will_end = code_line.length();
//        while (stock_name_end == false) {
//            if (++stock_name_start >= stcok_name_will_end) {
//                stock_name_end = true;
//            } else {
//                if (alphabets.find(code_line.at(stock_name_start)) != string::npos) {
//                    stock_name += code_line.at(stock_name_start);
//                }else{
//                    stock_name_end = true;
//                }
//            }
//        }
//        boost::replace_first(code_line, "buy_"+stock_name, "buys.push(window.\"+stock_name+\"_values.slice(-1).pop());");
//    }
//
//    if(code_line.find("sell_") != string::npos){
//        string alphabets = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//        string stock_name;
//        bool stock_name_end = false;
//        int stock_name_start = code_line.find("sell_")+3;
//        int stock_name_had_start = code_line.find("sell_");
//
//        int stcok_name_will_end = code_line.length();
//        while (stock_name_end == false) {
//            if (++stock_name_start >= stcok_name_will_end) {
//                stock_name_end = true;
//            } else {
//                if (alphabets.find(code_line.at(stock_name_start)) != string::npos) {
//                    stock_name += code_line.at(stock_name_start);
//                }else{
//                    stock_name_end = true;
//                }
//            }
//        }
//        boost::replace_first(code_line, "sell_"+stock_name, "sells.push(window."+stock_name+"_values.slice(-1).pop());");
//    }
//
//
//
//
//    /* Algo Init Declaration
//     * use algo_name eg. use sma
//     */
//    if (code_line.find("use") != string::npos){
//        boost::replace_first(code_line, "use", "");
//        boost::replace_all(code_line, " ", "");
//        string algo_name = code_line;
//        code_line = "";
//        code_line += "window." + algo_name+ "_socket = new WebSocket(" +algo_name+ "_socket_url); \n";
//        code_line += "window." + algo_name + "_socket.onmessage = function (event) { \n \t" +"window."+algo_name+ "_values.push(event.data)); \n} \n";
//        // TODO: Add aupdate sectoin here andsperate into init and init loop code
//    }
//


// lower case == algo, uppercase == ticker, jsut for the distinction thoguth no real benefit
    boost::regex r_algo_current{"\\b(?:current_)([a-z]*)\\b"};
    boost::smatch r_algo_current_m;

    while(boost::regex_search(code_line, r_algo_current_m, r_algo_current)){
        boost::replace_first(code_line, r_algo_current_m[0].str(), "window." + r_algo_current_m[1].str() + "_values.slice(-1).pop()");
        string algoname = r_algo_current_m[1].str();

        boost::smatch empty_match;
        r_algo_current_m = empty_match;
        if(std::find(algos.begin(), algos.end(), algoname) != algos.end()) {
            algos.push_back(algoname);

            init_code += "window." + algoname + "_values = []; \n";

        }
    }

    boost::regex r_algo_ago{"\\b(\\d*)(?:_ago_)([a-z]*)\\b"};
    boost::smatch r_algo_ago_m;

    while(boost::regex_search(code_line, r_algo_ago_m, r_algo_ago)){
        boost::replace_first(code_line, r_algo_ago_m[0].str(), "window."+r_algo_ago_m[2].str()+"_values[window."+r_algo_ago_m[2].str()+"_values.length-"+r_algo_ago_m[1].str()+"])");

        string algoname = r_algo_ago_m[1].str();

        boost::smatch empty_match;
        r_algo_ago_m = empty_match;

        if(std::find(algos.begin(), algos.end(), algoname) != algos.end()) {
            algos.push_back(algoname);

            init_code += "window." + algoname + "_values = []; \n";

        }
    }

    boost::regex r_price{"\\b(\\w{1,5})(?:_price)\\b"};
    boost::smatch r_price_m;

    while(boost::regex_search(code_line, r_price_m, r_price)){
        boost::replace_first(code_line, r_price_m[0].str(), "window." + r_price_m[1].str() + "_values.slice(-1).pop()");
        string ticker = r_price_m[1].str();

        boost::smatch empty_match;
        r_price_m = empty_match;

        if(std::find(tickers.begin(), tickers.end(), ticker) != tickers.end()) {
            tickers.push_back(ticker);

            init_code += "window." + ticker + "_values = [0]; \n";
            init_code += "window.tickers.push(\"" + ticker + "\"); \n";
        }
    }

    boost::regex r_ago_price ("\\b(\\d*)(?:_ago_)(\\w{1,5})(?:_price)\\b");
    boost::smatch r_ago_price_m;
    while(boost::regex_search(code_line, r_ago_price_m, r_ago_price)){
        boost::replace_first(code_line, r_ago_price_m[0].str(), "window."+r_ago_price_m[2].str()+"_values[window."+r_ago_price_m[2].str()+"_values.length-"+r_ago_price_m[1].str()+"]);");
        string ticker = r_ago_price_m[2].str();

        boost::smatch empty_match;
        r_ago_price_m = empty_match;

        if(std::find(tickers.begin(), tickers.end(), ticker) != tickers.end()) {
            tickers.push_back(ticker);

            init_code += "window." + ticker + "_values = [0]; \n";
            init_code += "window.tickers.push(\"" + ticker + "\"); \n";
        }
    }

    boost::regex r_if ("(?:if)[ ](.*)[ ](?:{)");
    boost::smatch r_if_m;
    while(boost::regex_search(code_line, r_if_m, r_if)){
        boost::replace_first(code_line, r_if_m[0].str(), "if("+r_if_m[1].str()+"){");
        boost::smatch empty_match;
        r_ago_price_m = empty_match;
    }

    boost::regex r_else_if ("(?:}else if)[ ](.*)[ ](?:{)");
    boost::smatch r_else_if_m;
    while(boost::regex_search(code_line, r_else_if_m, r_else_if)){
        boost::replace_first(code_line, r_else_if_m[0].str(), "}else if("+r_else_if_m[1].str()+"){");
        boost::smatch empty_match;
        r_ago_price_m = empty_match;
    }

    boost::regex r_while ("(?:while)[ ](.*)[ ](?:{)");
    boost::smatch r_while_m;
    while(boost::regex_search(code_line, r_while_m, r_while)){
        boost::replace_first(code_line, r_while_m[0].str(), "while("+r_while_m[1].str()+"){");
        boost::smatch empty_match;
        r_ago_price_m = empty_match;
    }

    boost::replace_all(code_line, "more_than", ">");
    boost::replace_all(code_line, "more_than_or_equals", ">=");
    boost::replace_all(code_line, "less_than", "<");
    boost::replace_all(code_line, "less_than_or_equals", "<=");
    boost::replace_all(code_line, "equals", "==");
    boost::replace_all(code_line, "not_equals", "!=");
    boost::replace_all(code_line, "and", "&&");
    boost::replace_all(code_line, "or", "||");



    boost::regex r_var{"\\b(?:var_)(\\w*)(?:_)(\\w*)\\b"};
    boost::smatch r_var_m;
    while(boost::regex_search(code_line, r_var_m, r_var)) {
        boost::replace_first(code_line, r_var_m[0].str(), "var " + r_var_m[1].str() + " = " + r_var_m[2].str() + ";");
        boost::smatch empty_match;
        r_var_m = empty_match;
    }

    //buy, sell, buy sell ui, algo finish, total, fundamentals, legend ?

    boost::regex r_buy{"\\b(?:buy_)(\\w*)\\b"};
    boost::smatch r_buy_m;

    while(boost::regex_search(code_line, r_buy_m, r_buy)) {
        boost::replace_first(code_line, r_buy_m[0].str(), "window.buys.push({"+r_buy_m[1].str() + ",window." + r_buy_m[1].str() + "_values.slice(-1).pop()})");
        boost::smatch empty_match;
        r_buy_m = empty_match;
    }


    boost::regex r_sell{"\\b(?:sell_)(\\w*)\\b"};
    boost::smatch r_sell_m;

    while(boost::regex_search(code_line, r_sell_m, r_sell)) {
        boost::replace_first(code_line, r_sell_m[0].str(), "window.sells.push({"+r_sell_m[1].str() + ",window." + r_sell_m[1].str() + "_values.slice(-1).pop()})");
        boost::smatch empty_match;
        r_sell_m = empty_match;
    }


    boost::regex r_fundamentals{"\\b(?:fundamentals_)([A-Z]*)(?:_)([a-z]*)(?:_)([A-Z0-9]*)\\b"};
    boost::smatch r_fundamentals_m;

    while(boost::regex_search(code_line, r_buy_m, r_buy)) {
        boost::replace_first(code_line, r_fundamentals[0].str(), r_fundamentals[1].str() + "_" + r_fundamentals[2].str() + "_" + r_fundamentals[3].str());

        string funda =  r_fundamentals[1].str() + "_" + r_fundamentals[2].str() + "_" + r_fundamentals[3].str();

        boost::smatch empty_match;
        r_price_m = empty_match;

        if(std::find(fundamentals.begin(), fundamentals.end(), funda) != fundamentals.end()) {
            fundamentals.push_back(funda);

            init_code += "window." + funda + "; \n";
        }
    }

    return code_line;

}

// This function produces an HTTP response for the given
// request. The type of the response object depends on the
// contents of the request, so the interface requires the
// caller to pass a generic lambda for receiving the response.`
template< class Body, class Allocator, class Send>
void handle_request(beast::string_view doc_root, http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send){
    if (req.method() == http::verb::options && req.target() == "/") {

        http_response_none res{http::status::ok, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::access_control_allow_origin, "*");
        res.set(http::field::access_control_allow_headers, "*");
        res.set(http::field::access_control_allow_methods, "*");
        res.set(http::field::allow, "OPTIONS, GET, HEAD, POST");
        res.prepare_payload();
        return send(std::move(res));
    }

    if (req.method() == http::verb::post && req.target() == "/"){
        http_response res{http::status::ok, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.set(http::field::access_control_allow_origin, "*");
        load_script(req.body());
        json req_code;
        req_code["code"] = code;
        res.body() = req_code.dump();
        code = "";
        init_code = "";

        res.prepare_payload();
        return send(std::move(res));
    }
}


#endif
