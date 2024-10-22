#ifndef CLASS_INFO_H
#define CLASS_INFO_H

#include "core/class/class_metadata.h"

class HClassInfo {
    private:
        const ClassMetadata& metadata; // This class's metadata
        const ClassMetadata& inherits_metadata; // The inherited class's metadata (if any)



};

#endif // CLASS_INFO_H