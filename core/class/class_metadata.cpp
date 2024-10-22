#include "class_metadata.h"
#include "core/variant/dictionary.h"

ClassMetadata::operator Dictionary() const {
    Dictionary dict;
    dict["name"] = this->name;
    dict["package"] = this->package;
    dict["api"] = get_variant_from(this->api);
    return dict;
}

ClassMetadata ClassMetadata::from_dict(const Dictionary &p_dict) {
    return ClassMetadata(p_dict["name"], ClassMetadata::get_api_type_from(p_dict["api"]), p_dict["package"]);
}
