#include "../.h"
SKBTX(struct GatewayDevice*GD,u16*type){
    if(!GD||GD->Default.block||!HasEnoughSpaceBytes(sizeof(struct sk_buff)+1514))
        return NULL;
    SKBTXGet(NetworkAdapter,GD->NAD);
    AtomicIncrements(&GD->status.response);
    memcpy(skb_put(skb,12),GD->Address,6);
    *(u16*)skb_put(skb,2)=*type;
    skb->protocol=*type;
    skb->network_header=skb->tail;
    SKBTXReturn;
}
DelayedBackgroundTask(GatewayDevice,worker){
    Lock(&this->lock.this);
    this->Default.block=true;
    Unlock(&this->lock.this);
    Gateway Memory.GatewayDevice.Free(this);
}
MemoryCacheBody(GatewayDevice,{
    CancelDelayedWorkGatewayDeviceworker(this);
    
    Print("MemoryCacheBody GatewayDevice Free");
    while(!this->Default.block&&(AtomicValue(&this->status.response)||AtomicValue(&this->status.request)))
        cond_resched();   
    Lock(&this->NAD->lock.GatewayDevices);
        list_del(&this->list.this);
    Unlock(&this->NAD->lock.GatewayDevices);
}){ 
    InitDelayedWorkGatewayDeviceworker(this);
    ListInit(&this->list.this);
    AtomicInit(&this->status.request);
    AtomicInit(&this->status.response);
    Atomic64Init(&this->status.expiry);
    LockInit(&this->lock.this);
    this->Default.block=false;
}
Void DefaultCancel(struct GatewayDevice*GD,struct sk_buff*skb){
    if(!GD)return;
    if(skb){
        AtomicDecrements(&GD->status.response);
        kfree_skb(skb);
    }
    if(!AtomicValue(&GD->status.response)&&!AtomicValue(&GD->status.request)){
        if(!ApplicationProgramming Default.Status){
            Lock(&GD->lock.this);
                if(Now<Atomic64Value(&GD->status.expiry)){
                    ScheduleDelayedWorkGatewayDeviceworker(GD,(Atomic64Value(&GD->status.expiry)>Now)?(Atomic64Value(&GD->status.expiry) - Now)/1000000ULL: 0);
                    Unlock(&GD->lock.this);
                    return;
                }  
            Unlock(&GD->lock.this);
        }
        Gateway Memory.GatewayDevice.Free(GD);  
    }
}

Void DefaultSend(struct GatewayDevice*GD,struct sk_buff*skb){
    Print("Gateway DefaultSend");
    dev_queue_xmit(skb);
    AtomicDecrements(&GD->status.response);
    DefaultCancel(GD,NULL);
}
Void DefaultExit(struct NetworkAdapterDevice*this){
    struct GatewayDevice *GD, *tmp;
    Lock(&this->lock.GatewayDevices);
    list_for_each_entry_safe(GD,tmp,&this->list.GatewayDevices,list.this)
      Gateway Memory.GatewayDevice.Free(GD);
    Unlock(&this->lock.GatewayDevices);
}


Void DefaultInit(struct NetworkAdapterDevice*nad){
    Lock(&nad->lock.GatewayDevices);
        struct GatewayDevice*gd=Gateway Memory.GatewayDevice.Create();
        if(!gd){
            Unlock(&nad->lock.GatewayDevices);
            return;
        }
        memcpy(gd->Address,Gateway Default.Broadcast,6);
        gd->NAD=nad;
        list_add(&gd->list.this,&nad->list.GatewayDevices);
    Unlock(&nad->lock.GatewayDevices);
    GatewayDeviceExpiry(5);
    Print("Gateway DefaultInit");
    //init to something new we know the packet wee need to make DHCP packet
}
Void DoRX(struct GatewayDevice*gd,struct NetworkAdapterInterfaceReceiver*nair){
    struct sysinfo info;
    si_meminfo(&info); 
    Atomic64Set(ApplicationProgramming Default.spaces,(u64)info.freeram * info.mem_unit);
    GatewayDeviceExpiry(5);
    GatewayOverFlowControl;
    AtomicIncrements(&gd->status.request);
    if(!gd||gd->Default.block||!HasEnoughSpaceBytes(1073741824))goto Cancel;
        Print("Gateway RX");  
    Cancel:
    AtomicDecrements(&gd->status.request);
    DefaultCancel(gd,NULL);
}
RX(struct NetworkAdapterInterfaceReceiver*nair){
    if(nair->NAD->Status==Overloaded||Now<MillisecondsAdd(nair->start,250))return;
    nair->start=Now;
    struct GatewayDevice*GD;
    list_for_each_entry(GD,&nair->NAD->list.GatewayDevices, list.this) {
        if(memcmp(nair->data,GD->Address,6)==0&&cancel_delayed_work_sync(&GD->BackgroundTask.worker)) {
            DoRX(GD,nair);
            return;
        }
    }
    Lock(&nair->NAD->lock.GatewayDevices);
        GD=NULL;
        list_for_each_entry(GD, &nair->NAD->list.GatewayDevices, list.this) {
            if (memcmp(nair->data,GD->Address, 6) == 0&&cancel_delayed_work_sync(&GD->BackgroundTask.worker)) {
                Unlock(&nair->NAD->lock.GatewayDevices);
                DoRX(GD,nair);
                return;
            }
        }
        GD=Gateway Memory.GatewayDevice.Create();
        if(!GD){
            Unlock(&nair->NAD->lock.GatewayDevices);
            return;
        }
        memcpy(GD->Address, nair->data,6);
        GD->NAD=nair->NAD;
        list_add(&GD->list.this,&nair->NAD->list.GatewayDevices);
    Unlock(&nair->NAD->lock.GatewayDevices);
    DoRX(GD,nair);
}
BootstrapBody({
    Print("Gateway Bootstrap Exit");
    Gateway Memory.GatewayDevice.Exit();
}){
    Atomic64Init(&ApplicationProgramming Default.spaces);
    Gateway Memory.GatewayDevice.Init();
}
LibraryBody(GatewayInterface,
    BootstrapLibraryBody,
    RXLibraryBody,
    SKBTXLibraryBody,
    {MemoryCacheBodyFunction(GatewayDevice)},
    {DefaultInit,DefaultExit,DefaultSend,DefaultCancel,{255,255,255,255,255,255}}
)
    