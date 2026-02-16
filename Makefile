TARGET = LinkRobot

SRCS = main.cpp \
		srcs/InverseKinematics.cpp \
		srcs/LinkRobot.cpp \
		srcs/TextCoords.cpp \

OBJS = $(SRCS:.cpp=.o)

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -pedantic-errors -I include

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(TARGET)

re: fclean all

.PHONY: all fclean clean re
