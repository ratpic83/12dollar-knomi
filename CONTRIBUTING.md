# 🤝 Contributing to PaWe i-print

Thank you for your interest in contributing! This document provides guidelines for contributing to the project.

---

## 📋 Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [Making Changes](#making-changes)
- [Pull Request Process](#pull-request-process)
- [Coding Standards](#coding-standards)
- [Testing](#testing)

---

## 📜 Code of Conduct

- Be respectful and inclusive
- Help others learn and grow
- Focus on constructive feedback
- Keep discussions on-topic

---

## 🚀 Getting Started

### Prerequisites

- PlatformIO IDE (VS Code extension)
- ESP32-2424S012C hardware
- Basic C++ knowledge
- Git

### Fork & Clone

```bash
# Fork the repository on GitHub
# Then clone your fork
git clone https://github.com/YOUR_USERNAME/12dollar-knomi.git
cd 12dollar-knomi
```

---

## 🛠️ Development Setup

### 1. Install Dependencies

```bash
# PlatformIO will auto-install libraries
cd firmware
pio lib install
```

### 2. Configure WiFi

```bash
cp src/WifiConfig.h.example src/WifiConfig.h
# Edit with your credentials
```

### 3. Build & Test

```bash
pio run
pio run -t upload
```

---

## ✏️ Making Changes

### Branch Naming

- `feature/` - New features
- `fix/` - Bug fixes
- `docs/` - Documentation updates
- `refactor/` - Code refactoring

**Example:**
```bash
git checkout -b feature/add-temperature-graph
```

### Commit Messages

Follow conventional commits:

```
<type>: <description>

[optional body]
[optional footer]
```

**Types:**
- `feat:` - New feature
- `fix:` - Bug fix
- `docs:` - Documentation
- `style:` - Formatting
- `refactor:` - Code restructuring
- `test:` - Adding tests
- `chore:` - Maintenance

**Examples:**
```
feat: Add temperature graph to printing screen

fix: Resolve touch gesture detection on CST816D

docs: Update HARDWARE.md with new pinout diagram
```

---

## 🔄 Pull Request Process

### 1. Update Your Fork

```bash
git remote add upstream https://github.com/ratpic83/12dollar-knomi.git
git fetch upstream
git rebase upstream/main
```

### 2. Test Your Changes

- Build successfully
- Test on hardware
- Check for memory leaks
- Verify all features work

### 3. Create Pull Request

1. Push to your fork
2. Open PR on GitHub
3. Fill out PR template
4. Link related issues

### PR Checklist

- [ ] Code builds without errors
- [ ] Tested on hardware
- [ ] Documentation updated
- [ ] No breaking changes (or documented)
- [ ] Follows coding standards
- [ ] Commit messages are clear

---

## 📝 Coding Standards

### C++ Style

```cpp
// Use camelCase for variables
int touchThreshold = 50;

// Use PascalCase for classes
class DisplayDriver {
  // ...
};

// Use UPPER_CASE for constants
#define MAX_BRIGHTNESS 255

// Use descriptive names
void updateTemperatureDisplay() {  // Good
  // ...
}

void upd() {  // Bad
  // ...
}
```

### Formatting

- **Indentation:** 2 spaces (no tabs)
- **Line length:** Max 100 characters
- **Braces:** K&R style
- **Comments:** Use `//` for single line, `/* */` for blocks

### File Organization

```cpp
// 1. Includes
#include <Arduino.h>
#include "DisplayDriver.h"

// 2. Defines
#define SCREEN_WIDTH 240

// 3. Type definitions
struct Point {
  int x, y;
};

// 4. Global variables (avoid if possible)
static int globalCounter = 0;

// 5. Function declarations
void setup();
void loop();

// 6. Function implementations
void setup() {
  // ...
}
```

---

## 🧪 Testing

### Manual Testing

Test on real hardware:

1. **Display** - All screens render correctly
2. **Touch** - All gestures work
3. **WiFi** - Connects reliably
4. **Klipper** - Data updates correctly
5. **Memory** - No leaks or crashes

### Serial Debugging

```cpp
Serial.println("Debug: Touch detected");
Serial.printf("Brightness: %d\n", brightness);
```

### Memory Check

```bash
pio run -t size
# Check RAM and Flash usage
```

---

## 📚 Documentation

### Update Documentation

When adding features, update:

- `README.md` - If user-facing
- `FEATURES.md` - New features/gestures
- `HARDWARE.md` - Hardware changes
- Code comments - Complex logic

### Documentation Style

- Use clear, concise language
- Include code examples
- Add diagrams where helpful
- Keep formatting consistent

---

## 🎨 Adding Features

### New Display Screen

1. Add enum to `UIManager.h`:
```cpp
enum ScreenType {
  // ...
  SCREEN_CUSTOM,
};
```

2. Implement in `UIManager.cpp`:
```cpp
void UIManager::drawCustomScreen(PrinterStatus& status) {
  display->clear();
  // Your code here
}
```

3. Add to update logic
4. Document in `FEATURES.md`

### New Touch Gesture

1. Add to `TouchDriver.h`:
```cpp
enum TouchEvent {
  // ...
  TOUCH_GESTURE_CUSTOM,
};
```

2. Implement detection in `TouchDriver.cpp`
3. Handle in `UIManager::handleTouchEvent()`
4. Document in `FEATURES.md`

### New Theme

1. Edit `ThemeManager.cpp`:
```cpp
themes[4] = {
  .background = 0x0000,
  // ... colors
};
```

2. Update theme count
3. Test with all screens
4. Document colors

---

## 🐛 Reporting Bugs

### Bug Report Template

```markdown
**Describe the bug**
Clear description of the issue

**To Reproduce**
1. Go to '...'
2. Tap on '...'
3. See error

**Expected behavior**
What should happen

**Hardware**
- Board: ESP32-2424S012C
- Firmware version: v1.1
- Touch IC: CST816D / FT6236

**Serial Output**
```
Paste relevant serial output
```

**Additional context**
Any other relevant information
```

---

## 💡 Feature Requests

### Feature Request Template

```markdown
**Feature Description**
Clear description of the feature

**Use Case**
Why is this feature needed?

**Proposed Solution**
How should it work?

**Alternatives**
Other ways to achieve this

**Additional context**
Mockups, diagrams, etc.
```

---

## 🏆 Recognition

Contributors will be:
- Listed in README.md
- Credited in release notes
- Thanked in commit messages

---

## 📞 Questions?

- **Issues:** [GitHub Issues](https://github.com/ratpic83/12dollar-knomi/issues)
- **Discussions:** [GitHub Discussions](https://github.com/ratpic83/12dollar-knomi/discussions)
- **Email:** Check repository for contact info

---

## 📄 License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

**Thank you for contributing to PaWe i-print!** 🎉
