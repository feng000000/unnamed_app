from pathlib import Path
import sys

assert len(sys.argv) > 1, "Missing exec path"

EXEC_PATH = Path(sys.argv[1]).resolve()
INCLUDE = EXEC_PATH / "include"
SRC = EXEC_PATH / "src"

# include, src
CREATE_LIST: list[tuple[Path, Path]] = []

SKIP_LIST: tuple[str] = (
    "window_base.cpp",
)

def gen_src_file(include_dir: Path, src_dir: Path):
    if not include_dir.exists() or not include_dir.is_dir():
        return

    if not src_dir.exists():
        src_dir.mkdir(parents=True)
    for item in include_dir.iterdir():
        if not item.is_dir():
            filename = item.name
            if filename.endswith(".hpp"):
                continue
            _src = src_dir / (filename.rsplit(".", maxsplit=1)[0] + ".cpp")

            if _src.exists():
                print(f"{_src} exists, skip")
                continue
            print(f"create {_src} for {item}")
            CREATE_LIST.append((item, _src))
        else:
            if item.name in (
                "spdlog"
            ):
                continue

            gen_src_file(
                include_dir=include_dir / item.name,
                src_dir=src_dir / item.name,
            )

def green(text) -> str:
    return f"\033[32m{str(text)}\033[0m"

def main():
    global CREATE_LIST
    assert INCLUDE.exists() and SRC.exists

    gen_src_file(INCLUDE, SRC)

    CREATE_LIST = [
        item
        for item in CREATE_LIST
        if item[1].name not in SKIP_LIST
    ]

    print("========\nwill create:")
    for include, src in CREATE_LIST:
        print(f"- {green(src)}\n    ({include})")


    if (answer := input("generate? Y/n: ")) and answer != "Y":
        return

    for include, src in CREATE_LIST:
        with open(src, "a") as f:
            f.write(
                f"""#include "{str(include.relative_to(INCLUDE))}"

// TODO: implement {src.name}
""")
    print("generated.")


if __name__ == "__main__":
    main()
