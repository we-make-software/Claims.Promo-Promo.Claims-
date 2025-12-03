#ifndef AddressResolutionProtocolInterface_H
#define AddressResolutionProtocolInterface_H
    #include "../InternetProtocolVersion4Interface/.h"
    
    LibraryHeader(AddressResolutionProtocolInterface){
        BootstrapLibraryHeader;
        RXLibraryHeader(struct GatewayDevice*);
        struct{
            u16 Type;
        }Default;
    };

    #define AddressResolutionProtocol\
        GetAddressResolutionProtocolInterface()->


#endif