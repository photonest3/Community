<h1 align="center">
  <img src="public/logo.png" alt="PhotoNest Logo" width="64"/>
  <br/>
  PhotoNest
</h1>

<p align="center">
  <b>🔐 你的私密照片管家 · 本地优先 · 安全加密 · AI 智能管理</b>
</p>

<p align="center">
  <a href="https://github.com/photonest3/Community"><img src="https://img.shields.io/badge/Platform-Windows%2010%2F11-0078D4?style=flat-square&logo=windows" alt="Platform"></a>
  <a href="https://github.com/photonest3/Community"><img src="https://img.shields.io/github/license/photonest3/Community?style=flat-square&color=blue" alt="License"></a>
  <a href="https://github.com/photonest3/Community"><img src="https://img.shields.io/github/stars/photonest3/Community?style=flat-square&color=yellow" alt="Stars"></a>
  <a href="https://github.com/photonest3/Community"><img src="https://img.shields.io/github/forks/photonest3/Community?style=flat-square&color=orange" alt="Forks"></a>
  <a href="https://github.com/photonest3/Community"><img src="https://img.shields.io/github/issues/photonest3/Community?style=flat-square&color=red" alt="Issues"></a>
  <a href="https://photonest.io"><img src="https://img.shields.io/badge/Website-photonest.io-4CAF50?style=flat-square" alt="Website"></a>
</p>

<p align="center">
  <a href="README-en.md">🇬🇧 English</a> &nbsp;|&nbsp;
  <a href="#-快速开始">🚀 快速开始</a> &nbsp;|&nbsp;
  <a href="#-功能特性">✨ 功能特性</a> &nbsp;|&nbsp;
  <a href="#-截图预览">🖼️ 截图预览</a> &nbsp;|&nbsp;
  <a href="#-贡献指南">🤝 贡献指南</a>
</p>

---

## 📖 项目简介

**PhotoNest** 是一款专注于隐私保护的本地照片管理软件。它帮助你集中管理所有媒体文件——简单、安全、免费。无需上传云端，所有数据存储在你的设备上，支持加密相册、AI 智能分类、内置照片编辑器，轻松应对数万甚至数十万张照片的管理需求。

> *"PhotoNest 是一款功能全面且直观的工具——更便宜、更简单、更强大。"*

### 🎯 核心理念

- **🔒 隐私至上** — 本地存储，快速导入，无需上传到网络
- **🔑 加密保护** — 加密版本只有知道密码的人才能解锁和访问相册
- **🧠 AI 赋能** — 人脸分类、地图浏览、智能检索
- **⚡ 高性能** — 轻松管理数十万张照片不卡顿

---

## ✨ 功能特性

### 📂 媒体管理
- **本地存储** — 快速导入，无需上传到网络，简单安全
- **海量支持** — 轻松管理数万至数十万张照片
- **多格式支持** — JPG、JPEG、TIFF、PNG 等主流格式

### 🗓️ 特色浏览
| 功能 | 描述 |
|------|------|
| 📅 日历浏览 | 按拍摄日期快速定位照片 |
| 🗺️ 地图浏览 | 基于 GPS 数据在地图上展示照片 |
| 👤 人脸排序 | AI 识别人脸并自动分组 |
| 🎬 幻灯片播放 | 全屏幻灯片沉浸式回顾 |
| ⭐ 质量评分 | 智能评估照片质量，快速筛选佳作 |

### 🖼️ 内置照片编辑器
- **AI 修复 (Inpaint)** — 智能去除照片中不需要的物体
- **AI 抠图 (Cutout)** — 一键精准抠图，无需专业技能
- **基础编辑** — 裁剪、旋转、调色等常用功能

### 🔐 加密相册
- 端到端加密，只有密码持有者才能解锁
- 保护你的私密照片不被未授权访问

### 📊 元数据管理 (Photonest Plus)
- 按光圈、焦距、ISO、日期、相机型号、位置等分组
- 可视化图表展示拍摄习惯
- 批量重命名、自动归类、去重清理
- 移除元数据保护隐私

---

## 🖥️ 系统要求

| 项目 | 要求 |
|------|------|
| **操作系统** | Windows 10 (版本 17763.0 或更高) / Windows 11 |
| **架构** | 64-bit |
| **存储空间** | 建议预留照片库大小的 1.5 倍空间 |
| **内存** | 建议 8GB 及以上（处理大量照片时） |

---

## 🚀 快速开始

### 方式一：直接下载安装

1. 前往 [Microsoft Store](https://www.microsoft.com/en-sb/p/photonest/9p7dm2lb17r4) 搜索 **PhotoNest** 安装
2. 或从 [Releases](https://github.com/photonest3/Community/releases) 页面下载最新安装包
3. 双击安装，启动即可使用

### 方式二：从源码构建

```bash
# 克隆仓库
git clone https://github.com/photonest3/Community.git
cd Community

# 安装依赖
npm install

# 开发模式运行
npm run dev

# 构建生产版本
npm run build

# 打包为 Windows 安装程序
npm run package
```

---

## 🖼️ 截图预览

<details>
<summary>点击展开截图</summary>

### 主界面 — 照片库浏览
> 支持日历视图、网格视图、地图视图等多种浏览方式

### 加密相册
> 输入密码解锁加密相册，保护隐私安全

### AI 照片编辑
> 内置修复、抠图等功能，无需专业技能

</details>

---

## 🏗️ 技术架构

```
PhotoNest/
├── src/
│   ├── main/              # Electron 主进程
│   │   ├── database/      # 数据库操作 (SQLite)
│   │   ├── encryption/    # 加密模块 (AES-256)
│   │   ├── importer/      # 照片导入引擎
│   │   └── thumbnail/     # 缩略图生成
│   ├── renderer/          # React 渲染进程
│   │   ├── components/    # UI 组件
│   │   ├── pages/         # 页面视图
│   │   ├── stores/        # 状态管理 (Zustand)
│   │   └── utils/         # 工具函数
│   └── shared/            # 共享类型与常量
├── resources/             # 静态资源
├── scripts/               # 构建脚本
└── package.json
```

### 核心技术栈

| 层级 | 技术 |
|------|------|
| 框架 | Electron + React + TypeScript |
| 状态管理 | Zustand |
| 数据库 | SQLite (better-sqlite3) |
| 加密 | AES-256-GCM (Node crypto) |
| 图像处理 | Sharp + Canvas |
| AI 引擎 | ONNX Runtime (人脸检测/Inpaint) |
| 地图 | Leaflet |
| 样式 | Tailwind CSS |

---

## 📝 使用指南

### 导入照片
1. 点击 **Add Photos** 按钮或直接将文件夹拖入窗口
2. 选择要导入的文件夹（如 `D:/102PHOTO`）
3. 等待导入完成，PhotoNest 会自动生成缩略图和元数据索引

### 创建加密相册
1. 在相册页面点击 **New Album**
2. 选择 **Encrypted Album**，设置强密码
3. 将私密照片拖入加密相册
4. 每次访问需输入密码解锁

### AI 编辑照片
1. 选中照片，点击 **Edit** 进入编辑模式
2. 选择 **Inpaint** 修复：涂抹要去除的区域，AI 自动填充
3. 选择 **Cutout** 抠图：一键自动识别主体并分离背景

### 地图浏览
1. 确保照片包含 GPS 元数据
2. 切换到 **Map View**，照片将自动标注在地图上
3. 点击地图标记查看该位置拍摄的所有照片

---

## 🤝 贡献指南

我们欢迎任何形式的贡献！请阅读以下指南：

### 开发流程

1. **Fork** 本仓库到你的 GitHub 账号
2. **Clone** 你的 Fork 到本地
   ```bash
   git clone https://github.com/YOUR_USERNAME/Community.git
   cd Community
   ```
3. 创建特性分支
   ```bash
   git checkout -b feature/your-feature-name
   ```
4. 提交更改
   ```bash
   git commit -m "feat: add some amazing feature"
   ```
5. 推送到你的 Fork
   ```bash
   git push origin feature/your-feature-name
   ```
6. 提交 **Pull Request**

### 提交规范

我们使用 [Conventional Commits](https://www.conventionalcommits.org/zh-hans/) 规范：

| 前缀 | 说明 |
|------|------|
| `feat:` | 新功能 |
| `fix:` | Bug 修复 |
| `docs:` | 文档更新 |
| `style:` | 代码格式调整 |
| `refactor:` | 重构 |
| `perf:` | 性能优化 |
| `test:` | 测试相关 |

### 行为准则

本项目遵循 [Contributor Covenant 2.1](https://www.contributor-covenant.org/zh-cn/version/2/1/code_of_conduct/) 行为准则。参与贡献即表示你同意遵守该准则。

---

## 🐛 问题反馈

遇到问题？请按以下步骤反馈：

1. 搜索 [已有 Issues](https://github.com/photonest3/Community/issues) 确认是否有人提过
2. 如果没有，点击 **New Issue** 创建
3. 请包含以下信息：
   - PhotoNest 版本号
   - 操作系统版本
   - 问题复现步骤
   - 错误截图或日志

也可以通过邮件联系：**support@photonest.io**

---

## 🗺️ 路线图

- [x] 本地照片导入与管理
- [x] 日历/地图/人脸浏览
- [x] 加密相册
- [x] AI 修复 & 抠图
- [ ] 跨平台支持 (macOS / Linux)
- [ ] RAW 格式支持 (CR2, NEF, ARW)
- [ ] 重复照片检测
- [ ] 批量元数据编辑
- [ ] 插件系统
- [ ] 多语言支持 (i18n)

---

## 📄 开源协议

本项目基于 [MIT License](LICENSE) 开源，欢迎自由使用和修改。

---

## 🙏 致谢

感谢以下开源项目和技术：

- [Electron](https://electronjs.org/) — 跨平台桌面应用框架
- [React](https://react.dev/) — UI 组件库
- [ONNX Runtime](https://onnxruntime.ai/) — AI 推理引擎
- [Sharp](https://sharp.pixelplumbing.com/) — 高性能图像处理
- [Leaflet](https://leafletjs.com/) — 交互式地图
- [Tailwind CSS](https://tailwindcss.com/) — 实用优先的 CSS 框架

---

## 📬 联系我们

- 🌐 官网：[photonest.io](https://photonest.io)
- 📧 邮箱：support@photonest.io
- 🐙 GitHub：[github.com/photonest3/Community](https://github.com/photonest3/Community)
- 💬 社区：欢迎在 GitHub Discussions 中交流

---

<p align="center">
  <b>如果你觉得 PhotoNest 有用，请给我们一个 ⭐ Star！</b>
  <br/><br/>
  Made with ❤️ by the PhotoNest Team
</p>
