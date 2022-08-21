##################
##	VARIABLES	##
##################

NAME			=	a.out
CXX				=	g++
CXX				=	clang++

SRC/SOURCES		=	$(wildcard *.cpp)
OBJ/OBJECTS		=	$(patsubst %.cpp, obj/%.o, $(SRC/SOURCES))
OBJ/DEPS		=	$(patsubst %.o, %.d, $(OBJ/OBJECTS))

# FLAGS 
INCLUDE_FLAGS	= -I.
CPPFLAGS		= ${INCLUDE_FLAGS} -MMD -MP
#Add -Werror before correction 
CXXFLAGS		=	-Wall -Wextra -g3 -std=c++98
LDFLAGS			=
LDLIBS			= 
#Our beloved address sanitizer
ASAN_FLAG		=	-fsanitize=address,undefined
CXXFLAGS		+=	$(ASAN_FLAG)	
LDFLAGS			+=	$(ASAN_FLAG)	

##############
##	RULES	##
##############

all:			$(NAME)

$(NAME):		${OBJ/OBJECTS} 
				@echo "Linking..."
				@# LDFLAGS (-L) always come before oject files !
				${CXX} -o $@ ${LDFLAGS} ${OBJ/OBJECTS} ${LDLIBS}

obj/%.o:		%.cpp Makefile | obj
				@mkdir -p $(dir $@)
				${CXX} ${CPPFLAGS} ${CXXFLAGS} -c $< -o $@
obj:			
				mkdir obj
clean:			
				rm -rf obj

fclean:			clean
				rm -rf $(NAME)

re:				fclean all

print_name:
				@echo $(NAME)

run:			all
				./$(NAME)

-include $(OBJ/DEPS)

.PHONY:			all clean fclean re print_name run
