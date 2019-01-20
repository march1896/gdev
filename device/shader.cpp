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

            // TODO: direct Mat44f * Vec3f?
            Vec4f normal = {inNormal.x, inNormal.y, inNormal.z, 0};
            normal = mWorldView * normal;
            outNormal = {normal.x, normal.y, normal.z};
        }
    }

    Shader loadVS_Simple()
    {
        Shader shader;

        shader.addSymbol(Shader::Input    , std::string("position")       , Type::FLOAT3   , Semantic::Position0   , (U8*)&VSSimple::inPosition);
        shader.addSymbol(Shader::Input    , std::string("normal")         , Type::FLOAT3   , Semantic::Normal0     , (U8*)&VSSimple::inNormal);
        shader.addSymbol(Shader::Input    , std::string("color")          , Type::FLOAT3   , Semantic::Color0      , (U8*)&VSSimple::inColor);

        shader.addSymbol(Shader::Output   , std::string("posClip")        , Type::FLOAT4   , Semantic::SV_Position , (U8*)&VSSimple::outPosClip);
        shader.addSymbol(Shader::Output   , std::string("posView")        , Type::FLOAT3   , Semantic::Position0   , (U8*)&VSSimple::outPosView);
        shader.addSymbol(Shader::Output   , std::string("normal")         , Type::FLOAT3   , Semantic::Normal0     , (U8*)&VSSimple::outNormal);
        shader.addSymbol(Shader::Output   , std::string("color")          , Type::FLOAT3   , Semantic::Color0      , (U8*)&VSSimple::outColor);

        shader.addSymbol(Shader::Constant , std::string("mWorldView")     , Type::FLOAT4X4 , Semantic{}            , (U8*)&VSSimple::mWorldView);
        shader.addSymbol(Shader::Constant , std::string("mWorldViewProj") , Type::FLOAT4X4 , Semantic{}            , (U8*)&VSSimple::mWorldViewProj);

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

        SHADER_CONST Vec3f cLightPos;
        SHADER_CONST Vec3f cLightAmbient;
        SHADER_CONST Vec3f cLightDiffuse;
        SHADER_CONST Vec3f cLightSpecular;
        SHADER_CONST float cLightPower;
        SHADER_CONST float cLightShininess;
        // SHADER_CONST float screenGamma; // Assume the monitor is calibrated to the sRGB color space

        static void Blinn_Phong();
        // [ref](https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model)
        static void ps_main()
        {
            // TODO: do we need to pass by this info in PS?
            outPosition = {inPosClip.x, inPosClip.y, inPosClip.z};
            // std::cout << "PS: " << inPosClip << std::endl;

            // outColor = inColor;
            Blinn_Phong();
        }

        static void Blinn_Phong()
        {
            Vec3f normal = normalize(inNormal);
            Vec3f lightDir = cLightPos - inPosView;

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
                specular = pow(specAngle, cLightShininess);

            }

            Vec3f ambientLinear = cLightAmbient;
            Vec3f diffuseLinear = lambertian * cLightDiffuse * cLightPower / distance;
            Vec3f specularLinear = specular * cLightSpecular * cLightPower / distance;

            outColor = ambientLinear + diffuseLinear + specularLinear;

            // apply gamma correction (assume cLightAmbient, cLightDiffuse and cLightSpecular
            // have been linearized, i.e. have no gamma correction in them)
            // Vec3f colorGammaCorrected = pow(colorLinear, Vec3f(1.0/screenGamma));

            // use the gamma corrected color in the fragment
            // gl_FragColor = vec4(colorGammaCorrected, 1.0);
        }
    }

    Shader loadPS_Simple()
    {
        Shader shader;

        shader.addSymbol(Shader::Input    , std::string("posClip")         , Type::FLOAT4 , Semantic::SV_Position , (U8*)&PSSimple::inPosClip);
        shader.addSymbol(Shader::Input    , std::string("posView")         , Type::FLOAT3 , Semantic::Position0   , (U8*)&PSSimple::inPosView);
        shader.addSymbol(Shader::Input    , std::string("normal")          , Type::FLOAT3 , Semantic::Normal0     , (U8*)&PSSimple::inNormal);
        shader.addSymbol(Shader::Input    , std::string("color")           , Type::FLOAT3 , Semantic::Color0      , (U8*)&PSSimple::inColor);

        shader.addSymbol(Shader::Output   , std::string("position")        , Type::FLOAT3 , Semantic::SV_Position , (U8*)&PSSimple::outPosition);
        shader.addSymbol(Shader::Output   , std::string("color")           , Type::FLOAT3 , Semantic::SV_Target   , (U8*)&PSSimple::outColor);

        shader.addSymbol(Shader::Constant , std::string("cLightPos")       , Type::FLOAT3  , Semantic{}           , (U8*)&PSSimple::cLightPos);
        shader.addSymbol(Shader::Constant , std::string("cLightAmbient")   , Type::FLOAT3  , Semantic{}           , (U8*)&PSSimple::cLightAmbient);
        shader.addSymbol(Shader::Constant , std::string("cLightDiffuse")   , Type::FLOAT3  , Semantic{}           , (U8*)&PSSimple::cLightDiffuse);
        shader.addSymbol(Shader::Constant , std::string("cLightSpecular")  , Type::FLOAT3  , Semantic{}           , (U8*)&PSSimple::cLightSpecular);
        shader.addSymbol(Shader::Constant , std::string("cLightPower")     , Type::FLOAT   , Semantic{}           , (U8*)&PSSimple::cLightPower);
        shader.addSymbol(Shader::Constant , std::string("cLightShininess") , Type::FLOAT   , Semantic{}           , (U8*)&PSSimple::cLightShininess);

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
