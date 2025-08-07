## 🧩 Macro Reference

### 🔷 `GCLASS(...)`

Registers a class with Godot.

**Modifiers:**
- `Resource` — Forces the class to be treated as a `Resource` (useful if detection fails — report as a bug if needed).
- `CustomBindings` — Enables a `custom_bind_methods()` function for manual binding access.
- `Virtual` — Marks the class as virtual; it won't be instantiable from scripts or the editor.
- `Abstract` — Prevents direct instantiation of the class entirely, including in C++ (must be subclassed).
- `Runtime` — Registers the class only at runtime; it won't be exposed to the editor or serialized.
- `Internal` — Hides the class from the editor and scripting APIs; useful for engine-only or implementation-detail types.

---

### 🔶 `GPROPERTY(...)`

Registers a property inside the class.

**Modifiers:**
- `Required` — Displays a warning if the property is unset in the inspector.
- `NoStorage` — Prevents the property from being serialized.
- `HideInInspector` — Hides the property from the inspector.
- `MultilineText` — Shows the property as a multiline text box.
- `ShowIf(condition)` — Only shows the property if the given condition evaluates to true.
- `Getter(getter)` — Uses the specified method name as the property's getter.
- `Setter(setter)` — Uses the specified method name as the property's setter.
- `Range(min, max, step)` - Same as hint range, step is optional. Available for both floats and ints
- `Group("MyGroup/MySubgroup/MySubSubGroup")` - Organizes the property under nested group categories in the inspector. Groups and subgroups are separated by /.

> ⚠️ **Note:** The methods used in `Getter` and `Setter` must be registered using `GFUNCTION(...)`,
> or manually bound using `ClassDB::bind_method`, otherwise the property binding **will not work**.

---

### 🟡 `GENUM()`

Registers an enum.
If you're using an enum inside a `GPROPERTY()`, make sure to register it first with `GENUM()`.

---

### 🟢 `GFUNCTION()`

Exposes a function to Godot (just like `ClassDB::bind_method`).

---

### 🔴 `GSIGNAL(...)`

Registers a signal.

**Example:**
```cpp
GSIGNAL(health_changed(int value, float percent))
```

Automatically generates:
```cpp
emit_health_changed(...)
connect_health_changed(Callable callable)
```

---

### 🟣 `GSTRUCT(...)`
Registers a struct as a serializable and editable container of grouped properties in the Godot inspector.

When used inside a class with GPROPERTY(...), the struct's fields are shown together under a collapsible section. Nested GSTRUCTs are also supported and displayed as nested sections.

Each field in the struct must be marked with GPROPERTY(...) to be visible and editable.

⚠️ GSTRUCTS currently do not support many GPROPERTY flags, GFUNCTIONs or GSIGNALs.

Example:

```cpp
GSTRUCT()
struct WeaponStats {
    GPROPERTY(Group("Damage"))
    int damage = 10;

    GPROPERTY(Group("Damage"))
    int damage_type = 0;

    GPROPERTY()
    float range = 15.0f;
};

GSTRUCT()
struct EnemyStats {
    GPROPERTY(Group("Equipment"))
    WeaponStats weapon;
    GPROPERTY()
    float speed = 4.0f;
};
```

When used in a class:

```cpp
GCLASS()
class MyEnemy : public Node {
    GPROPERTY() EnemyStats stats;
};
```
This displays in the inspector as a collapsible group "stats", containing:

- Equipment
    - weapon
        - Damage
            - damage
            - damage_type
        - range
- speed
