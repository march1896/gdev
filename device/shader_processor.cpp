#include "shader_processor.h"

namespace Device {
    void ShaderProcessor::attach(Shader* shader)
    {
        m_shader = shader;

        // reset all io ports.
        for (U32 portType = 0; portType < IOEnd; ++portType)
        {
            m_types[portType].clear();
            m_values[portType].clear();
            m_symbols[portType].clear();
            m_semantics[portType].clear();
        }

        // reset shader references.
        m_shaderInputAddrs.clear();
        m_shaderOutputAddrs.clear();

        // dynamically adjust input ports according to shader's input variables.
        std::vector<Shader::Symbol> const& inputDescs = shader->getSymbols(Shader::Input);
        for (U32 varIndex = 0u; varIndex < inputDescs.size(); ++varIndex)
        {
            Shader::Symbol const& symbol = inputDescs[varIndex];
            addIOPort(Input, symbol.name, symbol.type, symbol.semantic);
            m_shaderInputAddrs.push_back(symbol.addr);
        }

        // dynamically adjust output ports according to shader's output variables.
        std::vector<Shader::Symbol> const& outputDescs = shader->getSymbols(Shader::Output);
        for (U32 varIndex = 0u; varIndex < outputDescs.size(); ++varIndex)
        {
            Shader::Symbol const& symbol = outputDescs[varIndex];
            addIOPort(Output, symbol.name, symbol.type, symbol.semantic);
            m_shaderOutputAddrs.push_back(symbol.addr);
        }
    }

    bool ShaderProcessor::isOneInOneOut() const
    {
        return true;
    }

    void ShaderProcessor::runOne()
    {
        // copy to shader input
        for (U32 portIdx = 0; portIdx < m_values[Input].size(); ++portIdx)
        {
            Type& type = m_types[Input][portIdx];
            Value& value = m_values[Input][portIdx];
            U8* pShaderInput = m_shaderInputAddrs[portIdx];
            U8* pCompInPort = value.read();
            if (pShaderInput != nullptr && pCompInPort != nullptr)
            {
                std::memcpy(pShaderInput, pCompInPort, SizeOf(type));
            }
        }

        m_shader->execute();

        // copy to shader output
        for (U32 portIdx = 0; portIdx < m_values[Output].size(); ++portIdx)
        {
            Type& type = m_types[Output][portIdx];
            Value& value = m_values[Output][portIdx];
            U8* pShaderOutput = m_shaderOutputAddrs[portIdx];
            U8* pCompOutPort = value.read();
            if (pShaderOutput != nullptr && pCompOutPort)
            {
                std::memcpy(pCompOutPort, pShaderOutput, SizeOf(type));
            }
        }
    }

    void ShaderProcessor::comsumeOneInput()
    {
        assert(0);
    }

    bool ShaderProcessor::hasPendingOutput() const
    {
        return false;
    }

    void ShaderProcessor::produceOneOutput()
    {
        assert(0);
    }
}
