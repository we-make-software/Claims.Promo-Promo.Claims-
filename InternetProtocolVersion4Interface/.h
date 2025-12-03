#ifndef InternetProtocolVersion4Interface_H
#define InternetProtocolVersion4Interface_H
    #include "../InternetProtocolInterface/.h"

    LibraryHeader(InternetProtocolVersion4Interface){
        BootstrapLibraryHeader;
        RXLibraryHeader(struct GatewayDevice*);
        struct{
            u16 Type;
        }Default;
    };

    #define InternetProtocolVersion4 \
        GetInternetProtocolVersion4Interface()->


#endif