#include <algorithm>

#include "shader.h"

namespace Device {
    void Shader::addSymbol(Section section, std::string const& name, Type const& type, Semantic const& semantic, U8* addr)
    {
        m_symbolSections[section].push_back(Symbol{name, type, semantic, addr});
    }

    void Shader::setEntry(Shader::MainEntry mainProc)
    {
        m_entryFunc = mainProc;
    }

    std::vector<Shader::Symbol> Shader::getSymbols(Section section) const
    {
        return m_symbolSections[section];
    }

    Shader::Symbol Shader::getSymbol(Section section, std::string const& name) const
    {
        SymbolList const& symbolSection = m_symbolSections[section];

        std::vector<Symbol>::const_iterator itr = std::find_if(symbolSection.begin(), symbolSection.end(),
            [&](Symbol const& symbol) -> bool {
                return symbol.name == name;
            }
        );

        if (itr == symbolSection.end())
        {
            return Shader::Symbol{"", Type{}, Semantic{}, nullptr};
        }

        return *itr;
    }

    Type Shader::getConstantType(std::string const& name) const
    {
        return getSymbol(Shader::Constant, name).type;
    }

    U8* Shader::getConstantAddr(std::string const& name) const
    {
        return getSymbol(Shader::Constant, name).addr;
    }


    void Shader::execute()
    {
        m_entryFunc();
    }

    #define SHADER_IN static
    #define SHADER_OUT static
    #define SHADER_CONST static
    #define SHADER_MAIN static
    namespace VSSimple
    {
        SHADER_IN Vec3f inPosition;
        SHADER_IN Vec3f inNormal;
        SHADER_IN Vec3f inColor;

        SHADER_OUT Vec4f outPosClip;
        SHADER_OUT Vec3f outPosView;
        SHADER_OUT Vec3f outColor;
        SHADER_OUT Vec3f outNormal;

        SHADER_CONST Mat44f mWorldView;
        SHADER_CONST Mat44f mWorldViewProj;

        static void vs_main()
        {
            Vec4f pos{inPosition.x, inPosition.y, inPosition.z, 1.0};

            Vec4f posView = mWorldView * pos;
            outPosView = {posView.x, posView.y, posView.z};

            outPosClip = mWorldViewProj * pos;
            // std::cout << "VS: " << outPosClip << std::endl;

            outColor = inColor;

            Vec4f normal = {inNormal.x, inNormal.y, inNormal.z, 0};
            normal = mWorldView * normal;
            outNormal = {normal.x, normal.y, normal.z};
        }
    }

    Shader loadVS_Simple()
    {
        Shader shader;

        shader.addSymbol(Shader::Input, std::string("position"), Type::FLOAT3, Semantic::Position0, (U8*)&VSSimple::inPosition);
        shader.addSymbol(Shader::Input, std::string("normal"),   Type::FLOAT3, Semantic::Normal0, (U8*)&VSSimple::inNormal);
        shader.addSymbol(Shader::Input, std::string("color"),    Type::FLOAT3, Semantic::Color0, (U8*)&VSSimple::inColor);

        shader.addSymbol(Shader::Output, std::string("posClip"), Type::FLOAT4, Semantic::SV_Position, (U8*)&VSSimple::outPosClip);
        shader.addSymbol(Shader::Output, std::string("posView"), Type::FLOAT3, Semantic::Position0, (U8*)&VSSimple::outPosView);
        shader.addSymbol(Shader::Output, std::string("normal"),   Type::FLOAT3, Semantic::Normal0, (U8*)&VSSimple::outNormal);
        shader.addSymbol(Shader::Output, std::string("color"),    Type::FLOAT3, Semantic::Color0, (U8*)&VSSimple::outColor);

        shader.addSymbol(Shader::Constant, std::string("mWorldView"), Type::FLOAT4X4, Semantic{}, (U8*)&VSSimple::mWorldView);
        shader.addSymbol(Shader::Constant, std::string("mWorldViewProj"), Type::FLOAT4X4, Semantic{}, (U8*)&VSSimple::mWorldViewProj);

        shader.setEntry(&VSSimple::vs_main);

        return shader;
    }

    namespace PSSimple
    {
        SHADER_IN Vec4f inPosClip;
        SHADER_IN Vec3f inPosView;
        SHADER_IN Vec3f inColor;
        SHADER_IN Vec3f inNormal;

        SHADER_OUT Vec3f outPosition;
        SHADER_OUT Vec3f outColor;

        // TODO: bind this constant
        // SHADER_CONST Vec3f cLightDir;

        static void Blinn_Phone();
        // [ref](https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model)
        static void ps_main()
        {
            // TODO: do we need to pass by this info in PS?
            outPosition = {inPosClip.x, inPosClip.y, inPosClip.z};
            // std::cout << "PS: " << inPosClip << std::endl;

            // outColor = inColor;
            Blinn_Phone();
        }

        // TODO: move this to shader constants.
        const Vec3f lightPos{8.0, 8.0, 10.0};
        // const Vec3f lightColor{1.0, 1.0, 1.0};
        const float lightPower = 160.0;
        const Vec3f ambientColor{0.05, 0.05, 0.05};
        const Vec3f diffuseColor{1.0, 0.0, 0.0};
        const Vec3f specColor{1.0, 1.0, 1.0};
        const float shininess = 10.0;
        // const float screenGamma = 2.2; // Assume the monitor is calibrated to the sRGB color space
        void Blinn_Phone() {
            Vec3f normal = normalize(inNormal);
            Vec3f lightDir = lightPos - inPosView;
            float distance = lightDir.length();
            distance = distance * distance;
            lightDir.normalize();

            float lambertian = std::max(dot(lightDir, normal), 0.0f);
            float specular = 0.0;

            if(lambertian > 0.0) {
                Vec3f viewDir = normalize(0.0f - inPosView);

                // this is blinn phong
                Vec3f halfDir = normalize(lightDir + viewDir);

                float specAngle = std::max(dot(halfDir, normal), 0.0f);
                specular = pow(specAngle, shininess);
            }
            else
            {
                // This should not happen if back face is culled.
                // std::cout << inPosView << " : " << lightPos << std::endl;
            }
            Vec3f colorLinear = ambientColor;

            //Vec3f diffuseLinear = cross(diffuseColor * lambertian, lightColor * lightPower) / distance;
            //Vec3f specularLinear = cross(specColor * specular, lightColor * lightPower) / distance;
            Vec3f diffuseLinear = lambertian * diffuseColor * lightPower / distance;
            Vec3f specularLinear = specular * specColor * lightPower / distance;

            outColor = colorLinear + diffuseLinear + specularLinear;

            // apply gamma correction (assume ambientColor, diffuseColor and specColor
            // have been linearized, i.e. have no gamma correction in them)
            // Vec3f colorGammaCorrected = pow(colorLinear, Vec3f(1.0/screenGamma));

            // use the gamma corrected color in the fragment
            // gl_FragColor = vec4(colorGammaCorrected, 1.0);
        }
    }

    Shader loadPS_Simple()
    {
        Shader shader;

        shader.addSymbol(Shader::Input, std::string("posClip"),  Type::FLOAT4, Semantic::SV_Position, (U8*)&PSSimple::inPosClip);
        shader.addSymbol(Shader::Input, std::string("posView"),  Type::FLOAT4, Semantic::Position0, (U8*)&PSSimple::inPosView);
        shader.addSymbol(Shader::Input, std::string("normal"),   Type::FLOAT3, Semantic::Normal0, (U8*)&PSSimple::inNormal);
        shader.addSymbol(Shader::Input, std::string("color"),    Type::FLOAT3, Semantic::Color0, (U8*)&PSSimple::inColor);

        shader.addSymbol(Shader::Output, std::string("position"), Type::FLOAT3, Semantic::SV_Position, (U8*)&PSSimple::outPosition);
        shader.addSymbol(Shader::Output, std::string("color"),    Type::FLOAT3, Semantic::SV_Target, (U8*)&PSSimple::outColor);

        shader.setEntry(&PSSimple::ps_main);

        return shader;
    }


    Shader loadShader(std::string const& filePath)
    {
        // TODO:
        (void)filePath;
        assert(0);
    }
} // namespace Device
