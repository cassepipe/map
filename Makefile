##################
##  VARIABLES   ##
##################
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

DIFF			= diff -s
CXX				= g++
CXX				= clang++
SHELL			= bash
FT				= ft_containers_test
STD				= std_containers_test

SOURCES			= $(wildcard *.cpp)
OBJ/FT_OBJECTS	= $(patsubst %.cpp,  obj/ft_%.o, $(SOURCES))
OBJ/STD_OBJECTS	= $(patsubst %.cpp, obj/std_%.o, $(SOURCES))
OBJ/FT_DEPS		= $(patsubst %.o,           %.d, $(OBJ/FT_OBJECTS))
OBJ/STD_DEPS	= $(patsubst %.o,           %.d, $(OBJ/STD_OBJECTS))

# FLAGS 
DEBUG			= -DDEBUG
INCLUDE_FLAGS	= -I.
CPPFLAGS		= ${INCLUDE_FLAGS} ${DEBUG} -MMD 
#Add -Werror before correction 
CXXFLAGS		= -Wall -Wextra -g3 -std=c++98 -Wno-macro-redefined -Wno-return-type -O0
LDFLAGS			=
LDLIBS			= 
#Our beloved address sanitizer
ASAN_FLAG		=  -fsanitize=address,undefined
CXXFLAGS		+=	$(ASAN_FLAG)	
LDFLAGS			+=	$(ASAN_FLAG)	

##############
##  RULES   ##
##############

all:			$(FT) $(STD) diff

$(FT):			$(OBJ/FT_OBJECTS)
				@echo "Linking..."
				@# LDFLAGS (-L) always come before oject files !
				${CXX} -o $@ ${LDFLAGS} $^ ${LDLIBS}

$(STD):			${OBJ/STD_OBJECTS}
				@echo "Linking..."
				@# LDFLAGS (-L) always come before oject files !
				${CXX} -o $@ ${LDFLAGS} $^ ${LDLIBS}

obj/ft_%.o:		%.cpp Makefile | obj
				${CXX} -DNAMESPACE=ft  ${CPPFLAGS} ${CXXFLAGS} -c $< -o $@

obj/std_%.o:	%.cpp Makefile | obj
				${CXX} -DNAMESPACE=std ${CPPFLAGS} ${CXXFLAGS} -c $< -o $@
obj:			
				mkdir obj
clean:			
				rm -rf obj
				rm -rf tree*

fclean:			clean
				rm -rf $(FT)
				rm -rf $(STD)

re:				fclean all

run_ft:			
				./$(FT)
diff:			
				$(DIFF) <( ./${FT} )  <( ./${STD}  ) 
				#$(DIFF) <( ./${FT} 2>&1)  <( ./${STD} 2>&1 ) 


-include $(OBJ/FT_DEPS)
-include $(OBJ/STD_DEPS)

.PHONY:			all clean fclean re run_ft diff
