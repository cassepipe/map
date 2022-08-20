##################
##	VARIABLES	##
##################

SHELL			=	zsh

NAME			=	a.out

CXX				=	g++
CXX				=	g++-12
CXX				=	clang++

INCLUDE_FLAGS	=

#Add -Werror before correction 
CXXFLAGS		=	${INCLUDE_FLAGS} -Wall -Wextra -g3 -std=c++98

#Our beloved address sanitizer
ASAN_FLAG			=	-fsanitize=address	
CXXFLAGS			+=	$(ASAN_FLAG)	
LDFLAGS				+=	$(ASAN_FLAG)	

SRC/SOURCES		=	$(wildcard *.cpp)

HEADERS			=	$(wildcard *.hpp)

LDFLAGS			+=
LDLIBS			=

OBJ/OBJECTS		=	$(patsubst %.cpp, obj/%.o, $(SRC/SOURCES))
OBJ/PCH			=	$(patsubst %, obj/%.gch, $(HEADERS))

##############
##	RULES	##
##############

all:			$(NAME)

$(NAME):		${OBJ/PCH} ${OBJ/OBJECTS} 
				@echo "Linking..."
				@# LDFLAGS (-L) always come before oject files !
				${CXX} -o $@ ${LDFLAGS} ${OBJ/OBJECTS} ${LDLIBS}

obj/%.o:		%.cpp ${HEADERS} Makefile | obj
				${CXX} ${CXXFLAGS} -c $< -o $@

obj/%.gch:		% ${HEADERS} Makefile | obj
				${CXX} ${CXXFLAGS} -c $< -o $@

obj:			
				mkdir obj
clean:			
				rm -rf obj
				rm -rf tree*

fclean:			clean
				rm -rf $(NAME)

re:				fclean all

print_name:
				@echo $(NAME)

run:			all
				./$(NAME)
				./visu.zsh

.PHONY:			all clean fclean re print_name run
