#include "../.h"
WorkBackgroundTask(NetworkAdapterInterfaceReceiver,worker){
    if(this->NAD->Status==Overloaded||Now>MillisecondsAdd(this->start, 250))goto Cancel;
    this->start=Now;
    this->data+=6;
    Print("NetworkAdapterInterfaceReceiver RX");
    // RXCall(Gateway)(this);
    Cancel:
    NetworkAdapter Memory.NAIR.Free(this);
}
MemoryCacheBody(NetworkAdapterInterfaceReceiver,{
    if(this->skb)
        kfree_skb(this->skb); 
}){ 
    InitWorkNetworkAdapterInterfaceReceiverworker(this);
}
MemoryCacheBody(NetworkAdapterDevice,{
    list_del(&this->list.this);
}){ 
    ListInit(&this->list.this,&this->list.GatewayDevices);  
}
SKBTX(struct NetworkAdapterDevice*nad){
    if(!nad)return NULL;
    struct sk_buff*skb=alloc_skb(1518, GFP_KERNEL);
    if(!skb)return NULL;
    skb->priority=0;
    skb->mark=0;  
    skb_reset_mac_header(skb);
    memcpy(skb_mac_header(skb)+6,nad->packet.dev->dev_addr,6);
    skb->dev=nad->packet.dev;
    skb->ip_summed=CHECKSUM_NONE;
    return skb;
}
static int NAIPF(struct sk_buff*skb,struct net_device*,struct packet_type*pt,struct net_device*){
    if(!skb||!skb->dev||skb->len<34||skb->pkt_type==PACKET_OUTGOING||!ApplicationProgramming Default.Status)return NET_RX_SUCCESS;
    u16*header=(u16*)skb_mac_header(skb);
    if(!header)return NET_RX_SUCCESS;
    header+=6;
    if(*header!=InternetProtocolVersion4 Default.Type&&*header!=InternetProtocolVersion6 Default.Type&&*header!=AddressResolutionProtocol Default.Type)return NET_RX_SUCCESS;
    #ifdef MODULE
        if(*header==InternetProtocolVersion4 Default.Type||*header==InternetProtocolVersion6 Default.Type){  
            header+=(*header==InternetProtocolVersion4 Default.Type)?12:22;   
            if(*header==SecureShell Default.Port)return NET_RX_SUCCESS;
        }
    #endif  
    struct NetworkAdapterDevice*NAD=container_of(pt,struct NetworkAdapterDevice, packet); 
    if(NAD->Status==Overloaded&&Now<NAD->time.Status){
        kfree_skb(skb);
        return NET_RX_DROP;
    }
    struct NetworkAdapterInterfaceReceiver*NAIR=NetworkAdapter Memory.NAIR.Create();
    if(!NAIR){
        kfree_skb(skb);
        NAD->Status=Overloaded;
        NAD->time.Status=ktime_add(Now,ktime_set(20, 0));
        return NET_RX_DROP;
    }
    NAD->Status=Processed;
    NAIR->NAD=NAD;
    NAIR->skb=skb_get(skb);
    NAIR->data=skb_mac_header(skb);
    NAIR->start=Now;
    queue_work(system_highpri_wq,&NAIR->BackgroundTask.worker);
    return NET_RX_DROP;
}

Static u8 Exists(struct net_device*n)
{
    struct NetworkAdapterDevice*NAD;
    list_for_each_entry(NAD,&NetworkAdapter Default.this,list.this)
        if(memcmp(NAD->packet.dev->dev_addr,n->dev_addr,6)==0)
                return 1; 
    return 0;
}
BootstrapBody({
    struct NetworkAdapterDevice*NAD,*tmp_NAD;
    list_for_each_entry(NAD,&NetworkAdapter Default.this, list.this)
        Gateway Default.Exit(NAD);
        
    NAD=NULL;   
    list_for_each_entry(NAD,&NetworkAdapter Default.this, list.this)
        dev_remove_pack(&NAD->packet);
    synchronize_net();
    NAD=NULL;
    list_for_each_entry_safe(NAD,tmp_NAD,&NetworkAdapter Default.this, list.this)
        NetworkAdapter Memory.NAD.Free(NAD);
    NetworkAdapter Memory.NAIR.Exit();
    NetworkAdapter Memory.NAD.Exit();
}){
    ListInit(&NetworkAdapter Default.this);
    NetworkAdapter Memory.NAIR.Init();
    NetworkAdapter Memory.NAD.Init();
    struct net_device*n;
    for_each_netdev(&init_net,n){
        if((n->flags&IFF_LOOPBACK)||Exists(n))continue;
        struct NetworkAdapterDevice*NAD=NetworkAdapter Memory.NAD.Create();
        list_add(&NAD->list.this, &NetworkAdapter Default.this);
        NAD->packet=(struct packet_type){
            .dev=n,
            .type=htons(ETH_P_ALL),
            .func=NAIPF
        };
        NAD->Status=Processed;
        NAD->time.Status=0;
        dev_add_pack(&NAD->packet);
    }
    synchronize_net();
   // struct NetworkAdapterDevice*NAD;
    //list_for_each_entry(NAD,&NetworkAdapter Default.this, list.this)
      //  Gateway Default.Init(NAD);   
}
LibraryBody(NetworkAdapterInterface,
    BootstrapLibraryBody,
    SKBTXLibraryBody,
    {MemoryCacheBodyFunction(NetworkAdapterInterfaceReceiver),MemoryCacheBodyFunction(NetworkAdapterDevice)},
    {0})