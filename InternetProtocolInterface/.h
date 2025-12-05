#ifndef InternetProtocolInterface_H
#define InternetProtocolInterface_H
    #include "../GatewayInterface/.h"

    struct InternetProtocolFrame{
        u8 Version:5,Client:1,Verified:1,Block:1;
        struct{
            struct list_head this,Clients;
        }list;
        struct{
            spinlock_t this;
        }lock;
        struct{           
             struct delayed_work worker; 
        }BackgroundTask;
        struct{
            AtomicHeader(response,request);  
            Atomic64Header(expiry,worker);  
        }status;
        union{
            struct InternetProtocolFrame*Server;
            struct GatewayDevice*Router;
        }link;
    };
    LibraryHeader(InternetProtocolInterface){
        RXLibraryHeader(u8*,struct InternetProtocolFrame*,struct NetworkAdapterInterfaceReceiver*);
        struct{
            void(*Delete)(struct InternetProtocolFrame*);
            void(*Exit)(struct GatewayDevice*);
            void(*Init)(struct InternetProtocolFrame*);
        }Default;
    };

    #define InternetProtocol\
        GetInternetProtocolInterface()->

#endif