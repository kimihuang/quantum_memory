# Mermaid 图表引用

本文档介绍如何在 Markdown 文件中引用外部 `.mmd` 文件，使其在 MkDocs 编译后以图形方式渲染。

---

## 原理

通过 `pymdownx.snippets` + `pymdownx.superfences` 组合实现：

1. **`pymdownx.snippets`**（预处理器）在编译早期将 `--8<-- "filename"` 替换为文件内容
2. **`pymdownx.superfences`** 随后将包含 mermaid 内容的代码块渲染为 `<pre class="mermaid">`，由浏览器端 Mermaid.js 绘制图形

## 基本用法

在 Markdown 文件中使用以下语法：

````markdown
```mermaid
--8<-- "src/packages/embeded_logging/embedded_logging_arch.mmd"
```
````

### 示例：引用同目录下的 .mmd 文件

假设 `.mmd` 文件与 `.md` 文件在同一目录（如 `src/packages/embeded_logging/`）下：

````markdown
```mermaid
--8<-- "src/packages/embeded_logging/embedded_logging_arch.mmd"
```
````

### 示例：引用其他目录的 .mmd 文件

````markdown
```mermaid
--8<-- "src/packages/embeded_logging/embedded_logging_dataflow.mmd"
```
````

## .mmd 文件编写规范

- 使用标准 Mermaid 语法（graph、sequenceDiagram、classDiagram 等）
- 文件编码为 UTF-8
- 文件内不要包含 ` ``` ` 围栏标记，只需写 Mermaid 图表内容本身

```mermaid
graph LR
    A[编写 .mmd 文件] --> B[在 md 中用 --8<-- 引用]
    B --> C[mkdocs build]
    C --> D[浏览器中查看图形]
```

## 配置参考

`mkdocs.yml` 中的相关配置：

```yaml
markdown_extensions:
  - pymdownx.snippets:
      base_path:
        - docs    # snippets 搜索根目录
      check_paths: true   # 文件不存在时报错
  - pymdownx.superfences:
      custom_fences:
        - name: mermaid
          class: mermaid
          format: !!python/name:pymdownx.superfences.fence_code_format
```

## 常见问题

**Q: 构建报错 `File not found`**

检查文件路径是否相对于 `base_path` 正确。如果 `.mmd` 文件在 `docs/src/packages/embeded_logging/` 下，路径应为 `src/packages/embeded_logging/xxx.mmd`。

**Q: 图表不渲染，只显示代码文本**

确认浏览器控制台是否有 Mermaid.js 加载错误。MkDocs Material 主题内置了 Mermaid 支持（9.0+），确保主题版本 >= 9.0。
