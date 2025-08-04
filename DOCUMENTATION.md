## 🧩 Macro Reference

### 🔷 `GCLASS(...)`

Registers a class with Godot.

**Modifiers:**
- `Resource` — Forces the class to be treated as a `Resource` (useful if detection fails — report as a bug if needed).
- `CustomBindings` — Enables a `custom_bind_methods()` function for manual binding access.

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
