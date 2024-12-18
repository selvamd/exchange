#include <fstream>
#include <ostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "DataTypes.hpp"
#include "Logger.hpp"
#include <iomanip>
#include <curl/curl.h>

void curlreader(void *ptr, size_t size, size_t nmemb, void *uptr) { memcpy(uptr, ptr, size*nmemb); }

int main(int argc, char ** argv)
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    std::string log_level = "debug";

    if (setLogLevel(log_level) < 0)
    {
        LOG_ERROR << "Failed to set log level" << LOG_END;
        return -1;
    }


    CURL *curl;
    char userptr[1024];
    
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl)
    {
        for (int i=0; i < 10; i++)
        {
            memset(userptr,0,1024);
            curl_easy_setopt(curl, CURLOPT_URL, "http://finance.google.com/finance/info?client=ig&q=CTSH");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlreader);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)userptr);
            curl_easy_perform(curl);
            std::cout << userptr << std::endl;
        }
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
    return 0;
}
