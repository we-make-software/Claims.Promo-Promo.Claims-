#ifndef GatewayInterface_H
#define GatewayInterface_H
#define CONFIG_64BIT 1
    #include "../NetworkAdapterInterface/.h"
    #include <linux/kref.h>
    #include <linux/atomic.h>
    #include <linux/mm.h>

    struct GatewayDevice{
        struct NetworkAdapterDevice*NAD;
        u8 Address[6];
        struct{
            struct list_head this;
        }list;
        struct{
            struct mutex this;
        }lock;
        struct{
            AtomicHeader(response);
            AtomicHeader(request);    
            Atomic64Header(expiry);
        }status;
        struct{           
             struct delayed_work worker; 
        }BackgroundTask;
        struct{
            u8 block;
        }Default;
    };

    #define RXLibraryHeader(...)\
        struct{void(*RW)(__VA_ARGS__);}NALH
    
    LibraryHeader(GatewayInterface){
        BootstrapLibraryHeader;
        RXLibraryHeader(struct NetworkAdapterInterfaceReceiver*);
        SKBTXLibraryHeader(struct GatewayDevice*, u16 *);
        struct{
            MemoryCacheHeaderFunction(GatewayDevice)GatewayDevice;
        }Memory;
        struct{
            void(*Init)(struct NetworkAdapterDevice*);
            void(*Exit)(struct NetworkAdapterDevice*);
            void(*Send)(struct GatewayDevice*,struct sk_buff*);   
            void(*Cancel)(struct GatewayDevice*,struct sk_buff*); 
            u8 Broadcast[6];
        }Default;
    };
    
    #define GatewayOverFlowControl\
        if(gd->NAD->Status==Overloaded||Now>MillisecondsAdd(nair->start,100)||gd->Default.block){\
            Gateway Default.Cancel(gd,NULL);\
            return;\
        }\
        nair->start=Now



    #define SKBTXLibraryBody\
        {DefaultTXCreate}    

    #define Gateway\
        GetGatewayInterface()->


    #define RXData\
        nair->data

    #define RXMove(length) \
        RXData+=length

    #define IEEE802_3TXStart\
        struct sk_buff*skb=Gateway TXLH.Create(gd);\
        if(!skb)
    

    #define IEEE802_3TXSend\
        Gateway Default.Send(gd->NAD,skb)
    
    #define TXCancel\
        Gateway Default.Cancel(gd,skb)
    
    #define GatewayDeviceExpiry(m)\
        Atomic64AddMinutes(&gd->status.expiry,m);


    
    #define RXCall(name)\
        name NALH.RW
        
    #define RX(...)\
        Void RC(__VA_ARGS__)
    
    #define HasEnoughSpaceBytes(bytes) (atomic64_read(&ApplicationProgramming Default.spaces) >= (u64)(bytes))

#endif