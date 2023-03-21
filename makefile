SFMLLIBS = -lsfml-graphics -lsfml-window -lsfml-system

vstr_main.o: vstr_*.cpp 
	c++ -c vstr_*.cpp 

compile: vstr_*.cpp
	c++ vstr_*.cpp -o ex_vstr $(SFMLLIBS)

FT: vstr_*.cpp
	c++ vstr_*.cpp -D FT -o ex_vstr $(SFMLLIBS)

run: ex_vstr
	./ex_vstr

clean: *.o ex_*
	rm *.o ex_*

