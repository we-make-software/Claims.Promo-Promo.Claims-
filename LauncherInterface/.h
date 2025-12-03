#ifndef LauncherInterface_H
#define LauncherInterface_H
    #include <linux/module.h>
    #include <linux/kernel.h>
    #include <linux/notifier.h>
    #include <linux/reboot.h>
    #ifdef MODULE
        #define LauncherInterface_Init module_init
    #else
        #define LauncherInterface_Init late_initcall
    #endif
    #define LauncherInterface_Setup(name)\
        static void _LauncherInterface_Run(void);\
        MODULE_DESCRIPTION(#name);\
        MODULE_LICENSE("GPL");\
        MODULE_AUTHOR("Claims.Promo & Promo.Claims");\
        static int __init _LauncherInterface_Preinit(void){\
            _LauncherInterface_Run();\
            return 0;\
        }\
        LauncherInterface_Init(_LauncherInterface_Preinit);\
        static void _LauncherInterface_Run(void)

#endif