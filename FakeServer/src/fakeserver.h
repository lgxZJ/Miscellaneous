#ifndef FAKESERVER_H
#define FAKESERVER_H

#include "microhttpd.h"
#include <map>

class FakeServer
{
public:
    static bool start();
    static void stop();

    static int	getPort();
private:
    static int connectionHandler (   void *cls,
                                     struct MHD_Connection *connection,
                                     const char *url,
                                     const char *method,
                                     const char *version,
                                     const char *upload_data,
                                     size_t *upload_data_size, void **con_cls);


    static int respondFromBuffer(MHD_Connection *connection, const char* buffer);
    static const char* getProperMimeType(const std::string& urlString);

private:
    static int					m_ports[];
    static int                                  m_index;
    static struct MHD_Daemon*	m_daemon;
    static std::string				m_cacheFolder;
    static std::map<std::string, std::string> m_fileMimeMappings;
};

#endif // FAKESERVER_H
