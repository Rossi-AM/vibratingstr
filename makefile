#
# 03/2023
#                  
# Rossi A. M.:  alessandromatteo.rossi@studenti.unimi.it
# Tambini M. :  marco.tambini@studenti.unimi.it
#  

SFMLLIBS = -lsfml-graphics -lsfml-window -lsfml-system

all: compile link link_FT

compile: vstr_*.cpp 
	c++ -c vstr_*.cpp 

link: vstr_*.cpp
	c++ vstr_*.cpp -o ex_vstr_simulation $(SFMLLIBS)

link_FT: vstr_*.cpp
	c++ vstr_*.cpp -D FT -o ex_vstr_DFT $(SFMLLIBS)

run: ex_vstr
	./$<

FT: ex_vstr_DFT
	./$<

clean: *.o ex_*
	rm *.o ex_*

