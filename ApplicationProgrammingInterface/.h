#ifndef ApplicationProgrammingInterface_H
#define ApplicationProgrammingInterface_H
    #include "../LauncherInterface/.h"
    #define Atomic64Header(name) \
        atomic64_t name

    struct ApplicationProgrammingInterface{
        struct{
            void(*Init)(void);
            void(*Exit)(void);
            void(*Restart)(void);
            void*(*Get)(u8*);
            void(*Register)(u8*,void*);
            bool Status;
            Atomic64Header(spaces);
        }Default;    
    };
    extern
    #ifdef OnlyForApplicationProgrammingInterface
    struct ApplicationProgrammingInterface Main;
    #endif
    struct ApplicationProgrammingInterface*GetApplicationProgrammingInterface(void);

    #define OverwriteLauncherInterface_Setup(name)\
        static void __exit _LauncherInterfaceSetupExit(void){}\
        module_exit(_LauncherInterfaceSetupExit);\
        LauncherInterface_Setup(name)

    #define LibraryHeader(name) \
        struct WMS##name; \
        __attribute__((unused)) static struct WMS##name* Get##name(void){\
            static struct WMS##name*LibraryLink=NULL;\
            if(!LibraryLink) \
                LibraryLink=(struct WMS##name*)GetApplicationProgrammingInterface()->Default.Get(#name);\
            return LibraryLink;\
        }\
        struct WMS##name

     
    #define LibraryBody(name,...)\
        OverwriteLauncherInterface_Setup(name){\
            static u8*Name=#name;\
            static struct WMS##name Library={__VA_ARGS__};\
            GetApplicationProgrammingInterface()->Default.Register(Name,&Library);\
        }    
    
    #define ApplicationProgramming\
        GetApplicationProgrammingInterface()->


#endif