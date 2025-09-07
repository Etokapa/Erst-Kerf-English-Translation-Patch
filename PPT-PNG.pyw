#!/usr/bin/env python3
import pathlib
import tkinter as tk

def switch_ext_core(src_ext: str, dst_ext: str) -> int:
    root = pathlib.Path.cwd()
    renamed = 0
    for path in root.rglob(f'*{src_ext}'):
        if path.is_file():
            try:
                path.rename(path.with_suffix(dst_ext))
                renamed += 1
            except Exception as exc:
                print(f'⚠️  {path} → {dst_ext} failed: {exc}')
    return renamed

def do_switch(src_ext: str, dst_ext: str):
    count = switch_ext_core(src_ext, dst_ext)
    plural = '' if count == 1 else 's'
    msg = f"Renamed {count} file{plural} to {dst_ext} in {pathlib.Path.cwd()}"
    status_label.config(text=msg)
    status_label.after(5000, lambda: status_label.config(text=""))

app = tk.Tk()
app.title("PPT ↔ PNG Switcher")
app.configure(bg="#2e2e2e")

BTN_BG, FG = "#444444", "#e5e5e5"

tk.Button(app, text="PPT ➜ PNG", width=15,
          command=lambda: do_switch(".ppt", ".png"),
          bg=BTN_BG, fg=FG, activebackground="#007acc")\
    .pack(padx=30, pady=(30, 10))

tk.Button(app, text="PNG ➜ PPT", width=15,
          command=lambda: do_switch(".png", ".ppt"),
          bg=BTN_BG, fg=FG, activebackground="#007acc")\
    .pack(padx=30, pady=(0, 30))

status_label = tk.Label(app,
                        text="",
                        bg=app["bg"],
                        fg=FG,
                        anchor="w")
status_label.pack(side="bottom",
                  fill="x",
                  padx=30,
                  pady=(0, 10))
app.mainloop()