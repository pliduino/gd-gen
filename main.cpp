#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <unordered_map>

#include <parser/parser.h>
#include <core/gproperty/GProperty.h>
#include <core/gclass/GClass.h>
#include <core/general/GIndicator.h>
#include <core/general/GType.h>
#include <core/gstruct/GStruct.h>
#include <core/genum/GEnum.h>

std::string treatPropertyType(std::string str)
{
    for (auto &c : str)
    {
        if (c == '<')
        {
            c = '_';
        }
        else if (c == '>')
        {
            c = '_';
        }
    }

    return str;
}

const char *type_to_variant(GType type)
{
    switch (type)
    {
    case GType::Float:
        return "FLOAT";
    case GType::Int:
        return "INT";
    case GType::Node:
        return "OBJECT";
    case GType::Resource:
        return "OBJECT";
    case GType::NodePathToRaw:
        return "NODE_PATH";
    case GType::Object:
        return "OBJECT";
    case GType::Boolean:
        return "BOOL";
    case GType::PackedByteArray:
        return "PACKED_BYTE_ARRAY";
    case GType::String:
        return "STRING";
    case GType::Enum:
        return "INT";
    default:
        std::cerr << "Invalid type\n";
        exit(1);
    }
}

constexpr char srcFolder[] = "./src";

void dumpPropertiesGetSetDef(std::ofstream &output, const std::vector<GProperty> &properties)
{
    output << "public:\n";
    for (auto property : properties)
    {
        output << property.rawType << " generated_get_" << property.name << "();\n";
        output << property.rawType << " generated_set_" << property.name << "(" << property.rawType << " value" << ");\n";
    }
}

void generate_register_types(std::vector<GClass> classes)
{
    std::ofstream GeneratedFile("./generated/register_types.generated.h");

    for (auto &class_ : classes)
    {
        GeneratedFile << "#include <" << class_.path.string().substr(sizeof(srcFolder)) << ">\n";
    }

    GeneratedFile << "\n#define GENERATED_TYPES() ";
    for (auto &class_ : classes)
    {
        GeneratedFile << "GDREGISTER_CLASS(" << class_.name << ");\\\n";
    }
    GeneratedFile << "(void)0";

    // dumpPropertiesGetSetImpl(OutputSourceFile, className, properties);

    GeneratedFile.close();
}

void appendPaths(
    std::vector<std::filesystem::path> &srcFiles, std::filesystem::path path)
{
    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_directory())
        {
            std::filesystem::create_directory("./generated/" + entry.path().string().substr(sizeof(srcFolder)));
            appendPaths(srcFiles, entry.path());
        }
        else if (entry.path().extension() == ".h" && entry.path().filename() != "register_types.h")
        {
            srcFiles.push_back(entry.path());
        }
    }
}

std::string sanitize_path_to_id(std::string str)
{
    for (auto &c : str)
    {
        if (c == '\\')
        {
            c = '_';
        }
        else if (c == '/')
        {
            c = '_';
        }
    }
    return str;
}

std::string sanitize_accessor_to_identifier(std::string str)
{
    for (auto &c : str)
    {
        if (c == '.')
        {
            c = '_';
        }
    }
    return str;
}

std::string sanitize_accessor_to_extra(std::string str)
{
    for (auto &c : str)
    {
        if (c == '.')
        {
            c = '/';
        }
    }
    return str;
}

struct GeneratedFile
{
    std::filesystem::path src_path;
    std::queue<size_t> classes_indices;
};

class Generator
{
    std::vector<GeneratedFile> generatedFiles;
    std::vector<GClass> classes;
    std::unordered_map<std::string, GStruct> structs;
    std::unordered_map<std::string, GEnum> enums;

    inline void add_struct(GStruct gStruct)
    {
        structs[gStruct.name] = gStruct;
    }

    inline void add_enum(GEnum gEnum)
    {
        enums[gEnum.name] = gEnum;
    }

    void generate_property_getset(const GProperty &property, std::ofstream &GeneratedFile, std::string &core_functions,
                                  std::string &requirements, const GClass &_class, std::string accessor = "")
    {
        const auto &struct_ = structs.find(property.rawType);
        if (struct_ != structs.end())
        {
            for (auto struct_property : struct_->second.properties)
            {
                generate_property_getset(struct_property, GeneratedFile, core_functions, requirements, _class, accessor + property.name + ".");
            }
            return;
        }

        const auto final_property_name = sanitize_accessor_to_identifier(accessor) + property.name;
        const auto final_property_accessor = accessor + property.name;

        const auto &enum_ = enums.find(property.rawType);
        if (enum_ != enums.end())
        {
            GeneratedFile << "int generated_get_" << final_property_name << "() const {\\\n\treturn static_cast<int>(" << final_property_accessor << ");\\\n}\\\n";
            GeneratedFile << "void generated_set_" << final_property_name << "(int value" << "){\\\n\t" << final_property_accessor
                          << " = static_cast<" << property.rawType << ">(value);\\\n}\\\n";
            return;
        }

        if (property.variantType == GType::NodePathToRaw)
        {
            core_functions += "private:\\\nNodePath " + final_property_name + "_path;\\\npublic:\\\n";

            core_functions += property.rawType + " *get_" + final_property_name + "(){\\\n\tif(" + final_property_accessor + " == nullptr) { " +
                              final_property_accessor + " = Object::cast_to<" + property.rawType +
                              ">(get_node_or_null(" + final_property_name + "_path)); }\\\n\treturn " +
                              final_property_accessor + ";\\\n}\\\n";

            GeneratedFile << "NodePath generated_get_" << final_property_name << "(){\\\n\treturn " << final_property_accessor << "_path;\\\n}\\\n";
            GeneratedFile << "void generated_set_" << final_property_name << "(NodePath value){\\\n\t" << final_property_name
                          << "_path = value;\\\n\t" << final_property_accessor << " = nullptr;\\\n";

            if (_class.parentName != "Resource" && !_class.options.is_resource && property.options.isRequired)
            {
                GeneratedFile << "\tupdate_configuration_warnings();\\\n";
                requirements += "if (" + final_property_name + "_path.is_empty()) { array.append(\"Missing " + final_property_accessor + "\"); }\\\n";
            }

            GeneratedFile << "}\\\n";
            return;
        }

        auto pointer_accessor = property.isPointer
                                    ? "*"
                                    : "";

        GeneratedFile << property.rawType << pointer_accessor << " generated_get_" << final_property_name << "() const {\\\n\treturn " << final_property_accessor << ";\\\n}\\\n";
        GeneratedFile << "void generated_set_" << final_property_name << "(" << property.rawType << pointer_accessor << " value" << "){\\\n\t" << final_property_accessor
                      << " = value;\\\n";

        if (_class.parentName != "Resource" && !_class.options.is_resource && _class.parentName != "Object" && property.options.isRequired && property.variantType == GType::Resource)
        {
            GeneratedFile << "\tupdate_configuration_warnings();\\\n";
            requirements += "if (!" + final_property_accessor + ".is_valid()) { array.append(\"Missing " + final_property_name + "\"); }\\\n";
        }

        GeneratedFile << "}\\\n";
    }

    void generate_property_bindings(GProperty &property, std::ofstream &GeneratedFile, const GClass &_class, std::string accessor = "")
    {
        const auto &struct_ = structs.find(property.rawType);
        if (struct_ != structs.end())
        {
            for (auto struct_property : struct_->second.properties)
            {
                generate_property_bindings(struct_property, GeneratedFile, _class, accessor + property.name + ".");
            }
            return;
        }

        std::string hints = "PROPERTY_HINT_NONE, \"";
        std::string usage = "PROPERTY_USAGE_DEFAULT";

        const auto &enum_ = enums.find(property.rawType);
        if (enum_ != enums.end())
        {
            property.variantType = GType::Enum;
            hints = "PROPERTY_HINT_ENUM, \"";

            for (auto &enum_value : enum_->second.values)
            {
                hints += enum_value.name;
                if (enum_value.value.has_value())
                {
                    hints += '=' + std::to_string(enum_value.value.value());
                }
                hints += ',';
            }

            hints = hints.substr(0, hints.length() - 1);
        }

        switch (property.variantType)
        {
        case GType::Resource:
            hints = "PROPERTY_HINT_RESOURCE_TYPE, \"" +
                    treatPropertyType(property.rawType.substr(4, property.rawType.length() - 5));
            break;
        case GType::Node:
            hints = "PROPERTY_HINT_NODE_TYPE, \"" + treatPropertyType(property.rawType);
            break;
        case GType::NodePathToRaw:
            hints = "PROPERTY_HINT_NODE_PATH_VALID_TYPES, \"" + treatPropertyType(property.rawType);
            break;
        case GType::String:
            if (property.options.multiline_text)
            {
                hints = "PROPERTY_HINT_MULTILINE_TEXT, \"";
            }
        }

        const auto final_property_name = sanitize_accessor_to_identifier(accessor) + property.name;
        const auto final_property_accessor = sanitize_accessor_to_extra(accessor) + property.name;

        if (property.options.noStorage)
        {
            usage = "PROPERTY_USAGE_NO_EDITOR";
        }
        else if (property.options.hideInInspector)
        {
            usage = "PROPERTY_USAGE_STORAGE";
        }

        GeneratedFile << "ClassDB::bind_method(D_METHOD(\"get_" << final_property_name << "\"), &" << _class.name << "::generated_get_" << final_property_name << ");\\\n"
                      << "ClassDB::bind_method(D_METHOD(\"set_" << final_property_name << "\", \"value\"), &" << _class.name << "::generated_set_" << final_property_name << ");\\\n"
                      << "ADD_PROPERTY(PropertyInfo(Variant::" << type_to_variant(property.variantType) << ", \"" << final_property_accessor << "\", " << hints;

        // if (!property.options.description.empty())
        // {
        //     GeneratedFile << ";" << property.options.description;
        // }

        GeneratedFile << "\", " << usage << "), \"set_" << final_property_name << "\", \"get_" << final_property_name << "\");\\\n";
    }

public:
    void generate()
    {
        std::filesystem::create_directory("./generated/");
        std::vector<std::filesystem::path> srcFiles;

        appendPaths(srcFiles, srcFolder);

        for (auto &file : srcFiles)
        {
            GeneratedFile generatedFile;
            generatedFile.src_path = file;

            Parser parser(file);

            std::queue<TokenValue> tokens = parser.parse();

            bool should_generate = false;

            TokenValue token;
            while (!tokens.empty())
            {
                token = tokens.front();
                tokens.pop();

                switch (token.token)
                {
                case GToken::GPROPERTY:
                    classes.back().properties.push_back(GProperty(tokens));
                    break;
                case GToken::GSIGNAL:
                    classes.back().signals.push_back(GSignal(tokens));
                    break;
                case GToken::GCLASS:
                    classes.push_back(GClass(tokens));
                    classes.back().path = file;
                    generatedFile.classes_indices.push(classes.size() - 1);
                    should_generate = true;
                    break;
                case GToken::GSTRUCT:
                    // Missing struct support
                    break;
                case GToken::GENUM:
                    add_enum(GEnum(tokens));
                    break;
                case GToken::GENERATED_BODY:
                    classes.back().generator_line = token.line;
                    break;
                case GToken::GFUNCTION:
                    classes.back().functions.push_back(GFunction(tokens));
                    break;
                }
            }

            if (should_generate)
            {
                generatedFiles.push_back(generatedFile);
            }
        }

        for (auto &generatedFile : generatedFiles)
        {
            std::string requirements;
            size_t lastindex = generatedFile.src_path.string().find_last_of(".");
            std::string generated_filename = generatedFile.src_path.string().substr(sizeof(srcFolder), lastindex - sizeof(srcFolder));
            std::string file_id = sanitize_path_to_id(generated_filename);
            std::ofstream GeneratedFile("./generated/" + generated_filename + ".generated.h");

            size_t class_index;
            while (!generatedFile.classes_indices.empty())
            {
                class_index = generatedFile.classes_indices.front();
                generatedFile.classes_indices.pop();
                auto &_class = classes[class_index];
                std::string core_functions;

                GeneratedFile
                    << "#include <generator/lib.hpp>\n";
                GeneratedFile << "\n#undef FILE_IDENTIFIER"
                              << "\n#define FILE_IDENTIFIER " << file_id << '\n';
                GeneratedFile << "\n#define " << file_id << "_" << _class.generator_line << "_GENERATED_BODY() GDCLASS(" << _class.name << ", " << _class.parentName
                              << ")\\\n"
                              << file_id << "_" << _class.generator_line << "_CORE_GENERATED_BODY()\\\npublic :\\\n ";
                for (auto property : _class.properties)
                {
                    generate_property_getset(property, GeneratedFile, core_functions, requirements, _class);
                }

                for (auto signal : _class.signals)
                {
                    core_functions += "public:\\\nvoid emit_" + signal.name + "(";
                    bool once = true;
                    for (auto &argument : signal.arguments)
                    {
                        if (once)
                        {
                            core_functions += argument.raw_type + " " + argument.name;
                            once = false;
                        }
                        else
                        {
                            core_functions += ", " + argument.raw_type + " " + argument.name;
                        }
                    }
                    core_functions += "){\\\n\temit_signal(\"" + signal.name + '"';
                    if (signal.arguments.size() > 0)
                    {
                        core_functions += ", ";
                    }
                    once = true;
                    for (auto &argument : signal.arguments)
                    {
                        if (once)
                        {
                            core_functions += argument.name;
                            once = false;
                        }
                        else
                        {
                            core_functions += ", " + argument.name;
                        }
                    }
                    core_functions += ");\\\n}\\\n";

                    // core_functions += "/// " + signal.name + "(";
                    // once = true;
                    // for (auto &argument : signal.arguments)
                    // {
                    //     if (once)
                    //     {
                    //         core_functions += argument.raw_type + " " + argument.name;
                    //         once = false;
                    //     }
                    //     else
                    //     {
                    //         core_functions += ", " + argument.raw_type + " " + argument.name;
                    //     }
                    // }
                    // core_functions += ")\n";

                    core_functions += "void connect_" + signal.name + "(Callable callable){\\\n\tconnect(\"" + signal.name + "\", callable);\\\n}\\\n";
                }

                if (_class.parentName != "Resource" && !_class.options.is_resource && _class.parentName != "Object")
                {
                    GeneratedFile << "virtual PackedStringArray _get_configuration_warnings() const override\\\n"
                                     "{ PackedStringArray array;\\\n"
                                  << requirements << "return array;\\\n"
                                                     "}\\\n";
                }

                GeneratedFile << "protected:\\\nstatic void _bind_methods()\\\n{\\\n"
                              << _class.name << "_GENERATED_BINDINGS();\\\n";

                if (_class.options.custom_bindings)
                {
                    GeneratedFile << "custom_bind_methods();\\\n";
                }

                GeneratedFile << "}\\\n";

                GeneratedFile
                    << "\n#define " << _class.name << "_GENERATED_BINDINGS() ";
                for (auto property : _class.properties)
                {
                    generate_property_bindings(property, GeneratedFile, _class);
                }

                for (auto signal : _class.signals)
                {
                    GeneratedFile << "ADD_SIGNAL(MethodInfo(\"" << signal.name << "\"";
                    for (auto &argument : signal.arguments)
                    {
                        GeneratedFile << ", PropertyInfo(Variant::" << type_to_variant(argument.variantType) << ", \"" << argument.name << "\")";
                    }
                    GeneratedFile << "));\\\n";
                }

                for (auto function : _class.functions)
                {
                    GeneratedFile << "ClassDB::bind_method(D_METHOD(\"" << function.name << "\"";
                    for (auto &argument : function.arguments)
                    {
                        GeneratedFile << ", \"" << argument.name << "\"";
                    }
                    GeneratedFile << "), &" << _class.name << "::" << function.name << ");\\\n";
                }

                GeneratedFile << "(void)0\n";

                GeneratedFile << "\n#define " << file_id << "_" << _class.generator_line << "_CORE_GENERATED_BODY()\\\n"
                              << core_functions;
            }

            GeneratedFile.close();
        }

        // std::cout << "Generated " << structs[0].name << " classes" << std::endl;

        generate_register_types(classes);
    }
};

int main(int argc, char const *argv[])
{
    Generator generator;
    generator.generate();
    return 0;
}