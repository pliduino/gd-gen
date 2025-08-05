# GD-Gen

**Auto-generate C++ code for Godot, like Unreal Engine 5 (UE5) does — but for Godot.**

`gd-gen` is a code generation tool for Godot's C++ GDExtension system. It helps streamline development by generating boilerplate code for properties, signals, and other common patterns, similar to how UE5 handles reflection and metadata.

---

## ✨ Features

- ✅ Automatic generation of C++ boilerplate for Godot
- ✅ Similar to UE5's reflection system, but adapted for Godot
- ✅ Reduces manual code and improves consistency
- ✅ Integrates cleanly with Godot's GDExtension workflow

---

## 🚀 Getting Started

1. **Install**
   Clone this repository to your project folder, it has scons so just compile as normal with "scons".

   ```bash
   git clone https://github.com/pliduino/gd-gen.git
   ```

2. **Run Generator**
   ```bash
   ./gd-gen/main.exe ../src/
   ```

   If no source folder is provided it defaults to `../src/`

3. **Generated Output**
   - Boilerplate headers and source files are generated automatically.
   - Naming, property registration, and signal declarations handled for you.
   - Remember to call GENERATED_BODY() inside the class and include <generated/myclass.generated.h>
   - There's also a generated register_types file, also add it and add the macro GENERATED_TYPES() to initialize_xxx_module

---

## 📦 Example

Before:

```cpp
class MyNode : public godot::Node {
    GDCLASS(MyNode, godot::Node)

public:
    int health;
};
```

After `gd-gen`:

```cpp
GCLASS()
class MyNode : public godot::Node {
    GENERATED_BODY()

public:
    GPROPERTY()
    int health;

    GSIGNAL(health_changed(int new_health))
};
```

Generated code:

```cpp
void emit_health_changed();
void connect_health_changed(Callable callable);

void MyNode::_bind_methods() {
    godot::ClassDB::bind_method(D_METHOD("get_health"), &MyNode::get_health);
    godot::ClassDB::bind_method(D_METHOD("set_health", "value"), &MyNode::set_health);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "health", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "set_health", "get_health");
    ADD_SIGNAL(MethodInfo("health_changed", PropertyInfo(Variant::INT, "new_health")));
}
```

All you need to do is remember to add the proper GCLASS/GPROPERTY/GENUM/GSIGNAL/GFUNCTION anotations, add GENERATED_BODY() to the top of the class and include <generated/MYCLASS.generated.h> as your last include. Also there's a generated register_types macro that has a GENERATED_TYPES() macro, remember to use it inside "initialize_xxx_module" to automatically register classes.

---

## 📚 Documentation

[DOCUMENTATION](./DOCUMENTATION.md)

---

## 🧪 Development Status

This project is under active development. Contributions and feedback are welcome!

---

## 📄 License

Modified MIT License. See [LICENSE](./LICENSE) for more details.

---

## 💬 Contact

Questions or ideas? Open an issue or contact [pliduino](https://github.com/pliduino).
