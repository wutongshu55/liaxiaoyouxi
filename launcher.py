import tkinter as tk
import subprocess
import os
from pathlib import Path

BASE = Path(__file__).parent / "bin"

def launch_snake():
    subprocess.Popen([str(BASE / "dyg.exe")], cwd=str(BASE))

def launch_shooter():
    subprocess.Popen([str(BASE / "shooter.exe")], cwd=str(BASE))

# --- Window setup ---
root = tk.Tk()
root.title("Game Launcher")
root.configure(bg="#FFD700")
root.geometry("400x300")
root.resizable(False, False)

# Center on screen
root.update_idletasks()
sw = root.winfo_screenwidth()
sh = root.winfo_screenheight()
x = (sw - 400) // 2
y = (sh - 300) // 2
root.geometry(f"400x300+{x}+{y}")

# --- Widgets ---
tk.Label(
    root, text="Game Launcher",
    bg="#FFD700", fg="#8B6914",
    font=("Arial", 22, "bold")
).pack(pady=(30, 10))

tk.Label(
    root, text="Select a game to play:",
    bg="#FFD700", fg="#5C4A0A",
    font=("Arial", 12)
).pack(pady=(0, 25))

btn_font = ("Arial", 14, "bold")

tk.Button(
    root, text=" Snake Game", font=btn_font,
    bg="#FFD700", fg="#5C4A0A",
    activebackground="#FFC000", activeforeground="#3D2E00",
    relief="raised", bd=3, padx=20, pady=8,
    cursor="hand2", command=launch_snake
).pack(pady=8)

tk.Button(
    root, text="Shooter Game", font=btn_font,
    bg="#FFD700", fg="#5C4A0A",
    activebackground="#FFC000", activeforeground="#3D2E00",
    relief="raised", bd=3, padx=20, pady=8,
    cursor="hand2", command=launch_shooter
).pack(pady=8)

tk.Button(
    root, text="Exit", font=("Arial", 10),
    bg="#B8860B", fg="#FFFFFF",
    activebackground="#8B6914", activeforeground="#FFFFFF",
    relief="raised", bd=2, padx=15, pady=4,
    cursor="hand2", command=root.destroy
).pack(pady=(30, 0))

root.mainloop()
