# Austere Engine

**Austere Engine** is a minimal, open-source 3D game engine written in modern C++.
It’s designed for simplicity, performance, and full control through code — no visual editor, no drag-and-drop, just clean C++.

> ⚠️ This engine is still in early prototype stage. Expect bugs, unfinished features, and rapid changes.

---

## 📸 Screenshots
> Coming soon...

---

## 🙋 About

**Austere Engine** is a personal hobby project built for experience, learning, and portfolio purposes.
It is not meant to compete with Unity, Unreal, or Godot — it’s a lightweight foundation for developers who prefer working in code and want full control over their engine.

Use it, break it, fork it, or contribute — it's up to you.

---

## ✨ Features

### Current Features

- Forward rendering using OpenGL with batching and frustum culling
- Basic material system with support for diffuse, specular, normal, emissive and opacity maps
- Basic lighting system with directional, point and spot lights
- Asset loading via [Assimp](https://github.com/assimp/assimp) and [stb_image](https://github.com/nothings/stb)
- Hierarchical scene system
- Convenient built-in logger

### Planned Features
- Shadow mapping (WIP)
- Post processing
- Skeletal animation
- Particle system
- Terrain
- Audio system (OpenAL or similar)
- Jolt Physics integration

---

## 🖥️ Supported Platforms

- ✅ Linux (works perfect)
- ✅ Windows (tested on Windows 11)

---

## 📦 Supported Formats

**Model formats** (via [Assimp](https://github.com/assimp/assimp), only popular ones enabled):
- .obj, .fbx, .gltf, .glb

**Image formats** (via [stb_image](https://github.com/nothings/stb))
- .jpg, .png, .tga, .bmp, .psd, .gif, .hdr, .pic  
*(all formats supported by stb_image are enabled)*

---

## 📚 Dependencies

**Austere Engine** uses the following libraries:
- [SDL3](https://github.com/libsdl-org/SDL) - Cross-platform window management and input
- [Assimp](https://github.com/assimp/assimp) - 3D model loading (OBJ, FBX, GLTF, etc.)
- [GLM](https://github.com/g-truc/glm) - Graphics math library (OpenGL-style)
- GLAD – OpenGL function loader (embedded in source)
- [stb_image](https://github.com/nothings/stb) – Image loading (embedded in source)

---

## 🧰 Build Instructions

> Requires CMake 3.16+ and a C++20 compatible compiler

```bash
git clone https://github.com/c0d3m4nc3r/AustereEngine.git
cd AustereEngine
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
