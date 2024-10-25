#ifndef HMETHOD_BIND_H
#define HMETHOD_BIND_H

#include "core/object/method_info.h"
#include "core/variant/binder_common.h"
#include <cstdint>

VARIANT_BITFIELD_CAST(HMethodInfo::MethodFlags)

/*
HMethodBind is a wrapper around HMethodInfo, unlike Godot's versions of the same classes. Having
it built this way allows the wrapped MethodInfo to change, and have those changes be reflected
without needing to make a new and/or modifying the existing HMethodBind. This is still very much experimental,
and should be used with caution and will most likely change and adapt in the future.

The plan is to simplify how the Engine deals with the various classes and methods and signals and so on,
and also make it possible to easily add new features in the future (for instance, script namespaces).
It would be fundamentally harder to try and duct tape that functionality to the Godot implementation
than it would be to just scrap it and rewrite it from the ground up.
*/
class HMethodBind {
    private:
        const HMethodInfo& method_info;
        uint32_t bind_id; //HMethodBind specific id. Different from HMethodInfo::id
        
    public:
        _FORCE_INLINE_ const Vector<Variant>& get_default_arguments() const {
            return this->method_info.get_default_arguments();
        }

        uint32_t get_hash() const;
};

#endif // HMETHOD_BIND_H