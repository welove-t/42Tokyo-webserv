NAME = server
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
INCLUDES = -I.
SRCS		= main.cpp server.cpp request_handler.cpp
OBJS_DIR	= .objs
OBJS		= $(addprefix $(OBJS_DIR)/, $(SRCS:.cpp=.o))

.PHONY: depend clean fclean

all:$(NAME)
	@echo  Simple server has been compiled

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(NAME) $(OBJS)

# ソースファイルからオブジェクトファイルを生成
$(OBJS_DIR)/%.o: %.cpp
	mkdir -p $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) ${OBJS_DIR}/*.o

fclean: clean
	$(RM) $(NAME)

re: fclean all

# 依存関係の再構築
depend: $(SRCS)
	makedepend $(INCLUDES) $^

# 依存関係の終了
