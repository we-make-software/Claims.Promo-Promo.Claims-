#define OnlyForApplicationProgrammingInterface
#include "../.h"
struct Content{
    u8*Name;
    void*Library;
    struct list_head List;
};
static LIST_HEAD(Contents);
static void DefaultRegister(u8*Name,void*Library){
    struct Content*this=kmalloc(sizeof(*this), GFP_KERNEL);
    *this=(struct Content){Name,Library};
    INIT_LIST_HEAD(&this->List);
    list_add(&this->List,&Contents);
}
Void RemoveAll(void){
    struct Content*this,*temporary;
    list_for_each_entry_safe(this,temporary,&Contents,List){
        list_del(&this->List);
        kfree(this);
    }
}
static void DefaultInit(void){

   
    if(Main.Default.Status)return;
    Main.Default.Status=true;
    GetBootstrapInterface()->BI.BO();
}
Static bool ExitEvent=false;
static void DefaultExit(void){

    if(!Main.Default.Status||ExitEvent)return;
    ExitEvent=true;
    Main.Default.Status=false;
    GetBootstrapInterface()->BI.BC();
    RemoveAll();
}
static void*DefaultGet(u8*name){
    struct Content*this;
    list_for_each_entry(this,&Contents,List)
        if(strcmp(this->Name,name)==0)
            return this->Library;
    return NULL;
}
static void DefaultRestart(void){
    DefaultExit();
    kernel_restart(NULL);
}
struct ApplicationProgrammingInterface Main={{DefaultInit,DefaultExit,DefaultRestart,DefaultGet,DefaultRegister,false}};
struct ApplicationProgrammingInterface*GetApplicationProgrammingInterface(void){
    return &Main;
}
EXPORT_SYMBOL(GetApplicationProgrammingInterface);
OverwriteLauncherInterface_Setup(ApplicationProgrammingInterface){}