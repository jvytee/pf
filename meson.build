project('pf', 'c')

pngdep = dependency('libpng')
checkdep = dependency('check')

args = ['-O', '-Wall', '-Wextra', '-pedantic', '-Werror']
executable('pf', 'src/main.c', c_args: args, dependencies: pngdep)

maintest = executable('test_main', 'tests/test_main.c', dependencies: checkdep)
test('Main test', maintest)
