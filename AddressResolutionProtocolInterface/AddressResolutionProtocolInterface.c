#include "../.h"
RX(struct GatewayDevice*GD){

}
BootstrapBody({
}){
}
LibraryBody(AddressResolutionProtocolInterface, 
    BootstrapLibraryBody,
    RXLibraryBody,
    {htons(2054)})