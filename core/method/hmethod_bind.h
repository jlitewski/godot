#ifndef HMETHOD_BIND_H
#define HMETHOD_BIND_H

#include "core/object/method_info.h"
#include "core/variant/binder_common.h"
#include <cstdint>

VARIANT_BITFIELD_CAST(HMethodInfo::MethodFlags)

class HMethodBind {
    private:
        const HMethodInfo& method_info;
        
    public:
        [[nodiscard]] uint32_t get_hash() const;
};

#endif // HMETHOD_BIND_H