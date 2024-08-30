#ifndef HV_INI_PARSER_H_
#define HV_INI_PARSER_H_

#include <string>
#include <list>

#include "hexport.h"

#define DEFAULT_INI_COMMENT "#"
#define DEFAULT_INI_DELIM   "="

// fwd
class IniNode;
/*
HV_EXPORT可能是一个宏，用于处理类的导出属性，这取决于编译器和平台。
类的导出属性通常用于处理动态链接库（DLL）中的类。当你创建一个DLL时，你可能希望某些类或函数是可见的，即可以从DLL外部访问。
扩展性：DLL允许应用程序在不修改原始代码的情况下扩展功能。例如，Adobe Photoshop使用DLL来允许第三方插件扩展其功能。

模块化：通过将代码分割到不同的DLL中，可以提高应用程序的模块化，使得代码更易于管理和维护。

共享代码：DLL允许多个应用程序共享同一段代码，节省内存和存储空间。

更新和维护：更新DLL文件可以改进或修复应用程序的功能，而不需要重新分发整个应用程序。

安全性：使用DLL可以提高安全性，因为敏感操作可以封装在DLL中，并且可以控制对这些操作的访问。

性能优化：某些性能密集型的任务可以放在DLL中，以便在多个应用程序之间共享，避免重复计算。

国际化：DLL可以用于存储本地化资源，允许应用程序根据用户的语言偏好加载适当的资源。

插件架构：许多应用程序使用DLL作为其插件架构的一部分，允许用户安装和卸载插件来自定义应用程序的功能
*/ 
class HV_EXPORT IniParser {
public:
    IniParser();
    ~IniParser();

    int LoadFromFile(const char* filepath);
    int LoadFromMem(const char* data);
    int Unload();
    int Reload();

    std::string DumpString();
    int Save();
    int SaveAs(const char* filepath);

    std::list<std::string> GetSections();
    std::list<std::string> GetKeys(const std::string& section = "");
    std::string GetValue(const std::string& key, const std::string& section = "");
    void        SetValue(const std::string& key, const std::string& value, const std::string& section = "");

    // T = [bool, int, float]
    template<typename T>
    T Get(const std::string& key, const std::string& section = "", T defvalue = 0);

    // T = [bool, int, float]
    template<typename T>
    void Set(const std::string& key, const T& value, const std::string& section = "");

protected:
    void DumpString(IniNode* pNode, std::string& str);

public:
    std::string _comment;
    std::string _delim;
    std::string _filepath;
private:
    IniNode* root_;
};

#endif // HV_INI_PARSER_H_
