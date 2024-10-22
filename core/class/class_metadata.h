#ifndef CLASS_METADATA_H
#define CLASS_METADATA_H

#include "core/typedefs.h"
#include "core/string/ustring.h"
#include "core/object/class_db.h"

class ClassMetadata {
    private:
        String name;
        String package; //AKA namespace
        ClassDB::APIType api = ClassDB::API_NONE;
        void *class_ptr = nullptr;

        static ClassDB::APIType get_api_type_from(const Variant &p_variant) {
            if(p_variant.get_type() == Variant::INT) {
                const auto api_int = (uint8_t)p_variant;

                if(api_int > 4) {
                    return ClassDB::API_NONE; // Failsafe, set to none if over 4
                } else {
                    return (ClassDB::APIType)api_int;
                }
            }

            //TODO: Throw an exception if this happens
            return ClassDB::API_NONE; // Failsafe if the variant is not an int
        }

        const Variant get_variant_from(const ClassDB::APIType p_api) const {
            return Variant((uint8_t)p_api);
        }

    private:
        friend class ClassDB; //TODO: Add the relevant friend classes for this function
        _FORCE_INLINE_ void set_pointer(void* p_class_ptr) { this->class_ptr = p_class_ptr; }
        
    public:
        _FORCE_INLINE_ void* get_pointer() const { return this->class_ptr; }
        _FORCE_INLINE_ const String& get_name() const { return this->name; }
        _FORCE_INLINE_ const String& get_package() const { return this->package; }
        _FORCE_INLINE_ ClassDB::APIType get_api() const { return this->api; }

        operator Dictionary() const;
        static ClassMetadata from_dict(const Dictionary &p_dict);

        ClassMetadata(const String &p_name, const String &p_package = "")
            : name(p_name), package(p_package) {}

        ClassMetadata(const String &p_name, const ClassDB::APIType p_api, const String &p_package = "") 
            : name(p_name), package(p_package), api(p_api) {}
        
        ~ClassMetadata() = default;
};

#endif // CLASS_METADATA_H