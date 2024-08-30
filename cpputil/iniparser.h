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
HV_EXPORT������һ���꣬���ڴ�����ĵ������ԣ���ȡ���ڱ�������ƽ̨��
��ĵ�������ͨ�����ڴ���̬���ӿ⣨DLL���е��ࡣ���㴴��һ��DLLʱ�������ϣ��ĳЩ������ǿɼ��ģ������Դ�DLL�ⲿ���ʡ�
��չ�ԣ�DLL����Ӧ�ó����ڲ��޸�ԭʼ������������չ���ܡ����磬Adobe Photoshopʹ��DLL����������������չ�书�ܡ�

ģ�黯��ͨ��������ָ��ͬ��DLL�У��������Ӧ�ó����ģ�黯��ʹ�ô�������ڹ����ά����

������룺DLL������Ӧ�ó�����ͬһ�δ��룬��ʡ�ڴ�ʹ洢�ռ䡣

���º�ά��������DLL�ļ����ԸĽ����޸�Ӧ�ó���Ĺ��ܣ�������Ҫ���·ַ�����Ӧ�ó���

��ȫ�ԣ�ʹ��DLL������߰�ȫ�ԣ���Ϊ���в������Է�װ��DLL�У����ҿ��Կ��ƶ���Щ�����ķ��ʡ�

�����Ż���ĳЩ�����ܼ��͵�������Է���DLL�У��Ա��ڶ��Ӧ�ó���֮�乲�������ظ����㡣

���ʻ���DLL�������ڴ洢���ػ���Դ������Ӧ�ó�������û�������ƫ�ü����ʵ�����Դ��

����ܹ������Ӧ�ó���ʹ��DLL��Ϊ�����ܹ���һ���֣������û���װ��ж�ز�����Զ���Ӧ�ó���Ĺ���
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
