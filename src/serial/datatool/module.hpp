#ifndef ASNMODULE_HPP
#define ASNMODULE_HPP

#include <corelib/ncbistd.hpp>
#include <corelib/ncbistre.hpp>
#include <list>
#include <map>
#include "autoptr.hpp"

USING_NCBI_SCOPE;

class ASNType;
class CModuleSet;

class ASNModule {
public:
    ASNModule(CModuleSet& moduleSet, const string& name);
    virtual ~ASNModule();

    typedef list< string > TExports;
    class Import {
    public:
        string module;
        list<string> types;
    };
    typedef list< AutoPtr<Import> > TImports;
    typedef list< pair< string, AutoPtr<ASNType> > > TDefinitions;

    void AddDefinition(const string& name, const AutoPtr<ASNType> type);

    virtual CNcbiOstream& Print(CNcbiOstream& out) const;

    bool Check();
    bool CheckNames();

    CModuleSet& moduleSet;

    string name;
    TExports exports;
    TImports imports;
    TDefinitions definitions;

    class TypeInfo {
    public:
        TypeInfo(const string& n)
            : name(n), type(0)
            {
            }
        string name;
        string module;  // non empty for imports
        ASNType* type;  // non empty for non imports
    };

    const TypeInfo* FindType(const string& name) const;
    ASNType* Resolve(const string& name) const;

    typedef map<string, TypeInfo> TTypes;
    TTypes types;
};

#endif
