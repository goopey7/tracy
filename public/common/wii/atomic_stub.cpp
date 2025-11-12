#ifdef __wii__
#  include <cstdint>
#  include <ogc/irq.h>
extern "C"
{
    uint64_t __atomic_load_8( const volatile void* ptr, int )
    {
        uint32_t level = IRQ_Disable();
        uint64_t val = *(const volatile uint64_t*)ptr;
        IRQ_Restore( level );
        return val;
    }

    void __atomic_store_8( volatile void* ptr, uint64_t val, int )
    {
        uint32_t level = IRQ_Disable();
        *(volatile uint64_t*)ptr = val;
        IRQ_Restore( level );
    }

    uint64_t __atomic_fetch_add_8( volatile void* ptr, uint64_t val, int )
    {
        uint32_t level = IRQ_Disable();
        uint64_t old = *(volatile uint64_t*)ptr;
        *(volatile uint64_t*)ptr = old + val;
        IRQ_Restore( level );
        return old;
    }
}
#endif
