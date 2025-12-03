#ifndef SecureShellInterface_H
#define SecureShellInterface_H
    #include "../InternetProtocolVersion6Interface/.h"

    LibraryHeader(SecureShellInterface){
        struct{
            u16 Port;
        }Default;
    };

    #define SecureShell\
        GetSecureShellInterface()->

#endif