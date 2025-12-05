#include "../.h"
void DefaultAutoChoiceExit(struct InternetProtocolFrame*ipf){
    spinlock_t*lock=ipf->Client?&ipf->link.Server->lock.this:&ipf->link.Router->lock.Servers;
    Lock(lock);
    switch(ipf->Version)
    {
        case 4:
            Unlock(&lock);
                InternetProtocolVersion4 Memory.I.Free((struct InternetProtocolVersion4Frame*)ipf);
            Lock(&lock);
        break;
        case 6:
            Unlock(&lock);
                InternetProtocolVersion6 Memory.I.Free((struct InternetProtocolVersion6Frame*)ipf);
            Lock(&lock);
        break;
    }
    Unlock(&lock);
}
Void DefaultExit(struct GatewayDevice*gd){
    struct InternetProtocolFrame *entry, *tmp;
    list_for_each_entry_safe(entry,tmp,&gd->list.Servers, list.this)
        DefaultAutoChoiceExit(entry);
}
Void DefaultDelete(struct InternetProtocolFrame*ipf){
    //where are here when wee call DefaultAutoChoiceExit
    if(ipf->Client){
        Lock(&ipf->link.Server->lock.this);
            list_del(&ipf->list.this);
        Unlock(&ipf->link.Server->lock.this);
    }else{
        struct GatewayDevice*gd=ipf->link.Router;
        Lock(&gd->lock.Servers);
            list_del(&ipf->list.this);
        Unlock(&gd->lock.Servers);
    }
}
Void DefaultDelaySet(struct InternetProtocolFrame*ipf){

}
DelayedBackgroundTask(InternetProtocolFrame,worker){
    DefaultAutoChoiceExit(this);
}
Void DefaultInit(struct InternetProtocolFrame*ipf){
    ipf->Block=false;
    ipf->Client=false;
    ipf->Verified=false;
    ListInit(&ipf->list.this,&ipf->list.Clients);
    LockInit(&ipf->lock.this);
    AtomicInit(&ipf->status.request,&ipf->status.response);
    InitDelayedWorkInternetProtocolFrameworker(ipf);
}
RX(u8*nextHeader,struct InternetProtocolFrame*ipf,struct NetworkAdapterInterfaceReceiver*nair){
    if(AtomicIncrements(&ipf->link.Server->status.request)==1&&!Atomic64Value(&ipf->link.Server->status.response)){
        Lock(&ipf->link.Server->lock.this);
        CancelDelayedWorkInternetProtocolFrameworker(ipf->link.Server);
        Unlock(&ipf->link.Server->lock.this);
    }
    if(AtomicIncrements(&ipf->status.request)==1&&!Atomic64Value(&ipf->status.response)){
        Lock(&ipf->lock.this);
        CancelDelayedWorkInternetProtocolFrameworker(ipf);
        Unlock(&ipf->lock.this);
    }
    //here wee need to check if wee still got time
    RXTestTime;
    //handle in version specific interface
    AtomicDecrements(&ipf->status.request);
    AtomicDecrements(&ipf->link.Router->status.request);

}
LibraryBody(InternetProtocolInterface,RXLibraryBody,{DefaultDelete,DefaultExit,DefaultInit})