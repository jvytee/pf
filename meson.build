project('pf', 'c')

pngdep = dependency('libpng')
executable('pf', 'src/main.c', c_args: ['-O', '-Wall', '-Wextra', '-pedantic', '-Werror'], dependencies: pngdep)
