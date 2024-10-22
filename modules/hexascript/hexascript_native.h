#ifndef HEXASCRIPT_NATIVE_H
#define HEXASCRIPT_NATIVE_H

#include "core/typedefs.h"
#include "core/object/ref_counted.h"
#include "core/object/object.h"
#include "core/string/string_name.h"
#include "core/variant/variant.h"

class HexascriptNative : public RefCounted {
    GDCLASS(HexascriptNative, RefCounted);

    StringName script_name;
    StringName name_space;

    protected:
        _FORCE_INLINE_ void set_namespace(const StringName &ns) { this->name_space = ns; }
        _FORCE_INLINE_ void set_namespace(const char *ns) { this->name_space = StringName(ns); }
        bool _get(const StringName &name, Variant &ret) const;
        static void _bind_methods();

    public:
        _FORCE_INLINE_ const StringName &get_name() const { return this->script_name; }
        _FORCE_INLINE_ const StringName &get_namespace() const { return this->name_space; }
        Variant _new();
        Object *instantiate();
        Variant callp(const StringName &method_name, const Variant **args, int argcount, Callable::CallError &ret_err) override;
        HexascriptNative(const StringName &name, const StringName &ns = StringName(""));
};

#endif //HEXASCRIPT_NATIVE_H