# build-all by default
default: build-all

# show this information
help:
    @just --list

# setup build env
setup:
    meson setup --reconfigure build

# build all
build-all:
    meson compile -C build

# build a specific target
build TARGET:
    meson compile -C build {{TARGET}}

# test all
test-all:
    meson test -C build

# run a specific test
test TEST:
    meson test -C build {{TEST}} --verbose

# run pre-commit
pre-commit:
    pre-commit run -a

# run-clang-tidy auto fix
fix TARGET:
    run-clang-tidy {{TARGET}} -fix -p build -header-filter=tinyufo_cpp/

# install(dry-run)
dry-run:
    meson install -C build --no-rebuild --only-changed --dry-run

# install library
install:
    meson install -C build --no-rebuild --only-changed
