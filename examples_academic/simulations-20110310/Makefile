CC = gcc
INCLUDE = -I../../lib -I/usr/include/freetype2/
CFLAGS = -O3 -g
LDFLAGS = 
LIBS = -lm -lfreetype -lfbeggx -L../../lib

OBJS = 	snowfall_photo demomesh demo3d wave ca leaf drum particles schiff

#OBJS = 	leaf collision collision_xpm double_pendulum coupled_oscillation \
#       	snowfall particles snowfall_photo elastic_pendulum \
#       	dla dla_line wave schiff slope drum sphere river missile \
#       	duffing lorenz collision_circle ca rossler EBdrift stolen_planet \
#       	3body_pythagoras vanallen collision_3dxpm demo3d demomesh \
#       	gears3d_ctrl rod_ctrl

all:: $(OBJS)

clean::
	rm -f $(OBJS) *.exe

CMD = sh cmd.sh ../../egg -I. -L..
CCMD = gcc -Wall 

3body_pythagoras: 3body_pythagoras.c libfbeggx.a
	$(CC) -o 3body_pythagoras  3body_pythagoras.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

ca: ca_ctrl.c libfbeggx.a
	$(CC) -o ca ca_ctrl.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

collision: collision_ctrl.c libfbeggx.a
	$(CC) -o collision collision_ctrl.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

collision_3dxpm: collision_3d.c stone.h libfbeggx.a
	$(CC) -o collision_3dxpm collision_3d.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

collision_circle: collision_circle_ctrl.c gold.h libfbeggx.a
	$(CC) -o collision_circle collision_circle_ctrl.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

collision_xpm: collision_xpm_ctrl.c stone.h libfbeggx.a
	$(CC) -o collision_xpm collision_xpm_ctrl.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

coupled_oscillation: coupled_oscillation_ctrl.c spring.c stone.h libfbeggx.a
	$(CC) -o coupled_oscillation coupled_oscillation_ctrl.c spring.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

dla: dla_ctrl.c libfbeggx.a
	$(CC) -o dla dla_ctrl.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

dla_line: dla_line_ctrl.c libfbeggx.a
	$(CC) -o dla_line dla_line_ctrl.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

double_pendulum: double_pendulum_ctrl.c stone.h green.h libfbeggx.a
	$(CC) -o double_pendulum double_pendulum_ctrl.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

drum: drum_ctrl.c colormode.h libfbeggx.a
	$(CC) -o drum drum_ctrl.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

duffing: duffing_ctrl.c colormode.h libfbeggx.a
	$(CC) -o duffing duffing_ctrl.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

EBdrift: EBdrift_3d.c e_3d.h libfbeggx.a
	$(CC) -o EBdrift EBdrift_3d.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

elastic_pendulum: elastic_pendulum_ctrl.c spring.c stone.h libfbeggx.a
	$(CC) -o elastic_pendulum elastic_pendulum_ctrl.c spring.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

gears3d_ctrl: gears3d_ctrl.c e_3d.h libfbeggx.a
	$(CC) -o gears3d_ctrl gears3d_ctrl.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

leaf: leaf_ctrl.c tiny_complex.h libfbeggx.a
	$(CC) -o leaf leaf_ctrl.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

lorenz: lorenz_3d.c libfbeggx.a
	$(CC) -o lorenz lorenz_3d.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

missile: missile_ctrl.c penguin.h libfbeggx.a
	$(CC) -o missile missile_ctrl.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

particles: particles_ctrl.c orange.h libfbeggx.a
	$(CC) -o particles particles_ctrl.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

river: river_ctrl.c penguin.h libfbeggx.a
	$(CC) -o river river_ctrl.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

rod_ctrl: rod_ctrl.c rk4fix.h libfbeggx.a
	$(CC) -o rod_ctrl rod_ctrl.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

rossler: rossler_3d.c e_3d.h libfbeggx.a
	$(CC) -o rossler rossler_3d.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

schiff: schiff_ctrl.c colormode.h libfbeggx.a
	$(CC) -o schiff schiff_ctrl.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

slope: slope_ctrl.c slope.h libfbeggx.a
	$(CC) -o slope slope_ctrl.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

snowfall: snowfall_ctrl.c tiny_complex.h libfbeggx.a
	$(CC) -o snowfall snowfall_ctrl.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

snowfall_photo: snowfall_photo.c snowfall.h libfbeggx.a
	$(CC) -o snowfall_photo snowfall_photo.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

sphere: sphere_ctrl.c colormode.h libfbeggx.a
	$(CC) -o sphere sphere_ctrl.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

stolen_planet: stolen_planet_3d.c libfbeggx.a
	$(CC) -o stolen_planet stolen_planet_3d.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

vanallen: vanallen_3d.c libfbeggx.a
	$(CC) -o vanallen vanallen_3d.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)


wave: wave_ctrl.c colormode.h libfbeggx.a
	$(CC) -o wave wave_ctrl.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

demo3d: demo3d.c e_3d.h libfbeggx.a
	$(CC) -o demo3d demo3d.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

demomesh: demomesh.c e_3d.h libfbeggx.a
	$(CC) -o demomesh demomesh.c $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS)

libfbeggx.a:
	cd ../../lib;make