#include "../.h"
#ifdef MODULE
    static void __exit _LauncherInterface_Exit(void){
        ApplicationProgramming 
            Default.Exit();
    }
    module_exit(_LauncherInterface_Exit);
#else
    static int _LauncherInterface_Exit(struct notifier_block*,unsigned long, void*){
        ApplicationProgramming 
            Exit();
        return NOTIFY_OK;
    }
    static struct notifier_block _LauncherInterface_Notifier= {.notifier_call = _LauncherInterface_Exit};
#endif
LauncherInterface_Setup(Launcher){
    #ifndef MODULE
        register_reboot_notifier(&_LauncherInterface_Notifier);
    #endif
    ApplicationProgramming 
        Default.Init();
}