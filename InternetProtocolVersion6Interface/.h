#ifndef InternetProtocolVersion6Interface_H
#define InternetProtocolVersion6Interface_H
    #include "../AddressResolutionProtocolInterface/.h"

    LibraryHeader(InternetProtocolVersion6Interface){
        BootstrapLibraryHeader;
        RXLibraryHeader(struct GatewayDevice*);
        struct{
            u16 Type;
        }Default;
    };

    #define InternetProtocolVersion6\
        GetInternetProtocolVersion6Interface()->

#endif