#include "../.h"
RX(struct GatewayDevice*GD){

}
BootstrapBody({
}){
}
LibraryBody(InternetProtocolVersion6Interface,
    BootstrapLibraryBody,
    RXLibraryBody,
    {htons(34525)})