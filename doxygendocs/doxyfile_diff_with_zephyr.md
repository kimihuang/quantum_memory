# Doxyfile 与 Zephyr doxyfile.in 差异对比

> 当前 Doxyfile: `docs/quantum_memory/doxygendocs/Doxyfile`
> Zephyr 参考文件: `src/zephyrproject/zephyr/doc/zephyr.doxyfile.in`
> 当前 doxygen 版本: 1.9.8（Zephyr 配置面向 1.15.0）

---

## 一、版本兼容性说明

Zephyr 的 doxyfile.in 面向 doxygen 1.15.0，包含以下 1.9.8 不支持的设置（无法直接使用）：

| 设置 | Zephyr 值 | 说明 |
|------|-----------|------|
| `MARKDOWN_STRICT` | `YES` | 严格 Markdown 解析 |
| `HTML_COPY_CLIPBOARD` | `YES` | 代码片段复制按钮 |
| `PAGE_OUTLINE_PANEL` | `YES` | 页面大纲面板 |
| `WARN_LAYOUT_FILE` | `YES` | 布局文件警告 |
| `IMPLICIT_DIR_DOCS` | `NO` | 隐式目录文档 |
| `SHOW_ENUM_VALUES` | `NO` | 显示枚举值 |
| `UML_MAX_EDGE_LABELS` | `10` | UML 边标签数上限 |

---

## 二、项目信息

| 设置 | 当前值 | Zephyr 值 | 差异 | 建议 |
|------|--------|-----------|------|------|
| `PROJECT_NAME` | `"Quantum Memory Doxygen Docs"` | `"Zephyr API Documentation"` | YES | 保留当前 |
| `PROJECT_NUMBER` | 空 | `@ZEPHYR_VERSION@` | YES | 保留当前 |
| `PROJECT_BRIEF` | 空 | `"A Scalable Open Source RTOS"` | YES | 可选添加 |
| `PROJECT_LOGO` | 空 | 有 logo.svg | YES | 可选添加 |
| `PROJECT_ICON` | 无 | 有 favicon.png | YES | 可选添加 |
| `OUTPUT_LANGUAGE` | `Chinese` | `English` | YES | **保留当前** |
| `OUTPUT_DIRECTORY` | `html` | `@DOXY_OUT@` | YES | 保留当前 |

---

## 三、构建提取设置

| 设置 | 当前值 | Zephyr 值 | 差异 |
|------|--------|-----------|------|
| `EXTRACT_ALL` | `YES` | `YES` | - |
| `EXTRACT_PRIVATE` | `YES` | `NO` | **YES** |
| `EXTRACT_PACKAGE` | `NO` | `YES` | **YES** |
| `EXTRACT_STATIC` | `YES` | `YES` | - |
| `EXTRACT_LOCAL_CLASSES` | `YES` | `YES` | - |
| `EXTRACT_LOCAL_METHODS` | `NO` | `YES` | **YES** |
| `HIDE_UNDOC_MEMBERS` | `NO` | `NO` | - |
| `HIDE_UNDOC_CLASSES` | `NO` | `NO` | - |
| `HIDE_UNDOC_NAMESPACES` | 默认 `NO` | `YES` | **YES** |

**说明：** Zephyr 不提取 private 成员，但提取本地方法和 package 成员，并隐藏无文档的命名空间。

---

## 四、源码浏览设置

| 设置 | 当前值 | Zephyr 值 | 差异 | 建议 |
|------|--------|-----------|------|------|
| `SOURCE_BROWSER` | `YES` | `NO` | **YES** | **保留当前**（用户需要源码浏览） |
| `INLINE_SOURCES` | `NO` | `NO` | - | - |
| `STRIP_CODE_COMMENTS` | `NO` | `YES` | **YES** | **保留当前**（用户需要显示注释） |
| `REFERENCED_BY_RELATION` | `NO` | `NO` | - | - |
| `REFERENCES_RELATION` | `NO` | `NO` | - | - |

---

## 五、HTML 外观设置

| 设置 | 当前值 | Zephyr 值 | 差异 |
|------|--------|-----------|------|
| `HTML_COLORSTYLE` | `LIGHT` | `LIGHT` | - |
| `HTML_DYNAMIC_SECTIONS` | `NO` | `YES` | **YES** |
| `HTML_CODE_FOLDING` | `YES` | `YES` | - |
| `BINARY_TOC` | `NO` | `YES` | **YES** |
| `TREEVIEW_WIDTH` | `250` | `300` | **YES** |
| `HTML_EXTRA_STYLESHEET` | 本地 `zephy_doxygen_style/` | Zephyr `doc/_doxygen/` | **路径不同**，文件同名 |
| `HTML_EXTRA_FILES` | 空 | 有 JS 文件 | **YES** |
| `HTML_HEADER` | 空 | 有 header.html | **YES** |
| `HTML_COPY_CLIPBOARD` | 不支持(1.9.8) | `YES` | 需升级 doxygen |

**Zephyr 缺少的 HTML_EXTRA_FILES（暗色模式切换等）：**
```
doxygen-awesome-darkmode-toggle.js
doxygen-awesome-zephyr.js
```

**注意：** 当前 Doxyfile 引用了 `zephy_doxygen_style/` 目录，但该目录在仓库中不存在，需要从 Zephyr 源码复制 CSS/JS 文件。

---

## 六、排序与显示

| 设置 | 当前值 | Zephyr 值 | 差异 |
|------|--------|-----------|------|
| `SORT_MEMBER_DOCS` | `YES` | `YES` | - |
| `SORT_GROUP_NAMES` | `NO` | `YES` | **YES** |
| `SORT_BY_SCOPE_NAME` | `NO` | `YES` | **YES** |
| `STRICT_PROTO_MATCHING` | `NO` | `YES` | **YES** |
| `CASE_SENSE_NAMES` | `SYSTEM` | `YES` | **YES** |
| `ALWAYS_DETAILED_SEC` | `NO` | `YES` | **YES** |
| `INLINE_INHERITED_MEMB` | `NO` | `YES` | **YES** |
| `SHOW_GROUPED_MEMB_INC` | `NO` | `YES` | **YES** |
| `TAB_SIZE` | `4` | `8` | **YES** |
| `MAX_INITIALIZER_LINES` | `30` | `300` | **YES** |

---

## 七、预处理器设置

| 设置 | 当前值 | Zephyr 值 | 差异 |
|------|--------|-----------|------|
| `ENABLE_PREPROCESSING` | `YES` | `YES` | - |
| `MACRO_EXPANSION` | `NO` | `YES` | **YES** |
| `EXPAND_ONLY_PREDEF` | `NO` | `NO` | - |
| `SKIP_FUNCTION_MACROS` | `YES` | `NO` | **YES** |
| `PREDEFINED` | 空 | 大量 Kconfig 宏 | **YES** |

**Zephyr PREDEFINED 示例（部分）：**
```
__DOXYGEN__
CONFIG_ARCH_HAS_CUSTOM_BUSY_WAIT
CONFIG_ARCH_HAS_CUSTOM_SWAP_TO_MAIN
CONFIG_BT_CLASSIC
CONFIG_BT_EATT
...
```

**说明：** Zephyr 启用宏展开（`MACRO_EXPANSION=YES`）并预定义了大量 `CONFIG_*` Kconfig 宏，使得条件编译的代码在文档中正确展示。当前 Doxyfile 未使用此功能。

---

## 八、Dot/图表设置

| 设置 | 当前值 | Zephyr 值 | 差异 |
|------|--------|-----------|------|
| `HAVE_DOT` | `YES` | `NO` | **YES** |
| `CLASS_GRAPH` | `YES` | `TEXT` | **YES** |
| `COLLABORATION_GRAPH` | `YES` | `YES` | - |
| `GROUP_GRAPHS` | `YES` | `YES` | - |
| `INCLUDE_GRAPH` | `YES` | `YES` | - |
| `INCLUDED_BY_GRAPH` | `YES` | `YES` | - |
| `CALL_GRAPH` | `NO` | `NO` | - |
| `CALLER_GRAPH` | `NO` | `NO` | - |
| `GRAPHICAL_HIERARCHY` | `YES` | `YES` | - |
| `DIRECTORY_GRAPH` | `YES` | `YES` | - |
| `INTERACTIVE_SVG` | `NO` | `NO` | - |

**说明：** Zephyr 关闭 Graphviz（`HAVE_DOT=NO`），改用 `CLASS_GRAPH=TEXT`（纯文本类图），减少对 graphviz/dot 的依赖。当前配置启用了 Graphviz。

---

## 九、性能设置

| 设置 | 当前值 | Zephyr 值 | 差异 |
|------|--------|-----------|------|
| `LOOKUP_CACHE_SIZE` | `0` | `9` | **YES** |
| `NUM_PROC_THREADS` | `1` | `0` (auto) | **YES** |
| `QUIET` | `NO` | `YES` | **YES** |
| `TIMESTAMP` | `NO` | `YES` | **YES** |

---

## 十、输出格式

| 设置 | 当前值 | Zephyr 值 | 差异 |
|------|--------|-----------|------|
| `GENERATE_HTML` | `YES` | `YES` | - |
| `GENERATE_LATEX` | `NO` | `NO` | - |
| `GENERATE_XML` | `NO` | `YES` | **YES** |
| `GENERATE_DOCSET` | `NO` | `YES` | **YES** |
| `GENERATE_TODOLIST` | `YES` | `NO` | **YES** |
| `GENERATE_TESTLIST` | `YES` | `NO` | **YES** |
| `GENERATE_BUGLIST` | `YES` | `NO` | **YES** |

---

## 十一、输入设置

| 设置 | 当前值 | Zephyr 值 | 差异 |
|------|--------|-----------|------|
| `INPUT` | `zephyr/` | 指定多个精确路径 | **YES** |
| `FILE_PATTERNS` | 全类型（*.c, *.h, *.py, *.md, *.asm 等） | 仅 `*.c *.h *.S *.md` | **YES** |
| `RECURSIVE` | `YES` | `YES` | - |
| `EXCLUDE` | 空 | 排除部分 CMSIS 等文件 | **YES** |
| `EXCLUDE_SYMBOLS` | 空 | `_[^t]* _t[^r]* ... *.__unnamed__ z_* Z_*` | **YES** |
| `USE_MDFILE_AS_MAINPAGE` | 空 | 指定 mainpage.md | **YES** |
| `STRIP_FROM_PATH` | 空 | 多个 include 路径 | **YES** |
| `STRIP_FROM_INC_PATH` | 空 | 多个 include 路径 | **YES** |

**说明：** 当前使用宽泛的 `INPUT=zephyr/` + 全类型 FILE_PATTERNS；Zephyr 精确指定输入路径并限制 FILE_PATTERNS 为 `*.c *.h *.S *.md`。`EXCLUDE_SYMBOLS` 用于过滤内部符号。

---

## 十二、ALIASES（别名）

| 设置 | 当前值 | Zephyr 值 | 差异 |
|------|--------|-----------|------|
| `ALIASES` | 空 | 丰富的自定义别名 | **YES** |

**Zephyr 定义的别名（部分）：**
```
kconfig{1}=...        # Kconfig 选项引用
req{1}=...            # 需求追踪
satisfy{1}=...        # 需求满足标记
driver_ops{1}=...     # 驱动操作文档
def_driverbackendgroup{2}=...
```

---

## 十三、其他差异

| 设置 | 当前值 | Zephyr 值 | 差异 |
|------|--------|-----------|------|
| `QT_AUTOBRIEF` | `NO` | `YES` | **YES** |
| `CPP_CLI_SUPPORT` | `NO` | `YES` | **YES** |
| `ABBREVIATE_BRIEF` | 显式列表 | `YES`（简写） | **YES** |
| `GENERATE_TAGFILE` | 空 | 生成 zephyr.tag | **YES** |
| `LATEX_CMD_NAME` | 空 | `latex` | **YES** |
| `RTF_HYPERLINKS` | `NO` | `YES` | **YES** |
| `CLANG_ASSISTED_PARSING` | `NO` | 不存在(已移除) | 版本差异 |
| `SHOW_FILES` | `YES` | `YES` | - |
| `FULL_PATH_NAMES` | `YES` | `YES` | - |
| `GENERATE_TREEVIEW` | `YES` | `YES` | - |
| `SEARCHENGINE` | `YES` | `YES` | - |

---

## 十四、仅存在于一个文件中的设置

### 仅当前 Doxyfile 有（1.9.8 遗留，1.15.0 已移除）：
- `CLANG_ASSISTED_PARSING`
- `CLANG_ADD_INC_PATHS`
- `CLANG_OPTIONS`
- `CLANG_DATABASE_PATH`
- `XML_NS_MEMB_FILE_SCOPE`

### 仅 Zephyr doxyfile.in 有（1.15.0 新增）：
- `PROJECT_ICON`
- `IMPLICIT_DIR_DOCS`
- `MARKDOWN_STRICT`
- `AUTOLINK_IGNORE_WORDS`
- `EXTERNAL_TOOL_PATH`
- `WARN_LAYOUT_FILE`
- `HTML_COPY_CLIPBOARD`
- `HTML_PROJECT_COOKIE`
- `SHOW_ENUM_VALUES`
- `PAGE_OUTLINE_PANEL`
- `PLANTUMLFILE_DIRS`
- `UML_MAX_EDGE_LABELS`
