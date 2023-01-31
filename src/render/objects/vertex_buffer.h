#pragma once

#include <cinttypes>
#include <initializer_list>
#include <vector>

namespace element {

    enum vertex_buffer_layout_element_type {
        boolean, int1, int2, int3, int4, float1, float2, float3, float4, mat2, mat3, mat4
    };

    struct vertex_buffer_layout_element {
        public:
            vertex_buffer_layout_element_type type;
            std::size_t offset;
            bool normalized;
            
            uint32_t size() const;
            uint32_t count() const;

            vertex_buffer_layout_element(vertex_buffer_layout_element_type type, bool normalized);
    };

    class vertex_buffer_layout {
        public:
            std::vector<vertex_buffer_layout_element> elements;
            uint32_t stride;
            
            vertex_buffer_layout() = default;
            vertex_buffer_layout(std::initializer_list<vertex_buffer_layout_element> elements);
    };

    class vertex_buffer {
        private:
            vertex_buffer_layout layout;
        public:
            virtual ~vertex_buffer();
            virtual void bind() = 0;
            virtual void unbind() = 0;
            virtual uint32_t get_renderer_id() const = 0;

            void set_layout(const vertex_buffer_layout& layout);
            void set_layout(vertex_buffer_layout&& layout);
            const vertex_buffer_layout& get_layout() const;

            static vertex_buffer* create(void* data, uint32_t size);
    };

} // namespace element
