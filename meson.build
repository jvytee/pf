project('pf', 'c', default_options: ['werror=true', 'buildtype=debugoptimized', 'warning_level=3'])

pngdep = dependency('libpng')
checkdep = dependency('check')

src = ['src/main.c', 'src/cli.c', 'src/image.c']
include_dirs = include_directories('include')
executable('pf', src, dependencies: pngdep, include_directories: include_dirs)

src_check = ['tests/main.c', 'src/cli.c']
pfcheck = executable('pfcheck', src_check, dependencies: checkdep, include_directories: include_dirs)
test('Check pf', pfcheck)
