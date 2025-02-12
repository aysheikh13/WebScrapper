#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <algorithm>
#include <unordered_set>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ssl.hpp>

std::string convertURL(const std::string& INITIAL_URL, const std::string& URL_LINK);

int main()
{
    try{
        
        // Variables regarding the HTTPS, host/port, HTML, and links 
        boost::asio::io_context input_output;
        std::string hostName = "www.cnn.com";
        std::string portNumber = "443";
        std::string htmlContent;
        std::regex linkKey(R"(<a href=['"](http[^'"]+)['"])");
        std::smatch matchingLinks;
        std::unordered_set<std::string> links;
        boost::beast::flat_buffer responseBuffer;
        boost::beast::http::response<boost::beast::http::dynamic_body> response;

        // Here we begin setting up the SSL (secure sockets layer that encrypts the data along with providing authentification, resolver (convert domain names into IP Addresses), and socket (used to recieve/send data)
        boost::asio::ssl::context Context(boost::asio::ssl::context::tlsv12_client);
        Context.set_default_verify_paths(); 
        boost::asio::ip::tcp::resolver resolver(input_output);
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket(input_output, Context);
        
        // Instantiate the resolve results and perform an SSL handshake in which the client and the server are securely connected
        auto const resolverResults = resolver.resolve(hostName, portNumber);
        boost::asio::connect(socket.lowest_layer(), resolverResults.begin(), resolverResults.end());
        socket.handshake(boost::asio::ssl::stream_base::client);
        
        // Next we handle the get request (basically we are requesting data from the site/client)
        boost::beast::http::request<boost::beast::http::string_body> reqhttps{boost::beast::http::verb::get, "/", 11};
        reqhttps.set(boost::beast::http::field::host, hostName);
        reqhttps.set(boost::beast::http::field::user_agent, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36");
        
        // Afterwards, we send and read the HTTP request whilst using SSL
        boost::beast::http::write(socket, reqhttps);
        boost::beast::http::read(socket, responseBuffer, response);
        
        // htmlContent stores the HTML content/response in a buffer
        htmlContent = boost::beast::buffers_to_string(response.body().data());;
        
        // While loop to check thoroughly in the HTML for links and output it in a text file
        while(std::regex_search(htmlContent, matchingLinks, linkKey)) 
        {
            std::string linkObtained = matchingLinks[1];
            linkObtained = convertURL(hostName, linkObtained);
            links.insert(linkObtained);
            htmlContent = matchingLinks.suffix().str();
        }
        
        std::ofstream outputFile("Web_Scrapped.txt", std::ios::out);
        
        if (outputFile.is_open()) 
        {
            for(const auto& link:links) 
            {
                outputFile << "Link Obtained: " << link << std::endl;
            }
            outputFile.close();
        } 
        else 
        {
            std::cerr << "File Error: cannot be opened" << std::endl;
        }
    
    }
    catch(const std::exception& cerr)
    {
        std::cerr << "Error: " << cerr.what() << std::endl;
    }
    
}

// Lastly, we have a convertURL string function that converts appropriate URL's into tangible links that can be clicked/searched
std::string convertURL(const std::string& INITIAL_URL, const std::string& URL_LINK) 
{
    if(URL_LINK.find("https://") == 0 || URL_LINK.empty()) 
    {
        return URL_LINK;
    }
    if(URL_LINK[0] == '/') 
    {
        return INITIAL_URL + URL_LINK;
    }

    return INITIAL_URL + "/" + URL_LINK;
}
