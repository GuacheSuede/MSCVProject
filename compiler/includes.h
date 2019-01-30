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
#include <cpr/cpr.h>

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



ifstream code_script;
string init_code;
string code;

string gen_syntax(string code_line);
string init_code_gen();

void load_script(string name){
    code_script.open(name);
    if (code_script.is_open()){
        string code_script_buf;
        while (getline(code_script, code_script_buf)){
            code += gen_syntax(code_script_buf) + "\n";
        }
        code_script.close();
    }

    code.insert(0, init_code_gen());
    code.insert(0, init_code);
}


string init_code_gen(){
    string i_code = "function price(ticker){ \n var r_obj = {}; \n fetch( window.murl + ticker + window.url_end + \"&page=\"+price_page, { \n method: \'get\', \n headers:{ \n \'Authorization\': \'Basic '+btoa(\'admin:changeit\'), \n \'Content-Type\': \'application/json\' \n }\n}).then(response => {\nreturn response.json(); \n }).then(data => obj\[ticker\] = data\[\"_embedded\"\]\[0\]\[\"price\"\]) \n return r_obj; \n";
    return i_code;
}

/* 1. Normal Loop Code - normal code
 * 2. Init Loop Code - start code that assigns next value
 * 3. Init code - global start code like websocekts
 */
string gen_syntax(string code_line){
    /* Comment Declaration
     * # This will do this thing
     */
    if (code_line.find("#") != string::npos){
        // Do nothing
    }

    /* Variable Declaration
     * variable(graft,1)
     */
    if (code_line.find("variable") != string::npos){
        boost::replace_all(code_line, "variable(", "window.");
        boost::replace_all(code_line, ",", "=");
        boost::replace_all(code_line, ")", ";");
    }

    /* Array Init Declaration
     * array(graft,1)
     */
    if (code_line.find("array(") != string::npos){
        boost::replace_all(code_line, "array(", "window.");
        boost::replace_first(code_line, ",", "=[\"");
        boost::replace_all(code_line, ",", "\",\"");
        boost::replace_all(code_line, ")", "\"];");
    }

    /* Array Push Declaration
     * array_push(graft,1)
     */
    if (code_line.find("array_push") != string::npos){
        boost::replace_all(code_line, "array_push(", "");
        boost::replace_all(code_line, ",", ".push(\"");
        boost::replace_all(code_line, ")", "\")");
        code_line.insert(0, "window.");
    }

    /* Array Pop Declaration
     * array_pop(graft)
     */
    if (code_line.find("array_pop") != string::npos){
        boost::replace_all(code_line, "array_pop(", "");
        boost::replace_all(code_line, ")", ".pop();");
        code_line.insert(0, "window.");
    }


    /* Array Has Declaration
     * array_has(graft, "1")
     */
    if (code_line.find("array_has") != string::npos){
        boost::replace_all(code_line, "array_has(", "");
        boost::replace_first(code_line, ",", ".includes(\"");
        boost::replace_all(code_line, ")", "\")");
        code_line += ";";
        code_line.insert(0, "window.");
    }

    /* Algo Init Declaration
     * use algo_name eg. use sma
     */
    if (code_line.find("use") != string::npos){
        boost::replace_first(code_line, "use", "");
        boost::replace_all(code_line, " ", "");
        string algo_name = code_line;
        code_line = "";
        code_line += "window." + algo_name+ "_socket = new WebSocket(" +algo_name+ "_socket_url); \n";
        code_line += "window." + algo_name + "_socket.onmessage = function (event) { \n \t" +"window."+algo_name+ "_values.push(event.data)); \n} \n";
        // TODO: Add aupdate sectoin here andsperate into init and init loop code
    }


    /* Algo Reference Declaration, Current Variant
     * current_algo_name. eg. current_macd
     * Unable to use tokenisation due to space not consistent
     */
    if (code_line.find("current_") != string::npos){
        string alphabets = "abcdefghijklmnopqrstuvwxyz";
        int algo_name_length = 0;
        bool algo_name_end = false;
        string algo_name;
        int algo_name_start = code_line.find("current_")+7;

        while (algo_name_end == false){
            ++algo_name_start;
            if(alphabets.find(code_line.at(algo_name_start)) != string::npos){
                algo_name += code_line.at(algo_name_start);
            }else{
                algo_name_end = true;
            }
        }

        init_code += "window."+ algo_name + "_values = []; \n";

        boost::replace_first(code_line, "current_", "");
        // due to the nature of algorthims having variable amount of fields, wedo not specify what current_algo_name returns but rather it retursn an obbject if it's multiple values and an integer if it's a single value. data agnostic parser and tokeniser.
        boost::replace_first(code_line, algo_name, "window."+algo_name+"_values.slice(-1).pop()");

    }

    /* Algo Reference Declaration, Ago Variant
     * days_ago_algo_name. eg. 2_ago_algo_name
     * Unable to use tokenisation due to space not consistent
     */
    if (code_line.find("_ago_") != string::npos){
        string numbers = "0123456789";
        string alphabets = "abcdefghijklmnopqrstuvwxyz";
        int algo_name_length = 0;
        bool algo_name_end = false;
        string algo_name;
        int algo_name_start = code_line.find("_ago_")+4;

        while (algo_name_end == false){
            ++algo_name_start;
            if(alphabets.find(code_line.at(algo_name_start)) != string::npos){
                algo_name += code_line.at(algo_name_start);
            }else{
                algo_name_end = true;
            }
        }


        boost::replace_first(code_line, "_ago_", "");

        int days_end = code_line.find(algo_name);
        int days_ended = false;
        int actual_days = 0;
        string days = "";

        while(days_ended == false){
            --days_end;
            if(numbers.find(code_line.at(days_end)) != string::npos){
                days += code_line.at(days_end);
            }else{
                days_ended = true;
                boost::replace_first(code_line, days, "");
            }
        }

        boost::replace_first(code_line, algo_name, "window."+algo_name+"_values["+algo_name+"_values.length-"+days+"]");
    }

    // TODO: version with at vlaue
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
        // already string due to string body in sesion.h
            new_watchlist(req.body());

            http_response res{http::status::ok, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "application/json");
            res.set(http::field::access_control_allow_origin, "*");
            load_script("script.q");
            res.body() = code;

            res.prepare_payload();
            return send(std::move(res));
        }
    }
}

#endif
