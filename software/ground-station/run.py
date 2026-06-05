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
    "-Ilib",
    " -I./",
]

LDFLAGS = [
    "-lm",
]

CFLAGS  += subprocess.run(['pkg-config', '--cflags', LIBS], capture_output=True, text=True).stdout.split()
LDFLAGS += subprocess.run(['pkg-config',   '--libs', LIBS], capture_output=True, text=True).stdout.split()

# lib compilation


def write_ninja():
    # finds all compiled and to be compiled files for ninja
    src_files = list(SRC_DIR.rglob("*.cpp"))
    mm_files  = list(SRC_DIR.rglob("*.mm")) if PLATFORM == "darwin" else []

    lib_cpp = list(Path("lib").rglob("*.cpp"))
    lib_mm  = list(Path("lib").rglob("*.mm")) if PLATFORM == "darwin" else []

    objcxx_flags = [f for f in CFLAGS if not f.startswith("-std=")] + ["-fobjc-arc"]

    with open("build.ninja", "w") as ninja:
        # ninja rules
        ninja.write(
            f"ninja_required_version = 1.10\n"
            f"\n"
            f"cc = {CC}\n"
            f"cflags = {" ".join(CFLAGS)}\n"
            f"objcxxflags = {' '.join(objcxx_flags)}\n"
            f"ldflags = {" ".join(LDFLAGS)}\n"
            f"builddir = {BUILD_DIR}\n"
            f"\n"
            f"rule cc\n" # compilation rule
            f"  command = $cc $cflags -c $in -o $out\n"
            f"  description = CC $in\n"
            f"\n"
            f"rule objcxx\n"
            f"  command = $cc -x objective-c++ $objcxxflags -c $in -o $out\n"
            f"  description = OBJCXX $in\n"
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

        for src in mm_files:
            obj = OBJ_OUT / (src.stem + ".o")
            ninja.write(f"build {obj}: objcxx {src}\n")
            obj_out.append(obj)

        # lib includes
        for src in lib_cpp:
            # e.g. lib/imgui/imgui.o, lib/imgui/backends/imgui_impl_sdl3.o
            rel = src.relative_to("lib")
            obj = OBJ_OUT / "lib" / rel.with_suffix(".o")
            obj.parent.mkdir(parents=True, exist_ok=True)
            ninja.write(f"build {obj}: cc {src}\n")
            obj_out.append(obj)

        for src in lib_mm:
            rel = src.relative_to("lib")
            obj = OBJ_OUT / "lib" / rel.with_suffix(".o")
            obj.parent.mkdir(parents=True, exist_ok=True)
            ninja.write(f"build {obj}: objcxx {src}\n")
            obj_out.append(obj)

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


