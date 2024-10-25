#ifndef CLASS_INFO_H
#define CLASS_INFO_H

#include "core/class/class_metadata.h"
#include "core/typedefs.h"

#include <cstdint>

class HClassInfo {
    private:
        const ClassMetadata& metadata; // This class's metadata
        const ClassMetadata& inherits_metadata; // The inherited class's metadata (if any)

        enum ClassFlags : uint8_t {
            CLASS_FLAG_RUNTIME = 1,
            CLASS_FLAG_VIRTUAL = 2,
            CLASS_FLAG_RELOADABLE = 4,
            CLASS_FLAG_EXPOSED = 8,
            // Flags 16, 32, and 64 are reserved for future use
            CLASS_FLAG_DISABLED = 128
        };
        uint8_t flags = 0; // Class flags

        const bool has_flag(ClassFlags p_flag) const {
            return this->flags & p_flag;
        }

    public:
        const bool is_runtime() const { return has_flag(CLASS_FLAG_RUNTIME); }
        const bool is_virtual() const { return has_flag(CLASS_FLAG_VIRTUAL); }
        const bool is_reloadable() const { return has_flag(CLASS_FLAG_RELOADABLE); }
        const bool is_exposed() const { return has_flag(CLASS_FLAG_EXPOSED); }
        const bool is_disabled() const { return has_flag(CLASS_FLAG_DISABLED); }

        
        HClassInfo() = default;
        ~HClassInfo() = default;
};

#endif // CLASS_INFO_H