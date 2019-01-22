#ifndef _SHADER_H_
#define _SHADER_H_

#include <memory>

#include <cstring>
#include <cassert>

#include <string>
#include <vector>
#include <deque>
#include <map>

#include "vmath.h"
#include "buffer.h"
#include "semantic.h"
#include "component.h"

namespace Device {

    class Shader
    {
    public:
        enum Section
        {
            Input,     // unbound input variable
            Output,    // unbound output variable
            Constant,  // unbound global constant variable
            SectionCnt,
        };

        typedef void (*MainEntry)(void);

        struct Symbol
        {
            std::string name;
            BuiltinType type;
            Semantic semantic;
            U8* addr;
        };

    protected:
        typedef std::vector<Symbol> SymbolList;
        SymbolList m_symbolSections[SectionCnt];

        MainEntry m_entryFunc;

    public:
        void addSymbol(Section section, std::string const& name, BuiltinType const& type, Semantic const& semantic, U8* addr);

        void setEntry(MainEntry mainProc);

        std::vector<Symbol> getSymbols(Section section) const;

        Symbol getSymbol(Section, std::string const& name) const;

        BuiltinType getConstantType(std::string const& name) const;

        U8* getConstantAddr(std::string const& name) const;

        void execute();
    };

    Shader loadShader(std::string const& filePath);

    Shader loadVS_Simple();

    Shader loadPS_Simple();

} // namespace Device

#endif // _SHADER_H_
