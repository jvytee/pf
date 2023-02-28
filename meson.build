project('pf', 'c')

pngdep = dependency('libpng')
checkdep = dependency('check')

src = ['src/main.c', 'src/image.c']
args = ['-O', '-Wall', '-Wextra', '-pedantic', '-Werror']
executable('pf', src, c_args: args, dependencies: pngdep)

maintest = executable('test_main', 'tests/test_main.c', dependencies: checkdep)
test('Main test', maintest)
