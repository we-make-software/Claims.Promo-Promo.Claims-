#ifndef BootstrapInterface_H
#define BootstrapInterface_H
    #include "../ApplicationProgrammingInterface/.h"
    #define BootstrapLibraryBody\
        {BC,BO}

    #define BootstrapLibraryHeader\
        struct{\
            void(*BC)(void);\
            void(*BO)(void);\
        }BI   

    #define BootstrapBody(...)\
        Void BC(void)__VA_ARGS__\
        Void BO(void)

    LibraryHeader(BootstrapInterface){
        BootstrapLibraryHeader;
    };
   #ifdef MODULE

        #define PrintBinary(data,size)\
        {\
            int _PBB_i,_PBB_j;\
            char _PBB_line[(size)*8+(size)];\
            char*_PBB_ptr=_PBB_line;\
            for(_PBB_i=0;_PBB_i<(size);_PBB_i++){\
                u8 _PBB_byte=(data)[_PBB_i];\
                for(_PBB_j=0;_PBB_j<8;_PBB_j++)\
                    *_PBB_ptr++=(_PBB_byte&(1<<(7-_PBB_j)))?'1':'0';\
                *_PBB_ptr++=' ';\
            }\
            *(_PBB_ptr-1)='\0';\
            printk(KERN_INFO"%s\n",_PBB_line);\
        }

        #define Static __maybe_unused static

        #define Print(msg)\
                printk(KERN_INFO  msg "\n")

        #define PrintArg(msg,...) \
                printk(KERN_INFO msg "\n", ##__VA_ARGS__)           
        
    #else

        #define PrintBinary(data,size)
        
        #define Static static
        
        #define Print(msg)

        #define PrintArg(msg,...)

    #endif    
       
    #define Void Static void

    #define Struct Static struct
      
    #define MemoryCacheHeaderFunction(name)\
        struct{\
            void(*Init)(void);\
            void(*Exit)(void);\
            void(*Free)(struct name*);\
            struct name*(*Create)(void);\
        }

        
    #define MemoryCacheBodyFunction(name)\
        {\
            name##_CacheInit,\
            name##_CacheExit,\
            name##_CacheFree,\
            name##_CacheCreate\
        }


    #define MemoryCacheBody(name,...)\
        Struct kmem_cache*name##_MemoryCache;\
        Void name##_EventCacheFree(struct name* this)__VA_ARGS__\
        Void name##_CacheInit(void){\
            name##_MemoryCache=kmem_cache_create(#name,sizeof(struct name),0,SLAB_HWCACHE_ALIGN,NULL);\
        }\
        Void name##_CacheFree(struct name* this) {\
            if(!this)return;\
            static DEFINE_MUTEX(name##_cachefree_lock);\
            mutex_lock(&name##_cachefree_lock);\
            if (!this) {\
                mutex_unlock(&name##_cachefree_lock);\
                return;\
            }\
            name##_EventCacheFree(this);\
            kmem_cache_free(name##_MemoryCache, this);\
            this = NULL;\
            mutex_unlock(&name##_cachefree_lock);\
        }\
        Void name##_CacheExit(void){\
            kmem_cache_destroy(name##_MemoryCache);\
        }\
        Void name##_PreconfigCache(struct name*);\
        Struct name*name##_CacheCreate(void){\
            struct name*MCBCC=kmem_cache_alloc(name##_MemoryCache,GFP_KERNEL);\
            if(!MCBCC)return NULL;\
            name##_PreconfigCache(MCBCC);\
            return MCBCC;\
        }\
        Void name##_PreconfigCache(struct name*this)





    #define Bootstrap\
        GetBootstrapInterface()->

    #define Now\
        ktime_get()
        
    #define NanosecondsAdd(t,x)\
        ktime_add_ns((t),(x))

    #define MicrosecondsAdd(t,x)\
        ktime_add_us((t),(x))
    
    #define MillisecondsAdd(t,x)\
        ktime_add_ms((t),(x))
    
    #define SecondsAdd(t,x)\
        ktime_add_seconds((t),(x))
    
    #define MinutesAdd(t,x)\
        ktime_add_seconds((t),(x)*60) 
    
    #define HoursAdd(t,x)\
        ktime_add_seconds((t),(x)*3600)
    
    #define DaysAdd(t,x)\
        ktime_add_seconds((t),(x)*86400) 

    #define ListInit(...)\
        {\
            struct list_head*_LI_L[]={__VA_ARGS__,NULL};\
            u8 _LI_I=0;\
            while(_LI_L[_LI_I]!=NULL){\
                INIT_LIST_HEAD(_LI_L[_LI_I]);\
                _LI_I++;\
            }\
        }


    #define LockInit(...)\
        {\
            struct mutex*_LI_L[]={__VA_ARGS__,NULL};\
            u8 _LI_I=0;\
            while(_LI_L[_LI_I]!=NULL){\
                mutex_init(_LI_L[_LI_I]);\
                _LI_I++;\
            }\
        }

    #define Lock(name)\
            mutex_lock(name)

    #define Unlock(name)\
            mutex_unlock(name)
    
    #define AtomicInit(...)\
        {\
            atomic_t*_LI_L[]={__VA_ARGS__,NULL};\
            u8 _LI_I=0;\
            while(_LI_L[_LI_I]!=NULL){\
                atomic_set(_LI_L[_LI_I],0);\
                _LI_I++;\
            }\
        }    
     
    #define AtomicIncrements(name)\
        atomic_inc_return(name)

    #define AtomicDecrements(name)\
        atomic_dec_return(name)

    #define AtomicValue(name)\
        atomic_read(name)
    
    #define AtomicHeader(name)\
        atomic_t name    
        
    #define Atomic64Header(name) \
    atomic64_t name

    #define Atomic64Init(...) \
    { \
        atomic64_t *_LI_L[] = {__VA_ARGS__, NULL}; \
        u8 _LI_I = 0; \
        while (_LI_L[_LI_I] != NULL) { \
            atomic64_set(_LI_L[_LI_I], 0); \
            _LI_I++; \
        } \
    }



    #define Atomic64Value(name) \
        atomic64_read(name)

    #define Atomic64AddMinutes(name,minutes){\
            ktime_t _a64am_now=ktime_get()+((minutes)*60000000000ULL);\
            if (_a64am_now>Atomic64Value(name))\
                 atomic64_add(_a64am_now,name);\
        }
    
    #define Atomic64Set(name, value)\
        atomic64_set(&(name), (value))    

    #define Atomic64SetNow(name) \
        atomic64_set(&(name), ktime_to_ns(ktime_get()))


    #define WorkBackgroundTask(name, property)\
        Void work##name##property##BackgroundTaskCallback(struct name*);\
        Void work##name##property(struct work_struct*work){\
            work##name##property##BackgroundTaskCallback(container_of(work,struct name,BackgroundTask.property));\
        }\
        Void InitWork##name##property(struct name*this){\
            INIT_WORK(&this->BackgroundTask.property,work##name##property);\
        }\
        Void work##name##property##BackgroundTaskCallback(struct name*this)

 
        #define DelayedBackgroundTask(name,property) \
            Void ScheduleDelayedWork##name##property(struct name*this,unsigned long ms){\
                schedule_delayed_work(&this->BackgroundTask.property,msecs_to_jiffies(ms));\
            }\
            Void CancelDelayedWork##name##property(struct name*this){\
                if(delayed_work_pending(&this->BackgroundTask.property))\
                    cancel_delayed_work(&this->BackgroundTask.property);\
            }\
            Void delayed##name##property##Callback(struct name*);\
            Void delayed##name##property(struct work_struct*work){\
                delayed##name##property##Callback(\
                    container_of(container_of(work,struct delayed_work,work),struct name,BackgroundTask.property)\
                );\
            }\
            Void InitDelayedWork##name##property(struct name*this){\
                INIT_DELAYED_WORK(&this->BackgroundTask.property,delayed##name##property);\
            }\
            Void delayed##name##property##Callback(struct name*this)


        #ifdef OnlyForBootstrapInterfaceInterface
            #define Open\
                BI.BO()
           
            #define Close\
                BI.BC()

        #endif
#endif