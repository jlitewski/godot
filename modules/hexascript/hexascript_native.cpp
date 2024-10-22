#include "hexascript_native.h"

#include "core/error/error_macros.h"
#include "core/object/class_db.h"
#include "core/object/method_bind.h"
#include "core/object/ref_counted.h"
#include "core/string/string_name.h"
#include "core/variant/variant.h"

#include <stdint.h>

HexascriptNative::HexascriptNative(const StringName &name, const StringName &ns)
    : name_space(ns), script_name(name) { }

bool HexascriptNative::_get(const StringName &name, Variant &ret) const {
    bool is_valid = false;
    const int64_t var = ClassDB::get_integer_constant(this->script_name, name, &is_valid);

    if(is_valid) {
        ret = var;
        return true;
    }

    return false;
}

void HexascriptNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("new"), &HexascriptNative::_new);
}

Variant HexascriptNative::_new() {
    Object *object = instantiate();
    ERR_FAIL_NULL_V_MSG(object, Variant(), "Class type: '" + String(this->script_name) + "' is not instantiable.");

    RefCounted *ref = Object::cast_to<RefCounted>(object);

    if(ref) {
        return Ref<RefCounted>(ref);
    }

    return object;
}

Object *HexascriptNative::instantiate() {
    return ClassDB::instantiate_no_placeholders(this->script_name);
}

Variant HexascriptNative::callp(const StringName &method_name, const Variant **args, int argcount, Callable::CallError &ret_err) {
    if(method_name == SNAME("new")) { //Constructor Logic
        return Object::callp(method_name, args, argcount, ret_err);
    }

    MethodBind *bind = ClassDB::get_method(this->script_name, method_name);
    if(bind && bind->is_static()) {
        return bind->call(nullptr, args, argcount, ret_err);
    }

    //Unable to get the method we asked for, return empty Variant and set the return error
    ret_err.error = Callable::CallError::CALL_ERROR_INVALID_METHOD;
    return {};
}
