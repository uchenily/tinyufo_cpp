project('tinyufo_cpp', 'cpp',
  version: '0.0.1',
  default_options: [
    'buildtype=debug',
    'default_library=shared',
    'cpp_std=c++20',
    'warning_level=3'],
  meson_version: '>=0.63.0')

compile_args = []

includes = include_directories('tinyufo_cpp')

dependencies = []

subdir('tests')

# install_headers('tinyufo/tinyufo.hpp', install_dir: 'include')
