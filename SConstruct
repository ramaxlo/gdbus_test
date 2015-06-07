import os

cmd = 'gdbus-codegen --interface-prefix com.umbocv.Test. --c-namespace Test --generate-c-code test-server-generated test-server.xml'
env = Environment(CFLAGS = ['-Wall'])
env.ParseConfig('pkg-config --cflags --libs gio-2.0 gio-unix-2.0')

if not GetOption('clean'):
	Execute(cmd)
else:
	Execute('rm -f test-server-generated.*')
env.Program('server', ['server.c', 'test-server-generated.c'])
env.Program('client', ['client.c', 'test-server-generated.c'])
