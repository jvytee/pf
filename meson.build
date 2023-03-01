project('pf', 'c', default_options: ['werror=true', 'buildtype=debugoptimized', 'warning_level=3'])

pngdep = dependency('libpng')
checkdep = dependency('check')

src = ['src/main.c', 'src/cli.c', 'src/image.c']
executable('pf', src, dependencies: pngdep)

maintest = executable('test_main', 'tests/test_main.c', dependencies: checkdep)
test('Main test', maintest)
