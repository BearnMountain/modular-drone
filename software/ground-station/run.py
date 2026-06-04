#!/usr/bin/env python3 

# PLEASE DOWNLOAD ALL DEPENDENCIES YOURSELF

# chose build options
# - release: create release build
# - debug: builds and run with "DEBUG" flag
# - clean: cleans up everythin

import subprocess
import shutil
import sys
import os
from pathlib import Path

# configuration setup
BINARY = "Drone-Ground-Station"
BUILD_DIR = Path("build")
OBJ_OUT = BUILD_DIR / "obj"

SRC_DIR = Path("src")

LIBS = "sdl3"
CC = os.getenv("CXX", "clang++")
PLATFORM = "linux" if sys.platform.startswith("linux") else sys.platform # linux, win32, darwin

CFLAGS = [
    "-std=c++23",
    "-Iinclude",
    " -I./",
]

LDFLAGS = [
    "-lm",
]

CFLAGS  += subprocess.run(['pkg-config', '--cflags', LIBS], capture_output=True, text=True).stdout.split()
LDFLAGS += subprocess.run(['pkg-config',   '--libs', LIBS], capture_output=True, text=True).stdout.split()

def write_ninja():
    # finds all compiled and to be compiled files for ninja
    src_files = list(SRC_DIR.rglob("*.cpp"))

    with open("build.ninja", "w") as ninja:
        # ninja rules
        ninja.write(
            f"ninja_required_version = 1.10\n"
            f"\n"
            f"cc = {CC}\n"
            f"cflags = {" ".join(CFLAGS)}\n"
            f"ldflags = {" ".join(LDFLAGS)}\n"
            f"builddir = {BUILD_DIR}\n"
            f"\n"
            f"rule cc\n" # compilation rule
            f"  command = $cc $cflags -c $in -o $out\n"
            f"  description = CC $in\n"
            f"\n"
            f"rule link\n" # linking objs
            f"  command = $cc $in -o $out $ldflags\n"
            f"  description = LINK $out\n"
            f"\n"
        )

        # c build
        obj_out = []
        for src in src_files:
            object_file = OBJ_OUT / (src.stem + ".o")
            ninja.write(
                    f"build {object_file}: cc {src}\n"
            )
            obj_out.append(object_file)

        # linking everything together
        ninja.write(
            f"\n"
            f"build {BUILD_DIR}/{BINARY}: link {' '.join(str(o) for o in obj_out)}\n"
            f"\n"
            f"default {BUILD_DIR}/{BINARY}\n"
        )
    
def write_compile_flags():
    flags = CFLAGS if isinstance(CFLAGS, list) else CFLAGS.split()
    with open("compile_flags.txt", "w") as f:
        f.write("\n".join(flags))
        f.write("\n-x\n c++-header")

if __name__ == "__main__":
    cmd = sys.argv[1] if len(sys.argv) > 1 else "debug"

    if not os.path.exists(BUILD_DIR):
        os.makedirs(BUILD_DIR)
    if not os.path.exists(OBJ_OUT):
        os.makedirs(OBJ_OUT)

    match cmd:
        case "release":
            print("no release version yet")
        case "debug":
            CFLAGS += [ "-g", "-O0", "-Wall", "-Wextra", "-DDEBUG" ] 
            write_ninja()
            write_compile_flags()
            subprocess.run(["ninja", "-j4"])
            subprocess.run([f"{BUILD_DIR}/{BINARY}"])
        case "clean":
            if BUILD_DIR.exists():
                shutil.rmtree(BUILD_DIR)
            if Path("compile_flags.txt").exists():
                Path("compile_flags.txt").unlink()
            if Path("build.ninja").exists():
                Path("build.ninja").unlink()
        case _:
            print("default debug build")

            CFLAGS += [ "-g", "-O0", "-Wall", "-Wextra", "-DDEBUG" ] 
            write_ninja()
            write_compile_flags()
            subprocess.run(["ninja", "-j4"])
            subprocess.run([f"{BUILD_DIR}/{BINARY}"])


