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

const std::string type_to_variant(GType type)
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

void generate_register_types(std::vector<GClass> classes, std::filesystem::path srcFolder, std::filesystem::path genFolder)
{
    std::ofstream GeneratedFile(genFolder / "register_types.generated.h");

    for (auto &class_ : classes)
    {
        GeneratedFile << "#include <" << class_.path.string().substr(srcFolder.string().size() + 1) << ">\n";
    }

    GeneratedFile << "\n#define GENERATED_TYPES() ";
    for (auto &class_ : classes)
    {
        switch (class_.options.class_type)
        {
        case GClassType::Base:
            GeneratedFile << "GDREGISTER_CLASS(" << class_.name << ");\\\n";
            break;
        case GClassType::Virtual:
            GeneratedFile << "GDREGISTER_VIRTUAL_CLASS(" << class_.name << ");\\\n";
            break;
        case GClassType::Abstract:
            GeneratedFile << "GDREGISTER_ABSTRACT_CLASS(" << class_.name << ");\\\n";
            break;
        case GClassType::Runtime:
            GeneratedFile << "GDREGISTER_RUNTIME_CLASS(" << class_.name << ");\\\n";
            break;
        case GClassType::Internal:
            GeneratedFile << "GDREGISTER_INTERNAL_CLASS(" << class_.name << ");\\\n";
            break;
        }
    }
    GeneratedFile << "(void)0";

    GeneratedFile.close();
}

void appendPaths(
    std::vector<std::filesystem::path> &srcFiles, std::filesystem::path path, std::filesystem::path &gen_folder,
    std::filesystem::path srcFolder)
{
    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_directory())
        {
            std::filesystem::create_directory(gen_folder / entry.path().string().substr(srcFolder.string().size() + 1));
            appendPaths(srcFiles, entry.path(), gen_folder, srcFolder);
        }
        else if ((entry.path().extension() == ".h" || entry.path().extension() == ".hpp") && entry.path().filename() != "register_types.h")
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

        bool generate_get = property.options.custom_getter.empty();
        bool generate_set = property.options.custom_setter.empty();

        const auto &enum_ = enums.find(property.rawType);
        if (enum_ != enums.end())
        {
            if (generate_get)
            {
                GeneratedFile << "int generated_get_" << final_property_name << "() const {\\\n\treturn static_cast<int>(" << final_property_accessor << ");\\\n}\\\n";
            }
            if (generate_set)
            {
                GeneratedFile << "void generated_set_" << final_property_name << "(int value" << "){\\\n\t" << final_property_accessor
                              << " = static_cast<" << property.rawType << ">(value);\\\n notify_property_list_changed();\\\n}\\\n";
            }
            return;
        }

        if (property.variantType == GType::NodePathToRaw)
        {
            core_functions += "private:\\\nNodePath " + final_property_name + "_path;\\\npublic:\\\n";

            if (generate_get)
            {
                core_functions += property.rawType + " *get_" + final_property_name + "(){\\\n\tif(" + final_property_accessor + " == nullptr) { " +
                                  final_property_accessor + " = Object::cast_to<" + property.rawType +
                                  ">(get_node_or_null(" + final_property_name + "_path)); }\\\n\treturn " +
                                  final_property_accessor + ";\\\n}\\\n";

                GeneratedFile << "NodePath generated_get_" << final_property_name << "(){\\\n\treturn " << final_property_accessor << "_path;\\\n}\\\n";
            }

            if (generate_set)
            {
                GeneratedFile << "void generated_set_" << final_property_name << "(NodePath value){\\\n\t" << final_property_name
                              << "_path = value;\\\n\t" << final_property_accessor << " = nullptr;\\\n";
            }

            if (_class.parentName != "Resource" && !_class.options.is_resource && property.options.isRequired) // TODO check this later
            {
                GeneratedFile << "\tupdate_configuration_warnings();\\\n";
                requirements += "if (";
                if (generate_get)
                {
                    requirements += final_property_name + "_path.is_empty()";
                }
                else
                {
                    requirements += property.options.custom_getter + ".is_empty()";
                }
                requirements += ") { array.append(\"Missing " + final_property_accessor + "\"); }\\\n";
            }

            GeneratedFile << "notify_property_list_changed();\\\n}\\\n";
            return;
        }

        auto pointer_accessor = property.isPointer
                                    ? "*"
                                    : "";

        if (generate_get)
        {
            GeneratedFile << property.rawType << pointer_accessor << " generated_get_" << final_property_name << "() const {\\\n\treturn " << final_property_accessor << ";\\\n}\\\n";
        }
        if (generate_set)
        {
            GeneratedFile << "void generated_set_" << final_property_name << "(" << property.rawType << pointer_accessor << " value" << "){\\\n\t" << final_property_accessor
                          << " = value;\\\n";
        }

        if (_class.parentName != "Resource" && !_class.options.is_resource && _class.parentName != "Object" && property.options.isRequired && property.variantType == GType::Resource)
        {
            GeneratedFile << "\tupdate_configuration_warnings();\\\n";
            requirements += "if (!" + final_property_accessor + ".is_valid()) { array.append(\"Missing " + final_property_name + "\"); }\\\n";
        }

        GeneratedFile << "notify_property_list_changed();\\\n}\\\n";
    }

    std::string generate_property_hints(GProperty &property)
    {
        std::string hints = "PROPERTY_HINT_NONE, \"";

        const auto &enum_ = enums.find(property.rawType);
        if (enum_ != enums.end())
        {
            property.variantType = GType::Enum;

            // TODO: move this to switch case below
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
            break;
        case GType::Float:
            if (property.options.hint_range_enabled)
            {
                hints = "PROPERTY_HINT_RANGE, \"" + std::to_string(property.options.hint_range.min) +
                        "," + std::to_string(property.options.hint_range.max);

                if (property.options.hint_range.step != 0.0)
                {
                    hints += "," + std::to_string(property.options.hint_range.step);
                }
            }
            break;
        case GType::Int:
            if (property.options.hint_range_enabled)
            {
                hints = "PROPERTY_HINT_RANGE, \"" + std::to_string(static_cast<int>(property.options.hint_range.min)) +
                        "," + std::to_string(static_cast<int>(property.options.hint_range.max));

                if (property.options.hint_range.step != 0.0)
                {
                    hints += "," + std::to_string(static_cast<int>(property.options.hint_range.step));
                }
            }
            break;
        default:
            break;
        }

        return hints;
    }

    std::string generate_property_usage(GProperty &property)
    {
        std::string usage = "";
        bool any = false;

        if (property.options.noStorage)
        {
            if (any)
            {
                usage += "|";
            }
            usage += "PROPERTY_USAGE_NO_EDITOR";
            any = true;
        }
        if (property.options.hideInInspector)
        {
            if (any)
            {
                usage += "|";
            }
            usage = "PROPERTY_USAGE_STORAGE";
            any = true;
        }

        if (!any)
        {
            usage = "PROPERTY_USAGE_DEFAULT";
        }

        return usage;
    }

    void generate_property_bindings(GProperty &property, std::ofstream &GeneratedFile, const GClass &_class, std::string accessor = "")
    {
        const auto &struct_ = structs.find(property.rawType);

        /// Structs are added as subproperties
        if (struct_ != structs.end())
        {
            for (auto struct_property : struct_->second.properties)
            {
                generate_property_bindings(struct_property, GeneratedFile, _class, accessor + property.name + ".");
            }
            return;
        }

        const auto final_property_name = sanitize_accessor_to_identifier(accessor) + property.name;
        const auto final_property_accessor = sanitize_accessor_to_extra(accessor) + property.name;

        if (property.options.custom_getter.empty())
        {
            GeneratedFile << "ClassDB::bind_method(D_METHOD(\"get_" << final_property_name << "\"), &" << _class.name << "::generated_get_" << final_property_name << ");\\\n";
        }
        if (property.options.custom_setter.empty())
        {
            GeneratedFile << "ClassDB::bind_method(D_METHOD(\"set_" << final_property_name << "\", \"value\"), &" << _class.name << "::generated_set_" << final_property_name << ");\\\n";
        }

        // If show_if is set property will be added to property list manually
        if (property.options.show_if.empty())
        {
            std::string property_info = generate_property_info(property);
            GeneratedFile << "ADD_PROPERTY(" << property_info << ", \"";

            if (property.options.custom_setter.empty())
            {
                GeneratedFile << "set_" << final_property_name << "\", \"";
            }
            else
            {
                GeneratedFile << property.options.custom_setter << "\", \"";
            }

            if (property.options.custom_getter.empty())
            {
                GeneratedFile << "get_" << final_property_name << "\");\\\n";
            }
            else
            {
                GeneratedFile << property.options.custom_getter << "\");\\\n";
            }
        }
    }

    std::string
    generate_property_info(GProperty &property)
    {
        std::string hints = generate_property_hints(property);
        std::string usage = generate_property_usage(property);

        std::string variant = type_to_variant(property.variantType);

        return "PropertyInfo(Variant::" + variant + ", \"" + property.name + "\", " + hints + "\", " + usage + ")";
    }

    void generate_custom_property_list(std::ofstream &GeneratedFile, GClass &_class)
    {
        std::string get_override = "";
        std::string set_override = "";

        GeneratedFile << "void " << "_get_property_list(List<PropertyInfo> *p_list) {\\\n\t";

        for (GProperty &property : _class.properties)
        {
            if (property.options.show_if.empty())
            {
                continue;
            }

            GeneratedFile << "if (" << property.options.show_if << ") {\\\n";
            generate_property_list_entry(property, GeneratedFile, _class);
            GeneratedFile << "\t}\\\n";

            get_override += "if (p_name == String(\"" + property.name + "\")) {\\\n";
            get_override += "\tr_ret = ";
            if (property.options.custom_getter.empty())
            {
                get_override += "generated_get_" + property.name;
            }
            else
            {
                get_override += property.options.custom_getter;
            }
            get_override += "();\\\n";
            get_override += "\treturn true;\\\n";
            get_override += "}\\\n";

            set_override += "if (p_name == String(\"" + property.name + "\")) {\\\n\t";
            if (property.options.custom_setter.empty())
            {
                set_override += "generated_set_" + property.name;
            }
            else
            {
                set_override += property.options.custom_setter;
            }
            set_override += "(p_value);\\\n";
            set_override += "\treturn true;\\\n";
            set_override += "}\\\n";

            set_override += "\tgenerated_set_" + property.name + "(p_value);\\\n";
        }

        GeneratedFile << "};\\\n";

        GeneratedFile << "bool _get(const StringName &p_name, Variant &r_ret) const {\\\n";
        GeneratedFile << get_override;
        GeneratedFile << "\treturn false;\\\n";
        GeneratedFile << "}\\\n";

        GeneratedFile << "bool _set(const StringName &p_name, const Variant &p_value) {\\\n";
        GeneratedFile << set_override;
        GeneratedFile << "\treturn false;\\\n";
        GeneratedFile << "}\\\n";
    }

    void generate_property_list_entry(GProperty &property, std::ofstream &GeneratedFile, const GClass &_class)
    {
        std::string property_info = generate_property_info(property);
        GeneratedFile << "p_list->push_back(" << property_info << ");\\\n";
    }

public:
    void generate(std::filesystem::path srcFolder)
    {
        auto genFolder = (srcFolder.parent_path() / "generated");

        std::filesystem::create_directory(genFolder);

        std::vector<std::filesystem::path> srcFiles;

        /// Collect all source files and generates generated folder structure
        appendPaths(srcFiles, srcFolder, genFolder, srcFolder);

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
            std::string generated_filename = generatedFile.src_path.string().substr(srcFolder.string().size() + 1, lastindex - srcFolder.string().size() - 1);
            std::string file_id = sanitize_path_to_id(generated_filename);
            std::ofstream GeneratedFile(genFolder / (generated_filename + ".generated.h"));

            size_t class_index;
            while (!generatedFile.classes_indices.empty())
            {
                class_index = generatedFile.classes_indices.front();
                generatedFile.classes_indices.pop();
                auto &_class = classes[class_index];
                std::string core_functions;

                GeneratedFile
                    << "#include <gd-gen/lib.hpp>\n";
                GeneratedFile << "\n#undef FILE_IDENTIFIER"
                              << "\n#define FILE_IDENTIFIER " << file_id << '\n';
                GeneratedFile << "\n#define " << file_id << "_" << _class.generator_line << "_GENERATED_BODY() GDCLASS(" << _class.name << ", " << _class.parentName
                              << ")\\\n"
                              << file_id << "_" << _class.generator_line << "_CORE_GENERATED_BODY()\\\npublic :\\\n ";
                for (auto property : _class.properties)
                {
                    generate_property_getset(property, GeneratedFile, core_functions, requirements, _class);
                }

                generate_custom_property_list(GeneratedFile, _class);

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

        generate_register_types(classes, srcFolder, genFolder);
    }
};

int main(int argc, char const *argv[])
{
    std::filesystem::path folder_path;
    if (argc < 2)
    {
        std::cerr << "No path provided, defaulting to ../src\n";
        folder_path = "../src";
    }
    else
    {
        folder_path = argv[1];
    }

    folder_path = folder_path.lexically_normal();

    // Removes trailing slashes
    folder_path = folder_path.filename().empty() ? folder_path.parent_path() : folder_path;

    if (!std::filesystem::exists(folder_path))
    {
        std::cerr << "Error: Path does not exist: " << folder_path << '\n';
        return 1;
    }

    if (!std::filesystem::is_directory(folder_path))
    {
        std::cerr << "Error: Path is not a directory: " << folder_path << '\n';
        return 1;
    }

    std::cout << "Generating code for " << folder_path << '\n';

    Generator generator;
    generator.generate(folder_path);
    return 0;
}