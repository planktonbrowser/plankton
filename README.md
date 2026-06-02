# The Plankton Browser

Plankton is an experimental web browser built on top of Servo, designed with both handhelds and desktops in mind. 

At its core sits **Kelp**, a library that exposes Servo to Qt Quick. Think of `QtWebEngine` but backed by Servo.

In a sea of Chromium and Firefox forks, Plankton aims to be a *snappy* browser that gets the *basics* right and improves Servo and related projects in the process.

## Development

Currently the dependencies are:
- Qt (QtCore, QtQuick, QtGui, QtWidgets, QtQml)
- KDE framework: Kirigami
- Rust (cargo and rustc)

To build the project:
```sh
git clone https://github.com/planktonbrowser/plankton.git
cargo build
cargo run
```

## Roadmap

- [ ] Get a basic web page rendering (v0.1)
- [ ] Navigation controls (back, forward, refresh) (v0.2)
- [ ] URL bar (v0.3)
	- [ ] Top / bottom (Mobile) (v0.31)
- [ ] Tabs (v0.4)
	- [ ] Horizontal (desktop) (v0.41)
	- [ ] Vertical (desktop) (v0.42)
	- [ ] Tab overview (mobile) (v0.43)
- [ ] History (v0.5)
	- [ ] Fuzzy history find and completion (v0.51)
- [ ] Settings (v0.6)
	- [ ] Hamburger settings menu (v0.61)
	- [ ] Right click context menu (v0.62)
- [ ] Bookmarks (v0.7)
- [ ] Downloads manager (v0.8)
- [ ] Passwords manager (v0.9)

- [ ] Sync (TBD)
- [ ] Adblock (TBD)

