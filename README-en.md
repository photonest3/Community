<h1 align="center">
  <img src="public/logo.png" alt="PhotoNest Logo" width="64"/>
  <br/>
  PhotoNest
</h1>

<p align="center">
  <b>🔐 Your Private Photo Vault · Local-First · Secure Encryption · AI-Powered Management</b>
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
  <a href="README.md">🇨🇳 中文</a> &nbsp;|&nbsp;
  <a href="#-quick-start">🚀 Quick Start</a> &nbsp;|&nbsp;
  <a href="#-features">✨ Features</a> &nbsp;|&nbsp;
  <a href="#-screenshots">🖼️ Screenshots</a> &nbsp;|&nbsp;
  <a href="#-contributing">🤝 Contributing</a>
</p>

---

## 📖 Overview

**PhotoNest** is a privacy-focused local photo management application. It helps you centrally manage all your media files — simple, secure, and free. No cloud uploads required; all data stays on your device. With encrypted albums, AI-powered organization, and a built-in photo editor, PhotoNest handles tens or even hundreds of thousands of photos with ease.

> *"PhotoNest is a full-featured and intuitive tool — cheaper, simpler, and more powerful."*

### 🎯 Core Principles

- **🔒 Privacy First** — Local storage, fast import, no web uploads
- **🔑 Encrypted Protection** — Only password holders can unlock encrypted albums
- **🧠 AI-Powered** — Face clustering, map browsing, smart search
- **⚡ High Performance** — Smoothly manages hundreds of thousands of photos

---

## ✨ Features

### 📂 Media Management
- **Local Storage** — Fast import, no cloud dependency, simple & secure
- **Massive Library Support** — Handles tens to hundreds of thousands of photos
- **Multi-Format** — JPG, JPEG, TIFF, PNG and other mainstream formats

### 🗓️ Smart Browsing
| Feature | Description |
|---------|-------------|
| 📅 Calendar View | Quickly locate photos by capture date |
| 🗺️ Map View | Display photos on an interactive map via GPS data |
| 👤 Face Sorting | AI-powered face detection and auto-grouping |
| 🎬 Slideshow | Full-screen immersive photo playback |
| ⭐ Quality Rating | Smart quality assessment to filter the best shots |

### 🖼️ Built-in Photo Editor
- **AI Inpaint** — Intelligently remove unwanted objects from photos
- **AI Cutout** — One-click precise background removal, no expertise needed
- **Basic Editing** — Crop, rotate, color adjustment and other essentials

### 🔐 Encrypted Albums
- End-to-end encryption — only password holders can unlock
- Protect your private photos from unauthorized access

### 📊 Metadata Management (Photonest Plus)
- Group by aperture, focal length, ISO, date, camera model, location
- Visual charts showing your photography habits
- Batch rename, auto-sort, duplicate detection
- Strip metadata for privacy protection

---

## 🖥️ System Requirements

| Item | Requirement |
|------|-------------|
| **OS** | Windows 10 (Build 17763.0+) / Windows 11 |
| **Architecture** | 64-bit |
| **Storage** | Recommended 1.5x your photo library size |
| **Memory** | 8GB+ recommended (for large libraries) |

---

## 🚀 Quick Start

### Option 1: Download & Install

1. Search for **PhotoNest** in the [Microsoft Store](https://www.microsoft.com/en-sb/p/photonest/9p7dm2lb17r4)
2. Or download the latest installer from [Releases](https://github.com/photonest3/Community/releases)
3. Double-click to install and launch

### Option 2: Build from Source

```bash
# Clone the repository
git clone https://github.com/photonest3/Community.git
cd Community

# Install dependencies
npm install

# Run in development mode
npm run dev

# Build production version
npm run build

# Package as Windows installer
npm run package
```

---

## 🖼️ Screenshots

<details>
<summary>Click to expand</summary>

### Main Interface — Photo Library
> Supports calendar view, grid view, map view, and more

### Encrypted Album
> Enter password to unlock, keeping your privacy safe

### AI Photo Editor
> Built-in inpaint & cutout, no professional skills required

</details>

---

## 🏗️ Architecture

```
PhotoNest/
├── src/
│   ├── main/              # Electron main process
│   │   ├── database/      # Database operations (SQLite)
│   │   ├── encryption/    # Encryption module (AES-256)
│   │   ├── importer/      # Photo import engine
│   │   └── thumbnail/     # Thumbnail generation
│   ├── renderer/          # React render process
│   │   ├── components/    # UI components
│   │   ├── pages/         # Page views
│   │   ├── stores/        # State management (Zustand)
│   │   └── utils/         # Utility functions
│   └── shared/            # Shared types & constants
├── resources/             # Static assets
├── scripts/               # Build scripts
└── package.json
```

### Tech Stack

| Layer | Technology |
|-------|------------|
| Framework | Electron + React + TypeScript |
| State Management | Zustand |
| Database | SQLite (better-sqlite3) |
| Encryption | AES-256-GCM (Node crypto) |
| Image Processing | Sharp + Canvas |
| AI Engine | ONNX Runtime (Face Detection / Inpaint) |
| Maps | Leaflet |
| Styling | Tailwind CSS |

---

## 📝 User Guide

### Import Photos
1. Click the **Add Photos** button or drag a folder into the window
2. Select the folder to import (e.g., `D:/102PHOTO`)
3. Wait for the import to complete — PhotoNest auto-generates thumbnails and metadata index

### Create Encrypted Album
1. Click **New Album** on the album page
2. Select **Encrypted Album** and set a strong password
3. Drag private photos into the encrypted album
4. Password required every time you access it

### AI Photo Editing
1. Select a photo and click **Edit** to enter edit mode
2. Choose **Inpaint**: brush over the area to remove, AI fills it automatically
3. Choose **Cutout**: one-click subject detection and background separation

### Map View
1. Ensure your photos contain GPS metadata
2. Switch to **Map View** — photos are automatically pinned on the map
3. Click a marker to see all photos taken at that location

---

## 🤝 Contributing

We welcome all forms of contributions! Please read the guidelines below:

### Development Workflow

1. **Fork** this repository to your GitHub account
2. **Clone** your fork locally
   ```bash
   git clone https://github.com/YOUR_USERNAME/Community.git
   cd Community
   ```
3. Create a feature branch
   ```bash
   git checkout -b feature/your-feature-name
   ```
4. Commit your changes
   ```bash
   git commit -m "feat: add some amazing feature"
   ```
5. Push to your fork
   ```bash
   git push origin feature/your-feature-name
   ```
6. Submit a **Pull Request**

### Commit Convention

We follow the [Conventional Commits](https://www.conventionalcommits.org/) specification:

| Prefix | Description |
|--------|-------------|
| `feat:` | New feature |
| `fix:` | Bug fix |
| `docs:` | Documentation update |
| `style:` | Code formatting |
| `refactor:` | Code refactoring |
| `perf:` | Performance improvement |
| `test:` | Test-related changes |

### Code of Conduct

This project adheres to the [Contributor Covenant 2.1](https://www.contributor-covenant.org/version/2/1/code_of_conduct/). By participating, you agree to abide by its terms.

---

## 🐛 Bug Reports

Found a bug? Please follow these steps:

1. Search [existing Issues](https://github.com/photonest3/Community/issues) to avoid duplicates
2. If not found, click **New Issue** to create one
3. Include the following information:
   - PhotoNest version
   - OS version
   - Steps to reproduce
   - Screenshots or error logs

You can also reach us via email: **support@photonest.io**

---

## 🗺️ Roadmap

- [x] Local photo import & management
- [x] Calendar / Map / Face browsing
- [x] Encrypted albums
- [x] AI Inpaint & Cutout
- [ ] Cross-platform support (macOS / Linux)
- [ ] RAW format support (CR2, NEF, ARW)
- [ ] Duplicate photo detection
- [ ] Batch metadata editing
- [ ] Plugin system
- [ ] Internationalization (i18n)

---

## 📄 License

This project is open-source under the [MIT License](LICENSE). Feel free to use and modify.

---

## 🙏 Acknowledgements

Thanks to the following open-source projects and technologies:

- [Electron](https://electronjs.org/) — Cross-platform desktop framework
- [React](https://react.dev/) — UI component library
- [ONNX Runtime](https://onnxruntime.ai/) — AI inference engine
- [Sharp](https://sharp.pixelplumbing.com/) — High-performance image processing
- [Leaflet](https://leafletjs.com/) — Interactive maps
- [Tailwind CSS](https://tailwindcss.com/) — Utility-first CSS framework

---

## 📬 Contact

- 🌐 Website: [photonest.io](https://photonest.io)
- 📧 Email: support@photonest.io
- 🐙 GitHub: [github.com/photonest3/Community](https://github.com/photonest3/Community)
- 💬 Community: Join the conversation in GitHub Discussions

---

<p align="center">
  <b>If you find PhotoNest useful, please give us a ⭐ Star!</b>
  <br/><br/>
  Made with ❤️ by the PhotoNest Team
</p>
