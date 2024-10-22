#ifndef METHOD_INFO_H
#define METHOD_INFO_H

#include "core/extension/gdextension_interface.h"
#include "core/templates/list.h"
#include "core/templates/vector.h"
#include "core/typedefs.h"
#include "core/class/class_metadata.h"
#include "core/string/ustring.h"
#include "core/object/object.h"
#include "core/object/class_db.h"
#include "core/variant/dictionary.h"
#include "core/variant/variant.h"
#include <cstdint>

class HMethodInfo { //TODO: Rename to MethodInfo after fully refactoring everything into this class
    private:
        String name; //Name of the method
        const ClassMetadata& metadata; //Metadata of the class this method is a part of
        List<PropertyInfo> args; //List of arguments this method takes
        Vector<Variant> default_args; //List of default arguments
        Vector<uint32_t> args_metadata;
        PropertyInfo return_value; //The return value
        uint8_t flags = METHOD_FLAGS_DEFAULT; //The bitfield of method flags (See MethodInfo::MethodFlags)
        uint32_t id = 0;

#ifdef DEBUG_METHODS_ENABLED //Debug related variables goes here
        Vector<String> arg_names; //List of argument names (for debugging)
#endif

        _FORCE_INLINE_ void set_return_type(const Variant::Type p_return_type) {
            this->return_value.type = p_return_type;
            this->set_flags(METHOD_FLAG_RETURNS);
        }

        _FORCE_INLINE_ void set_return_value(const PropertyInfo& p_return_value) {
            this->return_value = p_return_value;
            this->set_flags(METHOD_FLAG_RETURNS);
        }

        _FORCE_INLINE_ void clear_all_flags() { this->flags = METHOD_FLAGS_DEFAULT; }

        _FORCE_INLINE_ void _push_params(const PropertyInfo &p_param) {
            args.push_back(p_param);
        }

        template <typename... VarArgs>
        _FORCE_INLINE_ void _push_params(const PropertyInfo &p_param, VarArgs... p_params) {
            args.push_back(p_param);
            _push_params(p_params...);
        }

        template <typename... MethodFlags>
        const void clear_flags(MethodFlags... p_flags) {
            for(auto flag : {p_flags...}) {
                if(this->has_flag(flag)) {
                    this->flags &= ~flag; //Bitwise AND with inverted flag to clear
                }
            }
        }

        template <typename... MethodFlags>
        const void set_flags(MethodFlags... p_flags) {
            for(auto flag : {p_flags...}) {
                if(!this->has_flag(flag)) {
                    this->flags |= flag; //Bitwise OR to set
                }
            }
        }

    private: // ClassDB specific debug functions
        friend class ClassDB;

        _FORCE_INLINE_ void set_id(uint32_t p_id) { this->id = p_id; }

#ifdef DEBUG_METHODS_ENABLED
        void set_argument_names(const Vector<String> &p_names) {
            this->arg_names = p_names;
        }
#endif

    public:
        //Enum of possible method flags, used as a bitfield
        enum MethodFlags : uint8_t {
	        METHOD_FLAG_NORMAL       = 1,  //00000001
	        METHOD_FLAG_EDITOR       = 2,  //00000010
	        METHOD_FLAG_CONST        = 4,  //00000100
	        METHOD_FLAG_VIRTUAL      = 8,  //00001000
	        METHOD_FLAG_VARARG      = 16,  //00010000
	        METHOD_FLAG_STATIC      = 32,  //00100000
	        METHOD_FLAG_OBJECT_CORE = 64,  //01000000
            METHOD_FLAG_RETURNS     = 128, //10000000
	        METHOD_FLAGS_DEFAULT = METHOD_FLAG_NORMAL,
        };

        _FORCE_INLINE_ const String& get_name() const { return this->name; }
        _FORCE_INLINE_ const PropertyInfo& get_return_value() const { return this->return_value; }
        _FORCE_INLINE_ const uint8_t& get_flags() const { return this->flags; }
        _FORCE_INLINE_ const uint32_t& get_id() const { return this->id; }

        _FORCE_INLINE_ bool has_return_value() const { return this->return_value.type != Variant::NIL; }

        const bool has_flag(MethodFlags p_flag) const {
            return this->flags & p_flag;
        }

        inline bool operator==(const HMethodInfo &p_method) const 
            { return this->id == p_method.id && this->name == p_method.name; }

        inline bool operator<(const HMethodInfo &p_method) const 
            { return this->id == p_method.id ? (this->name < p_method.name) : (this->id < p_method.id); }
        
        operator Dictionary() const;

        static HMethodInfo from_dict(const Dictionary &p_dict);

#ifdef DEBUG_METHODS_ENABLED //Public debug functions
        const Vector<String> get_argument_names() const;
#endif

        HMethodInfo(const String &p_name, const ClassMetadata &p_class_metadata)
                : name(p_name), metadata(p_class_metadata) {}

        template <typename... VarArgs>
        HMethodInfo(const String &p_name, const ClassMetadata &p_class_metadata, VarArgs... p_params)
                : name(p_name), metadata(p_class_metadata) {
            _push_params(p_params...);
        }

        HMethodInfo(const ClassMetadata &p_class_metadata, Variant::Type p_return_type)
                : metadata(p_class_metadata) {
            this->set_return_type(p_return_type);
        }

        HMethodInfo(const String &p_name, const ClassMetadata &p_class_metadata, Variant::Type p_return_type)
                : name(p_name), metadata(p_class_metadata) {
            this->set_return_type(p_return_type);
        }

        template <typename... VarArgs>
        HMethodInfo(const String &p_name, const ClassMetadata &p_class_metadata, Variant::Type p_return_type, VarArgs... p_params)
                : name(p_name), metadata(p_class_metadata) {
            this->set_return_type(p_return_type);
            _push_params(p_params...);
        }

        HMethodInfo(const String &p_name, const ClassMetadata &p_class_metadata, const PropertyInfo &p_return_value)
                : name(p_name), metadata(p_class_metadata) {
            this->return_value = p_return_value;
        }

        template <typename... VarArgs>
        HMethodInfo(const String &p_name, const ClassMetadata &p_class_metadata, const PropertyInfo &p_return_value, VarArgs... p_params)
                : metadata(p_class_metadata), name(p_name) {
            this->return_value = p_return_value;
            _push_params(p_params...);
        }

        explicit HMethodInfo(const GDExtensionMethodInfo &pinfo, const ClassMetadata &p_class_metadata)
                : metadata(p_class_metadata), name(*reinterpret_cast<String *>(pinfo.name)),
                return_value(PropertyInfo(pinfo.return_value)), flags(pinfo.flags), id(pinfo.id) {
            
            //TODO: Refactor to take uint8_t instead of uint32_t. 8-bits should be more than enough 
            for(uint32_t index = 0; index < pinfo.argument_count; index++) {
                args.push_back(PropertyInfo(pinfo.arguments[index]));
            }

            const auto *def_values = (const Variant *)pinfo.default_arguments;
            //TODO: Refactor to take uint8_t instead of uint32_t. 8-bits should be more than enough 
            for(uint32_t index = 0; index < pinfo.argument_count; index++) {
                default_args.push_back(def_values[index]);
            }

        }

        ~HMethodInfo() = default;
};

#endif // METHOD_INFO_H