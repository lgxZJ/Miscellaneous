#include "FakeServer.h"
#include "logger.h"

#include <ctime>
#include <exception>
#include <iostream>
#include <string>


using namespace std;

////////////////////////////////////////////////////////////////////////////////////////

int                 FakeServer::m_ports[]		 = { 8888 ,10240, 10860 };
int                 FakeServer::m_index = -1;
struct MHD_Daemon*  FakeServer::m_daemon		 = nullptr;
map<string, string> FakeServer::m_fileMimeMappings = {
                                { "html", "text/html" },
                                { "css", "text/css" },
                                { "txt", "text/plain" },

                                { "png", "image/png" },
                                { "gif", "image/gif" },
                                { "ico", "image/x-icon" },
                                { "jpg", "image/jpeg" },
                                { "svg", "image/svg+xml" },

                                { "ogg", "audio/ogg" },
                                { "wav", "audio/x-wav" },
                                { "mp3", "audio/mpeg" },

                                { "mp4", "video/mp4" },

                                { "swf", "application/x-shockwave-flash" },
                                { "js", "application/x-javascript" },

                                { "eot", "application/vnd.ms-fontobject" },
                                { "ttf", "application/font-sfnt" },
                                { "woff", "application/font-woff" },
                                { "woff", "font/woff2" },
    };

////////////////////////////////////////////////////////////////////////////////////////

bool FakeServer::start()
{
    std::time_t rawTime;
    time(&rawTime);
    auto localTime = localtime(&rawTime);

    Logger::instance()	<< "=====================================\n"
                        << "==    try starting local server\n"
                        << "==    " << asctime(localTime) << "\n"
                        << "=====================================\n";

    for (int i = 0; i < sizeof(m_ports) / sizeof(m_ports[0]); ++i) {
        Logger::instance() << "trying port:" + to_string(m_ports[i]);

        m_daemon = MHD_start_daemon(MHD_USE_POLL_INTERNAL_THREAD,
                                    m_ports[i], NULL, NULL,
                                    &connectionHandler, NULL, MHD_OPTION_END);
        if (m_daemon == NULL) {
            Logger::instance() << " failed!\n";
        }
        else {
            Logger::instance() << " succeed, server started!\n";
            m_index = i;
            break;
        }
    }

    if (m_daemon == NULL) {
        Logger::instance() << "--server failed to start--\n";
        return false;
    }
    return true;
}

void FakeServer::stop()
{
    if (m_daemon != nullptr)
        MHD_stop_daemon(m_daemon);

    Logger::instance() << "server stopped\n";
}

int FakeServer::getPort()
{
    return m_ports[m_index];
}

int FakeServer::connectionHandler(
                            void *cls,
                            MHD_Connection *connection,
                            const char *url,
                            const char *method,
                            const char *version,
                            const char *upload_data,
                            size_t *upload_data_size,
                            void **con_cls)
{
    Logger::instance() << string("request received(") + string(method) + string("):") + url + "\n";

    std::ifstream inputFile("./pattern.txt", ios::in);
    if (!inputFile.is_open()) {
        Logger::instance() << "file not exists : ./pattern.txt \n";
        return MHD_NO;
    }

    int returnValue = MHD_NO;
    const int maxLineLength = 200;
    char urlBuffer[maxLineLength] = {0};
    char responseBuffer[maxLineLength] = {0};
    while (!inputFile.eof()) {
        if (!inputFile.getline(&urlBuffer[0], maxLineLength, '\n') &&
            urlBuffer[0] == '[' &&
            urlBuffer[strlen(urlBuffer)] == ']') {

            Logger::instance() << "getline error: url\n";
            returnValue = MHD_NO;
            break;
        }
        if (!inputFile.getline(&responseBuffer[0], maxLineLength, '\n')) {
            Logger::instance() << "getline error: reponse\n";
            returnValue = MHD_NO;
            break;
        }

        if (string(urlBuffer + 1, strlen(urlBuffer) - 2) == url) {
            returnValue = respondFromBuffer(connection, responseBuffer);
            break;
        }
    }

    inputFile.close();
    if (returnValue == MHD_NO) {
        Logger::instance() << "no pattern found, no reponse!\n\n";
    }
    return returnValue;
}

int FakeServer::respondFromBuffer(MHD_Connection *connection, const char* buffer)
{
    struct MHD_Response* response;
    response = MHD_create_response_from_buffer(strlen(buffer),
                                                (void*)buffer,
                                                MHD_RESPMEM_MUST_COPY);

    const char* fileMime = getProperMimeType(".txt");
    if (MHD_add_response_header (response, "Content-Type", fileMime) == MHD_NO) {
        Logger::instance() << "MHD_add_response_header error!\n";
        return MHD_NO;
    }

    if (MHD_queue_response (connection, MHD_HTTP_OK, response) == MHD_NO) {
        Logger::instance() << "MHD_queue_response error!\n";
        return MHD_NO;
    }

    MHD_destroy_response (response);
    Logger::instance() << "reponse result: " << buffer << "\n\n";
    return MHD_YES;
}

const char* FakeServer::getProperMimeType(const std::string& urlString)
{
    auto dotPos = urlString.find_last_of('.');
    if (dotPos == std::string::npos) {
        Logger::instance() << "'.' not found, assume general purpose file!\n";
        return "application/octet-stream";
    } else {
        auto fileExtension = urlString.substr(dotPos + 1);
        auto iter = m_fileMimeMappings.find(fileExtension);
        if (iter == m_fileMimeMappings.end())
        {
            Logger::instance() << "file mime not found, assume general purpose file!\n";
            return "application/octet-stream";
        } else {
            return iter->second.c_str();
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
