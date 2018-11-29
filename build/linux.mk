#
#  CONTENT
#    common make rules for linux
#
#  AUTHOR
#    Sebastian Balz
#		based on Norbert Stoeffler
#
#  DATE
#    2018-03-10
#
#  LANGUAGE
#    make
#

CXXFLAGS +=		$(addprefix -I,$(APP_INCS)) -std=c++11
_APP_OBJS1 = 	$(patsubst %.cc,%.o,$(patsubst %.cpp,%.o,$(APP_SRCS) ))
_APP_OBJS = 	$(notdir $(_APP_OBJS1))

VPATH =			$(sort $(dir $(_APP_OBJS1)))

all:: $(APP)

clean::
	rm -f $(APP) $(_APP_OBJS)

run:	all
	./$(APP)


$(APP): $(_APP_OBJS)
	g++ -o $@ $^ $(addprefix -l,$(APP_SLIB))

debug::
	@echo APP_SRCS = $(APP_SRCS)
	@echo _APP_OBJS = $(_APP_OBJS)
	@echo _TEST_OBJS = $(_TEST_OBJS)
	@echo VPATH=$(VPATH)
