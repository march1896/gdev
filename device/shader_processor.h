#ifndef _SHADER_PROCESSOR_H_
#define _SHADER_PROCESSOR_H_

#include "vmath.h"
#include "component.h"
#include "shader.h"

namespace Device {
    class ShaderProcessor: public Comp
    {
    protected:
        Shader* m_shader;

        std::vector<U8*> m_shaderInputAddrs;

        std::vector<U8*> m_shaderOutputAddrs;

    public:
        void attach(Shader* shader);

        bool isOneInOneOut() const;

        void runOne();

        void comsumeOneInput();

        bool hasPendingOutput() const;

        void produceOneOutput();
    };
}
#endif // _SHADER_PROCESSOR_H_
