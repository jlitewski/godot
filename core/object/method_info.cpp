#include "method_info.h"
#include "core/class/class_metadata.h"
#include "core/object/object.h"
#include "core/variant/array.h"
#include "core/variant/dictionary.h"
#include "core/variant/typed_array.h"

HMethodInfo::operator Dictionary() const {
    Dictionary dict;
    dict["name"] = this->name; // Method name
    dict["class_metadata"] = Dictionary(this->metadata); // ClassMetadata

    dict["args"] = convert_property_list(&this->args);
    Array dict_array;

#ifdef DEBUG_METHODS_ENABLED
    for (const auto &arg_name : this->arg_names) {
        dict_array.push_back(arg_name);
    }
    dict["arg_names"] = dict_array.duplicate(true);
    dict_array.clear(); // Clear the array for reuse

#endif
    for (const auto &def_arg : this->default_args) {
        dict_array.push_back(def_arg);
    }
    dict["args_default"] = dict_array.duplicate(true);

    dict["flags"] = this->flags.get_raw_data();
    dict["id"] = this->id;

    dict["return"] = Dictionary(this->return_value);
    return dict;
}

HMethodInfo HMethodInfo::from_dict(const Dictionary &p_dict) {
    const ClassMetadata metadata = ClassMetadata::from_dict(p_dict["class_metadata"]);
    auto method_info = HMethodInfo(p_dict["name"], metadata); // MethodInfo needs a minimum of a name and class metadata

    Array dict_array;
    if (p_dict.has("args")) { // Populate the arguments, if any
        dict_array = p_dict["args"];

        for (const Variant &arg : dict_array) {
            method_info.args.push_back(PropertyInfo::from_dict(arg));
        }

        dict_array.clear(); // Clear the array for reuse
    }

#ifdef DEBUG_METHODS_ENABLED
    if (p_dict.has("arg_names")) { // Populate the argument names, if any
        dict_array = p_dict["arg_names"];

        for (const Variant &arg_name : dict_array) {
            method_info.arg_names.push_back(arg_name);
        }

        dict_array.clear(); // Clear the array for reuse
    }
#endif

    if (p_dict.has("args_default")) { // Populate default arguments, if any
        dict_array = p_dict["args_default"];

        for (const Variant &def_arg : dict_array) {
            method_info.default_args.push_back(def_arg);
        }

        dict_array.clear(); // Clear the array for reuse
    }

    if (p_dict.has("flags")) {
        //XXX: This might not return the uint8_t we passed into it. Need to test it
        method_info.flags.set_raw_data(p_dict["flags"]);
    }

    //FIXME: Not sure if this is needed, or if it was a bug that the original implementation doesn't have it
    if (p_dict.has("id")) { 
        method_info.id = p_dict["id"];
    }

    if (p_dict.has("return")) {
        method_info.return_value = PropertyInfo::from_dict(p_dict["return"]);
    }
    
    return method_info;
}
