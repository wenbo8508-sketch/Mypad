# Mypad 编辑器图标包（Google Material Icons）

来源仓库：<https://github.com/google/material-design-icons>（Material Icons 经典图标集，Apache License 2.0）

本目录下的所有图标均取自该仓库 `master` 分支：

- SVG 原文件：`https://raw.githubusercontent.com/google/material-design-icons/master/src/<分类>/<图标名>/<样式>/24px.svg`
- 风格目录名：`materialicons` = filled（填充）、`materialiconsoutlined` = outlined（描边）
- PNG 由上述官方 SVG 在本地渲染得到（24 px / 48 px，@2x 用于高分屏），未做任何形状改动。

## 目录结构

```
img/
├─ toolbar/        工具栏 16 个图标（filled 风格，已按功能重命名，可直接用）
│    new_24px.png / new_48px.png / new.svg ...
├─ png/
│    ├─ 24px/        56 个图标 24px  黑色 #202124（outlined）
│    ├─ 48px/        56 个图标 48px  黑色 #202124（outlined）
│    ├─ 24px_blue/   56 个图标 24px  蓝色 #1A73E8（outlined）
│    └─ 48px_blue/   56 个图标 48px  蓝色 #1A73E8（outlined）
├─ svg/            官方 SVG 原文件（每个图标 2 个：`<名称>_filled.svg`、`<名称>_outlined.svg`）
├─ icons.qrc       现成的 Qt 资源文件（把 img 目录整体加入资源）
└─ README_图标对照表.md
```

## 一、工具栏 16 个按钮 ↔ 图标对照

按截图从左到右的顺序：

| # | 功能 | 官方图标名 | 建议文件名 | 说明 |
|---|------|-----------|-----------|------|
| 1 | 新建 | `note_add` | `toolbar/new_24px.png` | 文档/页面 + 号 |
| 2 | 打开 | `folder_open` | `toolbar/open_24px.png` | 打开文件夹 |
| 3 | 保存 | `save` | `toolbar/save_24px.png` | 软盘 |
| 4 | 另存为 | `save_as` | `toolbar/save_as_24px.png` | 软盘 + 铅笔 |
| 5 | 撤销 | `undo` | `toolbar/undo_24px.png` | 向左弯箭头 |
| 6 | 重做 | `redo` | `toolbar/redo_24px.png` | 向右弯箭头 |
| 7 | 剪切 | `content_cut` | `toolbar/cut_24px.png` | 剪刀 |
| 8 | 复制 | `content_copy` | `toolbar/copy_24px.png` | 两页纸 |
| 9 | 粘贴 | `content_paste` | `toolbar/paste_24px.png` | 剪贴板 |
| 10 | 查找 | `find_in_page` | `toolbar/find_24px.png` | 页面 + 放大镜 |
| 11 | 替换 | `find_replace` | `toolbar/replace_24px.png` | 放大镜 + 循环箭头 |
| 12 | 字体 / 文字格式 | `text_format` | `toolbar/font_24px.png` | A + 色条 |
| 13 | 项目符号列表 | `format_list_bulleted` | `toolbar/list_bullet_24px.png` | 圆点 + 横线 |
| 14 | 增加缩进 | `format_indent_increase` | `toolbar/indent_24px.png` | 横线 + 右箭头 |
| 15 | 工具 / 设置 | `build` | `toolbar/tools_24px.png` | 扳手（截图中第 15 个图标） |
| 16 | 下划线 | `format_underlined` | `toolbar/underline_24px.png` | U + 底线（截图中第 16 个蓝色图标） |

> 说明：截图第 15 个图标看起来像扳手或钥匙，这里主选 `build`（扳手，对应"工具/设置"），
> 另附 `vpn_key`（钥匙）、`settings`（齿轮）作为备选；
> 第 16 个蓝色 U 形图标与 Material 的 `format_underlined`（下划线）形状一致，
> 若你的程序里它是"收件箱/托盘"含义，可改用附带的 `inbox`，或改用 `visibility`（显示/隐藏状态栏）。

## 二、其余备选 / 菜单图标

| 图标名 | 用途 | 分组 | 仓库分类 |
|--------|------|------|----------|
| `list_alt` | 列表(带框) 备选 List alt | alt | communication |
| `inbox` | 收件箱/托盘 备选 Inbox | alt | content |
| `vpn_key` | 密钥 备选 Key | alt | communication |
| `settings` | 设置 备选 Settings | alt | action |
| `format_indent_decrease` | 减少缩进 Decrease indent | alt | editor |
| `format_color_text` | 字体颜色 Font color | alt | editor |
| `description` | 文档/文本文件 Document | file | action |
| `insert_drive_file` | 空白文件 Blank file | file | editor |
| `upload_file` | 导入/上传 Import | file | file |
| `save_alt` | 保存副本 Save a copy | file | content |
| `print` | 打印 Print | file | action |
| `close` | 关闭 Close | file | navigation |
| `exit_to_app` | 退出 Exit | file | action |
| `help` | 帮助 Help | file | action |
| `info` | 关于/信息 About | file | action |
| `select_all` | 全选 Select all | edit | content |
| `delete` | 删除 Delete | edit | action |
| `search` | 搜索 Search | edit | action |
| `copy_all` | 复制全部 Copy all | edit | content |
| `spellcheck` | 拼写检查 Spell check | edit | action |
| `format_bold` | 加粗 Bold | format | editor |
| `format_italic` | 斜体 Italic | format | editor |
| `format_size` | 字号 Font size | format | editor |
| `title` | 标题 Title | format | editor |
| `highlight` | 突出显示/底色 Highlight | format | editor |
| `strikethrough_s` | 删除线 Strikethrough | format | editor |
| `format_list_numbered` | 编号列表 Numbered list | format | editor |
| `format_align_left` | 左对齐 Align left | format | editor |
| `format_align_center` | 居中对齐 Align center | format | editor |
| `format_align_right` | 右对齐 Align right | format | editor |
| `format_align_justify` | 两端对齐 Justify | format | editor |
| `wrap_text` | 自动换行 Wrap text | format | editor |
| `fullscreen` | 全屏 Fullscreen | view | navigation |
| `visibility` | 状态栏/可见性 Status bar | view | action |
| `view_list` | 列表视图 List view | view | action |
| `zoom_in` | 放大 Zoom in | view | action |
| `zoom_out` | 缩小 Zoom out | view | action |
| `refresh` | 刷新 Refresh | view | navigation |
| `dark_mode` | 深色模式 Dark mode | view | device |
| `light_mode` | 浅色模式 Light mode | view | device |

## 三、在 Qt 里怎么用

```cpp
// 1) 直接把 img 目录加入资源：把 icons.qrc 添加到 CMakeLists.txt 的 set(PROJECT_SOURCES ...)
//    或在 .pro 文件里写 RESOURCES += icons.qrc

// 2) 给工具栏按钮设置图标
ui->actNew->setIcon(QIcon(":/img/toolbar/new_24px.png"));
ui->actOpen->setIcon(QIcon(":/img/toolbar/open_24px.png"));
ui->actSave->setIcon(QIcon(":/img/toolbar/save_24px.png"));

// 3) 高分屏（125%/150% 缩放）建议同时提供 48px，Qt 会自动按 devicePixelRatio 选择
QIcon icon;
icon.addFile(":/img/toolbar/save_24px.png",  QSize(24, 24));
icon.addFile(":/img/toolbar/save_48px.png",  QSize(48, 48));
ui->actSave->setIcon(icon);

// 4) 也可以用 SVG（需要 Qt 的 svg 图像插件，Qt 官方安装包默认带）
ui->actSave->setIcon(QIcon(":/img/toolbar/save.svg"));
```

### 让图标"禁用时变灰"

```cpp
QIcon ic(":/img/toolbar/save_24px.png");
ic.addFile(":/img/toolbar/save_24px.png", QSize(), QIcon::Normal);
ui->actSave->setIcon(ic);   // QAction 在 setEnabled(false) 时会自动生成灰化版本
```

## 四、授权

- Material Icons 采用 Apache License 2.0，可自由用于商业/非商业项目，无需强制署名（在项目"关于"页注明即可）。
- 许可证原文：`https://github.com/google/material-design-icons/blob/master/LICENSE`
