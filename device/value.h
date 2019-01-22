#ifndef _VALUE_H_
#define _VALUE_H_

#include <cstring>

#include "vmath.h"
#include "semantic.h"
#include "texture.h"

namespace Device {
    // TODO: move this to standalong file?
    enum class BuiltinType
    {
        UNKNOWN,

        FLOAT,
        FLOAT2,
        FLOAT3,
        FLOAT4,
        FLOAT4X4,
        HALF,
        DOUBLE,
        INT,
        UINT,
        SAMPLER2D,
        TEXTURE2D,
    };

    // StructType's element is in a contiguous memory layout, quite like C structure type.
    // In addition, each field of has a unique semantic.
    class StructType
    {
    protected:
        // field information
        std::vector<U32> m_fieldOffsets;
        std::vector<Semantic> m_fieldSemantics;
        std::vector<BuiltinType> m_fieldTypes;
        U32 m_size;

    protected:
        void updateOffsets();

    public:
        StructType();

        // Returns field index.
        U32 addField(Semantic const& semantic, BuiltinType const& type);

        // Find field index according to semantic.
        U32 getFieldIndex(Semantic const& semantic) const;

        // Find semantic according to field index.
        Semantic getFieldSemantic(U32 fieldIndex) const;

        // Returns field offset
        U32 getFieldOffset(U32 fieldIndex) const;

        // Returns field offset
        U32 getFieldOffset(Semantic const& semantic) const;

        BuiltinType getFieldType(U32 fieldIndex) const;

        BuiltinType getFieldType(Semantic const& semantic) const;

        U32 numFields() const;

        U32 getSize() const;

        void reset();
    };

    U32 SizeOf(BuiltinType const& type);

    U32 SizeOf(StructType const& type);

    template <typename T>
    class ValueRef
    {
    public:
        typedef T Type;

    protected:
        Type const m_type;
        U8* m_pAddr;

    public:
        ValueRef(Type const& type)
            : m_type(type)
            , m_pAddr(nullptr)
        {
        }

        ValueRef(Type const& type, U8* addr)
            : m_type(type)
            , m_pAddr(addr)
        {
        }

        inline Type type() const { return m_type; }

        inline U8* read() const { return m_pAddr; }

        template <typename U>
        inline U const& readAs() const { return *reinterpret_cast<U*>(m_pAddr); }

        template <typename U>
        inline U& readAs() { return *reinterpret_cast<U*>(m_pAddr); }

        inline void write(U8* new_val) { std::memcpy(m_pAddr, new_val, SizeOf(m_type)); }

        inline void bind(U8* addr) { m_pAddr = addr; }

    };

    typedef ValueRef<BuiltinType> BuiltinValueRef;

    // Note: only inplace interpolate is support for the reference type.
    void interpolate_inplace(BuiltinValueRef& out, BuiltinValueRef const& a, float u, BuiltinValueRef const& b, float v);

    void interpolate_inplace(BuiltinValueRef& out, BuiltinValueRef const& a, float u, BuiltinValueRef const& b, float v, BuiltinValueRef const& c, float w);

    class StructValueRef: public ValueRef<StructType>
    {
    public:
        typedef StructType Type;

    public:
        StructValueRef(Type const& type)
            : ValueRef<StructType>(type)
        {
        }

        StructValueRef(Type const& type, U8* addr)
            : ValueRef<StructType>(type, addr)
        {
        }

        inline U8* getFieldAddr(U32 fieldIndex) const
        {
            return m_pAddr + m_type.getFieldOffset(fieldIndex);
        }

        inline U8* readField(U32 fieldIndex) const
        {
            return m_pAddr + m_type.getFieldOffset(fieldIndex);
        }

        inline void writeField(U32 fieldIndex, U8* new_val)
        {
            BuiltinType const& fieldType = m_type.getFieldType(fieldIndex);
            std::memcpy(m_pAddr, new_val, SizeOf(fieldType));
        }
    };

    std::ostream &operator<<(std::ostream &stream, BuiltinValueRef const& ob);

} // namespace Device
#endif // _VALUE_H_
