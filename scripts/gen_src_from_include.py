from pathlib import Path


INCLUDE = Path(__file__) / "include"
SRC = Path(__file__) / "src"

def gen_src_file(include_dir: Path, src_dir: Path):
    if not include_dir.exists() or not include_dir.is_dir():
        return

    if not src_dir.exists():
        src_dir.mkdir(parents=True)
    for item in include_dir.iterdir():
        if not item.is_dir():
            _src = src_dir / item.name

            if _src.exists():
                print(f"{_src} exists, skip")
                continue
            print(f"create {_src} for {item}")
            _src.touch()
            with open(_src, "a") as f:
                f.write(f"""#include "{str(include_dir.relative_to(INCLUDE))}"
                """)
        else:
            gen_src_file(
                include_dir=include_dir / item.name,
                src_dir=src_dir / item.name,
            )


def main():
    assert INCLUDE.exists() and SRC.exists

    gen_src_file(INCLUDE, SRC)

if __name__ == "__main__":
    main()
