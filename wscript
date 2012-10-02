VERSION = '0.0.1'
APPNAME = 'wm'

top = '.'

def options(opt):
    opt.load('compiler_cxx')
    opt.load('unittest_gtest')
    opt.recurse('src')

def configure(conf):
    conf.load('compiler_cxx')
    conf.load('unittest_gtest')
    conf.check_cfg(package = 'ux', args = '--cflags --libs', uselib_store = 'UX')
    
    conf.env.CXXFLAGS += ['-W', '-Wall', '-Wextra', '-O2', '-g']
    conf.recurse('src')

def build(bld):
    bld.recurse('src')
