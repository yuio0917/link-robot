TARGET = LinkRobot

SRCS = main.ino \
		srcs/InverseKinematics.cpp \
		srcs/LinkRobot.cpp \
		srcs/TextCoords.cpp \

OBJS = $(SRCS:.cpp=.o)

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic-errors #c++98となってますが，最新のコンパイラに変更しといてください．

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

.PHONEY: all fclean clean re