project('pf', 'c', default_options: ['werror=true', 'buildtype=release', 'warning_level=3'])

png_dep = dependency('libpng')
thread_dep = dependency('threads')
check_dep = dependency('check')

src = ['src/main.c', 'src/cli.c', 'src/image.c', 'src/network.c', 'src/log.c']
inc = include_directories('include')
executable('pf', src, dependencies: [png_dep, thread_dep], include_directories: inc)

pfcheck_src = ['tests/main.c', 'src/cli.c']
pfcheck = executable('pfcheck', pfcheck_src, dependencies: check_dep, include_directories: inc)
test('Check pf', pfcheck)
