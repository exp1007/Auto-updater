#define CURL_STATICLIB

#include <curl/curl.h>

#include <iostream>
#include <fstream>

#include "Networking.hpp"

// Callback function to capture the response data
size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append(static_cast<char*>(contents), total_size);
    return total_size;
}

size_t writeFileData(void* ptr, size_t size, size_t nmemb, void* stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE*)stream);
    return written;
}

std::string downloadFile(const char* url, const char* path) {
    CURL* curl;
    CURLcode res;
    struct curl_slist* headers = NULL;
    FILE* file = fopen(path, "wb");

    auto headerType = "Content-Type: application/json";

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (curl) {
        // Set the URL you want to request
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set the Authorization header with the Bearer token
        headers = curl_slist_append(headers, headerType);

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the write callback function to capture the response data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFileData);
        if (file)
        {
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

            // Perform the request
            res = curl_easy_perform(curl);

            fclose(file);
        }
        else
            return "Failed to open file.";

        // Always cleanup
        curl_easy_cleanup(curl);

        // Check for errors
        if (res != CURLE_OK) {
            return std::to_string(res);
        }
        else
            return "Success";
    }
}

std::string removeDoubleQuotes(const std::string& input) {
    std::string result;

    for (char c : input) {
        if (c != '"') {
            result += c;
        }
    }

    return result;
}