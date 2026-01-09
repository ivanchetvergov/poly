import os
import sys
import subprocess
import glob
from pathlib import Path

def run_command(cmd, cwd=None, check=True):
    """Run shell command and return success"""
    try:
        result = subprocess.run(cmd, shell=True, cwd=cwd, capture_output=True, text=True)
        if check and result.returncode != 0:
            print(f"Command failed: {cmd}")
            print(f"Error: {result.stderr}")
            return False
        return True
    except Exception as e:
        print(f"Error running command: {e}")
        return False

def main():
    project_root = Path(__file__).parent.parent
    build_dir = project_root / "build"

    print("=== Code Auto-Fix Script ===")
    print(f"Project root: {project_root}")

    # Check for required tools
    tools = ['clang-tidy', 'clang-format', 'make', 'cmake']
    missing_tools = []
    for tool in tools:
        if not run_command(f"which {tool}", check=False):
            missing_tools.append(tool)

    if missing_tools:
        print(f"Error: Missing tools: {', '.join(missing_tools)}")
        print("Install LLVM tools: brew install llvm")
        sys.exit(1)

    # Build project first to ensure compile_commands.json exists
    print("Building project...")
    os.chdir(project_root)
    run_command("make clean")
    run_command("cmake -S . -B build -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON")
    run_command("make -j$(sysctl -n hw.ncpu)")

    # Find all C++ source and header files
    print("Finding source files...")
    source_files = []
    exclude_dirs = {'build', 'cmake-build-*', '.git', 'node_modules'}

    for ext in ['*.cc', '*.cpp', '*.h', '*.hpp']:
        for pattern in [f"**/{ext}"]:
            for file_path in glob.glob(pattern, root_dir=project_root, recursive=True):
                path_parts = Path(file_path).parts
                if not any(excl in path_parts for excl in exclude_dirs):
                    source_files.append(project_root / file_path)

    source_files = list(set(source_files))  # Remove duplicates
    print(f"Found {len(source_files)} source files")

    if not source_files:
        print("No source files found!")
        sys.exit(1)

    # Apply clang-tidy fixes
    print("Applying clang-tidy fixes...")
    for file_path in source_files:
        cmd = f"clang-tidy --fix --quiet {file_path}"
        run_command(cmd, check=False)  # Don't fail on individual file errors

    # Apply clang-format
    print("Applying clang-format...")
    file_list = " ".join(str(f) for f in source_files)
    run_command(f"clang-format -i {file_list}")

    # Rebuild to check for errors
    print("Rebuilding after fixes...")
    run_command("make -j$(sysctl -n hw.ncpu)")

    print("=== Code fixes applied successfully! ===")
    print("Review changes with: git diff")
    print("Commit if satisfied: git add . && git commit -m 'Auto-fix code style and issues'")

if __name__ == "__main__":
    main()
