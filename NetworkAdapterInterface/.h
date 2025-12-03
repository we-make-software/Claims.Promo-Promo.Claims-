#ifndef NetworkAdapterInterface_H
#define NetworkAdapterInterface_H
    #include "../BootstrapInterface/.h"
    #include <linux/ethtool.h>
    #include <linux/netdevice.h>
    #include <linux/skbuff.h>
    #include <linux/etherdevice.h>
    #include <linux/if_packet.h>
    #include <linux/ktime.h>
    #include <linux/slab.h>

    struct NetworkAdapterInterfaceReceiver{
        struct NetworkAdapterDevice*NAD;
        struct sk_buff*skb;
        u8*data;
        ktime_t start;
        struct{
            struct list_head tasks,pointers;
        }list;
        struct{
            struct work_struct worker;
        }BackgroundTask;
    };
    struct NetworkAdapterDevice{
        enum{Overloaded,Processed}Status;
        struct packet_type packet;
        struct{
            struct list_head this,GatewayDevices;
        }list;
        struct{
            ktime_t Status;
        }time;
        struct{
            struct mutex GatewayDevices;
        }lock;
    };
    #define SKBTXLibraryHeader(...)\
        struct{struct sk_buff*(*C)(__VA_ARGS__);}TXLH
   
    LibraryHeader(NetworkAdapterInterface){
        BootstrapLibraryHeader;
        SKBTXLibraryHeader(struct NetworkAdapterDevice*);
        struct{
            MemoryCacheHeaderFunction(NetworkAdapterInterfaceReceiver)NAIR;
            MemoryCacheHeaderFunction(NetworkAdapterDevice)NAD;
        }Memory;
        struct{
            ktime_t Speed;
            struct list_head this;
        }Default;
    };

   

 
    #define RXLibraryBody\
            {RC}


    #define SKBTX(...)\
        Struct sk_buff*DefaultTXCreate(__VA_ARGS__)

    #define SKBTXGet(name,...)\
        struct sk_buff*skb=name TXLH.C(__VA_ARGS__);\
        if(!skb)return NULL

    #define SKBTXReturn\
        return skb    

    #define NetworkAdapter\
            GetNetworkAdapterInterface()->

   
            
        
#endif