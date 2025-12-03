#include "../.h"
RX(struct GatewayDevice*GD){

}
BootstrapBody({
}){
}
LibraryBody(InternetProtocolVersion4Interface,
    BootstrapLibraryBody,
    RXLibraryBody,
    {htons(2048)})