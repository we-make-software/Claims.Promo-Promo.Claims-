#define OnlyForBootstrapInterfaceInterface
#include "../.h"
BootstrapBody({
    NetworkAdapter Close;
    Gateway Close;
}){
    Gateway Open;
    NetworkAdapter Open;
}
LibraryBody(BootstrapInterface,BootstrapLibraryBody)