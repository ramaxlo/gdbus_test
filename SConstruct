import os

cmd = 'gdbus-codegen --interface-prefix com.umbocv.Test. --c-namespace Test --generate-c-code test-server-generated test-server.xml'
env = Environment(CFLAGS = ['-Wall'])
arm_cross_build = ARGUMENTS.get('arm_cross', 0)
if arm_cross_build:
	env['ENV'] = os.environ
	env['CC'] = os.environ['CC']
env.ParseConfig('pkg-config --cflags --libs gio-2.0 gio-unix-2.0')

if not GetOption('clean'):
	Execute(cmd)
else:
	Execute('rm -f test-server-generated.*')
env.Program('server', ['server.c', 'test-server-generated.c'])
env.Program('client', ['client.c', 'test-server-generated.c'])
